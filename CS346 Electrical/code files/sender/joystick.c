#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "nrf.h"
#include "nrf_delay.h"

#include "microbit_v2.h"
#include "joystick.h"

// Pointer to an initialized I2C instance to use for transactions
static const nrf_twi_mngr_t* i2c_manager = NULL;

static uint8_t i2c_reg_read(uint8_t i2c_addr, uint8_t reg_addr) {
  uint8_t rx_buf = 0;
  nrf_twi_mngr_transfer_t const read_transfer[] = {
    NRF_TWI_MNGR_WRITE(i2c_addr, &reg_addr, 1, NRF_TWI_MNGR_NO_STOP),
    NRF_TWI_MNGR_READ(i2c_addr, &rx_buf, 1, 0),

    //TODO: implement me
  };
  ret_code_t result = nrf_twi_mngr_perform(i2c_manager, NULL, read_transfer, 2, NULL);
  if (result != NRF_SUCCESS) {
    // Likely error codes:
    //  NRF_ERROR_INTERNAL            (0x0003) - something is wrong with the driver itself
    //  NRF_ERROR_INVALID_ADDR        (0x0010) - buffer passed was in Flash instead of RAM
    //  NRF_ERROR_BUSY                (0x0011) - driver was busy with another transfer still
    //  NRF_ERROR_DRV_TWI_ERR_OVERRUN (0x8200) - data was overwritten during the transaction
    //  NRF_ERROR_DRV_TWI_ERR_ANACK   (0x8201) - i2c device did not acknowledge its address
    //  NRF_ERROR_DRV_TWI_ERR_DNACK   (0x8202) - i2c device did not acknowledge a data byte
    printf("I2C transaction failed! Error: %lX\n", result);
  }

  return rx_buf;
}

static void i2c_reg_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t data) {
    //TODO: implement me
    //Note: there should only be a single two-byte transfer to be performed
    uint8_t reg_data[2] = {reg_addr, data};
    nrf_twi_mngr_transfer_t const write_transfer[] = {
    NRF_TWI_MNGR_WRITE(i2c_addr, reg_data, 2, 0),
    };
    
    ret_code_t result = nrf_twi_mngr_perform(i2c_manager, NULL, write_transfer, 1, NULL);
    if (result != NRF_SUCCESS) {
        // Likely error codes:
        //  NRF_ERROR_INTERNAL            (0x0003) - something is wrong with the driver itself
        //  NRF_ERROR_INVALID_ADDR        (0x0010) - buffer passed was in Flash instead of RAM
        //  NRF_ERROR_BUSY                (0x0011) - driver was busy with another transfer still
        //  NRF_ERROR_DRV_TWI_ERR_OVERRUN (0x8200) - data was overwritten during the transaction
        //  NRF_ERROR_DRV_TWI_ERR_ANACK   (0x8201) - i2c device did not acknowledge its address
        //  NRF_ERROR_DRV_TWI_ERR_DNACK   (0x8202) - i2c device did not acknowledge a data byte
        printf("I2C write transaction failed! Error: %lX\n", result);
    }
}

void joystick_init(const nrf_twi_mngr_t* i2c){
    i2c_manager = i2c;
    uint8_t ID = i2c_reg_read(sparkfun_qwiic_joystick_addr, JOYSTICK_ID);
    printf("Joystick ID = %x\n", ID);
}   

int get_dirx(){
    uint8_t dirx_msb = i2c_reg_read(sparkfun_qwiic_joystick_addr, JOYSTICK_X_MSB);
    uint8_t dirx_lsb = i2c_reg_read(sparkfun_qwiic_joystick_addr, JOYSTICK_X_LSB);
    uint16_t dirx = ((dirx_msb << 8) | (dirx_lsb)) >> 6;

    

    //printf("dir x = %d  dir y = %d\n", dirx, diry);

    /*
    0 degrees x = 507 y = 503 (joystick untouched)
    1/360 degrees (507, 0)
    90 (1023, 503)
    180 (507, 1023)
    270 (0, 503) 
    */
   return dirx;
}

int get_diry(){
    uint8_t diry_msb = i2c_reg_read(sparkfun_qwiic_joystick_addr, JOYSTICK_Y_MSB);
    uint8_t diry_lsb = i2c_reg_read(sparkfun_qwiic_joystick_addr, JOYSTICK_Y_LSB);
    uint16_t diry = ((diry_msb << 8) | (diry_lsb)) >> 6;

    return diry;
} 

const int x_center = 507, x_min = 0, x_max = 1023;
const int y_center = 503, y_min = 0, y_max = 1023;

void cartesian_to_polar(int x, int y, float* degrees, float* magnitude){
    int dx = x-x_center;
    int dy = y-y_center;

    float nx = (dx >= 0) ? (float)dx/(x_max-x_center) : (float)dx/(x_center-x_min);
    float ny = (dy >= 0) ? (float)dy/(y_max-y_center) : (float)dy/(y_center-y_min);

    float raw_mag = sqrtf((nx*nx) + (ny*ny));
    
    if (raw_mag > 1) raw_mag = 1.0f;

    if (raw_mag < 0.08) {
      *magnitude = 0.0f;
      *degrees = 0.0f; 
    }

    *magnitude = raw_mag;

    float angle = atan2f(nx, -ny) * 180.0f / (float)M_PI;
    
    if (angle < 0){
      angle += 360.0f;  
    }
    *degrees = angle; 

    return;
}

