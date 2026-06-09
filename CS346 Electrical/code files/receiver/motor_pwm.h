#pragma once
#include "nrf_twi_mngr.h"


// Initialize 4 pwm channels with shared frequency
void pwm_init();

// Initialize motor driver pins
void analog_input_pin_setup();

// Move input direction
void move_car(float direction, float magnitude);

// Helper for moving motor
void move_motor(int motor, float percentage);