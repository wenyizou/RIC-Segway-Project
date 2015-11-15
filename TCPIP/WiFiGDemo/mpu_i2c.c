
#include <peripheral/i2c.h>
#include "mpu_i2c.h"
#include "Configs/HWP MRF24W XC32.h"

/* Below are the function defined here but not in mpu_i2c.h.
 * The reason for this is to hide these function to the user.
 * just lile the private proprety in C++ class
 *
 * reference from internet device_i2c.c, someone's github , thanks for that one here
 *
 */

// Start condition of I2C
I2C_RESULT Mpu_I2c_Start(I2C_MODULE i2cnum);
I2C_RESULT Mpu_I2c_Start(I2C_MODULE i2cnum){
           // Wait for the bus to be idle, then start the transfer
      I2C_STATUS  status;
        while( !I2CBusIsIdle(i2cnum) );

        if(I2CStart(i2cnum) != I2C_SUCCESS)
        {
            return I2C_ERROR;
        }


    // Wait for the signal to complete
    do
    {
        status = I2CGetStatus(i2cnum);

    } while ( !(status & I2C_START) );

    return I2C_SUCCESS;
}

// Stop condition of I2C
void Mpu_I2c_Stop(I2C_MODULE i2cnum);
void Mpu_I2c_Stop(I2C_MODULE i2cnum){
	I2C_STATUS  status;

    // Send the Stop signal
    I2CStop(i2cnum);

    // Wait for the stop to complete
    do
    {
        status = I2CGetStatus(i2cnum);

    } while ( !(status & I2C_STOP) );
}

I2C_RESULT Mpu_I2c_Restart(I2C_MODULE i2cnum);
I2C_RESULT Mpu_I2c_Restart(I2C_MODULE i2cnum){
	BYTE status;
	I2CRepeatStart(i2cnum);
	// Wait for the signal to complete
    do
    {
        status = I2CGetStatus(i2cnum);

    } while ( !(status & I2C_START) );

    return I2C_SUCCESS;
}

// send 1 byte on the I2C bus
I2C_RESULT Mpu_I2c_SendByte(I2C_MODULE i2cnum, BYTE data);
I2C_RESULT Mpu_I2c_SendByte(I2C_MODULE i2cnum, BYTE data)
{
    // wait for the transmitter available
	while(!I2CTransmitterIsReady(i2cnum));
	if(I2CSendByte(i2cnum, data)!=I2C_SUCCESS)
		return I2C_ERROR;
   // wait for the complete
	while(!I2CTransmissionHasCompleted(i2cnum));
        // judge whether receive acknowledge or not
    if (I2CByteWasAcknowledged(i2cnum))
	{
		return I2C_SUCCESS;
	}
	else
	{
		return I2C_ERROR;
	}
}

// Read 1 byte from I2C receive buffer, store in input *buffer
I2C_RESULT Mpu_I2c_ReadByte(I2C_MODULE i2cnum, BYTE *buffer);
I2C_RESULT Mpu_I2c_ReadByte(I2C_MODULE i2cnum, BYTE *buffer)
{
	if(I2CReceiverEnable(i2cnum, TRUE) == I2C_RECEIVE_OVERFLOW)
	{
		//todo
		return I2C_ERROR;
	}
	else
	{
		while(!I2CReceivedDataIsAvailable(i2cnum));
		*buffer = I2CGetByte(i2cnum);
                return I2C_SUCCESS;
	}

}

// send acknowledge
void Mpu_I2c_Ack(I2C_MODULE i2cnum);
void Mpu_I2c_Ack(I2C_MODULE i2cnum)
{
	I2CAcknowledgeByte(i2cnum, TRUE);
	while ( !I2CAcknowledgeHasCompleted(i2cnum) );
}

// send not acknowledge
void Mpu_I2c_Nack(I2C_MODULE i2cnum);
void Mpu_I2c_Nack(I2C_MODULE i2cnum)
{
	I2CAcknowledgeByte(i2cnum, FALSE);
	while ( !I2CAcknowledgeHasCompleted(i2cnum) );
}

/* below are functions definition corresponding to mpu_i2c.h file's functions
 */

