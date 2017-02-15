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

//�������ж�
__irq void  touch_irq(void)
{
	int tempx,tempy;
	int lcd_x,lcd_y;
	
	rSUBSRCPND|=(1<<9);
	rSRCPND|=(1<<31);
	rINTPND|=(1<<31);
	
	
	/*//**********  ��һ�����ڵ��ʰ���ʱ����ӡ����ʱ������  ********
	rADCTSC = 4;  // X,Y�Զ�˳��ת��ģʽ
	rADCCON|=1;//����ADC, ����֮���Զ���0
	while((rADCCON &(1<<15))==0);//�ȴ�ADC���
	Uart_Printf( "x=%d,y=%d\n",rADCDAT0&0x3ff, rADCDAT1&0x3ff);
	 rADCTSC = 0xd3;

	 //********  ��һ�����ã����ʰ���ʱ����ͣ��ӡX��Y��ǰ���� *****
	*/
	if(flag)
	{
		flag=0;
		rADCDLY=5000;
		rADCTSC = 3<<2;  // X,Y�Զ�˳��ת��ģʽ
		rADCCON|=1;//����ADC, ����֮���Զ���0
		while((rADCCON &(1<<15))==0);//�ȴ�ADC���
	
	
	
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
	
	
	//�жϵĳ�ʼ��
	pISR_ADC =(U32)touch_irq;
	rINTSUBMSK &=~(1<<9);
	rINTMSK &=~(1<<31);
}