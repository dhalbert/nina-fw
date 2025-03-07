/*
  This file is part of the Arduino NINA firmware.
  Copyright (c) 2018 Arduino SA. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <lwip/netdb.h>
#include <lwip/sockets.h>
#include "esp_partition.h"

#include "WiFiSSLClient.h"

class __Guard {
public:
  __Guard(SemaphoreHandle_t handle) {
    _handle = handle;

    xSemaphoreTakeRecursive(_handle, portMAX_DELAY);
  }

  ~__Guard() {
    xSemaphoreGiveRecursive(_handle);
  }

private:
  SemaphoreHandle_t _handle;
};

#define synchronized __Guard __guard(_mbedMutex);

WiFiSSLClient::WiFiSSLClient() :
  _connected(false),
  _peek(-1)
{
  _netContext.fd = -1;

  _mbedMutex = xSemaphoreCreateRecursiveMutex();
}

int WiFiSSLClient::connect(const char* host, uint16_t port, bool sni)
{
  synchronized {
    _netContext.fd = -1;
    _connected = false;

    // free heap should be 140KB for this connect ssl to work
    ets_printf("Free heap before TLS %u\n", heap_caps_get_free_size(MALLOC_CAP_8BIT));
    ets_printf("*** connect init\n");

    mbedtls_ssl_init(&_sslContext);
    mbedtls_ctr_drbg_init(&_ctrDrbgContext);
    mbedtls_ssl_config_init(&_sslConfig);
// ADAFRUIT-CHANGE: enable mbedtls logging if requested.
#ifdef CONFIG_MBEDTLS_DEBUG
    mbedtls_esp_enable_debug_log(&_sslConfig, 2);
#endif
    mbedtls_entropy_init(&_entropyContext);
    mbedtls_x509_crt_init(&_caCrt);
    mbedtls_net_init(&_netContext);

    // Seeds and sets up CTR_DRBG for future reseeds, pers is device personalization (esp)
    const char *pers = "esp32-tls";
    if (mbedtls_ctr_drbg_seed(&_ctrDrbgContext, mbedtls_entropy_func, &_entropyContext, (const unsigned char *) pers, strlen(pers)) != 0) {
      ets_printf("Unable to set up mbedtls_entropy.\n");
      stop();
      return 0;
    }

    if (mbedtls_ssl_config_defaults(&_sslConfig, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT) != 0) {
      ets_printf("Error Setting up SSL Config\n");
      stop();
      return 0;
    }

    // we're always using the root CA cert from partition, so MBEDTLS_SSL_VERIFY_REQUIRED
    mbedtls_ssl_conf_authmode(&_sslConfig, MBEDTLS_SSL_VERIFY_REQUIRED);

    // setting up CA certificates from partition
    ets_printf("*** Loading CA Cert...\n");
    spi_flash_mmap_handle_t handle;
    const unsigned char* certs_data = {};

    const esp_partition_t* part = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, "certs");
    if (part == NULL) {
      stop();
      return 0;
    }

    int ret = esp_partition_mmap(part, 0, part->size, SPI_FLASH_MMAP_DATA, (const void**)&certs_data, &handle);
    if (ret != ESP_OK) {
      ets_printf("*** Error partition mmap %d\n", ret);
      stop();
      return 0;
    }
    ets_printf("  Length of certs_data: %d\n", strlen((char*)certs_data)+1);

    // mbedtls_x509_crt_parse() requires ~92000 bytes from heap
    ets_printf("*** connect crt parse\n");
    ets_printf("  Free heap before: %u\n", heap_caps_get_free_size(MALLOC_CAP_8BIT));
    ret = mbedtls_x509_crt_parse(&_caCrt, certs_data, strlen((char*)certs_data) + 1);
    if (ret < 0) {
      ets_printf("*** Error parsing CA chain.\n");
      stop();
      return 0;
    }
    ets_printf("  Free heap after : %u\n", heap_caps_get_free_size(MALLOC_CAP_8BIT));

    mbedtls_ssl_conf_ca_chain(&_sslConfig, &_caCrt, NULL);

    ets_printf("*** check for _cert and _private_key\n");
    if (_cert != NULL && _private_key != NULL) {
        mbedtls_x509_crt_init(&_clientCrt);
        mbedtls_pk_init(&_clientKey);

        ets_printf("*** Loading client certificate.\n");
        // note: +1 added for line ending
        ret = mbedtls_x509_crt_parse(&_clientCrt, (const unsigned char *)_cert, strlen(_cert) + 1);
        if (ret != 0) {
          ets_printf("ERROR: Client cert not parsed properly(%d)\n", ret);
          stop();
          return 0;
        }

        ets_printf("*** Loading private key.\n");
        ret = mbedtls_pk_parse_key(&_clientKey, (const unsigned char *)_private_key, strlen(_private_key)+1,
                                   NULL, 0);
        if (ret != 0) {
          ets_printf("ERROR: Private key not parsed properly:(%d)\n", ret);
          stop();
          return 0;
        }
        // set own certificate chain and key
        ret = mbedtls_ssl_conf_own_cert(&_sslConfig, &_clientCrt, &_clientKey);
        if (ret != 0) {
          if (ret == MBEDTLS_ERR_SSL_ALLOC_FAILED) {
            ets_printf("ERROR: Memory allocation failed, MBEDTLS_ERR_SSL_ALLOC_FAILED");
          }
          ets_printf("Private key not parsed properly(%d)\n", ret);
          stop();
          return 0;
        }
    }
    else {
      ets_printf("Client certificate and key not provided.\n");
    }

    mbedtls_ssl_conf_rng(&_sslConfig, mbedtls_ctr_drbg_random, &_ctrDrbgContext);

    // mbedtls_ssl_setup() requires ~36000 bytes from heap
    ets_printf("*** connect ssl setup\n");
    ets_printf("  Free heap before: %u\n", heap_caps_get_free_size(MALLOC_CAP_8BIT));
    if ((ret = mbedtls_ssl_setup(&_sslContext, &_sslConfig)) != 0) {
      ets_printf("Unable to connect ssl setup %d\n", ret);
      if (ret == MBEDTLS_ERR_SSL_ALLOC_FAILED){
        ets_printf("  Memory allocation failed (MBEDTLS_ERR_SSL_ALLOC_FAILED)\n");
        ets_printf("  %s", &_clientCrt);
      }
      stop();
      return 0;
    }
    ets_printf("  Free heap after : %u\n", heap_caps_get_free_size(MALLOC_CAP_8BIT));

    if (sni && mbedtls_ssl_set_hostname(&_sslContext, host) != 0) {
      stop();
      return 0;
    }

    char portStr[6];
    itoa(port, portStr, 10);

    if (mbedtls_net_connect(&_netContext, host, portStr, MBEDTLS_NET_PROTO_TCP) != 0) {
      stop();
      return 0;
    }

    mbedtls_ssl_set_bio(&_sslContext, &_netContext, mbedtls_net_send, mbedtls_net_recv, NULL);

    // mbedtls_ssl_handshake() requires ~5600 bytes from heap
    ets_printf("*** start SSL/TLS handshake...\n");
    ets_printf("  Free heap before: %u\n", heap_caps_get_free_size(MALLOC_CAP_8BIT));
    do {
      ret = mbedtls_ssl_handshake(&_sslContext);
    } while (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE);
    ets_printf("  Free heap after : %u\n", heap_caps_get_free_size(MALLOC_CAP_8BIT));

    if (ret != 0) {
      ets_printf("Error performing SSL handshake %d\n", ret);
      if (ret == MBEDTLS_ERR_SSL_ALLOC_FAILED) {
        ets_printf("  Memory allocation failed (MBEDTLS_ERR_SSL_ALLOC_FAILED)\n");
      } else if (ret == MBEDTLS_ERR_X509_ALLOC_FAILED){
        ets_printf(  "X509 allocation failed (MBEDTLS_ERR_X509_ALLOC_FAILED)\n");
      }
      stop();
      return 0;
    }


    if (_cert != NULL && _private_key != NULL) {
      ets_printf("Protocol is %s Ciphersuite is %s", mbedtls_ssl_get_version(&_sslContext), mbedtls_ssl_get_ciphersuite(&_sslContext));
    }
    ets_printf("Verifying peer X.509 certificate\n");
    char buf[512];
    int flags;
    if ((flags = mbedtls_ssl_get_verify_result(&_sslContext)) != 0) {
      bzero(buf, sizeof(buf));
      mbedtls_x509_crt_verify_info(buf, sizeof(buf), "  ! ", flags);
      ets_printf("Failed to verify peer certificate! verification info: %s\n", buf);
      stop(); // invalid certificate, stop
      return -1;
    } else {
      ets_printf("Certificate chain verified.\n");
    }

    mbedtls_net_set_nonblock(&_netContext);

    // free the heap
    ets_printf("Free heap before cleanup: %u\n", heap_caps_get_free_size(MALLOC_CAP_8BIT));
    if (certs_data != NULL) {
      mbedtls_x509_crt_free(&_caCrt);
    }
    if (_cert != NULL) {
      mbedtls_x509_crt_free(&_clientCrt);
    }
    if (_private_key !=NULL) {
      mbedtls_pk_free(&_clientKey);
    }
    ets_printf("Free heap after cleanup : %u\n", heap_caps_get_free_size(MALLOC_CAP_8BIT));

    _connected = true;

    return 1;
  }
}

int WiFiSSLClient::connect(const char* host, uint16_t port)
{
  return connect(host, port, true);
}

int WiFiSSLClient::connect(/*IPAddress*/uint32_t ip, uint16_t port)
{
  char ipStr[16];

  sprintf(ipStr, "%d.%d.%d.%d", ((ip & 0xff000000) >> 24), ((ip & 0x00ff0000) >> 16), ((ip & 0x0000ff00) >> 8), ((ip & 0x000000ff) >> 0)/*ip[0], ip[1], ip[2], ip[3]*/);

  return connect(ipStr, port, false);
}

