/***********************************************
		*	copyrite (c) 2007
		*�ļ���: 	  A7102.c
		*����:		  larry
		*��ǰ�汾:	  v1.0
		*����ʱ��:        2007-6-11 09:45
		*�������:
		
		*����:            A7102�����������,��MCU��IO
		*                 �ӿ��޹�,���ⲿ.h�ļ��궨��				     
************************************************/		
#include    "A7102.h"
//#include    "include.h"
#include "discover_board.h"
#include "stm8l15x.h"
uchar rf_power;
    /* �շ�Ƶ��У׼ֵ���� */
unsigned    int  TX_Freq_Calibration;
unsigned    int  RX_Freq_Calibration;
unsigned    char  unlock_id_buff[0x04]    = {0x6C,0x61,0x6E,0x62};//��ʼ�� ID  
unsigned    char  lock_id_buff[0x04]    = {0x6C,0x6F,0x63,0x6B};//����  ID 
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
�� ��:	Byte_Write()
�� ��:	д8bit���ݵ�sck��sdio
�� ��:	byte
�� ��:	/
�� ��:	����λ,bit7����,sck����Ե���� 
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
�� ��:	Byte_Read()
�� ��:	��8bit���ݴ�sck��sdio
�� ��:	/
�� ��:	mybyte
�� ��:	����λ,bit7����,sck����Ե���� 
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
        
        /* ���ض�ȡ��ֵ */
		A7102_RX(0);
        return    (mybyte);
}
/***********************************************
�� ��:	Word_Write()
�� ��:	д16bit���ݵ�sck��sdio
�� ��:	myword
�� ��:	/
�� ��:	����λ,bit15����,sck����Ե���� 
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
�� ��:	Word_Read()
�� ��:	��16bit���ݴ�sck��sdio
�� ��:	/
�� ��:	myword
�� ��:	����λ,bit16����,sck����Ե���� 
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
        /* ���ض�ȡ��ֵ */
        return    (myword);
}
/***********************************************
�� ��:	A7102_Reset_Chip()
�� ��:	��λA7102оƬ 
�� ��:	/
�� ��:	/
�� ��:	/
***********************************************/
void    A7102_Reset_Chip(void)
{
        //iSPI_Scs        = negative;  

        GPIO_LOW(GPIOB,GPIO_Pin_4);
        /* дA7102-Reset���� */
      Byte_Write(0x7a);     

           GPIO_HIGH(GPIOB,GPIO_Pin_4);  
        //iSPI_Scs        = positive;
}
/***********************************************
�� ��:	A7102_Reset_FifoTX()
�� ��:	��λA7102����FIFOָ�� 
�� ��:	/
�� ��:	/
�� ��:	/
***********************************************/
void    A7102_Reset_FifoTX(void)
{
        //iSPI_Sck        = negative;
	
                GPIO_LOW(GPIOB,GPIO_Pin_5);
		//iSPI_Scs        = negative;
        GPIO_LOW(GPIOB,GPIO_Pin_4);
        /* дA7102-FifoTX���� */
        Byte_Write(0x6a);
        GPIO_HIGH(GPIOB,GPIO_Pin_4);  
        //iSPI_Scs        = positive;
}
/***********************************************
�� ��:	A7102_Reset_FifoRX()
�� ��:	��λA7102����FIFOָ�� 
�� ��:	/
�� ��:	/
�� ��:	/
***********************************************/
void    A7102_Reset_FifoRX(void)
{
        //iSPI_Sck        = negative;
        GPIO_LOW(GPIOB,GPIO_Pin_5);
        //iSPI_Scs        = negative;
        GPIO_LOW(GPIOB,GPIO_Pin_4);
        /* дA7102-FifoRX���� */
        Byte_Write(0xea);
        GPIO_HIGH(GPIOB,GPIO_Pin_4);  
        //iSPI_Scs        = positive;
}
/***********************************************
�� ��:	A7102_Fifo_Read()
�� ��:	��FIFO���� 
�� ��:	read_buff
�� ��:	/
�� ��:	/
***********************************************/
void    A7102_Fifo_Read(unsigned char *read_buff)
{
        unsigned char i;
	
        //iSPI_Sck        = negative;
		 GPIO_LOW(GPIOB,GPIO_Pin_5);
		 GPIO_LOW(GPIOB,GPIO_Pin_4);
              

		//iSPI_Scs        = negative;
        
        /* ��A7102-FIFO���� */
        Byte_Write(0xca);
        
        declare_sdo_input();
        
        /* ��A7102-FIFO���� */
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
�� ��:	A7102_Fifo_Write()
�� ��:	дFIFO���� 
�� ��:	write_buff
�� ��:	/
�� ��:	/
***********************************************/
void    A7102_Fifo_Write(unsigned char *write_buff)
{
        unsigned char i;
        //iSPI_Sck        = negative;
		 GPIO_LOW(GPIOB,GPIO_Pin_5);
		Delayms(1);
		//iSPI_Scs        = negative;
		 GPIO_LOW(GPIOB,GPIO_Pin_4);
        /* дA7102-FIFO���� */
        Byte_Write(0x5a);
        /* дA7102-FIFO���� */
        for(i = 0x00;i < C_FIFO_Byte;i++)
        {
            Byte_Write(*write_buff);
            
            write_buff ++;
        }
        GPIO_HIGH(GPIOB,GPIO_Pin_4);  
        //iSPI_Scs        = positive;
}
/***********************************************
�� ��:	A7102_Id_Write()
�� ��:	дID��A7102�ڲ��Ĵ��� 
�� ��:	buff_id[]
�� ��:	/
�� ��:	дID����һ����д�� 
***********************************************/
void    A7102_Id_Write(unsigned char *id_buff)
{
        unsigned char i;
	
        //iSPI_Sck        = negative;
		 GPIO_LOW(GPIOB,GPIO_Pin_5);
        //iSPI_Scs        = negative;
        GPIO_LOW(GPIOB,GPIO_Pin_4);
        /* дA7102-ID���� */
        Byte_Write(0x2a);
        
        /* дA7102-ID���� */
        for(i = 0x00;i < 0x04;i++)
        {
            Byte_Write(*id_buff);
            
            id_buff ++;
        }
        GPIO_HIGH(GPIOB,GPIO_Pin_4);  
        //iSPI_Scs        = positive;
}
/***********************************************
�� ��:	A7102_Id_Read()
�� ��:	��ID��A7102�ڲ��Ĵ��� 
�� ��:	/
�� ��:	buff_id[]
�� ��:	��ID����һ���Զ��� 
***********************************************/
void    A7102_Id_Read(unsigned char *id_buff)
{
        unsigned char i;
	
        //iSPI_Sck        = negative;
		 GPIO_LOW(GPIOB,GPIO_Pin_5);
        //iSPI_Scs        = negative;
        GPIO_LOW(GPIOB,GPIO_Pin_4);
        /* ��A7102-ID���� */
        Byte_Write(0xaa);
        
        declare_sdo_input();
        
        /* ��A7102-ID���� */
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
�� ��:	A7102_Register_Write()
�� ��:	д�������ݵ�A7102�ڲ��Ĵ��� 
�� ��:	addr,parameter
�� ��:	/
�� ��:	/
***********************************************/
void    A7102_Register_Write(unsigned char addr,unsigned int parameter)
{
        //iSPI_Sck        = negative;
        GPIO_LOW(GPIOB,GPIO_Pin_5);
        mydelay();
        //iSPI_Scs        = negative;
        GPIO_LOW(GPIOB,GPIO_Pin_4);
        addr           &= 0x0f;
        /* дA7102-������������ */
        Byte_Write(addr);
        
        /* дA7102-������������ */
        Word_Write(parameter);
        GPIO_HIGH(GPIOB,GPIO_Pin_4);  
        //iSPI_Scs        = positive;
}
/***********************************************
�� ��:	A7102_Register_Read()
�� ��:	���������ݴ�A7102�ڲ��Ĵ��� 
�� ��:	addr
�� ��:	parameter
�� ��:	/
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
        
        /* ��A7102-������������ */
        Byte_Write(addr);
        
        declare_sdo_input();
        
        /* ��A7102-������������ */
        parameter       = Word_Read();
        
        declare_sdo_output();
        
        //iSPI_Scs        = positive;
        GPIO_HIGH(GPIOB,GPIO_Pin_4);  
        /* ���ض�ȡ��ֵ */
        return    (parameter);
}
/***********************************************
�� ��:	A7102_Status_Transmit()
�� ��:	A7102����Ϊ����״̬ 
�� ��:	/
�� ��:	/
�� ��:	A7102ʹ��12.8M����Ϊ��׼Դ 
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
            /* У׼ֵ */
            A7102_Register_Write(Reg_Calibration,TX_Freq_Calibration);
            /* transimit mode */
            A7102_Register_Write(Reg_ModeControl,0x00d8); 
        #else
            /* standby mode */
            A7102_Register_Write(Reg_ModeControl,0x0050);
            /* TX frequency */
            A7102_Register_Write(Reg_PLL_II,C_Tx_Frequency);
            /* У׼ֵ */
            A7102_Register_Write(Reg_Calibration,TX_Freq_Calibration);
            /* transimit mode */
            A7102_Register_Write(Reg_ModeControl,0x0058);
        #endif
#endif
}
/***********************************************
�� ��:	A7102_Receiver_Enable()
�� ��:	A7102����ʹ�� 
�� ��:	/
�� ��:	/
�� ��:	A7102ʹ��12.8M����Ϊ��׼Դ 
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
�� ��:	A7102_Status_Receiver()
�� ��:	A7102����Ϊ����״̬ 
�� ��:	/
�� ��:	/
�� ��:	A7102ʹ��12.8M����Ϊ��׼Դ 
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
            /* У׼ֵ */
            A7102_Register_Write(Reg_Calibration,RX_Freq_Calibration);
            /* enable receive mode */
            A7102_Receiver_Enable();
        #else
            /* standby mode */
            A7102_Register_Write(Reg_ModeControl,0x0040);
            /* RX frequency */
            A7102_Register_Write(Reg_PLL_II,C_Rx_Frequency);
            /* У׼ֵ */
            A7102_Register_Write(Reg_Calibration,RX_Freq_Calibration);
            /* enable receive mode */
            A7102_Receiver_Enable();
        #endif
