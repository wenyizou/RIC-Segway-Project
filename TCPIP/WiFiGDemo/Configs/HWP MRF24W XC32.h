 /*********************************************************************
 *
 *	Hardware specific definitions for:
 *    -  WiFi G Demo Board
 *          ~ PIC32MX695F512H
 *          ~ MRF24WG0MA
 *
 *********************************************************************
 * FileName:           HWP MRF24W XC32.h
 * Dependencies:    Compiler.h
 * Processor:          PIC32MX695F512H
 * Compiler:           Microchip  XC32 Compiler
 * Company:          Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright (C) 2002-2013 Microchip Technology Inc.  All rights
 * reserved.
 *
 * Microchip licenses to you the right to use, modify, copy, and
 * distribute:
 * (i)  the Software when embedded on a Microchip microcontroller or
 *      digital signal controller product ("Device") which is
 *      integrated into Licensee's product; or
 * (ii) ONLY the Software driver source files ENC28J60.c, ENC28J60.h,
 *		ENCX24J600.c and ENCX24J600.h ported to a non-Microchip device
 *		used in conjunction with a Microchip ethernet controller for
 *		the sole purpose of interfacing with the ethernet controller.
 *
 * You should refer to the license agreement accompanying this
 * Software for additional information regarding your rights and
 * obligations.
 *
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 * LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * MICROCHIP BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
 * BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE
 * THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER
 * SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT
 * (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE.
 *
 *
 * Author               Date		Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Amy Ong                         10/22/2012    Created for WiFi G Demo Board to conform to existing MLA file structures
 ********************************************************************/

#ifndef HARDWARE_PROFILE_H
#define HARDWARE_PROFILE_H

// Set configuration fuses (but only in MainDemo.c where THIS_IS_STACK_APPLICATION is defined)
#if defined(THIS_IS_STACK_APPLICATION)
	// DEVCFG0.CP              Code-protect bit
	// DEVCFG1.FNOSC        Oscillator selection bits
	// DEVCFG1.FSOSCEN    Secondary oscillator enable bit (Disabled)
	// DEVCFG1.POSCMOD   Primary oscillator configuration bits (Disabled)
	// DEVCFG1.FPBDIV       Peripheral bus clock divisor
	// DEVCFG1.FWDTEN      Watchdog timer enable bit
	// DEVCFG2.FPLLIDIV     PLL input divider bits
	// DEVCFG2.FPLLMUL      PLL multiplier bits
	// DEVCFG2.FPLLODIV    Default postscalar for PLL bits
	#pragma config FNOSC = FRCPLL, FPLLIDIV = DIV_2, FPLLMUL = MUL_20, FPLLODIV = DIV_2, FPBDIV = DIV_1, FWDTEN = OFF, POSCMOD = OFF, FSOSCEN = OFF, CP = OFF
#endif

#define GetSystemClock()		(40000000ul)                // 8MHz/FPLLIDIV*FPLLMUL/FPLLODIV
#define GetInstructionClock()	(GetSystemClock()/1)
#define GetPeripheralClock()	(GetInstructionClock()/1)	// Set your divider according to your Peripheral Bus Frequency configuration fuse setting


// ====================================================
//   Hardware mappings
// ====================================================


//----------------------------
// LED and Button I/O pins
//----------------------------
#define LED0_TRIS			(TRISAbits.TRISA0)// LED3 on WF32 board //(TRISEbits.TRISE0)	// Ref D10 Green
#define LED0_IO				(LATAbits.LATA0)
#define LED1_TRIS			(TRISAbits.TRISA1)// LED4...//(TRISFbits.TRISF1)	// Ref D9 Yellow
#define LED1_IO				(LATAbits.LATA1)
#define LED2_TRIS			(TRISFbits.TRISF0)// LED5... same RF0	// Ref D8 Red
#define LED2_IO				(LATFbits.LATF0)

#define LEDS_ON()           {LED0_ON(); LED1_ON(); LED2_ON();}
#define LEDS_OFF()          {LED0_OFF(); LED1_OFF(); LED2_OFF();}
#define LED0_ON()           LATASET = BIT_0;//LATESET = BIT_0;
#define LED0_OFF()          LATACLR = BIT_0;
#define LED0_INV()          LATAINV = BIT_0;

