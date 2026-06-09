#pragma once

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"

// Pin configurations
#include "microbit_v2.h"

#include "nrf_802154.h"

#define PSDU_MAX_SIZE (127) // Max length of a packet
#define FCS_LENGTH (2) // Length of the Frame Control Sequence

void nrf_802154_received_raw(uint8_t* p_data, int8_t power, uint8_t lqi);