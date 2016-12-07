#include    "A7102define.h"
//#include 	"include.h"
#include "discover_board.h"

/****************************************/
#define 	_nop_()		Delayms(1)
#define     positive              1
#define     negative              0

#define     mode_fifo             1                        /* 1=fifo mode ; 0=direct mode */
#define     mode_switch           0                        /* 1=pin(trs,tre) control ; 0= register control */
#define     mode_modulation       1                        /* 1=transimit with modulation ; 0=transmit without modulation */

#define     delay_A7102_us()     Delayms(1);// _nop_();_nop_();_nop_();_nop_();_nop_()

#define     Reg_SystemClock       0x00
#define     Reg_PLL_I             0x01
#define     Reg_PLL_II            0x02
#define     Reg_PLL_III           0x03
#define     Reg_PLL_IV            0x04
#define     Reg_Crystal           0x05
#define     Reg_TXI               0x06
#define     Reg_TXII              0x07
#define     Reg_RXI               0x08
#define     Reg_RXII              0x09
#define     Reg_ADC               0x0a
#define     Reg_FIFO              0x0b
#define     Reg_Code              0x0c
#define     Reg_PinControl        0x0d
#define     Reg_Calibration       0x0e
#define     Reg_ModeControl       0x0f
/****************************************/
#define  uchar  unsigned char

    /* FIFO发送和接收一次的字节数(十六进制) */
#define     C_FIFO_Byte           0x0b  //0x08

    /* Tx = 433.92Mhz */
//#define     C_Tx_Frequency        0xcccc
//小罗给的参数20130706
#define     C_Tx_Frequency  	0xcb32
#define     C_Rx_Frequency        0xcf32//0xd0cb

    /* Rx = 434.02Mhz */                                   /* 下变频 */
//#define     C_Rx_Frequency        0xd0cb
//#define     data_rate                          0x0079 //   0x3079 
#define     data_rate             0x3079 


    /* 收发频率校准值保存 */
//unsigned    int  TX_Freq_Calibration;
//unsigned    int  RX_Freq_Calibration;
    /* RF识别ID */
//unsigned    char  id_buff[0x04]    = {0x6C,0x61,0x6E,0x62};//{0x12,0x34,0x56,0x78};   
/****************************************/


//#define     declare_sdo_input()   iSPI_Sdo   =  positive
#define     declare_sdo_input()	  GPIO_LOW(GPIOB,GPIO_Pin_6);	
#define     declare_sdo_output()   A7102_RX(0);
/****************************************/
/****************************************/
void    Initialisation_RF(void);
void    A7102_Reset_FifoTX(void);
void    A7102_Reset_FifoRX(void);
void    A7102_Receiver_Enable(void);
void    A7102_Status_Receiver(void);
void    A7102_Status_Transmit(void);
//uchar   send_data(void);
void    A7102_Fifo_Read(unsigned char *read_buff);
void    A7102_Fifo_Write(unsigned char *write_buff);

//void    a7102_data_deal_remote_control(void);
uchar a7102_send_data(unsigned char *send_dat);

