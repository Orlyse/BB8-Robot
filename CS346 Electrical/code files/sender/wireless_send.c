// Radio 15.4 send app
//
// Sends wireless packets via the 802.15.4 radio

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "wireless_send.h"

// Pin configurations
#include "microbit_v2.h"

#include "nrf_802154.h"

// callback fn when tx starts
void nrf_802154_tx_started(const uint8_t* p_frame) {
	printf("tx started\n");
}

// callback fn when tx fails
void nrf_802154_transmit_failed(const uint8_t* p_frame, nrf_802154_tx_error_t error) {
	printf("tx failed error %u!\n", error);
}

// callback fn for successful tx
void nrf_802154_transmitted_raw(const uint8_t* p_frame, uint8_t* p_ack, int8_t power, uint8_t lqi) {
	printf("frame was transmitted!\n");
}

void pack_packet(uint8_t* src_extended_addr, uint8_t* src_pan_id, uint8_t* dst_extended_addr, uint8_t* pkt, uint8_t* payload){
    pkt[0] = 26 + FCS_LENGTH; /* Length for nrf_transmit (length of pkt + FCS) */
    pkt[1] = 0x01; /* Frame Control Field */
    pkt[2] = 0xcc; /* Frame Control Field */
    pkt[3] = 0x00; /* Sequence number */
    pkt[4] = 0xff; /* Destination PAN ID 0xffff */
    pkt[5] = 0xff; /* Destination PAN ID */
    memcpy(&pkt[6], dst_extended_addr, 8); /* Destination extended address */
    memcpy(&pkt[14], src_pan_id, 2); /* Source PAN ID */
    memcpy(&pkt[16], src_extended_addr, 8);/* Source extended address */ 
    pkt[24] = payload[0]; /* Payload */
    pkt[25] = payload[1]; /* */
    pkt[26] = payload[2]; /* */
}