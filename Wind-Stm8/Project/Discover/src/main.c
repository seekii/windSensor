 /* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"
#include "stm8l_discovery_lcd.h"
#include "discover_board.h"
#include "icc_measure.h"
#include "discover_functions.h"
#include "board.h"
#include "main.h"
#include "CC1101.h"
#include "icc_measure.h"

/*turn PWR */
#define PWR_On( )       GPIO_SetBits( GPIOB, GPIO_Pin_0 )

/*turn PWR*/
#define PWR_Off( )       GPIO_ResetBits( GPIOB, GPIO_Pin_0 )


/*turn on the LED*/
#define LED_On( )       GPIO_SetBits( GPIOA, GPIO_Pin_2 )

/*turn off the LED*/
#define LED_Off( )       GPIO_ResetBits( GPIOA, GPIO_Pin_2 )

/*toggle the LED*/
#define LED_Toggle( )   GPIO_ToggleBits( GPIOA, GPIO_Pin_2 )

//ASK RF SIGNAL REFERENCE
#define RF_GET_433 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)
#define RF_GET_315 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3)
#define UP      GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1)
#define DOWN    GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2)
//#define		RF_MIN	321
//#define		RF_MAX	3942
#define		RF_MIN	4000 //2000
#define		RF_MAX	1000000
#define        VER        1
unsigned char RF_get[11];
uint8_t state_machine;
uint8_t state_irq;
INT32U *addr=0;
extern void delay_10us(u16 n_10us);
/* LCD bar graph: used for display active function */
extern uint8_t t_bar[2];

extern bool Auto_test;
INT8U txBuffer[11] = {0,1,2,3,4,5,6,7,8,9,10 }, rxBuffer[11];
/* Set in interrupt handler for indicate that user button is pressed */ 
extern bool KeyPressed;
unsigned char	uart_tbuf[14];


void CRC_ENC()
{
  INT8U i;
  
  txBuffer[0]=0xaa;
  txBuffer[10]=0;
  for(i=1;i<10;i++)
      {
      txBuffer[10]^=txBuffer[i];
      }
  return ;
}
INTERRUPT_HANDLER(EXTI1_IRQHandler,9)
{
    uint32_t i=0;	
    disableInterrupts();
    PWR_On();
    LED_On();
    //COMMAND
    txBuffer[5]=0X10;
    //DATA
    if ((GPIOB->IDR & USER_GPIO_PIN) == 0x0) 
    {
        txBuffer[6]=0XFF;
    }
    else
    {
        txBuffer[6]=0X00;
    }    
    txBuffer[7]=0X00;
    txBuffer[8]=0X00;
    txBuffer[9]=0X00;
    txBuffer[10]=0Xaa;
    CRC_ENC();
    CC1101SendPacket( txBuffer,11, ADDRESS_CHECK );
  
     EXTI_ClearITPendingBit(EXTI_IT_Pin1);	
     enableInterrupts();
     LED_Off();
    // PWR_Off();
     state_irq++;
     return;
#if 0
    /* User button pressed */
   // KeyPressed = TRUE;
    if ((GPIOB->IDR & USER_GPIO_PIN) == 0x0) 
    {
    /* If user button pressed 4 seconds (approx.) set autotest */
    /* Autotest doesn't execute in Interrupt handler */
    while ((GPIOB->IDR & USER_GPIO_PIN) == 0x0)
    {
      i++;
      if (i == 0x011000)
      {
        EXTI_ClearITPendingBit(EXTI_IT_Pin1);	
        enableInterrupts();
        return;
      }
    }
    EXTI_ClearITPendingBit(EXTI_IT_Pin1);
    enableInterrupts();
  }
#endif
}
/**
  * @brief  Reads one word from flash memory
  * @param  Address : Address to read
  * @retval Value of the byte
  */
