/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ���������������޹�˾

 ******************************************************************************
  �� �� ��   : RNF24L01.C
  �� �� ��   : ����
  ��    ��   : huangli
  ��������   : 2016��12��16�� ������
  ����޸�   :
  ��������   : NRF24L01��������
  �����б�   :
              NRF24L01RxPacket
              NRF24L01SpiReadBuf
              NRF24L01SpiReadByte
              NRF24L01SpiRwReg
              NRF24L01SpiWriteBuf
              NRF24L01SpiWriteByte
              NRF24L01TxPacket
              RNF24L01Delay
              RNF24L01Iintial
              RNF24L01SetRxMode
  �޸���ʷ   :
  1.��    ��   : 2016��12��16�� ������
    ��    ��   : huangli
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include <reg52.h>
#include <intrins.h>
#include "RNF24L01.h"
#include <typedef.h>
/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/
sbit CE=P1^5;	 /* RX/TXģʽѡ��� */
sbit IRQ=P1^1;	 /* �������ж϶� */
sbit CSN=P1^2;	 /* SPIƬѡ��//����SS */
sbit MOSI=P1^0;  /* SPI��������ӻ������ */
sbit MISO=P1^3;  /* SPI��������ӻ������ */
sbit SCK=P1^4;	 /* SPIʱ�Ӷ� */

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/
//*********************************************NRF24L01*************************************
#define TX_ADR_WIDTH    5    /* ���͵�ַ���� */
#define RX_ADR_WIDTH    5    /* ���ܵ�ַ���� */
#define TX_PLOAD_WIDTH  9    /* ������Ч���ݳ��� */
#define RX_PLOAD_WIDTH  9    /* ������Ч���ݳ��� */
uint const TX_ADDRESS[TX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x02}; /* ���͵�ַ */
uint const RX_ADDRESS[RX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01}; /* ���ܵ�ַ */
//***************************************NRF24L01�Ĵ���ָ��********************************
#define READ_REG        0x00  /*  ���Ĵ���ָ�� */
#define WRITE_REG       0x20  /*  д�Ĵ���ָ��  */
#define RD_RX_PLOAD     0x61  /*  ��ȡ��������ָ��  */
#define WR_TX_PLOAD     0xA0  /*  д��������ָ��  */
#define FLUSH_TX        0xE1  /*  ��ϴ���� FIFOָ��  */
#define FLUSH_RX        0xE2  /*  ��ϴ���� FIFOָ��  */
#define REUSE_TX_PL     0xE3  /*  �����ظ�װ������ָ��  */
#define NOP             0xFF  /*  ����  */
/* *************************************SPI(nRF24L01)�Ĵ�����ַ********************/
#define CONFIG          0x00  /*  �����շ�״̬��CRCУ��ģʽ�Լ��շ�״̬��Ӧ��ʽ  */
#define EN_AA           0x01  /*  �Զ�Ӧ�������� */
#define EN_RXADDR       0x02  /*  �����ŵ����� */
#define SETUP_AW        0x03  /*  �շ���ַ�������� */
#define SETUP_RETR      0x04  /*  �Զ��ط��������� */
#define RF_CH           0x05  /*  ����Ƶ������ */
#define RF_SETUP        0x06  /*  �������ʡ����Ĺ������� */
#define STATUS          0x07  /*  ״̬�Ĵ��� */
#define OBSERVE_TX      0x08  /*  ���ͼ�⹦�� */
#define CD              0x09  /*  ��ַ��� */           
#define RX_ADDR_P0      0x0A  /*  Ƶ��0�������ݵ�ַ */
#define RX_ADDR_P1      0x0B  /*  Ƶ��1�������ݵ�ַ */
#define RX_ADDR_P2      0x0C  /*  Ƶ��2�������ݵ�ַ */
#define RX_ADDR_P3      0x0D  /*  Ƶ��3�������ݵ�ַ */
#define RX_ADDR_P4      0x0E  /*  Ƶ��4�������ݵ�ַ */
#define RX_ADDR_P5      0x0F  /*  Ƶ��5�������ݵ�ַ */
#define TX_ADDR         0x10  /*  ���͵�ַ�Ĵ��� */
#define RX_PW_P0        0x11  /*  ����Ƶ��0�������ݳ��� */
#define RX_PW_P1        0x12  /*  ����Ƶ��0�������ݳ��� */
#define RX_PW_P2        0x13  /*  ����Ƶ��0�������ݳ��� */
#define RX_PW_P3        0x14  /*  ����Ƶ��0�������ݳ��� */
#define RX_PW_P4        0x15  /*  ����Ƶ��0�������ݳ��� */
#define RX_PW_P5        0x16  /*  ����Ƶ��0�������ݳ��� */
#define FIFO_STATUS     0x17  /*  FIFOջ��ջ��״̬�Ĵ������� */

