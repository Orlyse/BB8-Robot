#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_delay.h"
#include "nrfx_pwm.h"

#include "microbit_v2.h"

// callback fn for successful rx
void nrf_802154_received_raw(uint8_t* p_data, int8_t power, uint8_t lqi) {
    nrf_gpio_pin_toggle(20);
    printf("Packet: [ ");
    for (int i=0; i<p_data[0]-2; i++) {
    printf("%02X ", p_data[i]);
    }
    printf("]\n");
    
    payload = {p_data[24], p_data[25], p_data[26]};

    nrf_delay(1000);
    nrf_802154_buffer_free_raw(p_data);
}