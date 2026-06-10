#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "microbit_v2.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_twi_mngr.h"

#include "app_timer.h"
#include "joystick.h"
#include "wireless_send.h"

// Global variables
NRF_TWI_MNGR_DEF(twi_mngr_instance, 1, 0);
//  APP_TIMER_DEF(joystick_timer);

int main(void){
    printf("Board started!\n");

    nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;

    i2c_config.scl = I2C_QWIIC_SCL;
    i2c_config.sda = I2C_QWIIC_SDA;
    i2c_config.frequency = NRF_DRV_TWI_FREQ_100K;
    i2c_config.interrupt_priority = 0;
    nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config);

    joystick_init(&twi_mngr_instance);

    // Configure 154 radio
    printf("About to init radio\n");
    nrf_802154_init();
    printf("Done with init\n");
    nrf_802154_channel_set(16);
    uint8_t src_pan_id[] = {0xbe, 0xef}; 
    nrf_802154_pan_id_set(src_pan_id);
    printf("Radio configured!\n");

    // Addresses (source and destination)
    uint8_t src_extended_addr[] = {0xdc, 0xa9, 0x35, 0x7b, 0x73, 0x36, 0xce, 0xf4};
    nrf_802154_extended_address_set(src_extended_addr);
    uint8_t dst_extended_addr[] = {0x50, 0xbe, 0xca, 0xc3, 0x3c, 0x36, 0xce, 0xf4};

    uint8_t pkt[PSDU_MAX_SIZE];
    
    float degree, magnitude;

    while(1){   

        cartesian_to_polar(get_dirx(), get_diry(), &degree, &magnitude);
        uint16_t degree_big = (uint16_t)degree;
        uint8_t degree_first = (degree_big >> 8) & 0xFF;
        uint8_t degree_second = (degree_big & (0xFF));
        uint8_t payload[3] = {degree_first, degree_second, (uint8_t)magnitude};
        
        printf("degree = %2f magnitude = %2f\n", degree, magnitude);
        pack_packet(src_extended_addr, src_pan_id, dst_extended_addr, pkt, payload);
        
        // printf("Packet: [ ");
        // for (int i=0; i<27; i++) {
        //  printf("%02X ", pkt[i]);
        // }
        // printf("]\n");

        if (!nrf_802154_transmit_raw(pkt, true)) {
        printf("Failure to send radio packet!\n");
        } else {
        // printf("Sent a radio packet!\n");
        }
        
        //nrf_delay_ms(1000);
    }
}