uint32_t FLASH_ReadWord(uint32_t Address)
{
    uint32_t    ret=0,addr;
    uint8_t     data;
    
    FLASH_DeInit();
    FLASH_Unlock(FLASH_MemType_Program);//FLASH_MemType_Data
    while(FLASH_GetFlagStatus(FLASH_FLAG_PUL)==RESET);
     addr= Address;
      
    data=FLASH_ReadByte( addr++);
    FLASH_WaitForLastOperation(FLASH_MemType_Data);
    ret+=data;
    data=FLASH_ReadByte( addr++);
    FLASH_WaitForLastOperation(FLASH_MemType_Data);
    ret<<=8;
    ret+=data;
    data=FLASH_ReadByte( addr++);
    FLASH_WaitForLastOperation(FLASH_MemType_Data);
    ret<<=8;
    ret+=data;
    data=FLASH_ReadByte( addr);
    FLASH_WaitForLastOperation(FLASH_MemType_Data);
    ret<<=8;
    ret+=data;
    FLASH_Lock(FLASH_MemType_Program);//FLASH_MemType_Data
  /* Read byte */
  return ret;
}
/***********************************************
************************************************/
uint32_t get_UID(void)
{
    uint32_t ret=0;
    uint8_t data;
    
    data=*(uint8_t*)0x4926;//low x
    ret<<=8;
    ret+=data;
    data=*(uint8_t*)0x4928;//low y
    ret<<=8;
    ret+=data;
    data=*(uint8_t*)0x492a;//wafer
    ret<<=8;
    ret+=data;
    data=*(uint8_t*)0x492b;//lot
    ret<<=8;
    ret+=data;
    return ret;
}
void CLK_init(void)
{
   CLK->CKDIVR=0x08;//内部16M的RC二分频后系统时钟为8M
}
void UART_init(void)//Pc2RX,pc3TX
{
   GPIOC->DDR&=~(1<<2);//输入
   GPIOC->CR1|=(1<<2);//带上拉的输入
   GPIOC->DDR|=(1<<3);//输出
   GPIOC->CR1|=(1<<3);
   CLK->PCKENR1|=0x20;//CLK_PCKENR1_USART1;//开启串口时钟
   USART1->BRR2=0x03;//设置波特率9600
   USART1->BRR1=0x68;//16M/9600=0x683
   USART1->CR2=0x2c;//允许接收中断，允许接收和发送
   USART1->CR3|=0x00;//停止位1，无奇偶效验
}
 
void UART_Send_Byte(unsigned char sendchar)
{
     while (!(USART1->SR&USART_SR_TXE));    //等待发送寄存器为空         
     USART1->DR = sendchar; 
   
}
 
