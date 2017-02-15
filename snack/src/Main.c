/*************************************************
Function name: ���ǻ���ʵ���һ��ģ��
Parameter    : ��
Description	 : ������ʵ�飬ֱ�ӵ��ø�ģ�弴��
Return		 : ��
Argument     : �� 
Autor & date : Daniel
**************************************************/
#define	GLOBAL_CLK		1
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
#include "lcd.h"
#include "touch.h"
#include "snack.h"
#include "uart.h"


/*������ȫ�ֱ�������*/


/*************************************************
Function name: delay
Parameter    : times
Description	 : ��ʱ����
Return		 : void
Argument     : void
Autor & date :
**************************************************/
void delay(int times)
{ 
    int i;
    for(;times>0;times--) 
      for(i=0;i<4000;i++);
}

/*************************************************
Function name: Main
Parameter    : void
Description	 : �����ܺ���
Return		 : void
Argument     : void
Autor & date :
**************************************************/
void Main(void)  
{ 
    
    
   Set_Clk();
   MMU_Init(); 
   
   //��Щ��Ӳ���ĳ�ʼ��
   lcd_init();
   touch_init();
   uart_init();
   
   //ѡ����棬ѡ����Ϸ�ٶ����ͼ
   map_init();
   speed_lcd();
   speed_choose();
   map_lcd();
   map_choose();
   
   snack_init();
   
   lcd_flush(0x111111);
   //��Щ����Ҫ����ˢ���Ժ�
   lcd_showinfo();
   env_init();
   show_snack();
   show_food();
   game_start=1;
   need_restart=0;
   
   while(game_start)
   {
   		delay(speed);
   		snack_update();
   		while(!game_start)
   		{
   			delay(500);
   			if(need_restart)
   			{
   				map_init();
   				speed_lcd();
			    speed_choose();
			    map_lcd();
			    map_choose();
				snack_init();
				lcd_flush(0x111111);
				lcd_showinfo();
	   			env_init();
	   			show_snack();
			   	show_food();
			   	need_restart=0;
 				game_start=1;
 			}
 		}
   		if(if_eaten)
   		{
   			show_food();
   		}
   		show_snack();
   	}
   		
  }




