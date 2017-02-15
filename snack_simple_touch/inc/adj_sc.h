#ifndef _ADJ_SC_H_
/*****************************************
  NAME: adj_sc.c
  DESC: Adjusting Touch screen & test
  HISTORY:
  2011.09.01:Joshua Chan: draft ver 1.0
 *****************************************/
 
#define _ADJ_SC_H_

#define F_COLOR 0xFFFF
#define B_COLOR 0x0

extern int collect_count;
extern int xdata;
extern int ydata;

/* 声明ascii码字模数组，数组在ascii.c文件中定义 */
extern unsigned char ASCII[];

extern volatile unsigned short LCD_BUFFER[SCR_YSIZE][SCR_XSIZE];

extern void draw_char(int x0, int y0, U16 front_color, U16 back_color, char *ch, int swi);
extern void scroll_display(char *ch);
extern void add_display(char *ch);
extern void clear_line(int line);
extern void refresh_screen(void);
extern void draw_cross(int x0, int y0, int siz, unsigned short d_color);
extern void move_cross(int x0, int y0, int siz, int x_of, int y_of);
extern int str_len(char *ch);
extern float abs_val(float f);
extern int check_data(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
extern void float_str(float f, char *ch);
extern void int_str(int n, char *ch);
extern void check_touch_down(void);
extern void check_touch_up(void);

extern void delay(int times);

#endif

