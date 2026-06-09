#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_delay.h"
#include "nrfx_pwm.h"

#include "microbit_v2.h"

#include "wireless_receive.h"

volatile int8_t payload[3];

// callback fn for successful rx
void nrf_802154_received_raw(uint8_t* p_data, int8_t power, uint8_t lqi) {
    /*
    printf("Packet: [ ");
    for (int i=0; i<p_data[0]-2; i++) {
    printf("%02X ", p_data[i]);
    }
    printf("]\n\n");
    */
    payload[0] = p_data[24];
    payload[1] = p_data[25];
    payload[2] = p_data[26];

    // nrf_delay_ms(500);
    nrf_802154_buffer_free_raw(p_data);
}