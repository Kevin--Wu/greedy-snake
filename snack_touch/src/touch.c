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
#include "uart.h"

int touch_x,touch_y;
int flag=1;

float mul_x,add_x,mul_y,add_y;
int adjust_x[10],adjust_y[10];//用于储存矫正点的数据
volatile int adjusting;//表示校正到第几个点
volatile int if_touch;

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
	*/
	if(rADCTSC==0xd3)
	{
		
		rADCDLY=50000;
		rADCTSC = 3<<2;  // X,Y自动顺序转换模式
		rADCCON|=1;//开启ADC, 开了之后自动清0
		while((rADCCON &(1<<15))==0);//等待ADC完成
	
		tempx=(rADCDAT0&0x3ff);
		tempy=(rADCDAT1&0x3ff);
		
		lcd_y = (int)(mul_x * tempx + add_x);
		lcd_x = (int)(mul_y * tempy + add_y);
		if (lcd_x < 0)
			lcd_x = 0;
		else if (lcd_x > 239)
			lcd_x = 239;
				
		if (lcd_y < 0)
			lcd_y = 0;
		else if (lcd_y > 319)
			lcd_y = 319;
	
		
		
		//触摸屏中断调试阶段
		if(adjusting)
		{
			if_touch=1;
			adjust_x[adjusting]=(rADCDAT0&0x3ff);
			adjust_y[adjusting]=(rADCDAT1&0x3ff);
		}
		//如果游戏处于设置阶段，则此中断用来设置参数
		else if(!if_speed)
		{
			
			
			Uart_Printf("%d %d\n",lcd_x,lcd_y);
			
			if(lcd_x>=100&&lcd_x<120&&lcd_y>=100&&lcd_y<=156)
			{
				speed=3000;
				if_speed=1;
			}
			else if(lcd_x>=120&&lcd_x<140&&lcd_y>=100&&lcd_y<=156)
			{
				speed=1000;
				if_speed=1;
			}
			else if(lcd_x>=140&&lcd_x<160&&lcd_y>=100&&lcd_y<=156)
			{
				speed=500;
				if_speed=1;
			}
			else if(lcd_x>=160&&lcd_x<180&&lcd_y>=100&&lcd_y<=156)
			{
				speed=300;
				if_speed=1;
			}
			
		}
		else if(!if_map)
		{
			
			
			Uart_Printf("%d %d\n",lcd_x,lcd_y);
			
			if(lcd_x>=100&&lcd_x<120&&lcd_y>=100&&lcd_y<=156)
			{
				which_map=0;
				if_map=1;
			}
			else if(lcd_x>=120&&lcd_x<140&&lcd_y>=100&&lcd_y<=188)
			{
				which_map=1;
				if_map=1;
			}
			else if(lcd_x>=140&&lcd_x<160&&lcd_y>=100&&lcd_y<=172)
			{
				which_map=2;
				if_map=1;
			}
			else if(lcd_x>=160&&lcd_x<180&&lcd_y>=100&&lcd_y<=188)
			{
				which_map=3;
				if_map=1;
			}
			
		}
		else if(lcd_x>=200&&lcd_x<=216&&lcd_y>=245&&lcd_y<=301)
		{
			//如果按了暂停区域，则游戏停下来或者恢复
			if(game_start)
			{
				pause^=1;
			}
		}
		//如果游戏已经开始，则触摸屏中断用来改变方向
		else if(game_start)
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
		else if(!game_start&&if_map)
		{
			
			if(lcd_x>=128&&lcd_x<=144&&lcd_y>=128&&lcd_y<=160)
				need_restart=1;
		}
		rADCTSC = 0x1d3;
	 }
	 else
	 {
	 	
	 	rADCTSC = 0xd3;
	 }
}

void touch_init(void)
{
	rADCCON = (1<<14)|(249<<6);
	rADCTSC = 0xd3; 
	
	adjusting=1;
	//中断的初始化
	pISR_ADC =(U32)touch_irq;
	rINTSUBMSK &=~(1<<9);
	rINTMSK &=~(1<<31);
}

//收集校验需要的点的数据
void touch_adjust(void)
{
	if_touch=0;
	while(!if_touch);
}

float abs_val(float f)
{
	float ret = f;
	if (ret < 0)
		ret = (-1) * ret;
	return ret;
}

int check_data(int x1, int y_1, int x2, int y2, int x3, int y3, int x4, int y4)
{
	int allow_val = 20;
	int ret = 0;
	
	if (abs_val(y2-y_1) > allow_val)
		ret = 1;
	if (abs_val(x3-x2) > allow_val)
		ret = 1;
	if (abs_val(y4-y3) > allow_val)
		ret = 1;
	if (abs_val(x4-x1) > allow_val)
		ret = 1;

	return ret;
}

void touch_beginadj(void)
{
	int i;
	for(i=1;i<=4;++i)
	{
		Uart_Printf("adjust %d %d %d\n",i,adjust_x[i],adjust_y[i]);
	}
	if(!check_data(adjust_x[1],adjust_y[1],adjust_x[2],adjust_y[2],adjust_x[3],adjust_y[3],adjust_x[4],adjust_y[4]))
	{
		mul_x = (280-40) / (((float)adjust_x[2]+(float)adjust_x[3]-(float)adjust_x[1]-(float)adjust_x[4])/2);
		add_x = 40 - (mul_x * (((float)adjust_x[1]+(float)adjust_x[4])/2));
		mul_y = (210-30) / (((float)adjust_y[3]+(float)adjust_y[4]-(float)adjust_y[1]-(float)adjust_y[2])/2);
		add_y = 30 - (mul_y * (((float)adjust_y[1]+(float)adjust_y[2])/2));
		//Uart_Printf("mul_x=%f \n",mul_x);
		//Uart_Printf("add_x=%f \n",add_x);
		//Uart_Printf("mul_y=%f \n",mul_y);
		Uart_Printf("add_y=%d \n",(int)add_y);
	}
	else
		adjusting=1;
}
	
	
	
	
	
	