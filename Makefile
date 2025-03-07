PROJECT_NAME := nina-fw

# Passthrough Board Port
M4_PORT := /dev/cu.usbmodem14121301
UPLOAD_BAUD = 115200
# ESP32 USB Serial Port
ESP_PORT := /dev/cu.usbserial-AH03B302

# Directories and Files
BOOT_VOLUME := /Volumes/FEATHERBOOT/.
CIRCUITPYTHON_UF2 := circuitpython.uf2

EXTRA_COMPONENT_DIRS := $(PWD)/arduino

CPPFLAGS += -DARDUINO

ifeq ($(RELEASE),1)
CFLAGS += -DNDEBUG -DCONFIG_FREERTOS_ASSERT_DISABLE -Os -DLOG_LOCAL_LEVEL=0
CPPFLAGS += -DNDEBUG -Os
endif

NINA_DEBUG ?= 0
ifeq ($(NINA_DEBUG),1)
CFLAGS += -DNINA_DEBUG
CPPFLAGS += -DNINA_DEBUG
endif

ifeq ($(UNO_WIFI_REV2),1)
CFLAGS += -DUNO_WIFI_REV2
CPPFLAGS += -DUNO_WIFI_REV2
endif

ifeq ($(NANO_RP2040_CONNECT),1)
CFLAGS += -DNANO_RP2040_CONNECT
CPPFLAGS += -DNANO_RP2040_CONNECT
endif

include $(IDF_PATH)/make/project.mk

load-passthrough:
	cp passthrough.UF2  $(BOOT_VOLUME)

load-nina:
	esptool.py --port $(M4_PORT) --before no_reset --baud $(UPLOAD_BAUD) write_flash 0 $(wildcard NINA_W102-*.bin)

load-circuitpython:
	cp $(CIRCUITPYTHON_UF2) $(BOOT_VOLUME)

serial:
	miniterm.py $(ESP_PORT) $(UPLOAD_BAUD)

firmware: all
	python combine.py

.PHONY: firmware

.PHONY: load-passthrough

.PHONY: load-nina

.PHONY: load-circuitpython

.PHONY: serial
