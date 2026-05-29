#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_delay.h"
#include "nrfx_pwm.h"

#include "microbit_v2.h"
#include "MotorPWM.h"

static const nrfx_pwm_t PWM_INST = NRFX_PWM_INSTANCE(0);

nrf_pwm_values_common_t sequence_data[1] = {0};

nrf_pwm_sequence_t pwm_sequence = {
  .values.p_common = sequence_data,
  .length = 1,
  .repeats = 0,
  .end_delay = 0,
};

void pwm_init(void) {
  // Initialize the PWM
  // This time EDGE_P0 should be your output pin
  // Set the clock to 500 kHz, count mode to Up, and load mode to Common
  // This time the COUNTERTOP value DOES matter. It should be configured for a 20 ms period.
  // TODO
  nrfx_pwm_config_t config = {
    .output_pins = {EDGE_P1, NRFX_PWM_PIN_NOT_USED, NRFX_PWM_PIN_NOT_USED, NRFX_PWM_PIN_NOT_USED},
    .base_clock  = NRF_PWM_CLK_500kHz,
    .count_mode  = NRF_PWM_MODE_UP,
    .top_value   = 500,
    .load_mode   = NRF_PWM_LOAD_COMMON,
    .step_mode   = NRF_PWM_STEP_AUTO,
  };

  nrfx_pwm_init(&PWM_INST, &config, NULL);
}

void set_motor(float percentage) {
  // Limit to 0 to 180 degrees
  if (percentage > 100) {
    percentage = 100;
  }

  if (percentage < 0) {
    percentage = 0;
  }

  // Calculate duty cycle based on duration (knowing the period of 20 ms)
  // TODO
  float duty_cycle = percentage / 100.0; // Percentage of being depends on time_ms

  // Modify the sequence data to match the duty cycle
  // HOWEVER, to control a servo, the PWM output MUST be left-aligned
  // To do so, the Most Significant bit of the 16-bit value must be set to 1
  // TODO
  sequence_data[0] = (int) (NRF_PWM0->COUNTERTOP * duty_cycle) | 0x8000; // Write a one to MSB

  // Start playback of the samples and loop indefinitely
  // TODO
  nrfx_pwm_simple_playback(&PWM_INST, &pwm_sequence, 1, NRFX_PWM_FLAG_LOOP);
}

// int main(void) {
//   printf("Board started!\n");

//   // initialize PWM
//   pwm_init();

//   // Continuously vary the servo angle
//   // Once per second it should update by 45 degrees
//   // It should range from 0 to 180 degrees
//   // TODO
//   int angles[5] = {0, 15, 30, 15, 7};
//   while (1) {
//     for (int i = 0; i < 5; i ++) {
//       set_motor(angles[i]);
//       nrf_delay_ms(1000);
//     }
//   }

// }