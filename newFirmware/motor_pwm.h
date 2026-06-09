#pragma once
#include "nrf_twi_mngr.h"

typedef enum {
    FORWARD,
    BACK,
    LEFT, 
    RIGHT,
    STOP
}car_dir;

// Initialize 4 pwm channels with shared frequency
void pwm_init();

// Initialize motor driver pins
void analog_input_pin_setup();

// Move input direction
void move_car(car_dir direction);