// set Mpu register
I2C_RESULT Mpu_I2c_SetReg(BYTE RegAddr, BYTE Value, I2C_MODULE i2cnum){
    BYTE MpuAddrAndRw = MPU_ADDR;     // set the Mpu I2c address
    if (Mpu_I2c_Start(i2cnum)!=I2C_SUCCESS)
        return I2C_ERROR;
    else{
        MpuAddrAndRw = (MpuAddrAndRw << 1) + 0x00;       // 7 bit address first, come with 1 bit stand for write or read, 0x00 means write
        if (Mpu_I2c_SendByte(i2cnum, MpuAddrAndRw)!=I2C_SUCCESS)    // send register address first
        {Mpu_I2c_Stop(i2cnum);return I2C_ERROR;}             // not the right address, stop I2C, return error
        else{
        Mpu_I2c_SendByte(i2cnum, RegAddr);    // send register address first
        Mpu_I2c_SendByte(i2cnum, Value);      // send the value we what
        Mpu_I2c_Stop(i2cnum);
        return I2C_SUCCESS;
        }
    }

}

// Initialize the I2C communication from MPU 9150 to PIC32MX695L
void Mpu_I2c_Init(I2C_MODULE i2cnum){
    I2CConfigure(I2C1, I2C_ENABLE_SLAVE_CLOCK_STRETCHING);
    I2CSetFrequency ( i2cnum, 4000000ul , 100000);             // 100KHz, GetPeripheralClock()
    I2C_SDA_PULLUP_TRIS = 0;     // set as output
    I2C_SCL_PULLUP_TRIS = 0;
    I2C_SDA_PULLUP_IO   = 1;
    I2C_SCL_PULLUP_IO   = 1;
    Mpu_INT_TRIS        = 1;    // set as input
    Mpu_INT_IO          = 0 ;
        I2CEnable(i2cnum, TRUE);          // enable it
}

// Initialize the Mpu register settings
I2C_RESULT Mpu_Init(I2C_MODULE i2cnum){
    int i,j;
    if(Mpu_I2c_SetReg(pwr_mgmt_1, 0x80, i2cnum)==I2C_ERROR)      // 0x80 means soft reset the device
        return I2C_ERROR;                                        // if not success, exit
    // wait at least 100 ms, based on 40Mhz sysclk 4M times
    for (i=0;i<2000;i++)
        for(j=0;j<2000;j++)
            _nop();
    Mpu_I2c_SetReg(pwr_mgmt_1, 0x00, i2cnum);      // go into normal mode
    //Mpu_I2c_SetReg(user_ctrl, 0x40, i2cnum);      // 0x40 enable fifo
    //Mpu_I2c_SetReg(fifo_en, 0xf8, i2cnum);      // 0xf8 means enable gyro, temp and accl value write into fifo
    Mpu_I2c_SetReg(cfg, 0x00, i2cnum);      // 0x01 means set gyro sample frequency to 1khz, and set corresponding bandwidth
    Mpu_I2c_SetReg(rate_div, 0x00, i2cnum);      // 0x00 means accl sample frequency is the same as gyro , div by 0+1=1
    Mpu_I2c_SetReg(gyro_cfg, 0x18, i2cnum);      // 0x08 means gyro sensitive rate is 500°/sec
    Mpu_I2c_SetReg(accel_cfg, 0x18, i2cnum);      // 0x08 means gyro sensitive rate is 4g/sec
    Mpu_I2c_SetReg(int_pin_cfg, 0x20, i2cnum);      // 0X20 HIGH,0xA0 means interrupt pin held LOW until interrupt is cleared, interrupt cleared by reading INT_STATUS register
    Mpu_I2c_SetReg(int_enable, 0x00, i2cnum);      // 0x00 disable,0x01 means generate interrupt when sensor data is ready
    return I2C_SUCCESS;
}

// Read the Mpu 9150 register value, return the number that read(should be 1 byte), store in input *buffer
int Mpu_I2c_ReadReg(BYTE RegAddr, BYTE *buffer, I2C_MODULE i2cnum){
        BYTE MpuAddrAndRw = MPU_ADDR;     // set the Mpu I2c address
    if (Mpu_I2c_Start(i2cnum)!=I2C_SUCCESS)
        return 0;
    else{
        MpuAddrAndRw = (MpuAddrAndRw << 1) + 0x00;       // 7 bit address first, come with 1 bit stand for write or read, 0x00 means write
        if (Mpu_I2c_SendByte(i2cnum, MpuAddrAndRw)!=I2C_SUCCESS)    // send register address first
        {Mpu_I2c_Stop(i2cnum);return 0;}             // not the right address, stop I2C, return error
        else{
        Mpu_I2c_SendByte(i2cnum, RegAddr);    // send register address first

        // According to the MPU 9150 datasheet, need to send restart signals to request read
        Mpu_I2c_Restart(i2cnum);
        MpuAddrAndRw = MpuAddrAndRw +0x01;  // add 1 LSB,  means write
        Mpu_I2c_SendByte(i2cnum, MpuAddrAndRw);
        if (Mpu_I2c_ReadByte(i2cnum, buffer)==I2C_SUCCESS){
            Mpu_I2c_Nack(i2cnum);
            Mpu_I2c_Stop(i2cnum);
            return 1;
        }
        else{
            Mpu_I2c_Stop(i2cnum);
            return 0;
         }
        }
    }
}

