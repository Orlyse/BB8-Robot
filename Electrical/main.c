#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_timer.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrfx_pwm.h"
#include "nrf_twi_mngr.h"

#include "microbit_v2.h"
#include "lsm303agr.h"
#include "MotorPWM.h"
#include "SinePWM.h"

APP_TIMER_DEF(my_timer_1);


static void read_vals(void* _unused) {
  // lsm303agr_measurement_t accel = lsm303agr_read_accelerometer();
  // lsm303agr_measurement_t magne = lsm303agr_read_magnetometer();

  // printf("accel: %f, %f, %f\n", accel.x_axis, accel.y_axis, accel.z_axis);
  // printf("magne: %f, %f, %f\n", magne.x_axis, magne.y_axis, magne.z_axis);

  float current_phi = read_phi();

  printf("phi (deg): %f\n", current_phi);
  set_motor(-current_phi);

  int new_freq = (int)fabs(-current_phi * 4.0);

  // Only update the audio buffer if the frequency has changed significantly
  static int last_freq = 0;
  if (abs(new_freq - last_freq) > 5) { // 5 Hz deadband to prevent constant restarts
    play_note(new_freq);
    last_freq = new_freq;
  }
}

NRF_TWI_MNGR_DEF(twi_mngr_instance, 1, 0);

int main(void) {
  printf("Board started!\n");

  // Initialize I2C peripheral and driver
  nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;
  // WARNING!!
  // These are NOT the correct pins for external I2C communication.
  // If you are using QWIIC or other external I2C devices, the are
  // connected to EDGE_P19 (a.k.a. I2C_QWIIC_SCL) and EDGE_P20 (a.k.a. I2C_QWIIC_SDA)
  i2c_config.scl = I2C_INTERNAL_SCL;
  i2c_config.sda = I2C_INTERNAL_SDA;
  i2c_config.frequency = NRF_DRV_TWI_FREQ_100K;
  i2c_config.interrupt_priority = 0;
  nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config);

  // Initialize the LSM303AGR accelerometer/magnetometer sensor
  lsm303agr_init(&twi_mngr_instance);

  pwm_init();
  sine_pwm_init();
  compute_sine_wave(500-1);

  app_timer_init();
  app_timer_create(&my_timer_1, APP_TIMER_MODE_REPEATED, read_vals);
  app_timer_start(my_timer_1, 1600, NULL);
  //TODO: implement me!

  // Loop forever
  while (1) {
    // Don't put any code in here. Instead put periodic code in a callback using a timer.
    nrf_delay_ms(1000);
    // printf("Temperature: %f\n", lsm303agr_read_temperature());
  }
}

// int main(void) {
//   printf("Board started!\n");

//   // initialize PWM
//   pwm_init();

//   // Continuously vary the servo angle
//   // Once per second it should update by 45 degrees
//   // It should range from 0 to 180 degrees
//   // TODO
//   int angles[5] = {0, 45, 80, 45, 30};
//   while (1) {
//     for (int i = 0; i < 5; i ++) {
//       set_motor(angles[i]);
//       nrf_delay_ms(1000);
//     }
//   }

// }


// nrf_pwm_values_common_t sequence_data[1] = {0};

// nrf_pwm_sequence_t pwm_sequence = {
//   .values.p_common = sequence_data,
//   .length = 1,
//   .repeats = 0,
//   .end_delay = 0,
// };

// static void pwm_init(void) {
//   // Initialize the PWM
//   // This time EDGE_P1 should be your output pin
//   // Set the clock to 500 kHz, count mode to Up, and load mode to Common
//   // This time the COUNTERTOP value DOES matter. It should be configured for a 20 ms period.
//   // TODO
//   nrfx_pwm_config_t config = {
//     .output_pins = {EDGE_P1, NRFX_PWM_PIN_NOT_USED, NRFX_PWM_PIN_NOT_USED, NRFX_PWM_PIN_NOT_USED},
//     .base_clock  = NRF_PWM_CLK_500kHz,
//     .count_mode  = NRF_PWM_MODE_UP,
//     .top_value   = 500,
//     .load_mode   = NRF_PWM_LOAD_COMMON,
//     .step_mode   = NRF_PWM_STEP_AUTO,
//   };

//   nrfx_pwm_init(&PWM_INST, &config, NULL);
// }

// static void set_motor(float percentage) {
//   // Limit to 0 to 180 degrees
//   if (percentage > 100) {
//     percentage = 100;
//   }

//   if (percentage < 0) {
//     percentage = 0;
//   }

//   // Calculate duty cycle based on duration (knowing the period of 20 ms)
//   // TODO
//   float duty_cycle = percentage / 100.0; // Percentage of being depends on time_ms

//   // Modify the sequence data to match the duty cycle
//   // HOWEVER, to control a servo, the PWM output MUST be left-aligned
//   // To do so, the Most Significant bit of the 16-bit value must be set to 1
//   // TODO
//   printf("Setting duty cycle to %f\n", duty_cycle);
//   sequence_data[0] = (int) (NRF_PWM0->COUNTERTOP * duty_cycle) | 0x8000; // Write a one to MSB

//   printf("Compare set to %d\n", (int)(NRF_PWM0->COUNTERTOP * duty_cycle));

//   // Start playback of the samples and loop indefinitely
//   // TODO
//   nrfx_pwm_simple_playback(&PWM_INST, &pwm_sequence, 1, NRFX_PWM_FLAG_LOOP);
// }

