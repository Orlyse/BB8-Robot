#pragma once

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"

// Pin configurations
#include "microbit_v2.h"

#include "nrf_802154.h"


#define PSDU_MAX_SIZE (127) // Max length of a packet
#define FCS_LENGTH (2) // Length of the Frame Control Sequence


// callback fn when tx starts
void nrf_802154_tx_started(const uint8_t* p_frame);

// callback fn when tx fails
void nrf_802154_transmit_failed(const uint8_t* p_frame, nrf_802154_tx_error_t error);

// callback fn for successful tx
void nrf_802154_transmitted_raw(const uint8_t* p_frame, uint8_t* p_ack, int8_t power, uint8_t lqi);

void pack_packet(uint8_t* src_extended_addr, uint8_t* src_pan_id, uint8_t* dst_extended_addr, uint8_t* pkt, uint8_t* payload);