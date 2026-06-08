#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

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

int get_dir(){
    uint8_t dirx_msb = i2c_reg_read(sparkfun_qwiic_joystick_addr, JOYSTICK_X_MSB);
    uint8_t dirx_lsb = i2c_reg_read(sparkfun_qwiic_joystick_addr, JOYSTICK_X_LSB);
    uint16_t dirx = ((dirx_msb << 8) | (dirx_lsb)) >> 6;

    uint8_t diry_msb = i2c_reg_read(sparkfun_qwiic_joystick_addr, JOYSTICK_Y_MSB);
    uint8_t diry_lsb = i2c_reg_read(sparkfun_qwiic_joystick_addr, JOYSTICK_Y_LSB);
    uint16_t diry = ((diry_msb << 8) | (diry_lsb)) >> 6;

    printf("dir x = %d  dir y = %d\n", dirx, diry);

    if (dirx > 500 && diry < 50){
      return 1;
    }
    else if (dirx > 500 && diry > 1000){
      return 2;
    }
    else if (dirx < 50 && diry > 500){
      return 3;
    }
    else if (dirx > 1000 && diry > 500){
      return 4;
    }
    else{
      return 5;
    }
    
    return 0;
}

