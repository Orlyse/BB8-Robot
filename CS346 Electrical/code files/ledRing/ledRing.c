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

uint8_t frame[746] = {0};

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
void generate_array(float angle, int red, int green, int blue, int fill_red, int fill_green, int fill_blue, int led_count) {
    uint8_t pattern_byte_index = 0;
    uint8_t pattern_bit_index = 0;
    
    uint8_t pattern[15] = {0};


    for (int i = 7; i >= 0; i--) {
        bool bit = (fill_green >> i) & 0x1;
        uint8_t overflow = pattern_bit_index % 8;
        if (bit == 1) {
            pattern[pattern_byte_index] |= (one >> overflow);
            pattern_bit_index += 5;

            if (5 + overflow >= 8) {
                pattern[pattern_byte_index + 1] |= (one << 8 - overflow);
                pattern_byte_index += 1;
            }
        } else if (bit == 0) {
            pattern[pattern_byte_index] |= (zero >> overflow);
            pattern_bit_index += 5;

            if (5 + overflow >= 8) {
                pattern[pattern_byte_index + 1] |= (zero << 8 - overflow);
                pattern_byte_index += 1;
            }
        }
    }

    for (int i = 7; i >= 0; i--) {
        bool bit = (fill_red >> i) & 0x1;
        uint8_t overflow = pattern_bit_index % 8;
        if (bit == 1) {
            pattern[pattern_byte_index] |= (one >> overflow);
            pattern_bit_index += 5;

            if (5 + overflow >= 8) {
                pattern[pattern_byte_index + 1] |= (one << 8 - overflow);
                pattern_byte_index += 1;
            }
        } else if (bit == 0) {
            pattern[pattern_byte_index] |= (zero >> overflow);
            pattern_bit_index += 5;

            if (5 + overflow >= 8) {
                pattern[pattern_byte_index + 1] |= (zero << 8 - overflow);
                pattern_byte_index += 1;
            }
        }
    }

    for (int i = 7; i >= 0; i--) {
        bool bit = (fill_blue >> i) & 0x1;
        uint8_t overflow = pattern_bit_index % 8;
        if (bit == 1) {
            pattern[pattern_byte_index] |= (one >> overflow);
            pattern_bit_index += 5;

            if (5 + overflow >= 8) {
                pattern[pattern_byte_index + 1] |= (one << 8 - overflow);
                pattern_byte_index += 1;
            }
        } else if (bit == 0) {
            pattern[pattern_byte_index] |= (zero >> overflow);
            pattern_bit_index += 5;

            if (5 + overflow >= 8) {
                pattern[pattern_byte_index + 1] |= (zero << 8 - overflow);
                pattern_byte_index += 1;
            }
        }
    }

    for (int i = 0; i < 48; i++) {
        memcpy(&frame[1 + i*15], pattern, 15);
    }
    
    float percentage = angle / 360.0;

    uint8_t led_index = percentage * led_count;

    uint16_t byte_index = 1 + led_index * 15;

    uint8_t bit_index = 0;

    memset(&frame[byte_index], 0, 15);
    
    for (int i = 7; i >= 0; i--) {
        bool bit = (green >> i) & 0x1;
        uint8_t overflow = bit_index % 8;
        if (bit == 1) {
            frame[byte_index] |= (one >> overflow);
            bit_index += 5;

            if (5 + overflow >= 8) {
                frame[byte_index + 1] |= (one << 8 - overflow);
                byte_index += 1;
            }
        } else if (bit == 0) {
            frame[byte_index] |= (zero >> overflow);
            bit_index += 5;

            if (5 + overflow >= 8) {
                frame[byte_index + 1] |= (zero << 8 - overflow);
                byte_index += 1;
            }
        }
    }

    for (int i = 7; i >= 0; i--) {
        bool bit = (red >> i) & 0x1;
        uint8_t overflow = bit_index % 8;
        if (bit == 1) {
            frame[byte_index] |= (one >> overflow);
            bit_index += 5;

            if (5 + overflow >= 8) {
                frame[byte_index + 1] |= (one << 8 - overflow);
                byte_index += 1;
            }
        } else if (bit == 0) {
            frame[byte_index] |= (zero >> overflow);
            bit_index += 5;

            if (5 + overflow >= 8) {
                frame[byte_index + 1] |= (zero << 8 - overflow);
                byte_index += 1;
            }
        }
    }

    for (int i = 7; i >= 0; i--) {
        bool bit = (blue >> i) & 0x1;
        uint8_t overflow = bit_index % 8;
        if (bit == 1) {
            frame[byte_index] |= (one >> overflow);
            bit_index += 5;

            if (5 + overflow >= 8) {
                frame[byte_index + 1] |= (one << 8 - overflow);
                byte_index += 1;
            }
        } else if (bit == 0) {
            frame[byte_index] |= (zero >> overflow);
            bit_index += 5;

            if (5 + overflow >= 8) {
                frame[byte_index + 1] |= (zero << 8 - overflow);
                byte_index += 1;
            }
        }
    }
}


void push_frame() {
    NRF_SPIM0->TXD.PTR = (uint32_t)frame;
    NRF_SPIM0->TXD.MAXCNT = 746;
    clock_start();
}
