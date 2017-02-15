/*****************************************
  NAME: adj_sc.c
  DESC: Adjusting Touch screen & test
  HISTORY:
  2011.09.01:Joshua Chan: draft ver 1.0
 *****************************************/

#include "def.h"
#include "option.h"
#include "2440addr.h"
#include "2440lib.h"
#include "2440slib.h"
#include "mmu.h"
#include "profile.h"
#include "memtest.h"
#include "main.h"
#include "adj_sc.h"
#include <stdio.h>

/* 定义320行，240列的数组，用于存放显示数据 */
volatile unsigned short LCD_BUFFER[SCR_YSIZE][SCR_XSIZE];

/* 用(current_x, current_y)来记录下次显示字符的起始坐标 */
int current_x = 0;
int current_y = 0;

/*
 * (x1, y1), (x2, y2), (x3, y3), (x4, y4)用来采集4个校准点物理坐标
 * 4个屏幕校准点象素坐标分别为: (40, 30), (280, 30), (280, 210), (40, 210)
 */
int x1=0, x2=0, x3=0, x4=0;
int y1=0, y2=0, y3=0, y4=0;

int collect_count = 0;

/* current_row记录下次打印起始行, 第一次打印从57行开始 */
int current_row = 57;

/*
 * 物理坐标和象素坐标的转换表达式如下:
 * hor_pix = (int)(mul_x * X + add_x)
 * ver_pix = (int)(mul_y * Y + add_y)
 * 其中: (X, Y)表示物理坐标, (hor_pix, ver_pix)表示象素坐标
 */
float mul_x = 0.0;
float mul_y = 0.0;
float add_x = 0.0;
float add_y = 0.0;


/*
 * 用象素点阵画出ascii码字符
 * 每个文字点阵8x16, 长度16字节, 即 1 bit 表示 1 象素
 * 排列方式:横向顺序,横排
 * @*ch: 字模数组地址
 * @swi: 起始坐标选择开关, 值为0时, 表示起始坐标为(x0, y0),
 *       值为1时, 表示起始坐标为(current_x, current_y),
 *       即最后画的那一个字符后面的位置.
 */
void draw_char(int x0, int y0, U16 front_color, U16 back_color, char *ch, int swi)
{
	int x=0, y=0;
	unsigned char tmp = 0;
	U32 c=0;
	char *p = ch;

	if (swi == 1) {
		x0 = current_x;
		y0 = current_y;
	}

	while (*p) {
		for (y=y0; y<y0+16; y++) {
			for (x=x0; x<x0+8; x++) {
				/* 用tmp取出数组中对应的元素, 长度为1字节,
				 * 取出tmp中相应的1位, 并转换为2字节的颜色信息,
				 * 并存入显存相应位置 */
				tmp = ASCII[(*p)*16+y-y0];
				c = (tmp & (1<<(8-(x-x0))))?(front_color):(back_color);
				LCD_BUFFER[y][x] = c;
			}
		}
		x0 = x0 + 8;
		p++;
	}
	current_x = x;
	current_y = y0;
}

/*
 * 在高度为128的象素区滚动显示字符点阵
 * 因为每个文字点阵8x16, 所以此区域共可显示8行文字
 * 起始垂直象素坐标57, 结束坐标为184
 * 第一行文字从坐标57开始, 末行文字从坐标169开始
 * @*ch: 要显示的字符串地址
 */
void scroll_display(char *ch)
{
	int x0 = 10;
	int i, j, k;

	if (current_row > 169) {
		current_row = 169;
		for (i=0; i<16; i++) {
			for (j=57; j<184-i; j++) {
				clear_line(j);
				for (k=1; k<320; k++) {
					LCD_BUFFER[j][k] = LCD_BUFFER[j+1][k];
				}
			}
			clear_line(184 - i);
		}		
	}
	
	draw_char(x0, current_row, F_COLOR, B_COLOR, ch, 0);
	current_row += 16;
	refresh_screen();
}

/*
 * 挨着上次的字符显示指定字符串
 */
void add_display(char *ch)
{
	draw_char(0, 0, F_COLOR, B_COLOR, ch, 1);
	refresh_screen();
}

/*
 * 用背景色填充指定行
 */
void clear_line(int line)
{
	int i;
	
	for (i=1; i<320; i++)
		LCD_BUFFER[line][i] = B_COLOR;
}

/*
 * 全屏刷新
 * 在屏幕滚动显示的时候, 显示效果与显存内容不同步,
 * 在滚动结束时, 需调用此函数处理
 */
void refresh_screen(void)
{
	int i, j;
	
	for( i = 0 ; i < SCR_YSIZE ; i++ )
		for( j = 0 ; j < SCR_XSIZE ; j++ )
			LCD_BUFFER[i][j] = LCD_BUFFER[i][j] ;
}