#endif
}
/***********************************************
�� ��:	A7102_Calibration_Frequency()
�� ��:	Ƶ��У׼IF��VCO
�� ��:	frequency(PLL II�Ĵ���)
�� ��:	/
�� ��:	A7102ʹ��12.8M����Ϊ��׼Դ 
***********************************************/
unsigned int    A7102_Calibration_Frequency(unsigned int frequency)
{
        unsigned int temp,a=0;
        
        /* ����Ƶ�� */
        A7102_Register_Write(Reg_PLL_II,frequency);

        /* ����У׼�Ĵ�����ֵ */
        temp    = A7102_Register_Read(Reg_ModeControl);
        
        /* ��У׼���� */
        temp   |= 0x0006;
        A7102_Register_Write(Reg_ModeControl,temp);
        
        /* �ȴ�У׼��� */
        do
        {
            temp    = A7102_Register_Read(Reg_ModeControl);
			a++;
        }while((temp&0x0006)&&(a<1000));
        
        /* ��ȡУ׼��IF��VCO�Ĳ���ֵ */
        temp    = A7102_Register_Read(Reg_Calibration);
        /* MVBS =1,MIFS = 1,��״̬�л�ʱʹ���ֶ�У׼ */
        temp   &= 0x00ef;
        temp   |= 0x4910;

        /* ����У׼��IF��VCO�Ĳ���ֵ */
        return    temp;
}
/***********************************************
�� ��:	A7102_Config_Chip()
�� ��:	����A7102�����Ĵ��� 
�� ��:	/
�� ��:	/
�� ��:	A7102ʹ��12.8M����Ϊ��׼Դ 
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
        /* MDIV = 0,IPѡ��Χ32 - 127 */
        A7102_Register_Write(Reg_PLL_I,0x0043);
        /* FP[15:0],Ĭ��434Mhz */
        A7102_Register_Write(Reg_PLL_II,0xcfff);
        /* default 0x0000,�޵�У */
        A7102_Register_Write(Reg_PLL_III,0x0000);
        /* PDL[2:0] =011,HFB =0(<500mhz),VCS[1:0] =00,CPS =0(normal),CPC[1:0] =01(charge pump 1mA) */
        /* VCS[1:0] = 01,VCO��������Ϊ1,SDPW = 1 */
        A7102_Register_Write(Reg_PLL_IV,0x066c);        
        
        