#define LED1_ON()           LATASET = BIT_1;
#define LED1_OFF()          LATACLR = BIT_1;
#define LED1_INV()          LATAINV = BIT_1;

#define LED2_ON()           LATFSET = BIT_0;
#define LED2_OFF()          LATFCLR = BIT_0;
#define LED2_INV()          LATFINV = BIT_0;

#define SW0_TRIS            (TRISAbits.TRISA7)//(TRISDbits.TRISD9)  ATTENTION!!!! here the bottom is different from WIFI G Board, that one is pull up and this one is pull down
                                              // so everytime in program , when SW0_IO appears with number status 0 , it should change to 1;
#define	SW0_IO              (PORTAbits.RA7)

#define VBAT_TRIS           (TRISBbits.TRISB0)     // Battery level ADC input,  need to be change, didn't change yet

// Added to support EZ_CONFIG_STORE
// 25LC256 I/O pins
//#define EEPROM_CS_TRIS		(TRISDbits.TRISD12)
#define EEPROM_CS_IO		(LATDbits.LATD12)
#define EEPROM_SCK_TRIS		(TRISGbits.TRISG6)
#define EEPROM_SDI_TRIS		(TRISGbits.TRISG7)
#define EEPROM_SDO_TRIS		(TRISGbits.TRISG8)
#define EEPROM_SPI_IF		(IFS1bits.SPI2RXIF)
#define EEPROM_SSPBUF		(SPI2BUF)
#define EEPROM_SPICON1		(SPI2CON)
#define EEPROM_SPICON1bits	(SPI2CONbits)
#define EEPROM_SPIBRG		(SPI2BRG)
#define EEPROM_SPISTAT		(SPI2STAT)
#define EEPROM_SPISTATbits	(SPI2STATbits)


//----------------------------
// MRF24WG0MA/B WiFi I/O pins
//----------------------------

#define WF_CS_TRIS			(TRISFbits.TRISF12)//(TRISGbits.TRISG9)
#define WF_CS_IO			(LATFbits.LATF12)//(LATGbits.LATG9)
#define WF_SDI_TRIS			(TRISFbits.TRISF4)//(TRISGbits.TRISG7)
#define WF_SCK_TRIS			(TRISFbits.TRISF13)//(TRISGbits.TRISG6)
#define WF_SDO_TRIS			(TRISFbits.TRISF5)//(TRISGbits.TRISG8)
#define WF_RESET_TRIS		        (TRISGbits.TRISG0)//(TRISDbits.TRISD1)
#define WF_RESET_IO			(LATGbits.LATG0)//(LATDbits.LATD1)

// NOTE:
//   Wifi comm demo declares  WF_INT_VECTOR as   _EXTERNAL_1_VECTOR and used in WF_Eint.c.
//   Wifi G demo adopts generic approach taken by demo/console/ezconfig by defining MRF24W_IN_SPI1.
//     _EXTERNAL_1_VECTOR  = MRF24W_IN_SPI1
#define MRF24W_IN_SPI1

#define WF_INT_TRIS	        (TRISAbits.TRISA15)//(TRISDbits.TRISD8)  // INT1
#define WF_INT_IO		   (PORTAbits.RA15)// (PORTDbits.RD8)

#define WF_HIBERNATE_TRIS	(TRISGbits.TRISG1)//(TRISEbits.TRISE4)
#define	WF_HIBERNATE_IO		(PORTGbits.RG1)//(PORTEbits.RE4)