/*###################################################
** 以(x0, y0)为中心画十字
                 
              (x0,y0+tmp)  
                  | 
                  |
 (x0-tmp,y0)---(x0,y0)---(x0+tmp,y0)
                  |
                  |
              (x0,y0-tmp)
 ###################################################*/
void draw_cross(int x0, int y0, int size, U16 d_color)
{
	int x=0, y=0;
	int tmp = size/2;
	
	for (x=(x0-tmp); x<(x0+tmp+1); x++)    
		LCD_BUFFER[y0][x] = d_color;

	for (y=(y0-tmp); y<(y0+tmp+1); y++)
		LCD_BUFFER[y][x0] = d_color;
}

/*
 * 水平或垂直移动十字, 并显示移动轨迹
 * 在移动的过程中, 用背景色画十字来清除轨迹
 * (x0, y0) 为起始坐标
 * @x_of: X 方向偏移量
 * @y_of: Y 方向偏移量
 * "x_of" 与 "y_of" 二者必须要有一个值为0
 */
void move_cross(int x0, int y0, int siz, int x_of, int y_of)
{
	int i = 0;
	
	if (x_of!=0 && y_of!=0)
		return;
	if (x_of > 0)
		for (i=0; i<x_of; i++) {
			draw_cross(x0+i-1, y0, siz, B_COLOR);
			draw_cross(x0+i+1, y0, siz, B_COLOR);
			draw_cross(x0, y0-1, siz, B_COLOR);
			draw_cross(x0, y0+1, siz, B_COLOR);
			
			draw_cross(x0+i, y0, siz, F_COLOR);
			delay(600);
		}
	else if (x_of < 0)
		for (i=0; i>x_of; i--) {
			draw_cross(x0+i-1, y0, siz, B_COLOR);
			draw_cross(x0+i+1, y0, siz, B_COLOR);
			draw_cross(x0, y0-1, siz, B_COLOR);
			draw_cross(x0, y0+1, siz, B_COLOR);
			
			draw_cross(x0+i, y0, siz, F_COLOR);
			delay(600);
		}
	else if (y_of > 0)
		for (i=0; i<y_of; i++) {
			draw_cross(x0-1, y0, siz, B_COLOR);
			draw_cross(x0+1, y0, siz, B_COLOR);
			draw_cross(x0, y0+i-1, siz, B_COLOR);
			draw_cross(x0, y0+i+1, siz, B_COLOR);
			
			draw_cross(x0, y0+i, siz, F_COLOR);
			delay(600);
		}
	else if (y_of < 0)
		for (i=0; i>y_of; i--) {
			draw_cross(x0-1, y0, siz, B_COLOR);
			draw_cross(x0+1, y0, siz, B_COLOR);
			draw_cross(x0, y0+i-1, siz, B_COLOR);
			draw_cross(x0, y0+i+1, siz, B_COLOR);
			
			draw_cross(x0, y0+i, siz, F_COLOR);
			delay(600);
		}
}

/*
 * 取字符串长度
 */
int str_len(char *ch)
{
	char *p = ch;
	int ret = 0;
	while (*p) {
		ret++;
		p++;
	}

	return ret;
}

/*
 * 取绝对值
 */
float abs_val(float f)
{
	float ret = f;
	if (ret < 0)
		ret = (-1) * ret;
	return ret;
}

/*
 * 对采集到的坐标进行有效性判断
 * 允许偏差值可以通过 @allow_val 设置
 */
int check_data(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
	int allow_val = 20;
	int ret = 0;
	
	if (abs_val(y2-y1) > allow_val)
		ret = 1;
	if (abs_val(x3-x2) > allow_val)
		ret = 1;
	if (abs_val(y4-y3) > allow_val)
		ret = 1;
	if (abs_val(x4-x1) > allow_val)
		ret = 1;

	return ret;
}

/*
 * 转换浮点数为字符串, 保留2位小数
 */
void float_str(float f, char *ch)
{
	char s[10] = {0};
	int sign = (f < 0) ? 1 : 0;
	int n = (sign == 1) ? ((int)(f * (-100))) : ((int)(f * 100));
	int i = 0;
	int j = 0;
	
	s[0] = (n % 10) + '0'; 
	n = n / 10;
	s[1] = (n % 10) + '0'; //s[0],s[1]为小数点后两位
	s[2] = '.';
	n = n / 10;
	i = 3;

	while (n) {
		s[i] = (n % 10) + '0';
		n = n / 10;
		i++;
	}

	if (i == 3) {
		s[i] = '0';
		i++;
	}

	if (sign == 1) {
		s[i] = '-';
		i++;
	}

	for (--i; i>-1; i--) {
		ch[j] = s[i];
		j++;
	}

	ch[j] = '\0';
}

/*
 * 转换整型数为字符串
 */
