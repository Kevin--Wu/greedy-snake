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

//�����жϴ�����
__irq void uart_irq()
{
	unsigned char ch;
	//������Ӧ�жϱ��
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
		//�����Ϸ�Ѿ����������ѡ����Ч
		//��ʱ��������һ�в��������¿�ʼ��Ϸ
			if(!game_start&&if_map)
			{
				need_restart=1;
				/*
				��ʶ��Ϸ��Ҫ��������
				�����ڴ����ж�����г�ʼ��
				��Ϊ��ʼ����Ҫ�õ����ڣ��������ж��޷�Ƕ��
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

//���ڳ�ʼ������
void uart_init(void)
{
	//������0��Ӧ�˿���Ϊ����
	rGPHCON&=~(0xf<<4);
	rGPHCON|=(0xa<<4);
	
	//���ö˿ڵĸ���״̬
	rULCON0=0x3;
	rUCON0=0x5;
	rUFCON0=0;
	rUMCON0=0;
	rUBRDIV0=26;
	
	//����uart�ж�
	
	rINTSUBMSK&=~1;
	pISR_UART0=(unsigned int)uart_irq;
	rINTMSK&=~(1<<28);
	
	pause=0;
}