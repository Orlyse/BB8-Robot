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
#include "ledRing.h"

APP_TIMER_DEF(my_timer_1);

float angle = 0.0;

static void rotate_ring(void* _unused) {
    angle += 5.0;
    if (angle == 360.0) {
        angle = 0.0;
    }
    generate_array(angle, 60, 0, 0, 0, 0, 60, 60, 48, 40, 32);
    push_frame();
}


int main(void) {
    printf("Board started!\n");

    clock_init();

    generate_array(0.0, 255, 255, 255, 255, 255, 255, 60, 48, 40, 32);

    push_frame();

    app_timer_init();
    app_timer_create(&my_timer_1, APP_TIMER_MODE_REPEATED, rotate_ring);
    app_timer_start(my_timer_1, 5000, NULL);


    while (1) {
        nrf_delay_ms(1000);
    }
}

// #include <stdint.h>
// #include "nrf.h"
// #include "nrf_delay.h"

// uint8_t test_frame[40] = {
//     0x84, 0x21, 0x08, 0x42, 0x10,
//     0xE7, 0x39, 0xCE, 0x73, 0x9C,  
//     0x84, 0x21, 0x08, 0x42, 0x10
// };

// int main(void) {
//     // Make P0.02 a real output BEFORE enabling SPIM
//     NRF_P0->OUTCLR = (1 << 2);          // idle low
//     NRF_P0->PIN_CNF[2] =
//           (1 << 0)    // DIR   = Output
//         | (0 << 1)    // INPUT = Connect
//         | (0 << 2)    // PULL  = Disabled
//         | (3 << 8)    // DRIVE = H0H1 (high drive both edges)
//         | (0 << 16);  // SENSE = Disabled

//     NRF_SPIM0->PSEL.MOSI = 2;
//     NRF_SPIM0->PSEL.MISO = 0xFFFFFFFF;
//     NRF_SPIM0->PSEL.SCK  = 28;
//     NRF_SPIM0->FREQUENCY = 0x40000000;  // M4
//     NRF_SPIM0->CONFIG    = 0;           // MSB first, mode 0
//     NRF_SPIM0->ENABLE    = 7;

//     NRF_SPIM0->TXD.PTR    = (uint32_t)test_frame;
//     NRF_SPIM0->TXD.MAXCNT = sizeof(test_frame);

//     while (1) {
//         NRF_SPIM0->EVENTS_ENDTX = 0;
//         NRF_SPIM0->TASKS_START  = 1;
//         while (NRF_SPIM0->EVENTS_ENDTX == 0) { }  // wait for DMA to finish
//         nrf_delay_ms(500);
//     }
// }