void int_str(int n, char *ch)
{
	char s[10] = {0};
	int i = 0;
	int j = 0;

	if (n == 0)
	{
		s[i] = '0';
		i++;
	}

	while (n)
	{
		s[i] = (n % 10) + '0';
		n = n / 10;
		i++;
	}

	for (--i; i>-1; i--)
	{
		ch[j] = s[i];
		j++;
	}

	ch[j] = '\0';
}

/*
 * 在触屏按下时, 根据当前状态作出相应处理
 * collect_count为0表示正按下第1个校准点
 * collect_count为1表示正按下第2个校准点
 * collect_count为2表示正按下第3个校准点
 * collect_count为3表示正按下第4个校准点
 * collect_count为-2表示正按下测试点
 */
void check_touch_down(void)
{
	char hor_pixa[10] = {0};
	char ver_pixa[10] = {0};
	int hor_pix = 0;
	int ver_pix = 0;
	
	if (collect_count == 0) {
		x1 = xdata;
		y1 = ydata;

		move_cross(40, 30, 15, 240, 0);	
		scroll_display("Please touch the 2nd point.");
		collect_count++;
	}
	else if (collect_count == 1) {
		x2 = xdata;
		y2 = ydata;
		
		move_cross(280, 30, 15, 0, 180);		
		scroll_display("Please touch the 3rd point.");
		collect_count++;
	}
	else if (collect_count == 2) {
		x3 = xdata;
		y3 = ydata;
		
		move_cross(280, 210, 15, -240, 0);		
		scroll_display("Please touch the 4th point.");
		collect_count++;
	}
	else if (collect_count == 3) {
		x4 = xdata;
		y4 = ydata;		

		collect_count++;
	}
	else if (collect_count == -2) {
		hor_pix = (int)(mul_x * xdata + add_x);
		ver_pix = (int)(mul_y * ydata + add_y);
		
		if (hor_pix < 0)
			hor_pix = 0;
		else if (hor_pix > 319)
			hor_pix = 319;
			
		if (ver_pix < 0)
			ver_pix = 0;
		else if (ver_pix > 239)
			ver_pix = 239;
			
		int_str(hor_pix, hor_pixa);
		int_str(ver_pix, ver_pixa);

		scroll_display("");
		scroll_display("coordinate(hor, ver) = (");
		add_display(hor_pixa);
		add_display(", ");
		add_display(ver_pixa);
		add_display(")");
	}
}

/*
 * 在触屏弹起时, 根据当前状态作出相应处理
 * collect_count为4表示第4个校准点已采集完毕
 * collect_count为-1表示校准工作已完成
 */
void check_touch_up(void)
{
	char mul_xa[10] = {0};
	char mul_ya[10] = {0};
	char add_xa[10] = {0};
	char add_ya[10] = {0};

	if (collect_count == 4) {
		if (!(check_data(x1, y1, x2, y2, x3, y3, x4, y4))) {
			mul_x = (280-40) / (((float)x2+(float)x3-(float)x1-(float)x4)/2);
			add_x = 40 - (mul_x * (((float)x1+(float)x4)/2));
			mul_y = (210-30) / (((float)y3+(float)y4-(float)y1-(float)y2)/2);
			add_y = 30 - (mul_y * (((float)y1+(float)y2)/2));

			float_str(mul_x, mul_xa);
			float_str(add_x, add_xa);
			float_str(mul_y, mul_ya);
			float_str(add_y, add_ya);
			
			scroll_display("");			
			scroll_display("Adjusting finished!");
			scroll_display("");
			
			scroll_display("hor_pix = (int)((");
			add_display(mul_xa);
			add_display(")*X+(");
			add_display(add_xa);
			add_display("))");
			
			scroll_display("ver_pix = (int)((");
			add_display(mul_ya);
			add_display(")*Y+(");
			add_display(add_ya);
			add_display("))");
			scroll_display("");
			
			delay(30000);
			scroll_display("Touch the screen to continue.");

			collect_count = -1;
			draw_cross(41, 210, 15, B_COLOR);
			refresh_screen();
		}
		else {
			scroll_display("");
			scroll_display("Adjusting failed, try again.");
			collect_count = 0;
			scroll_display("");
			
			delay(30000);
			scroll_display("Please touch the 1st point.");
			draw_cross(41, 210, 15, B_COLOR);
			
			refresh_screen();
			draw_cross(40, 30, 15, F_COLOR);
		}
	}
	else if (collect_count == -1) {
		scroll_display("");
		scroll_display("As the touch screen is adjusted,");
		delay(10000);
		scroll_display("let's have some test.");
		delay(10000);
		scroll_display("You can touch the screen,");
		delay(10000);
		scroll_display("and the coordinate will be shown.");
		
		collect_count = -2;
	}
}

void delay(int times)
{
	int i,j;
	for(i=0;i<times;i++)
	   for(j=0;j<400;j++);
}
