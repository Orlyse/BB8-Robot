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
#include "nrfx_i2s.h"

#include "audio_data1.h"

/*
Pin definitions:
DIN = EDGE_P11
BLCK = EDGE_P13
LRC = EDGE_P14
*/

void i2s_handler(nrfx_i2s_buffers_t const *p_buffers, uint32_t status);
void i2s_init();
void play_sound();