size_t WiFiSSLClient::write(uint8_t b)
{
  return write(&b, 1);
}

size_t WiFiSSLClient::write(const uint8_t *buf, size_t size)
{
  synchronized {
    int written = mbedtls_ssl_write(&_sslContext, buf, size);

    if (written < 0) {
      written = 0;
    }

    return written;
  }
}

int WiFiSSLClient::available()
{
  synchronized {
    int result = mbedtls_ssl_read(&_sslContext, NULL, 0);

    int n = mbedtls_ssl_get_bytes_avail(&_sslContext);

    if (n == 0 && result != 0 && result != MBEDTLS_ERR_SSL_WANT_READ) {
      stop();
    }

    return n;
  }
}

int WiFiSSLClient::read()
{
  uint8_t b;

  if (_peek != -1) {
    b = _peek;
    _peek = -1;
  } else if (read(&b, sizeof(b)) == -1) {
    return -1;
  }

  return b;
}

int WiFiSSLClient::read(uint8_t* buf, size_t size)
{
  synchronized {
    if (!available()) {
      return -1;
    }

    int result = mbedtls_ssl_read(&_sslContext, buf, size);

    if (result < 0) {
      if (result != MBEDTLS_ERR_SSL_WANT_READ && result != MBEDTLS_ERR_SSL_WANT_WRITE) {
        stop();
      }

      return -1;
    }

    return result;
  }
}

