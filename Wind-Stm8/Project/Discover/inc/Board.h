/*
================================================================================
Function : Operation for SI446x
网址：http://shop57165217.taobao.com
作者：李勇  原  亿和电子工作室  现 亿佰特电子科技有限公司
TEL：18615799380, e-mail: yihe_liyong@126.com
================================================================================
*/
#ifndef _BOARD_H_
#define _BOARD_H_

#include "STM8l15x_conf.h"
#include "mytypedef.h"

/*Exchange a byte via the SPI bus*/
INT8U SPI_ExchangeByte( INT8U input );

/*Initialize the SPI bus*/
void SPI_Initial( void );

/*Initialize the other GPIOs of the board*/
void GPIO_Initial( void );




//--------------operations for OLED module------------------------
#define OLED_CSN_H( )   GPIO_SetBits( GPIOB, GPIO_Pin_3)
#define OLED_CSN_L( )   GPIO_ResetBits( GPIOB, GPIO_Pin_3)

#define OLED_CMD_H( )   GPIO_SetBits( GPIOD, GPIO_Pin_0)
#define OLED_CMD_L( )   GPIO_ResetBits( GPIOD, GPIO_Pin_0)

//#define OLED_RST_H( )   GPIO_SetBits( GPIOB, GPIO_Pin_2)
//#define OLED_RST_L( )   GPIO_ResetBits( GPIOB, GPIO_Pin_2)




#endif //_BOARD_H_
/*
=================================================================================
------------------------------------End of FILE----------------------------------
=================================================================================
*/
