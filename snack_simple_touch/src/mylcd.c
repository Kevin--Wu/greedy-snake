#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "def.h"
#include "option.h"
#include "2440addr.h"
#include "2440lib.h"
#include "2440slib.h"
#include "mmu.h"
#include "profile.h"
#include "memtest.h"
#include "mylcd.h"
#include "touch.h"
#include "snack.h"
#include "HZK16.h"
#include "ASC16.h"



/*LCDCON1 macro*/
#define CLKVAL 		0x07
#define MMODE 		0x00
#define PNRMODE 	0x03
#define BPPMODE 	0x0d
#define ENVID   	0x01
/*LCDCON2 macro*/
#define VBPD 		10
#define LINEVAL 	239
#define VFPD		4
#define	VSPW    	1
/*LCDCON3 macro*/
#define HBPD 		0x44
#define HOZVAL		319
#define HFPD		4
/*LCDCON4 macro*/
#define HSPW 		1
/*LCDCON5 macro*/		
#define BPP24BL		0
#define FRM565		1
#define INVVCLK		0
#define INVVLINE	1
#define INVVFRAME	1
#define INVVD		0
#define INVVDEN		0
#define INVPWREN	0
#define INVLEND		0
#define PWREN		1
#define ENLEND		0
#define BSWP		0
#define HWSWP		0

unsigned int lcd_buf[240][320];

char str[20];
int grim[][2]={{0,0},{0,1},{1,0},{0,-1},{-1,0},{1,1},{1,-1},{-1,1},{-1,-1}};
char GameOver[]="GAME OVER";

void lcd_dischar_816(unsigned char ch[],int x,int y,int color);
void lcd_score(int colour);
void delete_score(int colour);

void lcd_init(void)
{
    /* set generic I/O pin function */
	rGPCCON = 0xaaaa02aa;
	rGPDCON = 0xaaaaaaaa;
	
	/* set generic register function */
	rLCDCON1 = (CLKVAL << 8) | (MMODE << 7) | (PNRMODE << 5) | (BPPMODE << 1);
	rLCDCON2 = (VBPD << 24) | (LINEVAL << 14) | (VFPD << 6) | (VSPW);
	rLCDCON3 = (HBPD << 19) | (HOZVAL << 8) | (HFPD);
	rLCDCON4 = (HSPW);
	rLCDCON5 = (BPP24BL << 12) | (FRM565 << 11) | (INVVCLK << 10) | (INVVLINE << 9) | 
				(INVVFRAME << 8) | (INVVD << 7) | (INVVDEN << 6) | (INVPWREN << 5) |
				(INVLEND << 4) | (PWREN << 3) | (ENLEND << 2) | (BSWP << 1) | (HWSWP);
	/*set VD buffer address*/
	rLCDSADDR1 = ((unsigned int)lcd_buf >> 1);
	rLCDSADDR2 = (((unsigned int)lcd_buf + 240 * 320 * sizeof(unsigned int)) >> 1) & (0x1fffff);
	rLCDCON1 |= 1; 

}

/********************

初始化游戏界面
其中，贪食蛇游戏区域为（4，4）到（236，238）
游戏区域中心点为(120,121)

********************/
void env_init(void)
{
	int i,j;
	
	
	for(i=0;i<240;++i)
		for(j=0;j<320;++j)
			if(map[which_map][i][j])lcd_buf[i][j]=0xa67d3d;
	
	lcd_score(0xff0000);
	
}	

void lcd_flush(int colour)
{
	int i,j;
	for(i=0;i<240;++i)
		for(j=0;j<320;++j)
			lcd_buf[i][j]=colour;
}

//游戏结束
void lcd_showGameover(void)
{
	char restart[]="按R键重新开始";
	int i,loc=124;
	lcd_flush(0xffffff);
	for(i=0;i<strlen(GameOver);++i)
		lcd_dischar_816(&szASC16[GameOver[i]*16],112,loc+i*8,0xff0000);
	loc=0;
	for(i=0;i<strlen(restart);)
	{
		if(restart[i]>127)
		{
			lcd_dischar_1616(&szHZK16[((restart[i]-161)*94+(restart[i+1]-161))*32],128,112+loc,0xff0000);
			loc+=16;
			i+=2;
		}
		else
		{
			lcd_dischar_816(&szASC16[restart[i]*16],128,112+loc,0xff0000);
			loc+=8;
			++i;
		}
	}
}
	

