/******************************************************************************

                  版权所有 (C), 2001-2011, 神州数码网络有限公司

 ******************************************************************************
  文 件 名   : RNF24L01.C
  版 本 号   : 初稿
  作    者   : huangli
  生成日期   : 2016年12月16日 星期五
  最近修改   :
  功能描述   : NRF24L01驱动代码
  函数列表   :
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
  修改历史   :
  1.日    期   : 2016年12月16日 星期五
    作    者   : huangli
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include <reg52.h>
#include <intrins.h>
#include "RNF24L01.h"
#include <typedef.h>
/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/
sbit CE=P1^5;	 /* RX/TX模式选择端 */
sbit IRQ=P1^1;	 /* 可屏蔽中断端 */
sbit CSN=P1^2;	 /* SPI片选端//就是SS */
sbit MOSI=P1^0;  /* SPI主机输出从机输入端 */
sbit MISO=P1^3;  /* SPI主机输出从机输出端 */
sbit SCK=P1^4;	 /* SPI时钟端 */

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
//*********************************************NRF24L01*************************************
#define TX_ADR_WIDTH    5    /* 发送地址宽度 */
#define RX_ADR_WIDTH    5    /* 接受地址宽度 */
#define TX_PLOAD_WIDTH  9    /* 发送有效数据长度 */
#define RX_PLOAD_WIDTH  9    /* 接受有效数据长度 */
uint const TX_ADDRESS[TX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x02}; /* 发送地址 */
uint const RX_ADDRESS[RX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01}; /* 接受地址 */
//***************************************NRF24L01寄存器指令********************************
#define READ_REG        0x00  /*  读寄存器指令 */
#define WRITE_REG       0x20  /*  写寄存器指令  */
#define RD_RX_PLOAD     0x61  /*  读取接收数据指令  */
#define WR_TX_PLOAD     0xA0  /*  写待发数据指令  */
#define FLUSH_TX        0xE1  /*  冲洗发送 FIFO指令  */
#define FLUSH_RX        0xE2  /*  冲洗接收 FIFO指令  */
#define REUSE_TX_PL     0xE3  /*  定义重复装载数据指令  */
#define NOP             0xFF  /*  保留  */
/* *************************************SPI(nRF24L01)寄存器地址********************/
#define CONFIG          0x00  /*  配置收发状态，CRC校验模式以及收发状态响应方式  */
#define EN_AA           0x01  /*  自动应答功能设置 */
#define EN_RXADDR       0x02  /*  可用信道设置 */
#define SETUP_AW        0x03  /*  收发地址宽度设置 */
#define SETUP_RETR      0x04  /*  自动重发功能设置 */
#define RF_CH           0x05  /*  工作频率设置 */
#define RF_SETUP        0x06  /*  发射速率、功耗功能设置 */
#define STATUS          0x07  /*  状态寄存器 */
#define OBSERVE_TX      0x08  /*  发送监测功能 */
#define CD              0x09  /*  地址检测 */           
#define RX_ADDR_P0      0x0A  /*  频道0接收数据地址 */
#define RX_ADDR_P1      0x0B  /*  频道1接收数据地址 */
#define RX_ADDR_P2      0x0C  /*  频道2接收数据地址 */
#define RX_ADDR_P3      0x0D  /*  频道3接收数据地址 */
#define RX_ADDR_P4      0x0E  /*  频道4接收数据地址 */
#define RX_ADDR_P5      0x0F  /*  频道5接收数据地址 */
#define TX_ADDR         0x10  /*  发送地址寄存器 */
#define RX_PW_P0        0x11  /*  接收频道0接收数据长度 */
#define RX_PW_P1        0x12  /*  接收频道0接收数据长度 */
#define RX_PW_P2        0x13  /*  接收频道0接收数据长度 */
#define RX_PW_P3        0x14  /*  接收频道0接收数据长度 */
#define RX_PW_P4        0x15  /*  接收频道0接收数据长度 */
#define RX_PW_P5        0x16  /*  接收频道0接收数据长度 */
#define FIFO_STATUS     0x17  /*  FIFO栈入栈出状态寄存器设置 */

/*----------------------------------------------*
 *全局变量                                      *
 *----------------------------------------------*/
uint  bdata sta;   //状态标志
sbit RX_DR =sta^6;
sbit TX_DS =sta^5;
sbit MAX_RT =sta^4;

