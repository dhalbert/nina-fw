#ifndef _BEAR_SSL_TRUST_ANCHORS_H_
#define _BEAR_SSL_TRUST_ANCHORS_H_

#include "bearssl/bearssl_ssl.h"

static const unsigned char TA0_DN[] = {
  0x30, 0x42, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13,
  0x02, 0x55, 0x53, 0x31, 0x16, 0x30, 0x14, 0x06, 0x03, 0x55, 0x04, 0x0A,
  0x13, 0x0D, 0x47, 0x65, 0x6F, 0x54, 0x72, 0x75, 0x73, 0x74, 0x20, 0x49,
  0x6E, 0x63, 0x2E, 0x31, 0x1B, 0x30, 0x19, 0x06, 0x03, 0x55, 0x04, 0x03,
  0x13, 0x12, 0x47, 0x65, 0x6F, 0x54, 0x72, 0x75, 0x73, 0x74, 0x20, 0x47,
  0x6C, 0x6F, 0x62, 0x61, 0x6C, 0x20, 0x43, 0x41
};

static const unsigned char TA0_RSA_N[] = {
  0xDA, 0xCC, 0x18, 0x63, 0x30, 0xFD, 0xF4, 0x17, 0x23, 0x1A, 0x56, 0x7E,
  0x5B, 0xDF, 0x3C, 0x6C, 0x38, 0xE4, 0x71, 0xB7, 0x78, 0x91, 0xD4, 0xBC,
  0xA1, 0xD8, 0x4C, 0xF8, 0xA8, 0x43, 0xB6, 0x03, 0xE9, 0x4D, 0x21, 0x07,
  0x08, 0x88, 0xDA, 0x58, 0x2F, 0x66, 0x39, 0x29, 0xBD, 0x05, 0x78, 0x8B,
  0x9D, 0x38, 0xE8, 0x05, 0xB7, 0x6A, 0x7E, 0x71, 0xA4, 0xE6, 0xC4, 0x60,
  0xA6, 0xB0, 0xEF, 0x80, 0xE4, 0x89, 0x28, 0x0F, 0x9E, 0x25, 0xD6, 0xED,
  0x83, 0xF3, 0xAD, 0xA6, 0x91, 0xC7, 0x98, 0xC9, 0x42, 0x18, 0x35, 0x14,
  0x9D, 0xAD, 0x98, 0x46, 0x92, 0x2E, 0x4F, 0xCA, 0xF1, 0x87, 0x43, 0xC1,
  0x16, 0x95, 0x57, 0x2D, 0x50, 0xEF, 0x89, 0x2D, 0x80, 0x7A, 0x57, 0xAD,
  0xF2, 0xEE, 0x5F, 0x6B, 0xD2, 0x00, 0x8D, 0xB9, 0x14, 0xF8, 0x14, 0x15,
  0x35, 0xD9, 0xC0, 0x46, 0xA3, 0x7B, 0x72, 0xC8, 0x91, 0xBF, 0xC9, 0x55,
  0x2B, 0xCD, 0xD0, 0x97, 0x3E, 0x9C, 0x26, 0x64, 0xCC, 0xDF, 0xCE, 0x83,
  0x19, 0x71, 0xCA, 0x4E, 0xE6, 0xD4, 0xD5, 0x7B, 0xA9, 0x19, 0xCD, 0x55,
  0xDE, 0xC8, 0xEC, 0xD2, 0x5E, 0x38, 0x53, 0xE5, 0x5C, 0x4F, 0x8C, 0x2D,
  0xFE, 0x50, 0x23, 0x36, 0xFC, 0x66, 0xE6, 0xCB, 0x8E, 0xA4, 0x39, 0x19,
  0x00, 0xB7, 0x95, 0x02, 0x39, 0x91, 0x0B, 0x0E, 0xFE, 0x38, 0x2E, 0xD1,
  0x1D, 0x05, 0x9A, 0xF6, 0x4D, 0x3E, 0x6F, 0x0F, 0x07, 0x1D, 0xAF, 0x2C,
  0x1E, 0x8F, 0x60, 0x39, 0xE2, 0xFA, 0x36, 0x53, 0x13, 0x39, 0xD4, 0x5E,
  0x26, 0x2B, 0xDB, 0x3D, 0xA8, 0x14, 0xBD, 0x32, 0xEB, 0x18, 0x03, 0x28,
  0x52, 0x04, 0x71, 0xE5, 0xAB, 0x33, 0x3D, 0xE1, 0x38, 0xBB, 0x07, 0x36,
  0x84, 0x62, 0x9C, 0x79, 0xEA, 0x16, 0x30, 0xF4, 0x5F, 0xC0, 0x2B, 0xE8,
  0x71, 0x6B, 0xE4, 0xF9
};

static const unsigned char TA0_RSA_E[] = {
  0x01, 0x00, 0x01
};

