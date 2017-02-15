/*
 * ASCII字符的点阵字模数组
 * 每个ASCII字符用8x16点阵表示, 占16字节
 * 包括值为(0x00 ~ 0xFF)的共256个ASCII字符
 */

const unsigned char ASCII[] = {

	/* line 0 */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	
	/* line 1 */
	0x00,0x00,0x00,0x00,0x7E,0xC3,0x81,0xA5,0x81,0xBD,0x99,0xC3,0x7E,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x7E,0xFF,0xFF,0xDB,0xFF,0xC3,0xE7,0xFF,0x7E,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x44,0xEE,0xFE,0xFE,0xFE,0x7C,0x38,0x10,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x10,0x38,0x7C,0xFE,0xFE,0x7C,0x38,0x10,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x18,0x3C,0x3C,0xFF,0xE7,0xE7,0x18,0x18,0x7E,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x18,0x3C,0x7E,0xFF,0xFF,0x7E,0x18,0x18,0x7E,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x7E,0x7E,0x3C,0x00,0x00,0x00,0x00,0x00,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC3,0x81,0x81,0xC3,0xFF,0xFF,0xFF,0xFF,0xFF,
	0x00,0x00,0x00,0x3C,0x7E,0x66,0x66,0x42,0x42,0x66,0x66,0x7E,0x3C,0x00,0x00,0x00,
	0xFF,0xFF,0xFF,0xC3,0x81,0x99,0x99,0xBD,0xBD,0x99,0x99,0x81,0xC3,0xFF,0xFF,0xFF,
	0x00,0x00,0x00,0x00,0x3E,0x0E,0x3A,0x72,0xF8,0xCC,0xCC,0xCC,0x78,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x3C,0x66,0x66,0x66,0x3C,0x18,0x7E,0x18,0x18,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x1F,0x19,0x19,0x1F,0x18,0x18,0x78,0xF8,0x70,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x7F,0x63,0x7F,0x63,0x63,0x63,0x67,0xE7,0xE6,0xC0,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x18,0xDB,0x7E,0xE7,0xE7,0x7E,0xDB,0x18,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x80,0xC0,0xE0,0xF8,0xFE,0xF8,0xE0,0xC0,0x80,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x02,0x06,0x0E,0x3E,0xFE,0x3E,0x0E,0x06,0x02,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x18,0x3C,0x7E,0x18,0x18,0x18,0x7E,0x3C,0x18,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x66,0x66,0x66,0x66,0x66,0x00,0x00,0x66,0x66,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x7F,0xDB,0xDB,0xDB,0x7B,0x1B,0x1B,0x1B,0x1B,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x7E,0x63,0x30,0x3C,0x66,0x66,0x3C,0x0C,0xC6,0x7E,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0xFE,0xFE,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x18,0x3C,0x7E,0x18,0x18,0x18,0x7E,0x3C,0x18,0x7E,0x00,0x00,
	0x00,0x00,0x00,0x00,0x18,0x3C,0x7E,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x18,0x18,0x18,0x18,0x18,0x18,0x7E,0x3C,0x18,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x0C,0xFE,0x0C,0x18,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x60,0xFE,0x60,0x30,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xC0,0xC0,0xFE,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x24,0x66,0xFF,0x66,0x24,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x10,0x10,0x38,0x38,0x7C,0x7C,0xFE,0xFE,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0xFE,0xFE,0x7C,0x7C,0x38,0x38,0x10,0x10,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x30,0x78,0x78,0x78,0x30,0x30,0x00,0x30,0x30,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x66,0x66,0x66,0x24,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x6C,0x6C,0xFE,0x6C,0x6C,0x6C,0xFE,0x6C,0x6C,0x00,0x00,0x00,
	0x00,0x00,0x00,0x30,0x30,0x7C,0xC0,0xC0,0x78,0x0C,0x0C,0xF8,0x30,0x30,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0xC4,0xCC,0x18,0x30,0x60,0xCC,0x8C,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x70,0xD8,0xD8,0x70,0xFA,0xDE,0xCC,0xDC,0x76,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x30,0x30,0x30,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x0C,0x18,0x30,0x60,0x60,0x60,0x30,0x18,0x0C,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x60,0x30,0x18,0x0C,0x0C,0x0C,0x18,0x30,0x60,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x66,0x3C,0xFF,0x3C,0x66,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x7E,0x18,0x18,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x38,0x60,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x38,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x02,0x06,0x0C,0x18,0x30,0x60,0xC0,0x80,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x7C,0xC6,0xCE,0xDE,0xD6,0xF6,0xE6,0xC6,0x7C,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x10,0x30,0xF0,0x30,0x30,0x30,0x30,0x30,0xFC,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x78,0xCC,0xCC,0x0C,0x18,0x30,0x60,0xCC,0xFC,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x78,0xCC,0x0C,0x0C,0x38,0x0C,0x0C,0xCC,0x78,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x0C,0x1C,0x3C,0x6C,0xCC,0xFE,0x0C,0x0C,0x1E,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xFC,0xC0,0xC0,0xC0,0xF8,0x0C,0x0C,0xCC,0x78,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x38,0x60,0xC0,0xC0,0xF8,0xCC,0xCC,0xCC,0x78,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xFE,0xC6,0xC6,0x06,0x0C,0x18,0x30,0x30,0x30,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x78,0xCC,0xCC,0xEC,0x78,0xDC,0xCC,0xCC,0x78,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x78,0xCC,0xCC,0xCC,0x7C,0x18,0x18,0x30,0x70,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x38,0x00,0x00,0x38,0x38,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x38,0x00,0x00,0x38,0x38,0x18,0x30,0x00,0x00,
	0x00,0x00,0x00,0x00,0x0C,0x18,0x30,0x60,0xC0,0x60,0x30,0x18,0x0C,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x00,0x7E,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x60,0x30,0x18,0x0C,0x06,0x0C,0x18,0x30,0x60,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x78,0xCC,0x0C,0x18,0x30,0x30,0x00,0x30,0x30,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x7C,0xC6,0xC6,0xDE,0xDE,0xDE,0xC0,0xC0,0x7C,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x30,0x78,0xCC,0xCC,0xCC,0xFC,0xCC,0xCC,0xCC,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xFC,0x66,0x66,0x66,0x7C,0x66,0x66,0x66,0xFC,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x3C,0x66,0xC6,0xC0,0xC0,0xC0,0xC6,0x66,0x3C,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xF8,0x6C,0x66,0x66,0x66,0x66,0x66,0x6C,0xF8,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xFE,0x62,0x60,0x64,0x7C,0x64,0x60,0x62,0xFE,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xFE,0x66,0x62,0x64,0x7C,0x64,0x60,0x60,0xF0,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x3C,0x66,0xC6,0xC0,0xC0,0xCE,0xC6,0x66,0x3E,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xCC,0xCC,0xCC,0xCC,0xFC,0xCC,0xCC,0xCC,0xCC,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x78,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x78,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x1E,0x0C,0x0C,0x0C,0x0C,0xCC,0xCC,0xCC,0x78,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xE6,0x66,0x6C,0x6C,0x78,0x6C,0x6C,0x66,0xE6,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xF0,0x60,0x60,0x60,0x60,0x62,0x66,0x66,0xFE,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xC6,0xEE,0xFE,0xFE,0xD6,0xC6,0xC6,0xC6,0xC6,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xC6,0xC6,0xE6,0xF6,0xFE,0xDE,0xCE,0xC6,0xC6,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x38,0x6C,0xC6,0xC6,0xC6,0xC6,0xC6,0x6C,0x38,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xFC,0x66,0x66,0x66,0x7C,0x60,0x60,0x60,0xF0,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x38,0x6C,0xC6,0xC6,0xC6,0xCE,0xDE,0x7C,0x0C,0x1E,0x00,0x00,
	0x00,0x00,0x00,0x00,0xFC,0x66,0x66,0x66,0x7C,0x6C,0x66,0x66,0xE6,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x78,0xCC,0xCC,0xC0,0x70,0x18,0xCC,0xCC,0x78,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xFC,0xB4,0x30,0x30,0x30,0x30,0x30,0x30,0x78,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0x78,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0x78,0x30,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xC6,0xC6,0xC6,0xC6,0xD6,0xD6,0x6C,0x6C,0x6C,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xCC,0xCC,0xCC,0x78,0x30,0x78,0xCC,0xCC,0xCC,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xCC,0xCC,0xCC,0xCC,0x78,0x30,0x30,0x30,0x78,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xFE,0xCE,0x98,0x18,0x30,0x60,0x62,0xC6,0xFE,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x3C,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x3C,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x40,0x60,0x30,0x18,0x0C,0x06,0x03,0x01,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x3C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x3C,0x00,0x00,0x00,
	0x00,0x00,0x00,0x10,0x38,0x6C,0xC6,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,
	0x00,0x00,0x00,0x30,0x30,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x78,0x0C,0x7C,0xCC,0xCC,0x76,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xE0,0x60,0x60,0x7C,0x66,0x66,0x66,0x66,0xDC,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x78,0xCC,0xC0,0xC0,0xCC,0x78,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x1C,0x0C,0x0C,0x7C,0xCC,0xCC,0xCC,0xCC,0x76,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x78,0xCC,0xFC,0xC0,0xCC,0x78,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x38,0x6C,0x60,0x60,0xF8,0x60,0x60,0x60,0xF0,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x76,0xCC,0xCC,0xCC,0x7C,0x0C,0xCC,0x78,0x00,
	0x00,0x00,0x00,0x00,0xE0,0x60,0x60,0x6C,0x76,0x66,0x66,0x66,0xE6,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x78,0x18,0x18,0x18,0x18,0x7E,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x0C,0x0C,0x00,0x3C,0x0C,0x0C,0x0C,0x0C,0xCC,0xCC,0x78,0x00,
	0x00,0x00,0x00,0x00,0xE0,0x60,0x60,0x66,0x6C,0x78,0x6C,0x66,0xE6,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x78,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x7E,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xD6,0xD6,0xD6,0xD6,0xC6,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xCC,0xCC,0xCC,0xCC,0xCC,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x78,0xCC,0xCC,0xCC,0xCC,0x78,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xDC,0x66,0x66,0x66,0x66,0x7C,0x60,0xF0,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x76,0xCC,0xCC,0xCC,0xCC,0x7C,0x0C,0x1E,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEC,0x6E,0x76,0x60,0x60,0xF0,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x78,0xCC,0x60,0x18,0xCC,0x78,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x20,0x60,0xFC,0x60,0x60,0x60,0x6C,0x38,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xCC,0xCC,0xCC,0xCC,0xCC,0x76,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xCC,0xCC,0xCC,0xCC,0x78,0x30,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC6,0xC6,0xD6,0xD6,0x6C,0x6C,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC6,0x6C,0x38,0x38,0x6C,0xC6,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x66,0x66,0x66,0x66,0x3C,0x0C,0x18,0xF0,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x8C,0x18,0x60,0xC4,0xFC,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x1C,0x30,0x30,0x60,0xC0,0x60,0x30,0x30,0x1C,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x18,0x18,0x18,0x18,0x00,0x18,0x18,0x18,0x18,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xE0,0x30,0x30,0x18,0x0C,0x18,0x30,0x30,0xE0,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x73,0xDA,0xCE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x38,0x6C,0xC6,0xC6,0xFE,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x78,0xCC,0xCC,0xC0,0xC0,0xC0,0xCC,0xCC,0x78,0x30,0xF0,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x60,0x20,0x20,0x00,
	0x00,0x00,0x07,0x04,0x04,0x08,0x0C,0x08,0x08,0x18,0x10,0x10,0x10,0x20,0x20,0xC0,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x44,0x6E,0x22,0x44,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x63,0x63,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x18,0x18,0x10,0x7E,0x10,0x18,0x18,0x10,0x10,0x10,0x10,0x10,0x10,0x00,
	0x00,0x00,0x18,0x18,0x76,0x7E,0x18,0x10,0x10,0x10,0x10,0x18,0x76,0x5A,0x18,0x18,
	0x00,0x00,0x30,0x50,0x88,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x30,0x48,0x48,0x48,0x49,0x32,0x04,0x08,0x11,0x22,0x42,0x82,0x02,0x01,0x00,
	0x00,0x3C,0x00,0x3C,0x42,0x40,0x40,0x30,0x0C,0x06,0x02,0x02,0x42,0x7C,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x20,0x60,0x60,0x20,0x10,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x3E,0x4A,0x48,0x48,0x4A,0x4E,0x48,0x48,0x48,0x49,0x3F,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x60,0x60,0x20,0x40,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x01,0x01,0x00,0x01,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x3C,0x3C,0x10,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x60,0xB8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0xE9,0x4B,0x4B,0x5D,0x55,0x5F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x88,0x70,0x20,0x30,0x48,0x40,0x60,0x38,0x0C,0x4C,0x78,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x20,0x30,0x10,0x20,0x20,0x40,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x02,0x7D,0x49,0x4F,0x48,0x48,0x4D,0x36,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x24,0x24,0x00,0x42,0x44,0x24,0x28,0x18,0x18,0x10,0x10,0x10,0x10,0x3C,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x00,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x00,0x00,0x04,0x04,0x04,0x18,0x2E,0x4E,0x50,0x50,0x50,0x22,0x3C,0x40,0x40,0x40,
	0x00,0x00,0x00,0x1E,0x22,0x20,0x20,0x20,0x38,0x20,0x20,0x21,0xFA,0x4C,0x00,0x00,
	0x00,0x40,0x27,0x18,0x10,0x20,0x20,0x20,0x20,0x20,0x20,0x10,0x18,0x27,0x40,0x00,
	0x00,0x00,0x00,0x62,0x24,0x24,0x14,0x18,0xFF,0x08,0xFF,0x08,0x08,0x3C,0x00,0x00,
	0x00,0x00,0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x00,0x40,0x40,0x40,0x40,0x40,0x40,
	0x07,0x08,0x08,0x04,0x02,0x03,0x04,0x08,0x08,0x05,0x02,0x01,0x04,0x08,0x08,0x07,
	0x00,0x00,0x00,0x0C,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x7E,0xA6,0xC1,0xC1,0xC1,0xA6,0x5A,0x24,0x00,0x00,0x00,0x00,
	0x00,0x00,0xE0,0x90,0xF0,0x90,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x11,0x22,0x24,0x44,0x4C,0x24,0x22,0x12,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x78,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x7E,0xAA,0xA5,0xB9,0xA9,0xA6,0x42,0x3C,0x00,0x00,0x00,0x00,
	0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x30,0x48,0x48,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x01,0x01,0x01,0x0F,0x01,0x01,0x01,0x00,0x0F,0x00,0x00,0x00,0x00,
	0x00,0x00,0x70,0x90,0x10,0x20,0x50,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x70,0x10,0x20,0x10,0x10,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x10,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x22,0x22,0x22,0x22,0x22,0x2F,0x50,0x40,0x40,0x00,0x00,
	0x00,0x00,0x3E,0xF4,0xF4,0xF4,0xF4,0x74,0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x00,
	0x00,0x00,0x00,0x00,0x00,0x03,0x07,0x07,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x10,0x60,
	0x00,0x00,0x20,0x20,0x20,0x20,0x20,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x70,0x98,0x88,0x50,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x88,0x48,0x24,0x22,0x32,0x24,0x44,0x88,0x00,0x00,0x00,
	0x00,0x00,0x42,0x44,0x44,0x48,0x48,0x50,0x12,0x26,0x26,0x27,0x40,0x00,0x00,0x00,
	0x00,0x00,0x44,0x44,0x44,0x48,0x48,0x50,0x17,0x21,0x22,0x44,0x47,0x00,0x00,0x00,
	0x00,0x00,0x62,0x24,0x44,0x28,0x28,0x50,0x12,0x16,0x26,0x27,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x10,0x10,0x00,0x00,0x10,0x20,0x20,0x44,0x44,0x3C,0x00,0x00,
	0x10,0x10,0x00,0x10,0x18,0x28,0x28,0x28,0x24,0x3C,0x44,0x44,0x42,0xC7,0x00,0x00,
	0x08,0x10,0x20,0x10,0x18,0x28,0x28,0x28,0x24,0x3C,0x44,0x44,0x42,0xC7,0x00,0x00,
	0x18,0x28,0x00,0x10,0x18,0x28,0x28,0x28,0x24,0x3C,0x44,0x44,0x42,0xC7,0x00,0x00,
	0x30,0x0C,0x00,0x10,0x18,0x28,0x28,0x28,0x24,0x3C,0x44,0x44,0x42,0xC7,0x00,0x00,
	0x24,0x3C,0x00,0x10,0x18,0x28,0x28,0x28,0x24,0x3C,0x44,0x44,0x42,0xC7,0x00,0x00,
	0x18,0x28,0x18,0x10,0x18,0x28,0x28,0x28,0x24,0x3C,0x44,0x44,0x42,0xC7,0x00,0x00,
	0x00,0x00,0x00,0x1E,0x18,0x28,0x28,0x2A,0x4E,0x78,0x48,0x48,0x89,0xDE,0x00,0x00,
	0x00,0x00,0x00,0x1C,0x23,0x41,0x40,0x40,0x40,0x40,0x40,0x61,0x32,0x0C,0x08,0x08,
	0x30,0x10,0x10,0x6E,0x42,0x40,0x44,0x44,0x7C,0x44,0x40,0x40,0x42,0xFE,0x00,0x00,
	0x0C,0x08,0x10,0x6E,0x42,0x40,0x44,0x44,0x7C,0x44,0x40,0x40,0x42,0xFE,0x00,0x00,
	0x18,0x28,0x44,0x7E,0x42,0x40,0x44,0x44,0x7C,0x44,0x40,0x40,0x42,0xFE,0x00,0x00,
	0x2C,0x2C,0x00,0x7E,0x42,0x40,0x44,0x44,0x7C,0x44,0x40,0x40,0x42,0xFE,0x00,0x00,
	0x30,0x10,0x10,0x38,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00,
	0x0C,0x08,0x10,0x38,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00,
	0x18,0x28,0x00,0x38,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00,
	0x24,0x2C,0x00,0x38,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00,
	0x00,0x00,0x00,0x3C,0x22,0x22,0x21,0x21,0x31,0x21,0x23,0x22,0x26,0x78,0x00,0x00,
	0x30,0x0C,0x00,0x42,0x62,0x62,0x52,0x52,0x4A,0x4A,0x46,0x46,0x42,0xE2,0x00,0x00,
	0x30,0x10,0x10,0x2C,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x18,0x00,0x00,
	0x08,0x10,0x10,0x2C,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x18,0x00,0x00,
	0x18,0x28,0x44,0x3C,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x18,0x00,0x00,
	0x30,0x0C,0x00,0x3C,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x18,0x00,0x00,
	0x24,0x24,0x00,0x3C,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x18,0x00,0x00,
	0x00,0x00,0x00,0x10,0x08,0x04,0x02,0x01,0x02,0x04,0x08,0x10,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x3C,0x42,0x46,0x4A,0x4A,0x53,0x52,0x62,0x62,0x44,0x38,0x00,0x00,
	0x30,0x10,0x00,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x44,0x38,0x00,0x00,
	0x08,0x08,0x10,0x62,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x44,0x38,0x00,0x00,
	0x18,0x28,0x00,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x44,0x38,0x00,0x00,
	0x24,0x24,0x00,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x44,0x38,0x00,0x00,
	0x08,0x08,0x10,0x62,0x44,0x24,0x28,0x18,0x18,0x10,0x10,0x10,0x10,0x3C,0x00,0x00,
	0x00,0x00,0x00,0x20,0x20,0x38,0x26,0x21,0x21,0x21,0x26,0x38,0x20,0x70,0x00,0x00,
	0x00,0x00,0x00,0x1C,0x22,0x22,0x22,0x2C,0x22,0x22,0x22,0x22,0x3A,0x6C,0x00,0x00,
	0x00,0x00,0x00,0x00,0x10,0x00,0x00,0x38,0x44,0x44,0x44,0x44,0x44,0x3A,0x00,0x00,
	0x00,0x00,0x00,0x00,0x08,0x18,0x00,0x38,0x44,0x44,0x44,0x44,0x44,0x3A,0x00,0x00,
	0x00,0x00,0x30,0x28,0x44,0x00,0x00,0x7C,0x44,0x1C,0x64,0x44,0x45,0x3A,0x00,0x00,
	0x00,0x00,0x20,0x5C,0x00,0x00,0x00,0x7C,0x44,0x1C,0x64,0x44,0x45,0x3A,0x00,0x00,
	0x00,0x00,0x28,0x6C,0x00,0x00,0x00,0x7C,0x44,0x1C,0x64,0x44,0x45,0x3A,0x00,0x00,
	0x00,0x00,0x38,0x28,0x10,0x00,0x00,0x7C,0x44,0x1C,0x64,0x44,0x45,0x3A,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x66,0x99,0x91,0x3F,0x50,0x90,0x99,0x6E,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x18,0x64,0x44,0x40,0x40,0x44,0x44,0x38,0x20,0x10,0x60,
	0x00,0x00,0x00,0x00,0x10,0x10,0x00,0x3C,0x42,0x42,0x7C,0x40,0x22,0x1C,0x00,0x00,
	0x00,0x00,0x00,0x04,0x08,0x08,0x00,0x3C,0x42,0x42,0x7C,0x40,0x22,0x1C,0x00,0x00,
	0x00,0x00,0x00,0x18,0x38,0x44,0x00,0x3C,0x42,0x42,0x7C,0x40,0x22,0x1C,0x00,0x00,
	0x00,0x00,0x00,0x2C,0x24,0x00,0x00,0x3C,0x42,0x42,0x7C,0x40,0x22,0x1C,0x00,0x00,
	0x00,0x00,0x00,0x20,0x20,0x10,0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x38,0x00,0x00,
	0x00,0x00,0x00,0x08,0x10,0x10,0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x38,0x00,0x00,
	0x00,0x00,0x10,0x28,0x48,0x00,0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x3C,0x00,0x00,
	0x00,0x00,0x28,0x6C,0x00,0x00,0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x3C,0x00,0x00,
	0x00,0x00,0x00,0x24,0x18,0x28,0x04,0x3E,0x42,0x42,0x42,0x42,0x42,0x3C,0x00,0x00,
	0x00,0x00,0x30,0x1C,0x00,0x00,0x00,0x7C,0x42,0x42,0x42,0x42,0x42,0xE6,0x00,0x00,
	0x00,0x00,0x00,0x00,0x10,0x00,0x00,0x3C,0x42,0x42,0x42,0x42,0x66,0x3C,0x00,0x00,
	0x00,0x00,0x00,0x00,0x0C,0x08,0x00,0x3C,0x42,0x42,0x42,0x42,0x66,0x18,0x00,0x00,
	0x00,0x00,0x18,0x28,0x04,0x00,0x00,0x3C,0x42,0x42,0x42,0x42,0x44,0x38,0x00,0x00,
	0x00,0x00,0x30,0x1C,0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x42,0x44,0x38,0x00,0x00,
	0x00,0x00,0x24,0x2C,0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x42,0x44,0x38,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x1F,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3E,0x46,0x4A,0x52,0x62,0x44,0xB8,0x00,0x00,
	0x00,0x00,0x00,0x00,0x10,0x00,0xC4,0x44,0x44,0x44,0x44,0x44,0x26,0x3A,0x00,0x00,
	0x00,0x00,0x00,0x04,0x08,0x18,0xC4,0x44,0x44,0x44,0x44,0x44,0x26,0x3A,0x00,0x00,
	0x00,0x00,0x18,0x28,0x44,0x00,0x02,0x42,0x42,0x42,0x42,0x42,0x46,0x38,0x00,0x00,
	0x00,0x00,0x00,0x00,0x24,0x24,0xC4,0x44,0x44,0x44,0x44,0x44,0x26,0x3A,0x00,0x00,
	0x00,0x00,0x0C,0x08,0x10,0x00,0x00,0x24,0x24,0x24,0x18,0x18,0x10,0x10,0x10,0x60,
	0x00,0x00,0xC0,0x40,0x40,0x40,0x5E,0x22,0x42,0x42,0x22,0x22,0x54,0x48,0x40,0x60,
	0x00,0x00,0x24,0x24,0x00,0x00,0x00,0x24,0x24,0x24,0x18,0x18,0x10,0x10,0x10,0x60,

};