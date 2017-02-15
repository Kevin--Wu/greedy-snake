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

//�������ж�
__irq void  touch_irq(void)
{
	rSUBSRCPND|=(1<<9);
	rSRCPND|=(1<<31);
	rINTPND|=(1<<31);
	
	
	//**********  ��һ�����ڵ��ʰ���ʱ����ӡ����ʱ������  ********
	rADCTSC = 4;  // X,Y�Զ�˳��ת��ģʽ
	rADCCON|=1;//����ADC, ����֮���Զ���0
	while((rADCCON &(1<<15))==0);//�ȴ�ADC���
	Uart_Printf( "x=%d,y=%d\n",rADCDAT0&0x3ff, rADCDAT1&0x3ff);
	 rADCTSC = 0xd3;

	 //********  ��һ�����ã����ʰ���ʱ����ͣ��ӡX��Y��ǰ���� *****
	
	 
	 
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