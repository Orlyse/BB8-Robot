#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "nrf.h"
#include "nrf_delay.h"
#include "nrfx_pwm.h"

#include "microbit_v2.h"
#include "ledRing.h"


APP_TIMER_DEF(led_timer);

// 48 pixels, each pixel needs 24 bits
// each bit takes 5 bits
// each pixel takes 120 bits, or 15 bytes
// 48 pixels * 15 bytes = 720 bytes for each frame
// 25 bytes of 0s at the end to latch


// breakdown:

// outer: 60 * 15 bytes = 900 bytes
// middle: 48 * 15 = 720 bytes
// middle inner: 40 * 15 = 600 bytes
// inner: 32 * 15 = 480 bytes
// buffer: 25 bytes
// sum: 900 + 720 + 600 + 480 + 25 = 1500 + 1200 + 25 = 2725

uint8_t frame[2726] = {0};

uint8_t zero = 0b10000000;
uint8_t one = 0b11100000;


// Create clock
void clock_init(void) {
    NRF_SPIM0->PSEL.MOSI = EDGE_P15;
    NRF_SPIM0->PSEL.MISO = 0xFFFFFFFF;
    NRF_SPIM0->PSEL.SCK = 28;
    NRF_SPIM0->FREQUENCY = 0x40000000;

    NRF_SPIM0->CONFIG = 0;
    NRF_SPIM0->ENABLE = 7;
}


// Start clock
void clock_start(void) {
    NRF_SPIM0->TASKS_START = 1;
}

// Stop clock
void clock_stop(void) {
    NRF_SPIM0->TASKS_SUSPEND = 1;
}

// Generate LED values for specific angle and color for a certain number of LEDs
void generate_array(float angle, int red, int green, int blue, int fill_red, int fill_green, int fill_blue, int led_count_out, int led_count_mid, int led_count_mid_in, int led_count_in) {
    uint16_t pattern_byte_index = 0;
    uint8_t pattern_bit_index = 0;
    uint8_t pattern[15] = {0};


    color_filler(fill_green, pattern, &pattern_byte_index, &pattern_bit_index);
    color_filler(fill_red, pattern, &pattern_byte_index, &pattern_bit_index);
    color_filler(fill_blue, pattern, &pattern_byte_index, &pattern_bit_index);


    int total_leds = led_count_out + led_count_mid + led_count_mid_in + led_count_in;

    for (int i = 0; i < total_leds; i++) {
        memcpy(&frame[1 + i*15], pattern, 15);
    }
    
    float percentage = angle / 360.0;

    int target_leds[4];

    target_leds[0] = (int)(percentage * led_count_out);
    target_leds[1] = led_count_out + (int)(percentage * led_count_mid);
    target_leds[2] = led_count_out + led_count_mid + (int)(percentage * led_count_mid_in);
    target_leds[3] = led_count_out + led_count_mid + led_count_mid_in + (int)(percentage * led_count_in);

    for (int i = 0; i < 4; i++) {
        uint16_t byte_index = 1 + target_leds[i] * 15;
        uint8_t bit_index = 0;

        memset(&frame[byte_index], 0, 15);

        color_filler(green, frame, &byte_index, &bit_index);
        color_filler(red, frame, &byte_index, &bit_index);
        color_filler(blue, frame, &byte_index, &bit_index);
    }

    int mid_relative_index = (int)(percentage * led_count_mid);

    int left_relative = (mid_relative_index - 1 + led_count_mid) % led_count_mid;
    int right_relative = (mid_relative_index + 1) % led_count_mid;

    int left_absolute = led_count_out + left_relative;
    int right_absolute = led_count_out + right_relative;

    // left of arrow

    uint16_t byte_index_left = 1 + left_absolute * 15;
    uint8_t bit_index_left = 0;

    memset(&frame[byte_index_left], 0, 15);

    color_filler(green, frame, &byte_index_left, &bit_index_left);
    color_filler(red, frame, &byte_index_left, &bit_index_left);
    color_filler(blue, frame, &byte_index_left, &bit_index_left);

    // right of arrow

    uint16_t byte_index_right = 1 + right_absolute * 15;
    uint8_t bit_index_right = 0;

    memset(&frame[byte_index_right], 0, 15);

    color_filler(green, frame, &byte_index_right, &bit_index_right);
    color_filler(red, frame, &byte_index_right, &bit_index_right);
    color_filler(blue, frame, &byte_index_right, &bit_index_right);
}

void color_filler(uint8_t color, uint8_t* dest_array, uint16_t* byte_index, uint8_t* bit_index) {
    for (int i = 7; i >= 0; i--) {
        bool bit = (color >> i) & 0x1;
        uint8_t overflow = (*bit_index) % 8;
        if (bit == 1) {
            dest_array[*byte_index] |= (one >> overflow);
            *bit_index += 5;

            if (5 + overflow >= 8) {
                dest_array[*byte_index + 1] |= (one << 8 - overflow);
                *byte_index += 1;
            }
        } else if (bit == 0) {
            dest_array[*byte_index] |= (zero >> overflow);
            *bit_index += 5;

            if (5 + overflow >= 8) {
                dest_array[*byte_index + 1] |= (zero << 8 - overflow);
                *byte_index += 1;
            }
        }
    }
}


void push_frame() {
    NRF_SPIM0->TXD.PTR = (uint32_t)frame;
    NRF_SPIM0->TXD.MAXCNT = sizeof(frame);
    clock_start();
}