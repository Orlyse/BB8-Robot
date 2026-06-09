// PWM Servo App
//
// Use PWM to control a servo

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "nrf.h"
#include "nrf_delay.h"
#include "nrfx_pwm.h"

#include "microbit_v2.h"
#include "motor_pwm.h"


// PWM configuration
static const nrfx_pwm_t PWM_INST0 = NRFX_PWM_INSTANCE(0);

// Holds duty cycle values to trigger PWM toggle
nrf_pwm_values_individual_t sequence_data0[1] = {0};

// Sequence structure for configuring DMA
nrf_pwm_sequence_t pwm_sequence0 = {
  .values.p_individual = sequence_data0,
  .length = 4,
  .repeats = 0,
  .end_delay = 0,
};

nrfx_pwm_config_t pwmConfig_0;

void analog_input_pin_setup(){
    // Red H-bridge
    // Motor 1
    nrf_gpio_pin_dir_set(EDGE_P5, NRF_GPIO_PIN_DIR_OUTPUT); // AIN1
    nrf_gpio_pin_dir_set(EDGE_P6, NRF_GPIO_PIN_DIR_OUTPUT); // AIN2

    //Motor 2
    nrf_gpio_pin_dir_set(EDGE_P7, NRF_GPIO_PIN_DIR_OUTPUT); // BIN1
    nrf_gpio_pin_dir_set(EDGE_P8, NRF_GPIO_PIN_DIR_OUTPUT); // BIN2

    
    // first clear
    nrf_gpio_pin_clear(EDGE_P5);
    nrf_gpio_pin_clear(EDGE_P6);

    nrf_gpio_pin_clear(EDGE_P7);
    nrf_gpio_pin_clear(EDGE_P8);

    // Black H-bridge9

    // Motor 3
    nrf_gpio_pin_dir_set(EDGE_P10, NRF_GPIO_PIN_DIR_OUTPUT); // IN2

    // Motor 4
    nrf_gpio_pin_dir_set(EDGE_P12, NRF_GPIO_PIN_DIR_OUTPUT); // IN4

    // first clear
    nrf_gpio_pin_clear(EDGE_P10);
    nrf_gpio_pin_clear(EDGE_P12);
   
}

void pwm_init(void) {
    pwmConfig_0.output_pins[0] = EDGE_P0;   // Motor 1
    pwmConfig_0.output_pins[1] = EDGE_P1;   // Motor 2
    pwmConfig_0.output_pins[2] = EDGE_P2;
    pwmConfig_0.output_pins[3] = EDGE_P3;

    pwmConfig_0.base_clock = NRF_PWM_CLK_500kHz; // (NRF_PWM_CLK_500kHz, 2, 4, 8, 16Mhz available)
    pwmConfig_0.count_mode = NRF_PWM_MODE_UP;
    pwmConfig_0.top_value = 500;   // counter top
    pwmConfig_0.load_mode = NRF_PWM_LOAD_INDIVIDUAL;  // each pin controlled individually
    pwmConfig_0.step_mode = 0;
    nrfx_pwm_init(&PWM_INST0, &pwmConfig_0, NULL);
}

/*
0, 1 controlled by red bridge 

Front left = EDGE_P0
Front right = EDGE_P1
Back left = EDGE_P2
Back right = EDGE_P3

Forward: 0->CW 1->CCW 2->CW  3->CCW
Backward: CCW   CW      CCW     CW
Left: CCW   CW      CW      CCW
Right: CW   CCW     CCW     CW
*/ 
void move_car(float direction, float magnitude){
    float theta = direction * (M_PI / 180.0);
    move_motor(1, magnitude * cosf(theta));
    move_motor(2, magnitude * sinf(theta));
    move_motor(3, -magnitude * cosf(theta));
    move_motor(4, -magnitude * sinf(theta));
}

void move_motor(int motor, float percentage) {
    uint16_t top_v = 500 * percentage;
    if (motor == 1) {
        sequence_data0[0].channel_0 = top_v | (1 << 15);
        if (percentage >= 0.0) {
            nrf_gpio_pin_set(EDGE_P5);
            nrf_gpio_pin_clear(EDGE_P6);
        } else if (percentage < 0.0) {
            nrf_gpio_pin_set(EDGE_P6);
            nrf_gpio_pin_clear(EDGE_P5);
        }
        
    } else if (motor == 2) {
        sequence_data0[0].channel_1 = top_v | (1 << 15);
        if (percentage >= 0.0) {
            nrf_gpio_pin_set(EDGE_P7);
            nrf_gpio_pin_clear(EDGE_P8);
        } else if (percentage < 0.0) {
            nrf_gpio_pin_set(EDGE_P8);
            nrf_gpio_pin_clear(EDGE_P7);
        }
        
    } else if (motor == 3) {
        sequence_data0[0].channel_2 = top_v | (1 << 15);
        if (percentage >= 0.0) {
            nrf_gpio_pin_set(EDGE_P10);
        } else if (percentage < 0.0) {
            nrf_gpio_pin_clear(EDGE_P10);
        }
        
    } else if (motor == 4) {
        sequence_data0[0].channel_3 = top_v | (1 << 15);
        if (percentage >= 0.0) {
            nrf_gpio_pin_set(EDGE_P12);
        } else if (percentage < 0.0) {
            nrf_gpio_pin_clear(EDGE_P12);
        }
    } 
}

/*
Base clock = 500kHz
Desired frequency = 1kHz
Counter_top = 500
*/