#define WF_INT_EDGE		    (INTCONbits.INT4EP)// (INTCONbits.INT1EP)  INT1 IS THE ORIGIN
#define WF_INT_IE			(IEC0bits.INT4IE)
#define WF_INT_IF			(IFS0bits.INT4IF)
#define WF_INT_IE_CLEAR     IEC0CLR         // no need to change here, IEC0CLR contains INT4, IEC1CLR contains other INT like I2C, UART...
#define WF_INT_IF_CLEAR     IFS0CLR         // totally have IEC0,IEC1,IEC2, corresponding to the interrupt vector from 0 to 75, 0-31,32-63,64-75
#define WF_INT_IE_SET       IEC0SET
#define WF_INT_IF_SET       IFS0SET
#define WF_INT_BIT          0x00080000      // ATTENTION. the number here corresponding to the INT vector number 0x00000080 means the number 7 vector, which is INT1, here we need to change to INT4, which is 0x00080000
#define WF_INT_IPCSET       IPC4SET//IPC1SET
#define WF_INT_IPCCLR       IPC4CLR//IPC1CLR   here need to change from int1 to int4, remmember to change INTERRUPT HANDLER as well
#define WF_INT_IPC_MASK     0xFF000000
#define WF_INT_IPC_VALUE    0x0C000000    // need talking about it, how to change

#define WF_SSPBUF			(SPI4BUF)//(SPI2BUF)
#define WF_SPISTAT			(SPI4STAT)//(SPI2STAT)
#define WF_SPISTATbits		      (SPI4STATbits)//(SPI2STATbits)

#define WF_SPICON1			(SPI4CON)//(SPI2CON)
#define WF_SPICON1bits		       (SPI4CONbits)//(SPI2CONbits)
#define WF_SPI_IE_CLEAR     IEC1CLR    // no need change, SPI2 and SPI4 IE all inside
#define WF_SPI_IF_CLEAR     IFS1CLR
#define WF_SPI_INT_BITS     0x00000700  // ATTENTION! this value is from low to high setup to the SPI_IE_CLEAR, that means lsb corresponding to INT vector at 32, msb corresponing to INT vector at 63
                                        // here original 0x000000e0 means, clear INT vector number 37,38,39, which are SPI2E,SPI2TX, SPI2RX, here need to change to INT vector 40,41,42, withc are SPI4E,TX,RX
                                        // that is 0x000000e0<<3 is 0x00000700
#define WF_SPI_BRG		    (SPI4BRG)//(SPI2BRG)
#define WF_MAX_SPI_FREQ     (10000000ul)	// Hz

//----------------------------
// UART to Telnet Mapping
//----------------------------
#define BusyUART()          (TelnetOutFree() ? 0 : (StackTask(), 1))
#define putcUART            putchar
#define putrsUART(a)        fputs((const char*)a,(FILE *)stdout)
#define putsUART(a)         fputs((const char*)a,(FILE *)stdout)
#define DataRdyUART()       TelnetInChars()
#define ReadUART()          TelnetGet()

#define SENSOR_UART         UART1 //UART2
#define SENSOR_UART2        UART4

//----------------------------
// UART to Telnet Mapping
//----------------------------
#define I2C_NUM             I2C2      // use i2c number 2 in PIC32

//----------------------------
// MPU 9150 tilt Sensor Port Mapping
//----------------------------
#define Mpu_INT             PORTDbits.RD15    // the interrupt pin of MPU 9150 connected to Port D 15, #40 on chipkit board on J6
#define Mpu_INT_TRIS        TRISDbits.TRISD15         // set bit 9 as input direction
#define Mpu_INT_IO          LATDbits.LATD15
#define I2C_SDA_PULLUP_TRIS      (TRISAbits.TRISA4)
#define I2C_SCL_PULLUP_TRIS      (TRISAbits.TRISA5)
#define I2C_SDA_PULLUP_IO        (LATAbits.LATA4)
#define I2C_SCL_PULLUP_IO        (LATAbits.LATA5)


//----------------------------
// Proximate Sensor Port Mapping
//----------------------------
// Do not assign channels to scan
//#define configscan  (~(SKIP_SCAN_AN2 | SKIP_SCAN_AN3))   // corresponding to A0 and A6 on ChipKIT WF32 Board
  // Define the port for ADC, need to be modified for practical prox sensors
 //#define configport  ENABLE_AN2_ANA | ENABLE_AN3_ANA    // corresponding to A0 and A6 on ChipKIT WF32 Board


#endif // #ifndef HARDWARE_PROFILE_H




