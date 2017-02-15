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
#include "touch.h"

int touch_x,touch_y;

//触摸屏中断
__irq void  touch_irq(void)
{
	rSUBSRCPND|=(1<<9);
	rSRCPND|=(1<<31);
	rINTPND|=(1<<31);
	
	
	//**********  这一段用于当笔按下时，打印出当时的坐标  ********
	rADCTSC = 4;  // X,Y自动顺序转换模式
	rADCCON|=1;//开启ADC, 开了之后自动清0
	while((rADCCON &(1<<15))==0);//等待ADC完成
	Uart_Printf( "x=%d,y=%d\n",rADCDAT0&0x3ff, rADCDAT1&0x3ff);
	 rADCTSC = 0xd3;

	 //********  这一段作用：当笔按下时，不停打印X、Y当前坐标 *****
	
	 
	 
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