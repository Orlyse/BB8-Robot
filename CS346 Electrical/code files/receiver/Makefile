PROJECT_NAME = $(shell basename "$(realpath ./)")

# Configurations
NRF_IC = nrf52833
SDK_VERSION = 16
SOFTDEVICE_MODEL = blank

# Source and header files
APP_HEADER_PATHS += .
APP_SOURCE_PATHS += .
APP_SOURCES = $(notdir $(wildcard ./*.c))

# Path to base of nRF52x-base repo
NRF_BASE_DIR = /root/nu-microbit-base/software/nrf52x-base/

# Include board Makefile (if any)
include /root/nu-microbit-base/software/boards/microbit_v2/Board.mk

# Include main Makefile
include $(NRF_BASE_DIR)/make/AppMakefile.mk


# PROJECT_NAME = $(shell basename "$(realpath ./)")

# # Configurations
# NRF_IC = nrf52833
# SDK_VERSION = 16
# SOFTDEVICE_MODEL = blank

# # Source and header files
# APP_HEADER_PATHS += .
# APP_SOURCE_PATHS += .
# APP_SOURCES = $(notdir $(wildcard ./*.c))
# <<<<<<< HEAD
# APP_SOURCES += nrfx_i2s.c
# CFLAGS += -DNRFX_I2S_ENABLED=1
# =======
# >>>>>>> 7dff2b4b2e9e08353ac4b814f074981e2838a713

# # Path to base of nRF52x-base repo
# NRF_BASE_DIR = /home/zhaohan/nu-microbit-base/software/nrf52x-base/

# # Include board Makefile (if any)
# include /home/zhaohan/nu-microbit-base/software/boards/microbit_v2/Board.mk
	
# # Include main Makefile
# include $(NRF_BASE_DIR)/make/AppMakefile.mk