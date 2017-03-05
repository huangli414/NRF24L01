/******************************************************************************

                  版权所有 (C), 2001-2011, 神州数码网络有限公司

 ******************************************************************************
  文 件 名   : LCD_1602.c
  版 本 号   : 初稿
  作    者   : huangli
  生成日期   : 2016年12月3日 星期六
  最近修改   :
  功能描述   : LCD1602液晶显示驱动
  函数列表   :
              LCD1602Clear
              LCD1602CursorGotoXY
              LCD1602CursorMoveLeft
              LCD1602CursorMoveRight
              LCD1602CursorOffFlashOff
              LCD1602CursorOffFlashOn
              LCD1602CursorOnFlashOff
              LCD1602CursorOnFlashOn
              LCD1602DisplayFloat
              LCD1602DisplayInteger
              LCD1602DisplayString
              LCD1602Initial
              LCD1602WriteCommand
              LCD1602WriteData
  修改历史   :
  1.日    期   : 2016年12月3日 星期六
    作    者   : huangli
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include <reg52.h> 
#include <string.h>
#include <Delay.h> 
#include <stdio.h> 
#include "LCD_1602.h"

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
sbit RS = P2^5;						 
sbit RW = P2^6;
sbit EN = P2^7;	 
#define RS_CLR RS=0 
#define RS_SET RS=1
#define RW_CLR RW=0 
#define RW_SET RW=1 
#define EN_CLR EN=0
#define EN_SET EN=1
#define LCD_SHOW			0x04    /* 显示开 */
#define LCD_HIDE			0x00    /* 显示关 */
#define LCD_CURSOR			0x02 	/* 显示光标 */
#define LCD_NO_CURSOR		0x00    /* 无光标 */
#define LCD_FLASH			0x01    /* 光标闪动 */
#define LCD_NO_FLASH		0x00    /* 光标不闪动 */

 
/*****************************************************************************
 函 数 名  : LCD1602WriteCommand
 功能描述  : 写入命令函数
 输入参数  : uint8 com  
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年12月3日 星期六
    作    者   : huangli
    修改内容   : 新生成函数

*****************************************************************************/
 void LCD1602WriteCommand(uint8 com) 
 {  
	 RS_CLR; 
	 RW_CLR; 
	 EN_SET; 
	 P0 = com; 
	 Delay_us(5); 
	 EN_CLR;
 }
 
/*****************************************************************************
 函 数 名  : LCD1602WriteData
 功能描述  : 写入数据函数
 输入参数  : uint8  Data  
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年12月3日 星期六
    作    者   : huangli
    修改内容   : 新生成函数

*****************************************************************************/
 void LCD1602WriteData(uint8  Data) 
 { 
	 RS_SET; 
	 RW_CLR; 
	 EN_SET; 
	 P0 = Data; 
	 Delay_us(5); 
	 EN_CLR;
 }
 
/*****************************************************************************
 函 数 名  : LCD1602Clear
 功能描述  : 清屏函数
 输入参数  : void  
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年12月3日 星期六
    作    者   : huangli
    修改内容   : 新生成函数

*****************************************************************************/
 void LCD1602Clear(void) 
 { 
	 LCD1602WriteCommand(0x01); 
	 Delay_ms(5);
 }

 
/*****************************************************************************
 函 数 名  : LCD1602Initial
 功能描述  : 初始化函数
 输入参数  : void  
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年12月3日 星期六
    作    者   : huangli
    修改内容   : 新生成函数

*****************************************************************************/
 void LCD1602Initial(void) 
 {
	   LCD1602WriteCommand(0x38);    /*显示模式设置*/ 
	   Delay_ms(5); 
	   LCD1602WriteCommand(0x38); 
	   Delay_ms(5); 
	   LCD1602WriteCommand(0x38); 
	   Delay_ms(5); 
	   LCD1602WriteCommand(0x38);  
	   LCD1602WriteCommand(0x08);    /*显示关闭*/ 
	   LCD1602WriteCommand(0x01);    /*显示清屏*/ 
	   LCD1602WriteCommand(0x06);    /*显示光标移动设置*/ 
	   Delay_ms(5); 
	   LCD1602WriteCommand(0x0C);    /*显示开及光标设置*/
 }

 
/*****************************************************************************
 函 数 名  : LCD1602DisplayString
 功能描述  : 写入字符串函数
 输入参数  : uint8  x   
             uint8  y   
             uint8  *s  
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年12月3日 星期六
    作    者   : huangli
    修改内容   : 新生成函数

*****************************************************************************/
 void LCD1602DisplayString(uint8  x,uint8  y,uint8  *s) 
 {     
	 if (y == 0) 
	 {     
	 LCD1602WriteCommand(0x80 + x);     
	 }
	 else 
	 {     
	 LCD1602WriteCommand(0xC0 + x);     
	 }
	 
	 while (*s) 
	 {     
	 LCD1602WriteData( *s);     
	 s++;     
	 }
 }			 

/*****************************************************************************
 函 数 名  : LCD1602DisplayInteger
 功能描述  : 写入整型数据
 输入参数  : uint8  x  
             uint8 y   
             int num   
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年12月3日 星期六
    作    者   : huangli
    修改内容   : 新生成函数

*****************************************************************************/
void LCD1602DisplayInteger(uint8  x,uint8 y,int num)
{
	  idata char Buf[8]={0};
	  sprintf(Buf,"%d",num);
	  LCD1602DisplayString(x,y,Buf);
} 

/*****************************************************************************
 函 数 名  : LCD1602DisplayFloat
 功能描述  : 显示浮点型数据
 输入参数  : uint8  x   
             uint8 y    
             float num  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年12月3日 星期六
    作    者   : huangli
    修改内容   : 新生成函数

*****************************************************************************/
void LCD1602DisplayFloat(uint8  x,uint8 y,float num)
{
	  idata char Buf[8]={0};
	  sprintf(Buf,"%0.1f",num);
	  LCD1602DisplayString(x,y,Buf);
} 



/************************************/
void LCD1602CursorOffFlashOn(void)
{
   LCD1602WriteCommand(0x0D);    /*显示开及光标不打开，但闪烁*/
}

/************************************/
void LCD1602CursorOnFlashOn(void)
{
   LCD1602WriteCommand(0x0F);    /*显示开及光标打开闪烁*/
}

/************************************/
void LCD1602CursorOnFlashOff(void)
{
   LCD1602WriteCommand(0x0E);    /*显示开及光标不打开，但闪烁*/
}

/************************************/
void LCD1602CursorOffFlashOff(void)
{
   LCD1602WriteCommand(0x0C);    /*显示开及光标不打开，但闪烁c*/
}

/************************************
　　　光标左移Bit位
************************************/
void LCD1602CursorMoveLeft(uint8 Bit)
{
   for(;Bit>0;Bit--)
   {
     LCD1602WriteCommand(0x10);    /*显示开及光标不打开，但闪烁*/
   }
}
/************************************
　　　光标右移Bit位
************************************/
void LCD1602CursorMoveRight(uint8 Bit)
{
   for(;Bit>0;Bit--)
   {
     LCD1602WriteCommand(0x14);    /*显示开及光标不打开，但闪烁*/
   }
}
/************************************
　　　显示光标位置坐标设定
************************************/
void LCD1602CursorGotoXY(unsigned char x, unsigned char y) 
{
	if(y==0)
	{
		LCD1602WriteCommand(0x80|x);
	}
	else if(y==1)
	{
		LCD1602WriteCommand(0x80|(x-0x40));
	}
}
