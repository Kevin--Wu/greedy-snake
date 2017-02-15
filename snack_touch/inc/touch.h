#ifndef _touch_h_
#define _touch_h_

extern volatile int adjusting;
extern volatile int if_touch;

extern void touch_init(void);
extern void touch_adjust(void);
extern void touch_beginadj(void);

#endif