#include <stdlib.h>
#include <string.h>
#include "def.h"
#include "option.h"
#include "2440addr.h"
#include "2440lib.h"
#include "2440slib.h"
#include "mmu.h"
#include "profile.h"
#include "memtest.h"
#include "snack.h"
#include "touch.h"

int touch_x,touch_y;
int flag=1;

//触摸屏中断
__irq void  touch_irq(void)
{
	int tempx,tempy;
	int lcd_x,lcd_y;
	
	rSUBSRCPND|=(1<<9);
	rSRCPND|=(1<<31);
	rINTPND|=(1<<31);
	
	
	/*//**********  这一段用于当笔按下时，打印出当时的坐标  ********
	rADCTSC = 4;  // X,Y自动顺序转换模式
	rADCCON|=1;//开启ADC, 开了之后自动清0
	while((rADCCON &(1<<15))==0);//等待ADC完成
	Uart_Printf( "x=%d,y=%d\n",rADCDAT0&0x3ff, rADCDAT1&0x3ff);
	 rADCTSC = 0xd3;

	 //********  这一段作用：当笔按下时，不停打印X、Y当前坐标 *****
	*/
	if(flag)
	{
		flag=0;
		rADCDLY=5000;
		rADCTSC = 3<<2;  // X,Y自动顺序转换模式
		rADCCON|=1;//开启ADC, 开了之后自动清0
		while((rADCCON &(1<<15))==0);//等待ADC完成
	
	
	
		tempx=rADCDAT0&0x3ff;
		tempy=rADCDAT1&0x3ff;
	
		lcd_y = (int)(-0.376470f * tempx + 351.905888f);
		lcd_x = (int)(0.307955f * tempy - 40.983740f);
		if (lcd_x < 0)
			lcd_x = 0;
		else if (lcd_x > 239)
			lcd_x = 239;
			
		if (lcd_y < 0)
			lcd_y = 0;
		else if (lcd_y > 319)
			lcd_y = 319;
		
	
	
		if(game_start)
		{
			Uart_Printf("lcd:   %d %d\n",lcd_x,lcd_y);
			Uart_Printf("snack: %d %d\n",head->locx,head->locy);
			Uart_Printf("dir:   %d \n",head->dir);
			if(head->dir==1||head->dir==3)
			{
				if(lcd_x>(head->locx))head->dir=2;
				else if(lcd_x<(head->locx))head->dir=4;
			}
			else if(head->dir==2||head->dir==4)
			{
				if(lcd_y>head->locy)head->dir=1;
				else if(lcd_y<head->locy)head->dir=3;
			}
		}
		rADCTSC = 0x1d3;
	 }
	 else
	 {
	 	flag=1;
	 	rADCTSC = 0xd3;
	 }
}

void touch_init(void)
{
	rADCCON = (1<<14)|(249<<6);
	rADCTSC = 0xd3; 
	
	
	//中断的初始化
	pISR_ADC =(U32)touch_irq;
	rINTSUBMSK &=~(1<<9);
	rINTMSK &=~(1<<31);
}