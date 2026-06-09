#pragma once

#include "nrf_twi_mngr.h"
#include "nrf.h"

// Create clock
void clock_init(void);


// Start clock
void clock_start(void);

// Stop clock
void clock_stop(void);

// Generate LED values for specific angle and color for a certain number of LEDs
void generate_array(float angle, int red, int green, int blue, int fill_red, int fill_green, int fill_blue, int led_count);