int WiFiSSLClient::peek()
{
  if (_peek == -1) {
    _peek = read();
  }

  return _peek;
}

void WiFiSSLClient::setCertificate(const char *client_ca)
{
  ets_printf("\n*** Setting client certificate...\n");
  _cert = client_ca;
  ets_printf("%s", _cert);
  ets_printf("\n*** Set client certificate\n");
}

void WiFiSSLClient::setPrivateKey(const char *private_key)
{
  ets_printf("Setting client private key...\n");
  _private_key = private_key;
  ets_printf("Set client private key\n");
}

void WiFiSSLClient::setHandshakeTimeout(unsigned long timeout)
{
  _handshake_timeout = timeout * 1000;
}

void WiFiSSLClient::flush()
{
}

void WiFiSSLClient::stop()
{
  synchronized {
    if (_netContext.fd > 0) {
      mbedtls_ssl_session_reset(&_sslContext);

      mbedtls_net_free(&_netContext);
      mbedtls_x509_crt_free(&_caCrt);
      mbedtls_x509_crt_free(&_clientCrt);
      mbedtls_pk_free(&_clientKey);
      mbedtls_entropy_free(&_entropyContext);
      mbedtls_ssl_config_free(&_sslConfig);
      mbedtls_ctr_drbg_free(&_ctrDrbgContext);
      mbedtls_ssl_free(&_sslContext);
    }

    _connected = false;
    _netContext.fd = -1;
  }

  vTaskDelay(1);
}

uint8_t WiFiSSLClient::connected()
{
  synchronized {
    if (!_connected) {
      return 0;
    }

    if (available()) {
      return 1;
    }

    return 1;
  }
}

WiFiSSLClient::operator bool()
{
  return ((_netContext.fd != -1) && _connected);
}

/*IPAddress*/uint32_t WiFiSSLClient::remoteIP()
{
  struct sockaddr_storage addr;
  socklen_t len = sizeof(addr);

  getpeername(_netContext.fd, (struct sockaddr*)&addr, &len);

  return ((struct sockaddr_in *)&addr)->sin_addr.s_addr;
}

uint16_t WiFiSSLClient::remotePort()
{
  struct sockaddr_storage addr;
  socklen_t len = sizeof(addr);

  getpeername(_netContext.fd, (struct sockaddr*)&addr, &len);

  return ntohs(((struct sockaddr_in *)&addr)->sin_port);
}
