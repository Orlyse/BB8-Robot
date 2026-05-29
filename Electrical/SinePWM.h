#pragma once

#include "nrf_twi_mngr.h"

void gpio_init(void);

void sine_pwm_init(void);

void compute_sine_wave(uint16_t max_value);

void play_note(uint16_t frequency);