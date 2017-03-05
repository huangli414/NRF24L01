/*****************温室大棚温湿度控制*********************
******************************************************/

#include <reg52.h>
#include <LCD_1602.h>
#include <Delay.h>	
#include "RNF24L01.h"
 sbit LED        = P1^6;
 sbit BEEP       = P2^4;
 sbit KEY_UP     = P2^0;
 sbit KEY_DOWM   = P2^1;
 sbit KEY_Protect= P2^2;
 sbit KEY_Find   = P2^3;
 #define WORK_MODE_FIND     1
 #define WORK_MODE_PROTECT  0
 
 unsigned char gubCommunicationFlag = 1;//通讯是否正常标志 
 unsigned char gubSystemWrokMode = 0;
 unsigned char gubSystemDisRank = 0;
 unsigned int  guwTimeCount = 10;
void main(void)
{
 
  unsigned char TxBuf[20]={8,91,10,11,12,13,14,15,16};  
  unsigned char RxBuf[20]={0}; 
  
  BEEP = 1;
  LED = 1;                 /* 基本设备初始化 */
  
  LCD1602Initial();        /* 液晶初始化 */
  RNF24L01Iintial() ;      /* NRF24L01初始化 */
  
  RNF24L01SetRxMode();
  RNF24L01Delay(6000);
  RNF24L01Delay(6000);
  LCD1602Clear();
  LCD1602DisplayString(0,0,"MODE:  Protect ");
  LCD1602DisplayString(0,1,"DIS_R: Rank_0 ");
 while(1)
 {
 ///////////////////////////////////////////////////
   	    RNF24L01SetRxMode();
        RNF24L01Delay(1000);
		if(NRF24L01RxPacket(RxBuf)) 
		{
			guwTimeCount = 10;
			gubCommunicationFlag = 1;
			//LED = ~LED;
		}
		else 
		{
			
			if(guwTimeCount==0)
			{
			    gubCommunicationFlag = 0;
			}
			else guwTimeCount--;
		}
   /////////////////////////////////////////////
		if(!KEY_Protect)
		{
			LCD1602DisplayString(0,0,"MODE:  Protect ");
			gubSystemWrokMode = WORK_MODE_PROTECT;						
		} 
		else if(!KEY_Find)
		{
			LCD1602DisplayString(0,0,"MODE:   Find   ");
			gubSystemWrokMode = WORK_MODE_FIND;
		}
		else if(!KEY_UP)
		{
			if(gubSystemDisRank==3)	
			{
			    gubSystemDisRank = 0;
			}
			else 
			{
			    gubSystemDisRank++;
			}
			
			while(!KEY_UP);			
			LCD1602DisplayInteger(12,1,gubSystemDisRank);
			TxBuf[0] = gubSystemDisRank;
		    RNF24L01Delay(1000);
		    NRF24L01TxPacket(TxBuf);
		}
	    else if(!KEY_DOWM)
		{
			if(gubSystemDisRank==0)	
			{
			    gubSystemDisRank = 3;
			}
			else 
			{
				gubSystemDisRank--;
			}
			while(!KEY_DOWM);
			LCD1602DisplayInteger(12,1,gubSystemDisRank);
			TxBuf[0] = gubSystemDisRank;
		    RNF24L01Delay(1000);
		    NRF24L01TxPacket(TxBuf);
		}
	///////////////////////////////////////////////////
	if(gubSystemWrokMode) 
	{
		if(gubCommunicationFlag) 
		{
			BEEP = 0;
			LED  = 0;
		}
		else                   
		{
			BEEP = 1;
			LED  = 1;
		}						  
	}
	else 
	{
		if(gubCommunicationFlag) 
		{
			BEEP = 1;
			LED  = 1;
		}
		 else                   
		{
			BEEP = 0;
			LED  = 0;
		}
	}
		Delay_ms(100);		    		
}

}