//Read the Mpu 9150 fifo value, return the number that read from fifo, store in input *buffer
int Mpu_I2c_ReadFIFO(BYTE *buffer, I2C_MODULE i2cnum){
    BYTE fifo_count[2];       // for store the fifo counter, to see how many bytes available is fifo
    int fifo_tcount,i;         // fifo total count, just combine 2 bytes to 1
    if (Mpu_I2c_ReadReg(0x73, fifo_count, i2cnum)!=1)    //  fifo counter low bytes
        return 0;
    if (Mpu_I2c_ReadReg(0x72, fifo_count+1, i2cnum)!=1)    // fifo counter high bytes
        return 0;
    fifo_tcount = (int)fifo_count[1]*256 + (int)fifo_count[0];       // calculate the total number
    for(i=0;i<fifo_tcount;i++)
    {
        Mpu_I2c_ReadReg(0x74, (buffer+i), i2cnum);
    }
    return fifo_tcount;

}

//Read the Gyro information, return the bytes number that read, gyro value store in *buffer
int Mpu_I2c_ReadGyro_x(BYTE *buffer, I2C_MODULE i2cnum){
       BYTE *sensor_int_status;
       Mpu_I2c_ReadReg(gyro_x_h, buffer, i2cnum);
       Mpu_I2c_ReadReg(gyro_x_l, buffer+1, i2cnum);     // read Gyro,
       Mpu_I2c_ReadReg(int_status, sensor_int_status, i2cnum);   // dummy read, clear the high interrupt signal
       return 1;
}

int Mpu_I2c_ReadGyro_y(BYTE *buffer, I2C_MODULE i2cnum){
       BYTE *sensor_int_status;
       Mpu_I2c_ReadReg(gyro_y_h, buffer, i2cnum);
       Mpu_I2c_ReadReg(gyro_y_l, buffer+1, i2cnum);     // read Gyro,
       Mpu_I2c_ReadReg(int_status, sensor_int_status, i2cnum);   // dummy read, clear the high interrupt signal
       return 1;
}

int Mpu_I2c_ReadGyro_z(BYTE *buffer, I2C_MODULE i2cnum){
       BYTE *sensor_int_status;
       Mpu_I2c_ReadReg(gyro_z_h, buffer, i2cnum);
       Mpu_I2c_ReadReg(gyro_z_l, buffer+1, i2cnum);     // read Gyro,
       Mpu_I2c_ReadReg(int_status, sensor_int_status, i2cnum);   // dummy read, clear the high interrupt signal
       return 1;
}

//Read the Acceleration information, return the bytes number that read, accel value store in *buffer
int Mpu_I2c_ReadAccel_x(BYTE *buffer, I2C_MODULE i2cnum){
       BYTE *sensor_int_status;
       Mpu_I2c_ReadReg(accel_x_h, buffer, i2cnum);
       Mpu_I2c_ReadReg(accel_x_l, buffer+1, i2cnum);     // read Accel,
       Mpu_I2c_ReadReg(int_status, sensor_int_status, i2cnum);   // dummy read, clear the high interrupt signal
       return 1;
}

int Mpu_I2c_ReadAccel_y(BYTE *buffer, I2C_MODULE i2cnum){
       BYTE *sensor_int_status;
       Mpu_I2c_ReadReg(accel_y_h, buffer, i2cnum);
       Mpu_I2c_ReadReg(accel_y_l, buffer+1, i2cnum);     // read Accel,
       Mpu_I2c_ReadReg(int_status, sensor_int_status, i2cnum);   // dummy read, clear the high interrupt signal
       return 1;
}

int Mpu_I2c_ReadAccel_z(BYTE *buffer, I2C_MODULE i2cnum){
       BYTE *sensor_int_status;
       Mpu_I2c_ReadReg(accel_z_h, buffer, i2cnum);
       Mpu_I2c_ReadReg(accel_z_l, buffer+1, i2cnum);     // read Accel,
       Mpu_I2c_ReadReg(int_status, sensor_int_status, i2cnum);   // dummy read, clear the high interrupt signal
       return 1;
}