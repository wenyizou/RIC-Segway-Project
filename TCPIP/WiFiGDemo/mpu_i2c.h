/* 
 * File:   mpu_i2c.h
 * Author: l
 *
 * Created on 2014年7月31日, 下午2:18
 */

#ifndef MPU_I2C_H
#define	MPU_I2C_H

#include <peripheral/i2c.h>
#define MPU_ADDR 0x68        // set AD0 to ground, then 0x68 is the MPU 9150 address

#ifdef	__cplusplus
extern "C" {
#endif

// Register address, reference of MPU 9150 datasheet
   #define who_am_i        0x75
   #define rate_div        0x19
   #define cfg             0x1A
   #define prod_id         0x0C
   #define user_ctrl       0x6A
   #define fifo_en         0x23
   #define gyro_cfg        0x1B
   #define accel_cfg       0x1C
   #define motion_thr      0x1F
   #define motion_dur      0x20
   #define fifo_count_h    0x72
   #define fifo_r_w        0x74
   #define raw_gyro        0x43
   #define raw_accel       0x3B
   #define temp            0x41
   #define int_enable      0x38
   #define dmp_int_status  0x39
   #define int_status      0x3A
   #define pwr_mgmt_1      0x6B
   #define pwr_mgmt_2      0x6C
   #define int_pin_cfg     0x37
   #define mem_r_w         0x6F
   #define accel_offs      0x06
   #define i2c_mst         0x24
   #define bank_sel        0x6D
   #define mem_start_addr  0x6E
   #define prgm_start_h    0x70

   #define accel_x_h       0x3b
   #define accel_x_l       0x3c
   #define accel_y_h       0x3d
   #define accel_y_l       0x3e
   #define accel_z_h       0x3f
   #define accel_z_l       0x40
   #define temp_h          0x41
   #define temp_l          0x42
   #define gyro_x_h        0x43
   #define gyro_x_l        0x44
   #define gyro_y_h        0x45
   #define gyro_y_l        0x46
   #define gyro_z_h        0x47
   #define gyro_z_l        0x48

/*******************************************************************************
  Macro:
    void Mpu_I2c_SetReg(BYTE RegAddr, BYTE Value, I2C_MODULE ID);

  Description:
    This is for set the MPU-9150 register.

  Precondition:
    None.

  Parameters:
    RegAddr    - The register address in MPU-9150.

    Value      - The value want to set.

    i2cnum     - i2c module used in PIC32

  Returns:
 I2C_RESULT



  *****************************************************************************/
I2C_RESULT Mpu_I2c_SetReg(BYTE RegAddr, BYTE Value, I2C_MODULE i2cnum);

// Init the MPU-9150 Chip I2C communication with PIC32MX695L for motor segway
void Mpu_I2c_Init(I2C_MODULE i2cnum);

// Init the MPU-9150 Chip for motor segway
I2C_RESULT Mpu_Init(I2C_MODULE i2cnum);

//Read the fifo information, return the bytes number that read, fifo value store in *buffer
int Mpu_I2c_ReadFIFO(BYTE *buffer, I2C_MODULE i2cnum);

// Read the register information, return the bytes number that read, though here is 1, value store in buffer
int Mpu_I2c_ReadReg(BYTE RegAddr, BYTE *buffer, I2C_MODULE i2cnum);

//Read the Gyro information, return the bytes number that read, fifo value store in *buffer
int Mpu_I2c_ReadGyro_x(BYTE *buffer, I2C_MODULE i2cnum);
int Mpu_I2c_ReadGyro_y(BYTE *buffer, I2C_MODULE i2cnum);
int Mpu_I2c_ReadGyro_z(BYTE *buffer, I2C_MODULE i2cnum);

//Read the Acceleration information, return the bytes number that read, fifo value store in *buffer
int Mpu_I2c_ReadAccel_x(BYTE *buffer, I2C_MODULE i2cnum);
int Mpu_I2c_ReadAccel_y(BYTE *buffer, I2C_MODULE i2cnum);
int Mpu_I2c_ReadAccel_z(BYTE *buffer, I2C_MODULE i2cnum);


#ifdef	__cplusplus
}
#endif

#endif	/* MPU_I2C_H */

