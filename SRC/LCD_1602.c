/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ���������������޹�˾

 ******************************************************************************
  �� �� ��   : LCD_1602.c
  �� �� ��   : ����
  ��    ��   : huangli
  ��������   : 2016��12��3�� ������
  ����޸�   :
  ��������   : LCD1602Һ����ʾ����
  �����б�   :
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
  �޸���ʷ   :
  1.��    ��   : 2016��12��3�� ������
    ��    ��   : huangli
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include <reg52.h> 
#include <string.h>
#include <Delay.h> 
#include <stdio.h> 
#include "LCD_1602.h"

/*----------------------------------------------*
 * �궨��                                       *
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
#define LCD_SHOW			0x04    /* ��ʾ�� */
#define LCD_HIDE			0x00    /* ��ʾ�� */
#define LCD_CURSOR			0x02 	/* ��ʾ��� */
#define LCD_NO_CURSOR		0x00    /* �޹�� */
#define LCD_FLASH			0x01    /* ������� */
#define LCD_NO_FLASH		0x00    /* ��겻���� */

 
/*****************************************************************************
 �� �� ��  : LCD1602WriteCommand
 ��������  : д�������
 �������  : uint8 com  
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��12��3�� ������
    ��    ��   : huangli
    �޸�����   : �����ɺ���

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
 �� �� ��  : LCD1602WriteData
 ��������  : д�����ݺ���
 �������  : uint8  Data  
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��12��3�� ������
    ��    ��   : huangli
    �޸�����   : �����ɺ���

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
 �� �� ��  : LCD1602Clear
 ��������  : ��������
 �������  : void  
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��12��3�� ������
    ��    ��   : huangli
    �޸�����   : �����ɺ���

*****************************************************************************/
 void LCD1602Clear(void) 
 { 
	 LCD1602WriteCommand(0x01); 
	 Delay_ms(5);
 }

 
/*****************************************************************************
 �� �� ��  : LCD1602Initial
 ��������  : ��ʼ������
 �������  : void  
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��12��3�� ������
    ��    ��   : huangli
    �޸�����   : �����ɺ���

*****************************************************************************/
 void LCD1602Initial(void) 
 {
	   LCD1602WriteCommand(0x38);    /*��ʾģʽ����*/ 
	   Delay_ms(5); 
	   LCD1602WriteCommand(0x38); 
	   Delay_ms(5); 
	   LCD1602WriteCommand(0x38); 
	   Delay_ms(5); 
	   LCD1602WriteCommand(0x38);  
	   LCD1602WriteCommand(0x08);    /*��ʾ�ر�*/ 
	   LCD1602WriteCommand(0x01);    /*��ʾ����*/ 
	   LCD1602WriteCommand(0x06);    /*��ʾ����ƶ�����*/ 
	   Delay_ms(5); 
	   LCD1602WriteCommand(0x0C);    /*��ʾ�����������*/
 }

 
/*****************************************************************************
 �� �� ��  : LCD1602DisplayString
 ��������  : д���ַ�������
 �������  : uint8  x   
             uint8  y   
             uint8  *s  
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��12��3�� ������
    ��    ��   : huangli
    �޸�����   : �����ɺ���

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
 �� �� ��  : LCD1602DisplayInteger
 ��������  : д����������
 �������  : uint8  x  
             uint8 y   
             int num   
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��12��3�� ������
    ��    ��   : huangli
    �޸�����   : �����ɺ���

*****************************************************************************/
void LCD1602DisplayInteger(uint8  x,uint8 y,int num)
{
	  idata char Buf[8]={0};
	  sprintf(Buf,"%d",num);
	  LCD1602DisplayString(x,y,Buf);
} 

/*****************************************************************************
 �� �� ��  : LCD1602DisplayFloat
 ��������  : ��ʾ����������
 �������  : uint8  x   
             uint8 y    
             float num  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��12��3�� ������
    ��    ��   : huangli
    �޸�����   : �����ɺ���

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
   LCD1602WriteCommand(0x0D);    /*��ʾ������겻�򿪣�����˸*/
}

/************************************/
void LCD1602CursorOnFlashOn(void)
{
   LCD1602WriteCommand(0x0F);    /*��ʾ����������˸*/
}

/************************************/
void LCD1602CursorOnFlashOff(void)
{
   LCD1602WriteCommand(0x0E);    /*��ʾ������겻�򿪣�����˸*/
}

/************************************/
void LCD1602CursorOffFlashOff(void)
{
   LCD1602WriteCommand(0x0C);    /*��ʾ������겻�򿪣�����˸c*/
}

/************************************
�������������Bitλ
************************************/
void LCD1602CursorMoveLeft(uint8 Bit)
{
   for(;Bit>0;Bit--)
   {
     LCD1602WriteCommand(0x10);    /*��ʾ������겻�򿪣�����˸*/
   }
}
/************************************
�������������Bitλ
************************************/
void LCD1602CursorMoveRight(uint8 Bit)
{
   for(;Bit>0;Bit--)
   {
     LCD1602WriteCommand(0x14);    /*��ʾ������겻�򿪣�����˸*/
   }
}
/************************************
��������ʾ���λ�������趨
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
