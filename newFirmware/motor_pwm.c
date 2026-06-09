// PWM Servo App
//
// Use PWM to control a servo

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

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

    pwmConfig_0.base_clock = NRF_PWM_CLK_1MHz; // (NRF_PWM_CLK_1MHz, 2, 4, 8, 16Mhz available)
    pwmConfig_0.count_mode = NRF_PWM_MODE_UP;
    pwmConfig_0.top_value = 1000;   // counter top
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
void move_car(car_dir direction){
    float duty_cyle = 0.5;
    uint16_t top_v = (uint16_t) (1000*duty_cyle);

    switch (direction){
        case FORWARD:
            // Red Hbridge (1, 2)
            // CCW
            nrf_gpio_pin_set(EDGE_P5);
            nrf_gpio_pin_clear(EDGE_P6);
            sequence_data0[0].channel_0 = top_v | (1 << 15);

            // CW
            nrf_gpio_pin_set(EDGE_P7);
            nrf_gpio_pin_clear(EDGE_P8);
            sequence_data0[0].channel_1 = top_v | (1 << 15);

            // Black Hbridge (3, 4)

            // CW
            nrf_gpio_pin_set(EDGE_P10);
            sequence_data0[0].channel_2 = top_v | (1 << 15);

            // CCW
            nrf_gpio_pin_clear(EDGE_P12);
            sequence_data0[0].channel_3 = top_v | (1 << 15);

            // Send PWM data
            nrfx_pwm_simple_playback(&PWM_INST0, &pwm_sequence0, 1, NRFX_PWM_FLAG_LOOP);

            break;
        
        case BACK:
            // Red Hbridge (1, 2)
            // CCW
            nrf_gpio_pin_set(EDGE_P6);
            nrf_gpio_pin_clear(EDGE_P5);
            sequence_data0[0].channel_0 = top_v | (1 << 15);

            // CW
            nrf_gpio_pin_set(EDGE_P8);
            nrf_gpio_pin_clear(EDGE_P7);
            sequence_data0[0].channel_1 = top_v | (1 << 15);

            // Black Hbridge (3, 4)

            // CW
            nrf_gpio_pin_clear(EDGE_P10);
            sequence_data0[0].channel_2 = top_v | (1 << 15);

            // CCW
            nrf_gpio_pin_set(EDGE_P12);
            sequence_data0[0].channel_3 = top_v | (1 << 15);

            // Send PWM data
            nrfx_pwm_simple_playback(&PWM_INST0, &pwm_sequence0, 1, NRFX_PWM_FLAG_LOOP);

            break;       
        
        case LEFT:   
            // Red Hbridge (1, 2)
            // CCW
            nrf_gpio_pin_set(EDGE_P5);
            nrf_gpio_pin_clear(EDGE_P6);
            sequence_data0[0].channel_0 = top_v | (1 << 15);

            // CW
            nrf_gpio_pin_set(EDGE_P8);
            nrf_gpio_pin_clear(EDGE_P7);
            sequence_data0[0].channel_1 = top_v | (1 << 15);

            // Black Hbridge (3, 4)

            // CW
            nrf_gpio_pin_set(EDGE_P10);
            sequence_data0[0].channel_2 = top_v | (1 << 15);

            // CCW
            nrf_gpio_pin_set(EDGE_P12);
            sequence_data0[0].channel_3 = top_v | (1 << 15);

            // Send PWM data
            nrfx_pwm_simple_playback(&PWM_INST0, &pwm_sequence0, 1, NRFX_PWM_FLAG_LOOP);

            break; 

        case RIGHT:
            break;
    }
}

/*
Base clock = 1MHz
Desired frequency = 1kHz
Counter_top = 1000
*/