/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ���������������޹�˾

 ******************************************************************************
  �� �� ��   : LCD_1602.h
  �� �� ��   : ����
  ��    ��   : huangli
  ��������   : 2016��12��3�� ������
  ����޸�   :
  ��������   : LCD_1602.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��12��3�� ������
    ��    ��   : huangli
    �޸�����   : �����ļ�

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