#if     mode_modulation  
        /* TME =1,���������,GS =0,��˹�˲����ر�,FX = 12.8M,FDP = 101,FD = 0010 0000,
           Ƶƫ fdev = FX/(RRC[3:0]+1)*(FD[7:0]*(2^(FDP[2:0]))/8/65536),
                     = 12800khz * 32 * (2^5) /(2^19) 
                     = 25khz */
        A7102_Register_Write(Reg_TXI,0x1520);
           
#else         
        /* TME =0,����δ������,GS =0,��˹�˲����ر�,FX = 12.8M,
           Ƶƫ fdev = FX/(RRC[3:0]+1)*(FD[7:0]*(2^(FDP[2:0]))/8/65536),
                     = 12800khz * 128 * (2^5) /(2^19)
                     = 100khz */
        A7102_Register_Write(Reg_TXI,0x0580);    
#endif

        /* ���书������Ϊ����� */
        A7102_Register_Write(Reg_TXII,0x0337);//0X0335

        /* MPL[1:0] = 01,SLF[2:0] = 100,ETH[1:0] = 00,��λ����ѡ��,DMOS = 0,DMG[1:0] = 01,����X3 */
        /* BW[1:0] = 00,50Khz,ULS= 1,���Բ���,HGM =1,������ */
        A7102_Register_Write(Reg_RXI,0x1813);

        /* Amic�Ƽ�ֵ */
        A7102_Register_Write(Reg_RXII,0x500b);

        /* CDM =1,carrier �� */
        A7102_Register_Write(Reg_ADC,0x0000);
        
        /* TX fifo <8�ֽ�,RX fifo > 56�ֽ�ʱ����, */
        A7102_Register_Write(Reg_FIFO,0x4000|(C_FIFO_Byte-1));
        
        /* MCS = 0,Manchester��,FECS = 0,FEC��,CRCS =0,CRC��,IDL =1��id code =4�ֽ�,PML[1:0] = 11,ǰ��4�ֽ� */
        A7102_Register_Write(Reg_Code,0x150f);

#if     mode_switch
        /* ��λʱ��,�ܽſ����շ�,���ز���� */
        A7102_Register_Write(Reg_PinControl,0x0222);
#else
        // ��λʱ��,�Ĵ��������շ�,���ز���� */
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
�� ��:	Initialisation_RF()
�� ��:	�ϵ��ʼ��A7102
�� ��:	/
�� ��:	/
�� ��:	/
***********************************************/
unsigned char rece_buff[4]={0x00,0x00,0x00,0x00};
void    Initialisation_RF(void)
{
//        uint i;
        A7102_Reset_Chip();

   	Delayms(200);
        /* XCC =1(crystal high current),XS =1(crystal on) */
        /* CGS = 0,��Ϊʹ�ñ�׼12.8mhz���� */
        A7102_Register_Write(Reg_Crystal,0x0015);
	/* FMT =0,normal mode,FMS =1,FIFO mode,CER = 1,
	RF chip ʹ��,PLLE = 0,ʹ��12.8Mhz��׼����,PLL�ر� */
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
        /* Ĭ�Ͻ���״̬ */
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



