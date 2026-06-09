#pragma once
#include "nrf_twi_mngr.h"

static const uint8_t sparkfun_qwiic_joystick_addr = 0x20;

// Register definitions for Joystick
typedef enum {
    JOYSTICK_ID = 0x00,
    JOYSTICK_VERSION1, // 0x01
    JOYSTICK_VERSION2, // 0x02
    JOYSTICK_X_MSB, // 0x03
    JOYSTICK_X_LSB, // 0x04
    JOYSTICK_Y_MSB, // 0x05
    JOYSTICK_Y_LSB, // 0x06
    JOYSTICK_BUTTON, // 0x07
    JOYSTICK_STATUS, // 0x08 //1 - button clicked
    JOYSTICK_I2C_LOCK, // 0x09
    JOYSTICK_CHANGE_ADDRESS, 
} joystick_reg_t;

static uint8_t i2c_reg_read(uint8_t i2c_addr, uint8_t reg_addr);
void joystick_init(const nrf_twi_mngr_t* i2c);
int get_dirx();
int get_diry();
void cartesian_to_polar(int x, int y, float* degrees, float* magnitude);