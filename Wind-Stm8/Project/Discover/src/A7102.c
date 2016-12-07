/***********************************************
		*	copyrite (c) 2007
		*文件名: 	  A7102.c
		*作者:		  larry
		*当前版本:	  v1.0
		*建立时间:        2007-6-11 09:45
		*完成日期:
		
		*描述:            A7102驱动抽象程序,与MCU和IO
		*                 接口无关,由外部.h文件宏定义				     
************************************************/		
#include    "A7102.h"
//#include    "include.h"
#include "discover_board.h"
#include "stm8l15x.h"
uchar rf_power;
    /* 收发频率校准值保存 */
unsigned    int  TX_Freq_Calibration;
unsigned    int  RX_Freq_Calibration;
unsigned    char  unlock_id_buff[0x04]    = {0x6C,0x61,0x6E,0x62};//初始化 ID  
unsigned    char  lock_id_buff[0x04]    = {0x6C,0x6F,0x63,0x6B};//加锁  ID 
uchar send_dat[8]={0xff,0x55,0x55,0x55,0x55,0x55,0x55,0x55};

void A7102_RX(unsigned char dir)
{
	
	//GPIO_InitTypeDef GPIO_InitStructure;
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	if(dir!=0)//
		{
		//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
                GPIO_Init( GPIOB, GPIO_Pin_6, GPIO_Mode_In_FL_No_IT);
		}
	else
		{
		//GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;
                GPIO_Init( GPIOB, GPIO_Pin_6, GPIO_Mode_Out_PP_Low_Fast);
		}
	//GPIO_Init(GPIOA, &GPIO_InitStructure);

}