//刷新贪食蛇的位置
void show_snack()
{
	int i;
	struct snack *temp=head;
	while(temp)
	{
		for(i=0;i<9;++i)
		{
			lcd_buf[temp->locx+grim[i][0]][temp->locy+grim[i][1]]=0xbc1717;
		}
		temp=temp->next;
	}
	for(i=0;i<9;++i)
	{
		lcd_buf[last_tail->locx+grim[i][0]][last_tail->locy+grim[i][1]]=0x111111;
	}
}

//刷新食物的位置
void show_food()
{
	int i;
	for(i=0;i<9;++i)
	{
		lcd_buf[food_x+grim[i][0]][food_y+grim[i][1]]=0xff0000;
	}
	if_eaten=0;
	delete_score(0x111111);
	score+=5;
	lcd_score(0xff0000);
	for(i=0;i<9;++i)
	{
		lcd_buf[lastfood_x+grim[i][0]][lastfood_y+grim[i][1]]=0x111111;
	}

}

//16*16的字符显示
void lcd_dischar_1616(unsigned char ch[],int x,int y,int color)
{	
	int i,j,cur=0;
	for(i=0;i<16;++i)
	{
		for(j=0;j<8;++j)
			if(ch[cur]&(1<<(7-j)))lcd_buf[x+i][y+j]=color;
		++cur;
		for(j=0;j<8;++j)
			if(ch[cur]&(1<<(7-j)))lcd_buf[x+i][y+j+8]=color;
		++cur;
	}
}

//8*16的字符显示
void lcd_dischar_816(unsigned char ch[],int x,int y,int color)
{	
	int i,j,cur=0;
	for(i=0;i<16;++i)
	{
		for(j=0;j<8;++j)
			if(ch[cur]&(1<<(7-j)))lcd_buf[x+i][y+j]=color;
		++cur;
	}
}


//显示分数
void lcd_score(int colour)
{	
	int i,loc=250;
	sprintf(str,"%d分",score);
	for(i=0;i<strlen(str);)
	{
		if(str[i]>127)
		{
			lcd_dischar_1616(&szHZK16[((str[i]-161)*94+(str[i+1]-161))*32],20,loc,colour);
			loc+=16;
			i+=2;
		}
		else
		{
			lcd_dischar_816(&szASC16[str[i]*16],20,loc,colour);
			loc+=8;
			++i;
		}
	}
}	

//删去旧分数，以供显示新分数
void delete_score(int colour)
{	
	int i,loc=250;
	sprintf(str,"%d分",score);
	for(i=0;i<strlen(str);)
	{
		if(str[i]>127)
		{
			lcd_dischar_1616(&szHZK16[((str[i]-161)*94+(str[i+1]-161))*32],20,loc,colour);
			loc+=16;
			i+=2;
		}
		else
		{
			lcd_dischar_816(&szASC16[str[i]*16],20,loc,colour);
			loc+=8;
			++i;
		}
	}
}

