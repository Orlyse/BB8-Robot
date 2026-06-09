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
#include "motor_pwm.h"
#include "wireless_receive.h"
#include "nrf_802154.h"

int main(void) {
    printf("Board started!\n");

    pwm_init();  
    analog_input_pin_setup();

    // Configure 154 radio
    printf("About to init radio\n");
    nrf_802154_init();
    printf("Done with init\n");
    nrf_802154_channel_set(11);
    nrf_802154_auto_ack_set(false);
    nrf_802154_promiscuous_set(true);
    uint8_t src_pan_id[] = {0xcd, 0xab}; 
    nrf_802154_pan_id_set(src_pan_id);
    printf("Radio configured!\n");

    // Addresses (source and destination)
    uint8_t extended_addr[] = {0x50, 0xbe, 0xca, 0xc3, 0x3c, 0x36, 0xce, 0xf4};
    nrf_802154_extended_address_set(extended_addr);

    if (nrf_802154_receive()) {
        printf("Entered receive mode\n");
    } else {
        printf("Could not enter receive mode\n");
    }

    float direction = 0.0;
    // Loop forever
    while (1) {
        for (int i=0; i<3; i++){
            printf("%02X ", payload[i]);
        }
        printf("\n");

        move_car(direction, 0.8);

        // move_motor(1, 0.5);
        // move_motor(2, 0.5);
        // move_motor(3, 0.5);
        // move_motor(4, 0.5);

        update_motors();

        nrf_delay_ms(1000);

        direction += 45.0;
        printf("Direction %f\n", direction);
        if (direction == 360) {
            direction = 0.0;
        }
    }
}