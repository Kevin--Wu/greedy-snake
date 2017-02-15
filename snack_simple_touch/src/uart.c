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
#include "lcd.h"


int pause;

//串口中断处理函数
__irq void uart_irq()
{
	unsigned char ch;
	//清理相应中断标记
	rSUBSRCPND|=1;
	rSRCPND|=(1<<28);
	rINTPND|=(1<<28);
	
	ch=rURXH0;	
	switch(ch)
	{
		case 'd':
			if(head->dir!=3)head->dir=1;
			break;
		case 's':
			if(head->dir!=4)head->dir=2;
			break;
		case 'a':
			if(head->dir!=1)head->dir=3;
			break;
		case 'w':
			if(head->dir!=2)head->dir=4;
			break;
		case 'p':
			if(pause)
			{
				pause=0;
				game_start=1;
			}
			else
			{
				pause=1;
				game_start=0;
			}
			break;
		case 'r':
		//如果游戏已经结束，则此选项有效
		//此时重新设置一切参数，重新开始游戏
			if(!game_start&&if_map)
			{
				need_restart=1;
				/*
				标识游戏需要重新启动
				不能在串口中断里进行初始化
				因为初始化需要用到串口，而它的中断无法嵌套
				*/
 			}
 			break;
		case '1':
			if(!if_speed)
			{
				speed=3000;
				if_speed=1;
			}
			else if(!if_map)
			{
				which_map=0;
				if_map=1;
			}
			break;
		case '2':
			if(!if_speed)
			{
				speed=1000;
				if_speed=1;
			}
			else if(!if_map)
			{
				which_map=1;
				if_map=1;
			}
			break;
		case '3':
			if(!if_speed)
			{
				speed=500;
				if_speed=1;
			}
			else if(!if_map)
			{
				which_map=2;
				if_map=1;
			}
			break;
		case '4':
			if(!if_speed)
			{
				speed=300;
				if_speed=1;
			}
			else if(!if_map)
			{
				which_map=3;
				if_map=1;
			}
			break;
		default:
			break;
	}
}

//串口初始化设置
void uart_init(void)
{
	//将串口0对应端口设为串口
	rGPHCON&=~(0xf<<4);
	rGPHCON|=(0xa<<4);
	
	//配置端口的各种状态
	rULCON0=0x3;
	rUCON0=0x5;
	rUFCON0=0;
	rUMCON0=0;
	rUBRDIV0=26;
	
	//配置uart中断
	
	rINTSUBMSK&=~1;
	pISR_UART0=(unsigned int)uart_irq;
	rINTMSK&=~(1<<28);
	
	pause=0;
}