void UART_Send_Str(unsigned char *ptr)
{
      while(*ptr != '\0')
      {
            UART_Send_Byte(*ptr++);                     // 发送数据
      }
      while (!(USART1->SR&USART_SR_TXE));    //等待发送寄存器为空
      USART1->DR = '\n';                              //发送换行指令
}
void UartSendBuf(unsigned char * buf, unsigned char len)
{
   while(len)
   {
      UART_Send_Byte(*buf++);
      len--;
   }
}
unsigned char get_rfdata_All_315()
{
    unsigned char i,j;
    unsigned char dat[3]={0};
    unsigned long  timer_counter=0;
    unsigned int   seek_long,seek_short;
    unsigned char seek_sum1,seek_sum2;
    unsigned int RF_X[5]={0x00,0x00,0x00,0x00,0x00};//Len_H,Len_L,Data_H,Data_L,Data_Vol;

    while(RF_GET_315!=0);
    {
            timer_counter=0; 
            while(RF_GET_315==0)
            {
                    timer_counter++;
                    
            }
            if(RF_MIN< timer_counter && timer_counter<RF_MAX)
            {
            seek_sum1=0;
            seek_sum2=0;
            seek_short=0;
            seek_long=0;
            RF_X[2]= timer_counter/31;	  // 0
            RF_X[3]= RF_X[2]*3; 	  // 1
            RF_X[4]= RF_X[2]>>1;
            for(i=0;i<3;i++)
                {
                for(j=0;j<8;j++)
                {
                dat[i] = dat[i]<<1; 	
                timer_counter=0; 
                while(RF_GET_315!=0);
                do
                {
                        timer_counter++;
                        
                }	
                
                while(RF_GET_315==0);
                if(10< timer_counter && timer_counter <RF_X[2]+RF_X[4])
                        {
                                dat[i] |= 0x01;
                                seek_short+=timer_counter;
                                seek_sum1++;
                                //dat[i] &=0xfe;
                        }
                else if(RF_X[2]+RF_X[4]< timer_counter && timer_counter<RF_X[3]+RF_X[4])
                        {
                                dat[i] &=0xfe;
                                seek_long+=timer_counter;
                                seek_sum2++;
                                //dat[i] |= 0x01;
                        }
                else
                        {
                                return 0;
                        }
                }
                }
            if(seek_sum1>0&&seek_sum2>0)
                            {
                            seek_short=seek_short/seek_sum1;
                            seek_long=seek_long/seek_sum2;
                            }
            RF_get[0]=seek_short>>8;
            RF_get[1]=seek_short;
            RF_get[2]=seek_long>>8;
            RF_get[3]=seek_long;
            if(seek_short==0||seek_long==0)
            {
              return 0;
            }
            RF_get[4]=0xa0;
            RF_get[5]=dat[0];
            RF_get[6]=dat[1];
            RF_get[7]=dat[2];
            return 1;	 //学习成功
            }
    }
    return 0;
}
unsigned char get_rfdata_All_433()
{
    unsigned char i,j;
    unsigned char dat[3]={0};
    unsigned long  timer_counter=0;
    unsigned int   seek_long,seek_short;
    unsigned char seek_sum1,seek_sum2;
    unsigned int RF_X[5]={0x00,0x00,0x00,0x00,0x00};//Len_H,Len_L,Data_H,Data_L,Data_Vol;

    while(RF_GET_433!=0);
    {
            timer_counter=0; 
            while(RF_GET_433==0)
            {
                    timer_counter++;

            }
            if(RF_MIN< timer_counter && timer_counter<RF_MAX)
            {
            seek_sum1=0;
            seek_sum2=0;
            seek_short=0;
            seek_long=0;
            RF_X[2]= timer_counter/31;	  // 0
            RF_X[3]= RF_X[2]*3; 	  // 1
            RF_X[4]= RF_X[2]>>1;
                    for(i=0;i<3;i++)
                            {
                            for(j=0;j<8;j++)
                            {
                            dat[i] = dat[i]<<1; 	
                            timer_counter=0; 
                            while(RF_GET_433!=0);
                            do
                            {
                                    timer_counter++;
                                    
                            }	
                            
                            while(RF_GET_433==0);
                            if(10< timer_counter && timer_counter <RF_X[2]+RF_X[4])
                                    {
                                            dat[i] |= 0x01;
                                            seek_short+=timer_counter;
                                            seek_sum1++;
                                            //dat[i] &=0xfe;
                                    }
                            else if(RF_X[2]+RF_X[4]< timer_counter && timer_counter<RF_X[3]+RF_X[4])
                                    {
                                            dat[i] &=0xfe;
                                            seek_long+=timer_counter;
                                            seek_sum2++;
                                            //dat[i] |= 0x01;
                                    }
                            else
                                    {
                                            return 0;
                                    }
                            }
                            }
            if(seek_sum1>0&&seek_sum2>0)
                            {
                            seek_short=seek_short/seek_sum1;
                            seek_long=seek_long/seek_sum2;
                            }
            RF_get[0]=seek_short>>8;
            RF_get[1]=seek_short;
            RF_get[2]=seek_long>>8;
            RF_get[3]=seek_long;
            if(seek_short==0||seek_long==0)
            {
              return 0;
            }
            RF_get[4]=0xa0;
            RF_get[5]=dat[0];
            RF_get[6]=dat[1];
            RF_get[7]=dat[2];
            return 1;	 //学习成功
            }
    }
    return 0;
}
void delayled(INT8U cnt)
{
    INT8U i,j;
     for(i=0;i<cnt;i++)
          for(j=0;j<0xff;j++);
     for(i=0;i<cnt;i++)
          for(j=0;j<0xff;j++);
     for(i=0;i<cnt;i++)
          for(j=0;j<0xff;j++);
     for(i=0;i<cnt;i++)
          for(j=0;j<0xff;j++);
     for(i=0;i<cnt;i++)
         for(j=0;j<0xff;j++);
     for(i=0;i<cnt;i++)
         for(j=0;j<0xff;j++);
     for(i=0;i<cnt;i++)
         for(j=0;j<0xff;j++);
     for(i=0;i<cnt;i++)
         for(j=0;j<0xff;j++);
     for(i=0;i<cnt;i++)
         for(j=0;j<0xff;j++);


}
void ledBlink(INT8U cnt)
{
  INT8U i;
  for(i=0;i<cnt;i++)
  {
      GPIO_SetBits( GPIOB, GPIO_Pin_0 );
      delayled(250);
      GPIO_ResetBits( GPIOB, GPIO_Pin_0 );
      delayled(250);
  
  }

}
#define		SEND_RF_TIMES	10
#define	RF_H 	GPIO_SetBits(GPIOB,GPIO_Pin_3)
#define RF_L 	GPIO_ResetBits(GPIOB,GPIO_Pin_3)
#define		POINT	15
unsigned char send_rfdata_All(unsigned char *inp)
{
	unsigned char i,j,cnt,t;
	unsigned char *dat;
	unsigned int  time_len=0;
	unsigned char data=0;
	unsigned int  seekii_counter=0;
        unsigned long  timer_counter=0;
        unsigned int RF_X[5]={0x00,0x00,0x00,0x00,0x00};//Len_H,Len_L,Data_H,Data_L,Data_Vol;
        unsigned int  timer_cnt=0;
     
        
	dat=inp;
        // syniot version
        time_len=18600;
      
	RF_X[2]= 610;	  // short 
	RF_X[3]= 1760;		  // long

        
        //for jingkong custurm
	//time_len=11800;
      
	//RF_X[2]= 305;//610;	  // short 
	//RF_X[3]= 1094;//1760;		  // long

	for(cnt=0;cnt<SEND_RF_TIMES;cnt++)
	{
	
			timer_cnt=0;
			RF_H;
			while(timer_cnt<RF_X[2])
				{
				timer_cnt++;
				}	
			RF_L;
	   		
			timer_cnt=0;
			while(timer_cnt<time_len)
				{
				timer_cnt++;
				}	

			for(j=2;j<5;j++)
			{
				data = 	dat[j];
				for(i=0;i<8;i++)
				{
					t=data&0x80;
					data=data<<1;
					if(t== 0x80)
					{
					   RF_H;
					
					   timer_cnt=0;
					   while(timer_cnt<RF_X[3])
					   {

                                              timer_cnt++;
								
					   }
					   RF_L;
					
					   timer_cnt=0;
					while(timer_cnt<RF_X[2])
					{
						
							timer_cnt++;
							
					}						
					}	
					else
					{
					   	RF_H;
					  
						timer_cnt=0;
						while(timer_cnt<RF_X[2])
						{
							
								timer_cnt++;
									
						}
					   RF_L;
					
						timer_cnt=0;
					while(timer_cnt<RF_X[3])
						{
							
								timer_cnt++;
								
						}					
					}	
				}
		}		
	}
	return 0;
}
#if 0
unsigned char send_rfdata_All(unsigned char *inp)
{
	unsigned char i,j,cnt,t;
	unsigned char *dat;
	unsigned int  time_len=0;
	unsigned char data=0;
	unsigned int  seekii_counter=0;
        unsigned long  timer_counter=0;
        unsigned int RF_X[5]={0x00,0x00,0x00,0x00,0x00};//Len_H,Len_L,Data_H,Data_L,Data_Vol;
        unsigned int  timer_cnt=0;
     
        
	dat=inp;
	time_len=610;
      
	RF_X[2]= 20;	  // short 
	RF_X[3]= 60;		  // long

	for(cnt=0;cnt<SEND_RF_TIMES;cnt++)
	{
		   	timer_counter=0;
			timer_cnt=0;
			RF_H;
			while(timer_cnt<RF_X[2])
				{
					timer_counter++;
					if(timer_counter==POINT)
					{					
						timer_counter=0;
						timer_cnt++;
					}		
				}	
			RF_L;
	   		timer_counter=0;
			timer_cnt=0;
			while(timer_cnt<time_len)
				{
					timer_counter++;
					if(timer_counter==POINT)
					{					
						timer_counter=0;
						timer_cnt++;
					}		
				}	

			for(j=2;j<5;j++)
			{
				data = 	dat[j];
				for(i=0;i<8;i++)
				{
					t=data&0x80;
					data=data<<1;
					if(t== 0x80)
					{
					   RF_H;
					   timer_counter=0;
					   timer_cnt=0;
					   while(timer_cnt<RF_X[3])
					   {
							timer_counter++;
							if(timer_counter==POINT)
							{					
								timer_counter=0;
								timer_cnt++;
							}		
					   }
					   RF_L;
					   timer_counter=0;
					   timer_cnt=0;
					while(timer_cnt<RF_X[2])
					{
						timer_counter++;
						if(timer_counter==POINT)
						{					
							timer_counter=0;
							timer_cnt++;
						}		
					}						
					}	
					else
					{
					   	RF_H;
					   	timer_counter=0;
						timer_cnt=0;
						while(timer_cnt<RF_X[2])
						{
							timer_counter++;
							if(timer_counter==POINT)
							{					
								timer_counter=0;
								timer_cnt++;
							}		
						}
					   RF_L;
					   timer_counter=0;
						timer_cnt=0;
					while(timer_cnt<RF_X[3])
						{
							timer_counter++;
							if(timer_counter==POINT)
							{					
								timer_counter=0;
								timer_cnt++;
							}		
						}					
					}	
				}
		}		
	}
	return 0;
}
#endif
/*********************************
Flash Address:
//920us  310us  300us 9440us
cur:1480us   600us 7640
page1: 0x00 9800 --0x00 983f
*********************************/
void main(void)
{
	INT8U i,j;
	INT32U x;
	INT8U testbuffer[10] = {"00000"};
      	uint32_t tx_counter=0, itmp;
        state_irq=0;
        INT8U read_data=0;
        INT8U buff[5]; 
        
        
	//SPI_Initial( );
	GPIO_Initial( );
	CLK_init();
        //UART_init();
        GPIO_ResetBits( GPIOB, GPIO_Pin_0);
        GPIO_ResetBits( GPIOC, GPIO_Pin_2);
        GPIO_ResetBits( GPIOC, GPIO_Pin_3);
        
        buff[0]=01;
        buff[1]=0xf0;
        buff[2]=0x11;
        buff[3]=0x23;
        buff[4]=0x0c;
        ledBlink(1);
        
       
        //GPIO_SetBits( GPIOC, GPIO_Pin_2);
        //GPIO_SetBits( GPIOC, GPIO_Pin_3);

	while(1)
	{


          //433m

          if(DOWN==0)
          {
                buff[0]=01;
                buff[1]=0xf0;
                buff[2]=0x11;
                buff[3]=0x23;
                buff[4]=0x0c;
                //ledBlink(1);
                send_rfdata_All(buff);
                
                GPIO_SetBits( GPIOB, GPIO_Pin_0 );//LED ON
                GPIO_SetBits( GPIOC, GPIO_Pin_2);//OPEN 
                delayled(250);
                delayled(250);//DELAY
                delayled(250);
                delayled(250);//DELAY
                
                delayled(250);
                delayled(250);
                           delayled(250);
                delayled(250);
                delayled(250);

                delayled(250);
                delayled(250);
                delayled(250);
                delayled(250);
                delayled(250);
                delayled(250);
                delayled(250);
                delayled(250);
                GPIO_ResetBits( GPIOB, GPIO_Pin_0 );//LED OFF
                GPIO_ResetBits( GPIOC, GPIO_Pin_2);//CLOSE

                //GPIO_ResetBits( GPIOC, GPIO_Pin_3);

           }

          if(UP==0)
          {
   
                buff[0]=01;
                buff[1]=0xf0;
                buff[2]=0x11;
                buff[3]=0x23;
                buff[4]=0x03;
                //ledBlink(2);
                send_rfdata_All(buff);
                
                GPIO_SetBits( GPIOB, GPIO_Pin_0 );//LED ON
                GPIO_SetBits( GPIOC, GPIO_Pin_3);//OPEN 
                delayled(250);
                delayled(250);//DELAY
                delayled(250);
                delayled(250);//DELAY
                
                delayled(250);
                delayled(250);
                delayled(250);
           delayled(250);
                delayled(250);
                delayled(250);
                
                delayled(250);
                delayled(250);
                delayled(250);
                delayled(250);
                delayled(250);
                delayled(250);
                delayled(250);
                GPIO_ResetBits( GPIOB, GPIO_Pin_0 );//LED OFF
                GPIO_ResetBits( GPIOC, GPIO_Pin_3);//CLOSE


           }
       

	}

	while( 0 )
	{ 
#if 0 
// save and read one byte;
        FLASH_DeInit();
        FLASH_Unlock(FLASH_MemType_Program);//FLASH_MemType_Data
        while(FLASH_GetFlagStatus(FLASH_FLAG_PUL)==RESET);
        FLASH_SetProgrammingTime(FLASH_ProgramTime_TProg);
        FLASH_WaitForLastOperation(FLASH_MemType_Data);
        FLASH_ProgramWord(0x8000,0x12345678);//
        FLASH_WaitForLastOperation(FLASH_MemType_Data);
        FLASH_Lock(FLASH_MemType_Program);//FLASH_MemType_Data
            
        read_data=FLASH_ReadByte(0x8000);//
        FLASH_WaitForLastOperation(FLASH_MemType_Data);
        
        if(read_data==0x12)
        {
         //LED_On();
         }
#endif 
#if 0
        
        tx_couter= FLASH_ReadWord(0x8000);
        if(tx_couter==0x12345678)
        {
         LED_On();
         }
#endif 
		//发送数据包，每发送一次，LED闪烁一次
		//CC1101SendPacket( txBuffer,10, ADDRESS_CHECK );
		CC1101SetTRMode( RX_MODE );
	
			while( GPIO_ReadInputDataBit( GPIOB, GPIO_Pin_2 ) == 0 );
			if( CC1101RecPacket( rxBuffer ) >0 )
			{
				LED_Toggle( );
			
              
			}
			
		
	}
}		