void Delayms(u32 nTime)
{
        unsigned int counter;
	int i,j,k;

        for(i=20;i>0;i--)
		{
		for(j=100;j>0;j--);
		}
}
/***********************************************
函 数:	Byte_Write()
功 能:	写8bit数据到sck和sdio
输 入:	byte
输 出:	/
描 述:	左移位,bit7在先,sck负边缘触发 
***********************************************/
void mydelay()
{
	int i,j;
	
        for(i=2;i>0;i--)
		{
	
			for(j=10;j>0;j--);
		}
}
void    Byte_Write(unsigned char byte)
{
        unsigned char i;
        
        for(i = 0x00; i < 0x08; i++)
        {
            if(byte&0x80)
               // iSPI_Sdi     = positive;//
                GPIO_HIGH(GPIOB,GPIO_Pin_6);
        
            else
              //  iSPI_Sdi     = negative;
			
                          GPIO_LOW(GPIOB,GPIO_Pin_6);	
                
            mydelay();
            
           // iSPI_Sck         = positive;
		
                        GPIO_HIGH(GPIOB,GPIO_Pin_5);
            
            mydelay();
            
            //iSPI_Sck         = negative;

             GPIO_LOW(GPIOB,GPIO_Pin_5);	
            mydelay();            
            
            byte <<= 0x01;
        }
}
/***********************************************
函 数:	Byte_Read()
功 能:	读8bit数据从sck和sdio
输 入:	/
输 出:	mybyte
描 述:	左移位,bit7在先,sck负边缘触发 
***********************************************/
unsigned char   Byte_Read(void)
{
        unsigned char i;
        unsigned char mybyte=0;
        
        for(i = 0x00; i < 0x08; i++)
        {
            mybyte           <<= 0x01;
            
            mydelay(); 
            
          //  if(iSPI_Sdo)
          A7102_RX(1);
  
          if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6))
                mybyte        |= 0x01;
            else
                mybyte        &= 0xfe;
                
            //iSPI_Sck         = positive; 
              GPIO_HIGH(GPIOB,GPIO_Pin_5);          
            mydelay();
             GPIO_LOW(GPIOB,GPIO_Pin_5);
            //iSPI_Sck         = negative;              
        }
        
        /* 返回读取数值 */
		A7102_RX(0);
        return    (mybyte);
}
/***********************************************
函 数:	Word_Write()
功 能:	写16bit数据到sck和sdio
输 入:	myword
输 出:	/
描 述:	左移位,bit15在先,sck负边缘触发 
***********************************************/
void    Word_Write(unsigned int myword)
{
        unsigned char i;
        
        for(i = 0x00; i < 0x10; i++)
        {           
            if(myword&0x8000)
                //iSPI_Sdi     = positive;
                 GPIO_HIGH(GPIOB,GPIO_Pin_6);
            else
                //iSPI_Sdi     = negative;
                GPIO_LOW(GPIOB,GPIO_Pin_6);
            
            mydelay();
            
            //iSPI_Sck         = positive;
            GPIO_HIGH(GPIOB,GPIO_Pin_5);
            mydelay();
            
            //iSPI_Sck         = negative;
            GPIO_LOW(GPIOB,GPIO_Pin_5);
            mydelay();            
            
            myword           <<= 0x01;
        }
}
/***********************************************
函 数:	Word_Read()
功 能:	读16bit数据从sck和sdio
输 入:	/
输 出:	myword
描 述:	左移位,bit16在先,sck负边缘触发 
***********************************************/
unsigned int    Word_Read(void)
{
        unsigned char i;
        unsigned int  myword=0;
        
        for(i = 0x00; i < 0x10; i++)
        {
            myword  <<= 0x01;
            
            mydelay();
            
            //if(iSPI_Sdo)
           // #define	RF_GET_433 isBit32(GPIO_ReadInputData(GPIOB),9)
           A7102_RX(1);
           if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6))
                myword|= 0x0001;
            else
                myword&= 0xfffe;
                
            //iSPI_Sck         = positive; 
              GPIO_HIGH(GPIOB,GPIO_Pin_5);          
            mydelay();
            GPIO_LOW(GPIOB,GPIO_Pin_5);
            //iSPI_Sck         = negative;
        }
        A7102_RX(0);
        /* 返回读取数值 */
        return    (myword);
}
/***********************************************
函 数:	A7102_Reset_Chip()
功 能:	复位A7102芯片 
输 入:	/
输 出:	/
描 述:	/
***********************************************/
void    A7102_Reset_Chip(void)
{
        //iSPI_Scs        = negative;  

        GPIO_LOW(GPIOB,GPIO_Pin_4);
        /* 写A7102-Reset命令 */
      Byte_Write(0x7a);     

           GPIO_HIGH(GPIOB,GPIO_Pin_4);  
        //iSPI_Scs        = positive;
}
/***********************************************
函 数:	A7102_Reset_FifoTX()
功 能:	复位A7102发送FIFO指针 
输 入:	/
输 出:	/
描 述:	/
***********************************************/
void    A7102_Reset_FifoTX(void)
{
        //iSPI_Sck        = negative;
	
                GPIO_LOW(GPIOB,GPIO_Pin_5);
		//iSPI_Scs        = negative;
        GPIO_LOW(GPIOB,GPIO_Pin_4);
        /* 写A7102-FifoTX命令 */
        Byte_Write(0x6a);
        GPIO_HIGH(GPIOB,GPIO_Pin_4);  
        //iSPI_Scs        = positive;
}
/***********************************************
函 数:	A7102_Reset_FifoRX()
功 能:	复位A7102接收FIFO指针 
输 入:	/
输 出:	/
描 述:	/
***********************************************/
void    A7102_Reset_FifoRX(void)
{
        //iSPI_Sck        = negative;
        GPIO_LOW(GPIOB,GPIO_Pin_5);
        //iSPI_Scs        = negative;
        GPIO_LOW(GPIOB,GPIO_Pin_4);
        /* 写A7102-FifoRX命令 */
        Byte_Write(0xea);
        GPIO_HIGH(GPIOB,GPIO_Pin_4);  
        //iSPI_Scs        = positive;
}
/***********************************************
函 数:	A7102_Fifo_Read()
功 能:	读FIFO数据 
输 入:	read_buff
输 出:	/
描 述:	/
***********************************************/
void    A7102_Fifo_Read(unsigned char *read_buff)
{
        unsigned char i;
	
        //iSPI_Sck        = negative;
		 GPIO_LOW(GPIOB,GPIO_Pin_5);
		 GPIO_LOW(GPIOB,GPIO_Pin_4);
              

		//iSPI_Scs        = negative;
        
        /* 读A7102-FIFO命令 */
        Byte_Write(0xca);
        
        declare_sdo_input();
        
        /* 读A7102-FIFO数据 */
        for(i = 0x00;i < C_FIFO_Byte;i++)
        {
            *read_buff  = Byte_Read();
            
            read_buff ++;
        }
        
       // declare_sdo_output();
        GPIO_HIGH(GPIOB,GPIO_Pin_4);  
        //iSPI_Scs        = positive;
}
/***********************************************
函 数:	A7102_Fifo_Write()
功 能:	写FIFO数据 
输 入:	write_buff
输 出:	/
描 述:	/
***********************************************/
void    A7102_Fifo_Write(unsigned char *write_buff)
{
        unsigned char i;
        //iSPI_Sck        = negative;
		 GPIO_LOW(GPIOB,GPIO_Pin_5);
		Delayms(1);
		//iSPI_Scs        = negative;
		 GPIO_LOW(GPIOB,GPIO_Pin_4);
        /* 写A7102-FIFO命令 */
        Byte_Write(0x5a);
        /* 写A7102-FIFO数据 */
        for(i = 0x00;i < C_FIFO_Byte;i++)
        {
            Byte_Write(*write_buff);
            
            write_buff ++;
        }
        GPIO_HIGH(GPIOB,GPIO_Pin_4);  
        //iSPI_Scs        = positive;
}
/***********************************************
函 数:	A7102_Id_Write()
功 能:	写ID到A7102内部寄存器 
输 入:	buff_id[]
输 出:	/
描 述:	写ID必须一次性写入 
***********************************************/
void    A7102_Id_Write(unsigned char *id_buff)
{
        unsigned char i;
	
        //iSPI_Sck        = negative;
		 GPIO_LOW(GPIOB,GPIO_Pin_5);
        //iSPI_Scs        = negative;
        GPIO_LOW(GPIOB,GPIO_Pin_4);
        /* 写A7102-ID命令 */
        Byte_Write(0x2a);
        
        /* 写A7102-ID数据 */
        for(i = 0x00;i < 0x04;i++)
        {
            Byte_Write(*id_buff);
            
            id_buff ++;
        }
        GPIO_HIGH(GPIOB,GPIO_Pin_4);  
        //iSPI_Scs        = positive;
}
/***********************************************
函 数:	A7102_Id_Read()
功 能:	读ID从A7102内部寄存器 
输 入:	/
输 出:	buff_id[]
描 述:	读ID必须一次性读出 
***********************************************/
void    A7102_Id_Read(unsigned char *id_buff)
{
        unsigned char i;
	
        //iSPI_Sck        = negative;
		 GPIO_LOW(GPIOB,GPIO_Pin_5);
        //iSPI_Scs        = negative;
        GPIO_LOW(GPIOB,GPIO_Pin_4);
        /* 读A7102-ID命令 */
        Byte_Write(0xaa);
        
        declare_sdo_input();
        
        /* 读A7102-ID数据 */
        for(i = 0x00;i < 0x04;i++)
        {
            *id_buff    = Byte_Read();
            
            id_buff ++;
        }
        
        declare_sdo_output();
        GPIO_HIGH(GPIOB,GPIO_Pin_4);  
        //iSPI_Scs        = positive;
}
/***********************************************
函 数:	A7102_Register_Write()
功 能:	写控制数据到A7102内部寄存器 
输 入:	addr,parameter
输 出:	/
描 述:	/
***********************************************/
void    A7102_Register_Write(unsigned char addr,unsigned int parameter)
{
        //iSPI_Sck        = negative;
        GPIO_LOW(GPIOB,GPIO_Pin_5);
        mydelay();
        //iSPI_Scs        = negative;
        GPIO_LOW(GPIOB,GPIO_Pin_4);
        addr           &= 0x0f;
        /* 写A7102-参数配置命令 */
        Byte_Write(addr);
        
        /* 写A7102-参数配置数据 */
        Word_Write(parameter);
        GPIO_HIGH(GPIOB,GPIO_Pin_4);  
        //iSPI_Scs        = positive;
}
/***********************************************
函 数:	A7102_Register_Read()
功 能:	读控制数据从A7102内部寄存器 
输 入:	addr
输 出:	parameter
描 述:	/
***********************************************/
unsigned int    A7102_Register_Read(unsigned char addr)
{
        unsigned int    parameter;
	
        //iSPI_Sck        = negative;
        GPIO_LOW(GPIOB,GPIO_Pin_5);
        mydelay();
       // iSPI_Scs        = negative;
		 GPIO_LOW(GPIOB,GPIO_Pin_4);
        
        addr           &= 0x0f;
        addr           |= 0x80;
        
        /* 读A7102-参数配置命令 */
        Byte_Write(addr);
        
        declare_sdo_input();
        
        /* 读A7102-参数配置数据 */
        parameter       = Word_Read();
        
        declare_sdo_output();
        
        //iSPI_Scs        = positive;
        GPIO_HIGH(GPIOB,GPIO_Pin_4);  
        /* 返回读取数值 */
        return    (parameter);
}
/***********************************************
函 数:	A7102_Status_Transmit()
功 能:	A7102设置为发射状态 
输 入:	/
输 出:	/
描 述:	A7102使用12.8M晶体为基准源 
***********************************************/
void    A7102_Status_Transmit(void)
{
#if  mode_switch
        iSPI_Tre        = negative;
        iSPI_Trs        = positive;
        iSPI_Tre        = positive;
#else
        #if mode_fifo
            /* standby mode */
            A7102_Register_Write(Reg_ModeControl,0x00d0);
            /* TX frequency */
            A7102_Register_Write(Reg_PLL_II,C_Tx_Frequency);
            /* 校准值 */
            A7102_Register_Write(Reg_Calibration,TX_Freq_Calibration);
            /* transimit mode */
            A7102_Register_Write(Reg_ModeControl,0x00d8); 
        #else
            /* standby mode */
            A7102_Register_Write(Reg_ModeControl,0x0050);
            /* TX frequency */
            A7102_Register_Write(Reg_PLL_II,C_Tx_Frequency);
            /* 校准值 */
            A7102_Register_Write(Reg_Calibration,TX_Freq_Calibration);
            /* transimit mode */
            A7102_Register_Write(Reg_ModeControl,0x0058);
        #endif
#endif
}
/***********************************************
函 数:	A7102_Receiver_Enable()
功 能:	A7102接收使能 
输 入:	/
输 出:	/
描 述:	A7102使用12.8M晶体为基准源 
***********************************************/
void    A7102_Receiver_Enable(void)
{
#if mode_fifo
        A7102_Register_Write(Reg_ModeControl,0x00c8);
#else
        A7102_Register_Write(Reg_ModeControl,0x0048);
#endif
}
/***********************************************
函 数:	A7102_Status_Receiver()
功 能:	A7102设置为接收状态 
输 入:	/
输 出:	/
描 述:	A7102使用12.8M晶体为基准源 
***********************************************/
void    A7102_Status_Receiver(void)
{
#if  mode_switch
        iSPI_Tre        = negative;
        iSPI_Trs        = negative;
        iSPI_Tre        = positive;
#else
        #if mode_fifo
            /* standby mode */
            A7102_Register_Write(Reg_ModeControl,0x00c0); 
            /* RX frequency */
            A7102_Register_Write(Reg_PLL_II,C_Rx_Frequency);
            /* 校准值 */
            A7102_Register_Write(Reg_Calibration,RX_Freq_Calibration);
            /* enable receive mode */
            A7102_Receiver_Enable();
        #else
            /* standby mode */
            A7102_Register_Write(Reg_ModeControl,0x0040);
            /* RX frequency */
            A7102_Register_Write(Reg_PLL_II,C_Rx_Frequency);
            /* 校准值 */
            A7102_Register_Write(Reg_Calibration,RX_Freq_Calibration);
            /* enable receive mode */
            A7102_Receiver_Enable();
        #endif
#endif
}
/***********************************************
函 数:	A7102_Calibration_Frequency()
功 能:	频率校准IF和VCO
输 入:	frequency(PLL II寄存器)
输 出:	/
描 述:	A7102使用12.8M晶体为基准源 
***********************************************/
unsigned int    A7102_Calibration_Frequency(unsigned int frequency)
{
        unsigned int temp,a=0;
        
        /* 设置频率 */
        A7102_Register_Write(Reg_PLL_II,frequency);

        /* 读出校准寄存器内值 */
        temp    = A7102_Register_Read(Reg_ModeControl);
        
        /* 打开校准功能 */
        temp   |= 0x0006;
        A7102_Register_Write(Reg_ModeControl,temp);
        
        /* 等待校准完成 */
        do
        {
            temp    = A7102_Register_Read(Reg_ModeControl);
			a++;
        }while((temp&0x0006)&&(a<1000));
        
        /* 获取校准的IF和VCO的补偿值 */
        temp    = A7102_Register_Read(Reg_Calibration);
        /* MVBS =1,MIFS = 1,在状态切换时使用手动校准 */
        temp   &= 0x00ef;
        temp   |= 0x4910;

        /* 返回校准的IF和VCO的补偿值 */
        return    temp;
}
/***********************************************
函 数:	A7102_Config_Chip()
功 能:	配置A7102参数寄存器 
输 入:	/
输 出:	/
描 述:	A7102使用12.8M晶体为基准源 
***********************************************/
void Delay_1s(u32 nTime);
void    A7102_Config_Chip(void)
{
//        uint    temp = 0x0000;

//        iSPI_Tre        = negative;
//        iSPI_Trs        = negative;

        /* rate = 50kbps,IF = 100k,CSC[2:0] = 001,System clock = 64*IF = 6.4M */
        A7102_Register_Write(Reg_SystemClock,data_rate);
        
        /* TX = 434MHZ * 2 = (12.8mhz/(RRC[3:0]+1)) * (IP[7:0]+(FP[15:0]/65535)),RRC[3:0] = 0000,IP[7:0] = 0100 0011 */
        /* MDIV = 0,IP选择范围32 - 127 */
        A7102_Register_Write(Reg_PLL_I,0x0043);
        /* FP[15:0],默认434Mhz */
        A7102_Register_Write(Reg_PLL_II,0xcfff);
        /* default 0x0000,无调校 */
        A7102_Register_Write(Reg_PLL_III,0x0000);
        /* PDL[2:0] =011,HFB =0(<500mhz),VCS[1:0] =00,CPS =0(normal),CPC[1:0] =01(charge pump 1mA) */
        /* VCS[1:0] = 01,VCO电流控制为1,SDPW = 1 */
        A7102_Register_Write(Reg_PLL_IV,0x066c);        
        
        
#if     mode_modulation  
        /* TME =1,发射带调制,GS =0,高斯滤波器关闭,FX = 12.8M,FDP = 101,FD = 0010 0000,
           频偏 fdev = FX/(RRC[3:0]+1)*(FD[7:0]*(2^(FDP[2:0]))/8/65536),
                     = 12800khz * 32 * (2^5) /(2^19) 
                     = 25khz */
        A7102_Register_Write(Reg_TXI,0x1520);
           
#else         
        /* TME =0,发射未带调制,GS =0,高斯滤波器关闭,FX = 12.8M,
           频偏 fdev = FX/(RRC[3:0]+1)*(FD[7:0]*(2^(FDP[2:0]))/8/65536),
                     = 12800khz * 128 * (2^5) /(2^19)
                     = 100khz */
        A7102_Register_Write(Reg_TXI,0x0580);    
#endif

        /* 发射功率设置为最大功率 */
        A7102_Register_Write(Reg_TXII,0x0337);//0X0335

        /* MPL[1:0] = 01,SLF[2:0] = 100,ETH[1:0] = 00,无位错误选择,DMOS = 0,DMG[1:0] = 01,增益X3 */
        /* BW[1:0] = 00,50Khz,ULS= 1,下旁波带,HGM =1,高增益 */
        A7102_Register_Write(Reg_RXI,0x1813);

        /* Amic推荐值 */
        A7102_Register_Write(Reg_RXII,0x500b);

        /* CDM =1,carrier 打开 */
        A7102_Register_Write(Reg_ADC,0x0000);
        
        /* TX fifo <8字节,RX fifo > 56字节时报警, */
        A7102_Register_Write(Reg_FIFO,0x4000|(C_FIFO_Byte-1));
        
        /* MCS = 0,Manchester打开,FECS = 0,FEC打开,CRCS =0,CRC打开,IDL =1，id code =4字节,PML[1:0] = 11,前导4字节 */
        A7102_Register_Write(Reg_Code,0x150f);

#if     mode_switch
        /* 打开位时钟,管脚控制收发,无载波侦测 */
        A7102_Register_Write(Reg_PinControl,0x0222);
#else
        // 打开位时钟,寄存器控制收发,无载波侦测 */
        A7102_Register_Write(Reg_PinControl,0x0022);
#endif

        /* VTL[2:0] = 100,VTH = VDD-0.5V, VTH[2:0] = 100,VTL =0.5V */
        A7102_Register_Write(Reg_Calibration,0x4886);
        
   	    A7102_Register_Write(Reg_ModeControl,0x00e0); //STB2==>syn mode
	    //delay 80us	
	    /*
        for(temp = 0;temp < 0xffff;temp++)
        {
            _nop_();
            _nop_();
        }*///seekii
		//Delay_1s(1);
		Delayms(200);
}
/***********************************************
函 数:	Initialisation_RF()
功 能:	上电初始化A7102
输 入:	/
输 出:	/
描 述:	/
***********************************************/
unsigned char rece_buff[4]={0x00,0x00,0x00,0x00};
void    Initialisation_RF(void)
{
//        uint i;
        A7102_Reset_Chip();

   	Delayms(200);
        /* XCC =1(crystal high current),XS =1(crystal on) */
        /* CGS = 0,因为使用标准12.8mhz晶体 */
        A7102_Register_Write(Reg_Crystal,0x0015);
	/* FMT =0,normal mode,FMS =1,FIFO mode,CER = 1,
	RF chip 使能,PLLE = 0,使用12.8Mhz标准晶体,PLL关闭 */
        A7102_Register_Write(Reg_ModeControl,0x00c0); //STB1==>standby mode
    
		//if(rf)
		//{A7102_Id_Write(&lock_id_buff);}
		//else
		//{A7102_Id_Write(&unlock_id_buff);}
	A7102_Id_Write(&unlock_id_buff[0]); 

		//A7102_Id_Read(rece_buff);
		//DebugSendBuf(rece_buff,4);
		
        A7102_Config_Chip();        
        TX_Freq_Calibration    = A7102_Calibration_Frequency(C_Tx_Frequency); 
        RX_Freq_Calibration    = A7102_Calibration_Frequency(C_Rx_Frequency);
     
        /* reset fifo */
        A7102_Reset_FifoRX();
        A7102_Reset_FifoTX();
        /* 默认接收状态 */
        A7102_Status_Receiver();     
}
uchar a7102_send_data(unsigned char *send_dat)
{
    unsigned int t=0;
    A7102_Fifo_Write(send_dat);
    A7102_Status_Transmit();           
    while((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3))&&(t<700))
	{
	   t++;
	   //Delayms(2);
	   Delayms(1);
	}
	if(t>699){
		//A7102_Status_Receiver();
		return(wrong);
		}
    A7102_Status_Receiver();
	return(right);
}



