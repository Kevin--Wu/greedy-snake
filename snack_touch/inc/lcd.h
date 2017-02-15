#ifndef _lcd_h_
#define _lcd_h_

extern unsigned int lcd_buf[240][320];

extern void lcd_init(void);
extern void lcd_flush(int);
extern void show_snack(void);
extern void env_init(void);
extern void show_food(void);
extern void lcd_dischar_1616(unsigned char ch[],int x,int y,int color);
extern void lcd_dischar_816(unsigned char ch[],int x,int y,int color);
extern void lcd_showGameover(void);
extern void lcd_showinfo(void);
extern void speed_lcd(void);
extern void map_lcd(void);
extern void lcd_adjust(void);


#endif