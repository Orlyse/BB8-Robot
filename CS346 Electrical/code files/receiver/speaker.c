/*
Sources: 
https://github.com/runtimeco/mynewt_nordic/blob/master/hw/mcu/nordic_sdk/src/ext/nRF5_SDK_11.0.0_89a8197/components/drivers_nrf/i2s/nrf_drv_i2s.h
https://nrfconnectdocs.nordicsemi.com/ncs/latest/nrfx/html/group__nrf__i2s__hal.html#ga05fa70f696366c5a06fa5f5e673735af

*/

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "nrf.h"
#include "nrf_delay.h"
#include "nrfx_pwm.h"

#include "microbit_v2.h"

#include "speaker.h"
#include "nrfx_i2s.h"
#include "microbit_v2.h"


#define RAM_BUFFER_SIZE1 (22560/4)
static nrfx_i2s_config_t i2s_config = {
    .sck_pin = EDGE_P13,      ///< SCK pin number.
    .lrck_pin = EDGE_P14,     ///< LRCK pin number.
    .mck_pin = NRFX_I2S_PIN_NOT_USED,      ///< MCK pin number.
                          /**< Optional. Use @ref NRF_DRV_I2S_PIN_NOT_USED
                           *   if this signal is not needed. */
    .sdout_pin = EDGE_P11,    ///< SDOUT pin number.
                          /**< Optional. Use @ref NRF_DRV_I2S_PIN_NOT_USED
                           *   if this signal is not needed. */
    .sdin_pin = NRFX_I2S_PIN_NOT_USED,     ///< SDIN pin number.
                          /**< Optional. Use @ref NRF_DRV_I2S_PIN_NOT_USED
                           *   if this signal is not needed. */
    .irq_priority = APP_IRQ_PRIORITY_LOW, ///< Interrupt priority.

    .mode = NRF_I2S_MODE_MASTER,      ///< Mode of operation.
    .format = NRF_I2S_FORMAT_I2S,       ///< Frame format.
    .alignment = NRF_I2S_ALIGN_LEFT,    ///< Alignment of sample within a frame.
    .sample_width = NRF_I2S_SWIDTH_16BIT, ///< Sample width.
    .channels = NRF_I2S_CHANNELS_LEFT,     ///< Enabled channels.
    .mck_setup = NRF_I2S_MCK_32MDIV21,    ///< Master clock setup.
    .ratio = NRF_I2S_RATIO_96X,        ///< MCK/LRCK ratio.
};

void i2s_handler(nrfx_i2s_buffers_t const *p_buffers, uint32_t status){
    nrfx_i2s_stop();
}
void i2s_init(){
    nrfx_err_t err = nrfx_i2s_init(&i2s_config, i2s_handler);
    printf("Initialization %ld\n", err);
}

static uint32_t audio_ram_buffer1[RAM_BUFFER_SIZE1];

void play_sound(){
    memcpy(audio_ram_buffer1, audio_data1, audio_data_len1); // save buffer into ram
    nrfx_i2s_buffers_t bufs = {
        .p_tx_buffer = audio_ram_buffer1,
        .p_rx_buffer = NULL,
    };
    printf("Starting sound production\n");
    nrfx_err_t soundplay = nrfx_i2s_start(&bufs, audio_data_len1/4, 0);
    printf("Sound play %ld\n", soundplay);
}

