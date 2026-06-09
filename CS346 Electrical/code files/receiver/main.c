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
    printf("\n");
    printf("Board started!\n");

    pwm_init();  
    i2s_init();

    // Configure 154 radio
    printf("About to init radio\n");
    nrf_802154_init();
    printf("Done with init\n");
    nrf_802154_channel_set(16);
    nrf_802154_auto_ack_set(false);
    nrf_802154_promiscuous_set(true);
    uint8_t src_pan_id[] = {0xef, 0xef}; 
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
        // move_car(FORWARD);
        /*
        printf("Payload received ");
        for (int i=0; i<3; i++){
            printf("%02X ", payload[i]);
        }
        
        printf("\n");
        */
        nrf_delay_ms(2000);
    
    }
}