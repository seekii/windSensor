/*
================================================================================
Copyright   : Ebyte electronic co.,LTD
Website     : http://yh-ebyte.taobao.com
              http://yiheliyong.cn.alibaba.com
Description : This module contains the low level operations for CC1101
================================================================================
*/
#ifndef _CC1101_H_
#define _CC1101_H_

#include "mytypedef.h"		//DO NOT modify
#include "CC1101_REG.h"		//DO NOT modify


#define CCxxx0_IOCFG2       0x00        // GDO2 output pin configuration
#define CCxxx0_IOCFG1       0x01        // GDO1 output pin configuration
#define CCxxx0_IOCFG0       0x02        // GDO0 output pin configuration
#define CCxxx0_FIFOTHR      0x03        // RX FIFO and TX FIFO thresholds
#define CCxxx0_SYNC1        0x04        // Sync word, high INT8U
#define CCxxx0_SYNC0        0x05        // Sync word, low INT8U
#define CCxxx0_PKTLEN       0x06        // Packet length
#define CCxxx0_PKTCTRL1     0x07        // Packet automation control
#define CCxxx0_PKTCTRL0     0x08        // Packet automation control
#define CCxxx0_ADDR         0x09        // Device address
#define CCxxx0_CHANNR       0x0A        // Channel number
#define CCxxx0_FSCTRL1      0x0B        // Frequency synthesizer control
#define CCxxx0_FSCTRL0      0x0C        // Frequency synthesizer control
#define CCxxx0_FREQ2        0x0D        // Frequency control word, high INT8U
#define CCxxx0_FREQ1        0x0E        // Frequency control word, middle INT8U
#define CCxxx0_FREQ0        0x0F        // Frequency control word, low INT8U
#define CCxxx0_MDMCFG4      0x10        // Modem configuration
#define CCxxx0_MDMCFG3      0x11        // Modem configuration
#define CCxxx0_MDMCFG2      0x12        // Modem configuration
#define CCxxx0_MDMCFG1      0x13        // Modem configuration
#define CCxxx0_MDMCFG0      0x14        // Modem configuration
#define CCxxx0_DEVIATN      0x15        // Modem deviation setting
#define CCxxx0_MCSM2        0x16        // Main Radio Control State Machine configuration
#define CCxxx0_MCSM1        0x17        // Main Radio Control State Machine configuration
#define CCxxx0_MCSM0        0x18        // Main Radio Control State Machine configuration
#define CCxxx0_FOCCFG       0x19        // Frequency Offset Compensation configuration
#define CCxxx0_BSCFG        0x1A        // Bit Synchronization configuration
#define CCxxx0_AGCCTRL2     0x1B        // AGC control
#define CCxxx0_AGCCTRL1     0x1C        // AGC control
#define CCxxx0_AGCCTRL0     0x1D        // AGC control
#define CCxxx0_WOREVT1      0x1E        // High INT8U Event 0 timeout
#define CCxxx0_WOREVT0      0x1F        // Low INT8U Event 0 timeout
#define CCxxx0_WORCTRL      0x20        // Wake On Radio control
#define CCxxx0_FREND1       0x21        // Front end RX configuration
#define CCxxx0_FREND0       0x22        // Front end TX configuration
#define CCxxx0_FSCAL3       0x23        // Frequency synthesizer calibration
#define CCxxx0_FSCAL2       0x24        // Frequency synthesizer calibration
#define CCxxx0_FSCAL1       0x25        // Frequency synthesizer calibration
#define CCxxx0_FSCAL0       0x26        // Frequency synthesizer calibration
#define CCxxx0_RCCTRL1      0x27        // RC oscillator configuration
#define CCxxx0_RCCTRL0      0x28        // RC oscillator configuration
#define CCxxx0_FSTEST       0x29        // Frequency synthesizer calibration control
#define CCxxx0_PTEST        0x2A        // Production test
#define CCxxx0_AGCTEST      0x2B        // AGC test
#define CCxxx0_TEST2        0x2C        // Various test settings
#define CCxxx0_TEST1        0x2D        // Various test settings
#define CCxxx0_TEST0        0x2E        // Various test settings

/*
================================================================================
------------------------------Internal IMPORT functions-------------------------
you must offer the following functions for this module
1. INT8U SPI_ExchangeByte( INT8U input ); //SPI Send and Receive function
2. CC_CSN_LOW( );                        //Pull down the CSN line
3. CC_CSN_HIGH( );                       //Pull up the CSN Line
================================================================================
*/
#include "main.h"     //import SPI_ExchangeByte( );

#include "STM8l15x_conf.h"
#define CC_CSN_LOW( )   GPIO_ResetBits( GPIOB, GPIO_Pin_4 );\
						while( GPIO_ReadInputDataBit( GPIOB, GPIO_Pin_7 ) != 0 );
#define CC_CSN_HIGH( )  GPIO_SetBits( GPIOB, GPIO_Pin_4 )


/*
================================================================================
-----------------------------------macro definitions----------------------------
================================================================================
*/
typedef enum { TX_MODE, RX_MODE }TRMODE;
typedef enum { BROAD_ALL, BROAD_NO, BROAD_0, BROAD_0AND255 }ADDR_MODE;
typedef enum { BROADCAST, ADDRESS_CHECK} TX_DATA_MODE;

/*
================================================================================
-------------------------------------exported APIs------------------------------
================================================================================
*/

/*read a byte from the specified register*/
INT8U CC1101ReadReg( INT8U addr );

/*Read a status register*/
INT8U CC1101ReadStatus( INT8U addr );

/*Set the device as TX mode or RX mode*/
void CC1101SetTRMode( TRMODE mode );

/*Write a command byte to the device*/
void CC1101WriteCmd( INT8U command );

/*Set the CC1101 into IDLE mode*/
void CC1101SetIdle( void );

/*Send a packet*/
void CC1101SendPacket( INT8U *txbuffer, INT8U size, TX_DATA_MODE mode );

/*Set the address and address mode of the CC1101*/
void CC1101SetAddress( INT8U address, ADDR_MODE AddressMode);

/*Set the SYNC bytes of the CC1101*/
void CC1101SetSYNC( INT16U sync );

/*Receive a packet*/
INT8U CC1101RecPacket( INT8U *rxBuffer );

/*Initialize the WOR function of CC1101*/
void  CC1101WORInit( void );

/*Initialize the CC1101, User can modify it*/
void CC1101Init( void );

#endif // _CC1101_H_
/*
================================================================================
------------------------------------THE END-------------------------------------
================================================================================
*/