static const unsigned char TA1_DN[] = {
  0x30, 0x81, 0x83, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06,
  0x13, 0x02, 0x55, 0x53, 0x31, 0x10, 0x30, 0x0E, 0x06, 0x03, 0x55, 0x04,
  0x08, 0x13, 0x07, 0x41, 0x72, 0x69, 0x7A, 0x6F, 0x6E, 0x61, 0x31, 0x13,
  0x30, 0x11, 0x06, 0x03, 0x55, 0x04, 0x07, 0x13, 0x0A, 0x53, 0x63, 0x6F,
  0x74, 0x74, 0x73, 0x64, 0x61, 0x6C, 0x65, 0x31, 0x1A, 0x30, 0x18, 0x06,
  0x03, 0x55, 0x04, 0x0A, 0x13, 0x11, 0x47, 0x6F, 0x44, 0x61, 0x64, 0x64,
  0x79, 0x2E, 0x63, 0x6F, 0x6D, 0x2C, 0x20, 0x49, 0x6E, 0x63, 0x2E, 0x31,
  0x31, 0x30, 0x2F, 0x06, 0x03, 0x55, 0x04, 0x03, 0x13, 0x28, 0x47, 0x6F,
  0x20, 0x44, 0x61, 0x64, 0x64, 0x79, 0x20, 0x52, 0x6F, 0x6F, 0x74, 0x20,
  0x43, 0x65, 0x72, 0x74, 0x69, 0x66, 0x69, 0x63, 0x61, 0x74, 0x65, 0x20,
  0x41, 0x75, 0x74, 0x68, 0x6F, 0x72, 0x69, 0x74, 0x79, 0x20, 0x2D, 0x20,
  0x47, 0x32
};

static const unsigned char TA1_RSA_N[] = {
  0xBF, 0x71, 0x62, 0x08, 0xF1, 0xFA, 0x59, 0x34, 0xF7, 0x1B, 0xC9, 0x18,
  0xA3, 0xF7, 0x80, 0x49, 0x58, 0xE9, 0x22, 0x83, 0x13, 0xA6, 0xC5, 0x20,
  0x43, 0x01, 0x3B, 0x84, 0xF1, 0xE6, 0x85, 0x49, 0x9F, 0x27, 0xEA, 0xF6,
  0x84, 0x1B, 0x4E, 0xA0, 0xB4, 0xDB, 0x70, 0x98, 0xC7, 0x32, 0x01, 0xB1,
  0x05, 0x3E, 0x07, 0x4E, 0xEE, 0xF4, 0xFA, 0x4F, 0x2F, 0x59, 0x30, 0x22,
  0xE7, 0xAB, 0x19, 0x56, 0x6B, 0xE2, 0x80, 0x07, 0xFC, 0xF3, 0x16, 0x75,
  0x80, 0x39, 0x51, 0x7B, 0xE5, 0xF9, 0x35, 0xB6, 0x74, 0x4E, 0xA9, 0x8D,
  0x82, 0x13, 0xE4, 0xB6, 0x3F, 0xA9, 0x03, 0x83, 0xFA, 0xA2, 0xBE, 0x8A,
  0x15, 0x6A, 0x7F, 0xDE, 0x0B, 0xC3, 0xB6, 0x19, 0x14, 0x05, 0xCA, 0xEA,
  0xC3, 0xA8, 0x04, 0x94, 0x3B, 0x46, 0x7C, 0x32, 0x0D, 0xF3, 0x00, 0x66,
  0x22, 0xC8, 0x8D, 0x69, 0x6D, 0x36, 0x8C, 0x11, 0x18, 0xB7, 0xD3, 0xB2,
  0x1C, 0x60, 0xB4, 0x38, 0xFA, 0x02, 0x8C, 0xCE, 0xD3, 0xDD, 0x46, 0x07,
  0xDE, 0x0A, 0x3E, 0xEB, 0x5D, 0x7C, 0xC8, 0x7C, 0xFB, 0xB0, 0x2B, 0x53,
  0xA4, 0x92, 0x62, 0x69, 0x51, 0x25, 0x05, 0x61, 0x1A, 0x44, 0x81, 0x8C,
  0x2C, 0xA9, 0x43, 0x96, 0x23, 0xDF, 0xAC, 0x3A, 0x81, 0x9A, 0x0E, 0x29,
  0xC5, 0x1C, 0xA9, 0xE9, 0x5D, 0x1E, 0xB6, 0x9E, 0x9E, 0x30, 0x0A, 0x39,
  0xCE, 0xF1, 0x88, 0x80, 0xFB, 0x4B, 0x5D, 0xCC, 0x32, 0xEC, 0x85, 0x62,
  0x43, 0x25, 0x34, 0x02, 0x56, 0x27, 0x01, 0x91, 0xB4, 0x3B, 0x70, 0x2A,
  0x3F, 0x6E, 0xB1, 0xE8, 0x9C, 0x88, 0x01, 0x7D, 0x9F, 0xD4, 0xF9, 0xDB,
  0x53, 0x6D, 0x60, 0x9D, 0xBF, 0x2C, 0xE7, 0x58, 0xAB, 0xB8, 0x5F, 0x46,
  0xFC, 0xCE, 0xC4, 0x1B, 0x03, 0x3C, 0x09, 0xEB, 0x49, 0x31, 0x5C, 0x69,
  0x46, 0xB3, 0xE0, 0x47
};

static const unsigned char TA1_RSA_E[] = {
  0x01, 0x00, 0x01
};

static const br_x509_trust_anchor TAs[2] = {
  {
    { (unsigned char *)TA0_DN, sizeof TA0_DN },
    BR_X509_TA_CA,
    {
      BR_KEYTYPE_RSA,
      { .rsa = {
        (unsigned char *)TA0_RSA_N, sizeof TA0_RSA_N,
        (unsigned char *)TA0_RSA_E, sizeof TA0_RSA_E,
      } }
    }
  },
  {
    { (unsigned char *)TA1_DN, sizeof TA1_DN },
    BR_X509_TA_CA,
    {
      BR_KEYTYPE_RSA,
      { .rsa = {
        (unsigned char *)TA1_RSA_N, sizeof TA1_RSA_N,
        (unsigned char *)TA1_RSA_E, sizeof TA1_RSA_E,
      } }
    }
  }
};

#define TAs_NUM   2

#endif
