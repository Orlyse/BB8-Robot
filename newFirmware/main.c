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

    // Loop forever
    while (1) {
        printf("Was here\n");
        move_car(LEFT);
        nrf_delay_ms(1000);
    }
}

// #include <stdlib.h>
// #include <math.h>
// #include <stdbool.h>
// #include <stdint.h>
// #include <stdio.h>

// #include "app_timer.h"
// #include "nrf.h"
// #include "nrf_delay.h"
// #include "nrfx_pwm.h"
// #include "nrf_twi_mngr.h"

// #include "microbit_v2.h"
// #include "motor_pwm.h"
// #include "wireless_receive.h"
// #include "nrf_802154.h"

// APP_TIMER_DEF(my_timer_1);

// static void read_vals(void* _unused) {
//     nrf_802154_received_raw(uint8_t* p_data, int8_t power, uint8_t lqi)
// }

// int main(void) {
//     printf("Board started!\n");

//     pwm_init();  
//     analog_input_pin_setup();

//     // Configure 154 radio
//     printf("About to init radio\n");
//     nrf_802154_init();
//     printf("Done with init\n");
//     nrf_802154_channel_set(11);
//     nrf_802154_auto_ack_set(false);
//     nrf_802154_promiscuous_set(true);
//     uint8_t src_pan_id[] = {0xcd, 0xab}; 
//     nrf_802154_pan_id_set(src_pan_id);
//     printf("Radio configured!\n");

//     app_timer_init();
//     app_timer_create(&my_timer_1, APP_TIMER_MODE_REPEATED, read_vals);
//     app_timer_start(my_timer_1, 1600, NULL);

//     // Addresses (source and destination)
//     uint8_t extended_addr[] = {0x50, 0xbe, 0xca, 0xc3, 0x3c, 0x36, 0xce, 0xf4};
//     nrf_802154_extended_address_set(extended_addr);

//     if (nrf_802154_receive()) {
//         printf("Entered receive mode\n");
//     } else {
//         printf("Could not enter receive mode\n");
//     }

//     // Loop forever
//     while (1) {
//         move_car(LEFT);
//         nrf_delay_ms(1000);
//     }
// }