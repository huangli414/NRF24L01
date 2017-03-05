/******************************************************************************

                  版权所有 (C), 2001-2011, 神州数码网络有限公司

 ******************************************************************************
  文 件 名   : LCD_1602.h
  版 本 号   : 初稿
  作    者   : huangli
  生成日期   : 2016年12月3日 星期六
  最近修改   :
  功能描述   : LCD_1602.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2016年12月3日 星期六
    作    者   : huangli
    修改内容   : 创建文件

******************************************************************************/
#ifndef __LCD_1602_H__
#define __LCD_1602_H__

#define uint8 unsigned char
extern void LCD1602WriteCommand(uint8 com);
extern void LCD1602WriteData(uint8  Data);
extern void LCD1602Clear(void);
extern void LCD1602DisplayString(uint8  x,uint8  y,uint8  *s);
extern void LCD1602Initial(void);
extern void LCD1602DisplayInteger(uint8  x,uint8 y,int num); 
extern void LCD1602DisplayFloat(uint8  x,uint8 y,float num);
extern void LCD1602CursorOffFlashOn(void);
extern void LCD1602CursorOnFlashOn(void);
extern void LCD1602CursorOnFlashOff(void);
extern void LCD1602CursorOffFlashOff(void);
extern void LCD1602CursorMoveLeft(uint8 Bit);
extern void LCD1602CursorMoveRight(uint8 Bit);
extern void LCD1602CursorGotoXY(unsigned char x, unsigned char y);

#endif					