/*****************************************************************************
 函 数 名  : RNF24L01Delay
 功能描述  : RNF24L01 延时函数
 输入参数  : unsigned char n  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
*****************************************************************************/
void RNF24L01Delay(unsigned char n)
{
    for(;n>0;n--)
    {
        _nop_();
    }
  
}
/*****************************************************************************
 函 数 名  : RNF24L01Iintial
 功能描述  : RNF24L01初始化
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
*****************************************************************************/
void RNF24L01Iintial(void)
{
    RNF24L01Delay(100);
    CE = 0;     /* chip enable */
    CSN = 1;    /* SPI disable */
    SCK = 0;    /* set clk */
    IRQ = 1;
    NRF24L01SpiWriteBuf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);       /* 写本地地址 */
    NRF24L01SpiWriteBuf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); /* 写接收端0地址 */
    NRF24L01SpiRwReg(WRITE_REG + EN_AA, 0x3f);                      /* 频道0~5,ACK应答允许  */
    NRF24L01SpiRwReg(WRITE_REG + EN_RXADDR, 0x03f);              /* 允许接收地址只有频道0~5 */                                             
    NRF24L01SpiRwReg(WRITE_REG + RF_CH, 0);               /* 信道工作为2.4GHZ，收发必须一致 */
    NRF24L01SpiRwReg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH);     /* 设置接收数据长度,=32字节 */
    NRF24L01SpiRwReg(WRITE_REG + RF_SETUP, 0x07); /* 设置发射速率为1MHZ,发射功率为最大值0dB */
}


/*****************************************************************************
 函 数 名  : RNF24L01SetRxMode
 功能描述  : 数据接受配置
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
*****************************************************************************/
void RNF24L01SetRxMode(void)
{
   CE=0;
   NRF24L01SpiRwReg(WRITE_REG + CONFIG, 0x0f);     /*  IRQ收发完成中断响应，16位CRC 主接收 */
   CE = 1; 
   RNF24L01Delay(130);
}


/*****************************************************************************
 函 数 名  : NRF24L01SpiWriteByte
 功能描述  : NRF24L01的SPI写时序 单字节
 输入参数  : uint uchar  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
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
 函 数 名  : NRF24L01SpiReadByte
 功能描述  : NRF24L01的SPI读操作
 输入参数  : uchar reg  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
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
 函 数 名  : NRF24L01SpiRwReg
 功能描述  : NRF24L01读写寄存器函数
 输入参数  : uchar reg    
             uchar value  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
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
 函 数 名  : NRF24L01SpiReadBuf
 功能描述  : 用于读数据，reg：为寄存器地址，pBuf：为待读出数据地址，uchars：
             读出数据的个数
 输入参数  : uchar reg     
             uchar *pBuf   
             uchar uchars  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
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
 函 数 名  : NRF24L01SpiWriteBuf
 功能描述  : 用于写数据：为寄存器地址，pBuf：为待写入数据地址，uchars：写入
             数据的个数
 输入参数  : uchar reg     
             uchar *pBuf   
             uchar uchars  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
*****************************************************************************/
uint NRF24L01SpiWriteBuf(uchar reg, uchar *pBuf, uchar uchars)
{
	 uint status,uchar_ctr; 
	 CSN = 0;            /* SPI使能 */       
	 status = NRF24L01SpiWriteByte(reg);   
	 for(uchar_ctr=0; uchar_ctr<uchars; uchar_ctr++) /* */ 
	 NRF24L01SpiWriteByte(*pBuf++);
	 CSN = 1;           //关闭SPI
	 return(status);    /*   */
}

/*****************************************************************************
 函 数 名  : NRF24L01RxPacket
 功能描述  : 数据读取后放如rx_buf接收缓冲区中
 输入参数  : unsigned char* rx_buf  
 输出参数  : 无
 返 回 值  : unsigned
 调用函数  : 
 被调函数  : 
*****************************************************************************/
unsigned char NRF24L01RxPacket(unsigned char* rx_buf)
{
     unsigned char revale=0;
	 sta=NRF24L01SpiReadByte(STATUS); /*  读取状态寄存其来判断数据接收状况 */
	 
	 if(RX_DR)    /*  判断是否接收到数据 */
	 {
	     CE = 0;    /* SPI使能 */
	     NRF24L01SpiReadBuf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH); /*  read receive payload from RX_FIFO buffer */
	     revale =1;   /* 读取数据完成标志 */
	 }

	 /* 接收到数据后RX_DR,TX_DS,MAX_PT都置高为1，通过写1来清楚中断标志*/
	 NRF24L01SpiRwReg(WRITE_REG+STATUS,sta);  
	 return revale;
}

/*****************************************************************************
 函 数 名  : NRF24L01TxPacket
 功能描述  : 发送tx buf 中数据
 输入参数  : unsigned char * tx_buf  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
*****************************************************************************/
void NRF24L01TxPacket(unsigned char * tx_buf)
{
	 CE=0;   /* StandBy I模式 */
	 //NRF24L01SpiWriteBuf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); /*  装载接收端地址 */
	 NRF24L01SpiWriteBuf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH);     /*  装载数据  */
	 NRF24L01SpiRwReg(WRITE_REG + CONFIG, 0x0e);      /*  IRQ收发完成中断响应，16位CRC，主发送 */
	 CE=1;   /* 置高CE,激发数据发送 */
	 RNF24L01Delay(100);
	 CE=0; 
}