/*----------------------------------------------*
 *ȫ�ֱ���                                      *
 *----------------------------------------------*/
uint  bdata sta;   //״̬��־
sbit RX_DR =sta^6;
sbit TX_DS =sta^5;
sbit MAX_RT =sta^4;

/*****************************************************************************
 �� �� ��  : RNF24L01Delay
 ��������  : RNF24L01 ��ʱ����
 �������  : unsigned char n  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
*****************************************************************************/
void RNF24L01Delay(unsigned char n)
{
    for(;n>0;n--)
    {
        _nop_();
    }
  
}
/*****************************************************************************
 �� �� ��  : RNF24L01Iintial
 ��������  : RNF24L01��ʼ��
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
*****************************************************************************/
void RNF24L01Iintial(void)
{
    RNF24L01Delay(100);
    CE = 0;     /* chip enable */
    CSN = 1;    /* SPI disable */
    SCK = 0;    /* set clk */
    IRQ = 1;
    NRF24L01SpiWriteBuf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);       /* д���ص�ַ */
    NRF24L01SpiWriteBuf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); /* д���ն�0��ַ */
    NRF24L01SpiRwReg(WRITE_REG + EN_AA, 0x3f);                      /* Ƶ��0~5,ACKӦ������  */
    NRF24L01SpiRwReg(WRITE_REG + EN_RXADDR, 0x03f);              /* �������յ�ַֻ��Ƶ��0~5 */                                             
    NRF24L01SpiRwReg(WRITE_REG + RF_CH, 0);               /* �ŵ�����Ϊ2.4GHZ���շ�����һ�� */
    NRF24L01SpiRwReg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH);     /* ���ý������ݳ���,=32�ֽ� */
    NRF24L01SpiRwReg(WRITE_REG + RF_SETUP, 0x07); /* ���÷�������Ϊ1MHZ,���书��Ϊ���ֵ0dB */
}


/*****************************************************************************
 �� �� ��  : RNF24L01SetRxMode
 ��������  : ���ݽ�������
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
*****************************************************************************/
void RNF24L01SetRxMode(void)
{
   CE=0;
   NRF24L01SpiRwReg(WRITE_REG + CONFIG, 0x0f);     /*  IRQ�շ�����ж���Ӧ��16λCRC ������ */
   CE = 1; 
   RNF24L01Delay(130);
}


/*****************************************************************************
 �� �� ��  : NRF24L01SpiWriteByte
 ��������  : NRF24L01��SPIдʱ�� ���ֽ�
 �������  : uint uchar  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
*****************************************************************************/
uint NRF24L01SpiWriteByte(uint uchar)
{
    uint bit_ctr;
	
	/* output 8-bit  */
    for(bit_ctr=0;bit_ctr<8;bit_ctr++)  
    {
	  MOSI = (uchar & 0x80);          /*  output 'uchar', MSB to MOSI */
	  uchar = (uchar << 1);           /*  shift next bit into MSB.. */
	  SCK = 1;                        /*  Set SCK high.. */
	  uchar |= MISO;                  /*  capture current MISO bit */
	  SCK = 0;                        /*  ..then set SCK low again */
    }
    return(uchar);                    /*  return read uchar */
}

/*****************************************************************************
 �� �� ��  : NRF24L01SpiReadByte
 ��������  : NRF24L01��SPI������
 �������  : uchar reg  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
*****************************************************************************/
uchar NRF24L01SpiReadByte(uchar reg)
{
	 uchar reg_val; 
	 /*  CSN low, initialize SPI communication... */
	 CSN = 0; 
	 
	 NRF24L01SpiWriteByte(reg);            /*  Select register to read from.. */
	 reg_val = NRF24L01SpiWriteByte(0);    /*  ..then read registervalue */

	 /*  CSN high, terminate SPI communication */ 
	 CSN = 1;                
	 return(reg_val);                      /*  return register value */
}


