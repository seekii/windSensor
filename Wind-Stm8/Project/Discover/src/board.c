#include "stm8l15x_spi.h"
#include "stm8l15x.h"
#include "stm8l_discovery_lcd.h"
#include "discover_board.h"
#include "icc_measure.h"
#include "discover_functions.h"
	
#include "board.h"
#include "main.h"
#include "CC1101.h"



/*
=================================================================================
SPI_ExchangeByte( );
Function : Exchange a byte via the SPI bus
INTPUT   : input, The input byte
OUTPUT   : The output byte from SPI bus
=================================================================================
*/
INT8U SPI_ExchangeByte( INT8U input )
{
    //wait for last transmitt finishing
	while (SPI_GetFlagStatus(SPI1,SPI_FLAG_TXE) == RESET);
	SPI_SendData(SPI1,input );
	//wait for receiving a byte
	while (SPI_GetFlagStatus(SPI1,SPI_FLAG_RXNE) == RESET);
	return SPI_ReceiveData( SPI1);
}
/*
=================================================================================
SPI_Initial( );
Function : Initialize the SPI bus
INTPUT   : None
OUTPUT   : None
=================================================================================
*/



void SPI_Initial( void )
{
	int i;
    //enable clock for SPI bus
	CLK_PeripheralClockConfig(CLK_Peripheral_SPI1, ENABLE);
	//Set as default
	SPI_DeInit( SPI1);
	//Set the priority of the SPI
	SPI_Init( SPI1,SPI_FirstBit_MSB, SPI_BaudRatePrescaler_4,
            SPI_Mode_Master, SPI_CPOL_Low, SPI_CPHA_1Edge,
            SPI_Direction_2Lines_FullDuplex, SPI_NSS_Soft ,0x07);
    //Enable SPi
	SPI_Cmd( SPI1,ENABLE );
	for(i=0;i<5;i++);
    //Config the GPIOs for SPI bus
	GPIO_Init( GPIOB, GPIO_Pin_7,GPIO_Mode_In_PU_No_IT );
	GPIO_Init( GPIOB, GPIO_Pin_5 | GPIO_Pin_6, GPIO_Mode_Out_PP_High_Slow );
}
/*
=================================================================================
GPIO_DeInitial( );
Function : Initialize the other GPIOs of the board
INTPUT   : None
OUTPUT   : None
=================================================================================
*/
void GPIO_DeInitial( void )
{
    GPIO_LowPower_Config();

}
/*
=================================================================================
GPIO_Initial( );
Function : Initialize the other GPIOs of the board
INTPUT   : None
OUTPUT   : None
=================================================================================
*/
void GPIO_Initial( void )
{

    GPIO_Init( GPIOB, GPIO_Pin_0, GPIO_Mode_Out_PP_High_Slow );
    GPIO_SetBits( GPIOB, GPIO_Pin_0 );
    //config the LED and CSN for RF module, CSN = PB4,LED = PB0
    //GPIO_Init( GPIOC, GPIO_Pin_2, GPIO_Mode_Out_PP_High_Slow );//LED
    //GPIO_Init( GPIOC, GPIO_Pin_3, GPIO_Mode_Out_PP_High_Slow );//LED
       
    GPIO_Init( GPIOC, GPIO_Pin_2, GPIO_Mode_Out_PP_Low_Fast );//LED
    GPIO_Init( GPIOC, GPIO_Pin_3, GPIO_Mode_Out_PP_Low_Fast );//LED  
    
    GPIO_Init( GPIOB, GPIO_Pin_3, GPIO_Mode_Out_PP_High_Slow );//LED
    GPIO_Init( GPIOB, GPIO_Pin_2,GPIO_Mode_In_FL_No_IT );//RF INPUT
    GPIO_Init( GPIOB, GPIO_Pin_1,GPIO_Mode_In_FL_No_IT );//SHWN
   
    //GPIO_ExternalPullUpConfig(GPIOC,GPIO_Pin_3,ENABLE);
   // GPIO_ExternalPullUpConfig(GPIOC,GPIO_Pin_2,ENABLE);
    
    //GPIO_Init(GPIOC, GPIO_Pin_3, GPIO_Mode_Out_PP_Low_Fast);//串口发送端初始化 
   // GPIO_Init(GPIOC, GPIO_Pin_2, GPIO_Mode_In_PU_No_IT);//串口接收端初始化
}
/*
=================================================================================
------------------------------------End of FILE----------------------------------
=================================================================================
*/