//显示控制界面的文字
void lcd_showinfo()
{
	int x=200,y=245;
	char info[]="按P暂停";
	int i;
	lcd_dischar_1616(&szHZK16[((info[0]-161)*94+(info[1]-161))*32],x,y,0xff00);
	lcd_dischar_816(&szASC16[info[2]*16],x,y+16,0xff00);
	for(i=3;i<strlen(info);i+=2)
	{
		lcd_dischar_1616(&szHZK16[((info[i]-161)*94+(info[i+1]-161))*32],x,y+(i-3)*8+24,0xff00);
	}
}
	
	
void speed_lcd(void)
{
	char str1[]="请选择游戏速度";
	char str2[]="1、新手";
	char str3[]="2、进阶";
	char str4[]="3、专家";
	char str5[]="4、自虐";
	int i,loc=0;
	lcd_flush(0xffffff);
	for(i=0;i<strlen(str1);i+=2)
	{
		lcd_dischar_1616(&szHZK16[((str1[i]-161)*94+(str1[i+1]-161))*32],80,100+loc,0xff);
		loc+=16;
	}
	loc=0;
	for(i=0;i<strlen(str2);)
	{
		if(str2[i]>127)
		{
			lcd_dischar_1616(&szHZK16[((str2[i]-161)*94+(str2[i+1]-161))*32],100,100+loc,0xff);
			loc+=16;
			i+=2;
		}
		else
		{
			lcd_dischar_816(&szASC16[str2[i]*16],100,100+loc,0xff);
			loc+=8;
			++i;
		}
	}
	loc=0;
	for(i=0;i<strlen(str3);)
	{
		if(str3[i]>127)
		{
			lcd_dischar_1616(&szHZK16[((str3[i]-161)*94+(str3[i+1]-161))*32],120,100+loc,0xff);
			loc+=16;
			i+=2;
		}
		else
		{
			lcd_dischar_816(&szASC16[str3[i]*16],120,100+loc,0xff);
			loc+=8;
			++i;
		}
	}
	loc=0;
	for(i=0;i<strlen(str4);)
	{
		if(str4[i]>127)
		{
			lcd_dischar_1616(&szHZK16[((str4[i]-161)*94+(str4[i+1]-161))*32],140,100+loc,0xff);
			loc+=16;
			i+=2;
		}
		else
		{
			lcd_dischar_816(&szASC16[str4[i]*16],140,100+loc,0xff);
			loc+=8;
			++i;
		}
	}
	loc=0;
	for(i=0;i<strlen(str5);)
	{
		if(str5[i]>127)
		{
			lcd_dischar_1616(&szHZK16[((str5[i]-161)*94+(str5[i+1]-161))*32],160,100+loc,0xff);
			loc+=16;
			i+=2;
		}
		else
		{
			lcd_dischar_816(&szASC16[str5[i]*16],160,100+loc,0xff);
			loc+=8;
			++i;
		}
	}
}
	
	
void map_lcd(void)
{
	char str1[]="请选择游戏地图";
	char str2[]="1、双杠";
	char str3[]="2、十字准星";
	char str4[]="3、纳粹旗";
	char str5[]="4、测试用练习图";
	int i,loc=0;
	lcd_flush(0xffffff);
	for(i=0;i<strlen(str1);i+=2)
	{
		lcd_dischar_1616(&szHZK16[((str1[i]-161)*94+(str1[i+1]-161))*32],80,100+loc,0xff);
		loc+=16;
	}
	loc=0;
	for(i=0;i<strlen(str2);)
	{
		if(str2[i]>127)
		{
			lcd_dischar_1616(&szHZK16[((str2[i]-161)*94+(str2[i+1]-161))*32],100,100+loc,0xff);
			loc+=16;
			i+=2;
		}
		else
		{
			lcd_dischar_816(&szASC16[str2[i]*16],100,100+loc,0xff);
			loc+=8;
			++i;
		}
	}
	loc=0;
	for(i=0;i<strlen(str3);)
	{
		if(str3[i]>127)
		{
			lcd_dischar_1616(&szHZK16[((str3[i]-161)*94+(str3[i+1]-161))*32],120,100+loc,0xff);
			loc+=16;
			i+=2;
		}
		else
		{
			lcd_dischar_816(&szASC16[str3[i]*16],120,100+loc,0xff);
			loc+=8;
			++i;
		}
	}
	loc=0;
	for(i=0;i<strlen(str4);)
	{
		if(str4[i]>127)
		{
			lcd_dischar_1616(&szHZK16[((str4[i]-161)*94+(str4[i+1]-161))*32],140,100+loc,0xff);
			loc+=16;
			i+=2;
		}
		else
		{
			lcd_dischar_816(&szASC16[str4[i]*16],140,100+loc,0xff);
			loc+=8;
			++i;
		}
	}
	loc=0;
	for(i=0;i<strlen(str5);)
	{
		if(str5[i]>127)
		{
			lcd_dischar_1616(&szHZK16[((str5[i]-161)*94+(str5[i+1]-161))*32],160,100+loc,0xff);
			loc+=16;
			i+=2;
		}
		else
		{
			lcd_dischar_816(&szASC16[str5[i]*16],160,100+loc,0xff);
			loc+=8;
			++i;
		}
	}
}	
	
	
	
	
	
	
	
	
	
	
	
	