/*****************************************************************************
 �� �� ��  : NRF24L01SpiRwReg
 ��������  : NRF24L01��д�Ĵ�������
 �������  : uchar reg    
             uchar value  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
*****************************************************************************/
uint NRF24L01SpiRwReg(uchar reg, uchar value)
{
	 uint status;
	 
	 CSN = 0;                              /*  CSN low, init SPI transaction */
	 status = NRF24L01SpiWriteByte(reg);   /*  select register */
	 NRF24L01SpiWriteByte(value);          /*  ..and write value to it.. */
	 CSN = 1;                              /*  CSN high again */
	 
	 return(status);                       /*  return nRF24L01 status uchar */
}

/*****************************************************************************
 �� �� ��  : NRF24L01SpiReadBuf
 ��������  : ���ڶ����ݣ�reg��Ϊ�Ĵ�����ַ��pBuf��Ϊ���������ݵ�ַ��uchars��
             �������ݵĸ���
 �������  : uchar reg     
             uchar *pBuf   
             uchar uchars  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
*****************************************************************************/
uint NRF24L01SpiReadBuf(uchar reg, uchar *pBuf, uchar uchars)
{
	 uint status,uchar_ctr; 
	 CSN = 0;                                    /*  Set CSN low, init SPI tranaction */
	 status = NRF24L01SpiWriteByte(reg);         /*  Select register to write to and read status uchar */
	 
	 for(uchar_ctr=0;uchar_ctr<uchars;uchar_ctr++)
	 {
	     pBuf[uchar_ctr] = NRF24L01SpiWriteByte(0); 
	 }
	 
	 CSN = 1;                           	 
	 return(status);                             /*  return nRF24L01 status uchar */
}

/*****************************************************************************
 �� �� ��  : NRF24L01SpiWriteBuf
 ��������  : ����д���ݣ�Ϊ�Ĵ�����ַ��pBuf��Ϊ��д�����ݵ�ַ��uchars��д��
             ���ݵĸ���
 �������  : uchar reg     
             uchar *pBuf   
             uchar uchars  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
*****************************************************************************/
uint NRF24L01SpiWriteBuf(uchar reg, uchar *pBuf, uchar uchars)
{
	 uint status,uchar_ctr; 
	 CSN = 0;            /* SPIʹ�� */       
	 status = NRF24L01SpiWriteByte(reg);   
	 for(uchar_ctr=0; uchar_ctr<uchars; uchar_ctr++) /* */ 
	 NRF24L01SpiWriteByte(*pBuf++);
	 CSN = 1;           //�ر�SPI
	 return(status);    /*   */
}

/*****************************************************************************
 �� �� ��  : NRF24L01RxPacket
 ��������  : ���ݶ�ȡ�����rx_buf���ջ�������
 �������  : unsigned char* rx_buf  
 �������  : ��
 �� �� ֵ  : unsigned
 ���ú���  : 
 ��������  : 
*****************************************************************************/
unsigned char NRF24L01RxPacket(unsigned char* rx_buf)
{
     unsigned char revale=0;
	 sta=NRF24L01SpiReadByte(STATUS); /*  ��ȡ״̬�Ĵ������ж����ݽ���״�� */
	 
	 if(RX_DR)    /*  �ж��Ƿ���յ����� */
	 {
	     CE = 0;    /* SPIʹ�� */
	     NRF24L01SpiReadBuf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH); /*  read receive payload from RX_FIFO buffer */
	     revale =1;   /* ��ȡ������ɱ�־ */
	 }

	 /* ���յ����ݺ�RX_DR,TX_DS,MAX_PT���ø�Ϊ1��ͨ��д1������жϱ�־*/
	 NRF24L01SpiRwReg(WRITE_REG+STATUS,sta);  
	 return revale;
}

/*****************************************************************************
 �� �� ��  : NRF24L01TxPacket
 ��������  : ����tx buf ������
 �������  : unsigned char * tx_buf  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
*****************************************************************************/
void NRF24L01TxPacket(unsigned char * tx_buf)
{
	 CE=0;   /* StandBy Iģʽ */
	 //NRF24L01SpiWriteBuf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); /*  װ�ؽ��ն˵�ַ */
	 NRF24L01SpiWriteBuf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH);     /*  װ������  */
	 NRF24L01SpiRwReg(WRITE_REG + CONFIG, 0x0e);      /*  IRQ�շ�����ж���Ӧ��16λCRC�������� */
	 CE=1;   /* �ø�CE,�������ݷ��� */
	 RNF24L01Delay(100);
	 CE=0; 
}