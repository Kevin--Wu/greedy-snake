/****************************************************************
 NAME: MAIN.h
 DESC: 
 HISTORY:
 Mar.29.2002:purnnamu: created first
 ****************************************************************/
 
#ifndef __MAIN_H__
#define __MAIN_H__

#define LCD_W35

#if defined(LCD_W35)
#define LCD_PIXCLOCK 4

#define LCD_WIDTH 320
#define LCD_HEIGHT 240

#define LCD_RIGHT_MARGIN 0x44
#define LCD_LEFT_MARGIN 0x04
#define LCD_HSYNC_LEN 0x01

#define LCD_UPPER_MARGIN 10
#define LCD_LOWER_MARGIN 4
#define LCD_VSYNC_LEN 1

#define LCD_CON5 ((1<<11) | (1<<8) | (1<<9) | (1<<0) ) 

#endif

#define LCD_XSIZE  LCD_WIDTH
#define LCD_YSIZE  LCD_HEIGHT
#define SCR_XSIZE  LCD_WIDTH
#define SCR_YSIZE  LCD_HEIGHT

#define	M5D(n)	((n)&0x1fffff)
#define LCD_ADDR ((U32)LCD_BUFFER)

#define ADC_FREQ 2500000


#endif /*__MAIN_H__*/
