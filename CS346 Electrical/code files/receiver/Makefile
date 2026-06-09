PROJECT_NAME = $(shell basename "$(realpath ./)")

# Configurations
NRF_IC = nrf52833
SDK_VERSION = 16
SOFTDEVICE_MODEL = blank

# Source and header files
APP_HEADER_PATHS += .
APP_HEADER_PATHS += /root/nu-microbit-base/software/nrf52x-base/sdk/nrf5_sdk_16.0.0/modules/nrfx/drivers/include
APP_SOURCE_PATHS += .
APP_SOURCE_PATHS += /root/nu-microbit-base/software/nrf52x-base/sdk/nrf5_sdk_16.0.0/modules/nrfx/drivers/src
APP_SOURCES = $(notdir $(wildcard ./*.c))
APP_SOURCES += nrfx_i2s.c
CFLAGS += -DNRFX_I2S_ENABLED=1

# Path to base of nRF52x-base repo
NRF_BASE_DIR = /root/nu-microbit-base/software/nrf52x-base/

# Include board Makefile (if any)
include /root/nu-microbit-base/software/boards/microbit_v2/Board.mk

# Include main Makefile
include $(NRF_BASE_DIR)/make/AppMakefile.mk