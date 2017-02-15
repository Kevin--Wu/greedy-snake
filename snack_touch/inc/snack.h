#ifndef _snack_h_
#define _snack_h_

//̰ʳ������
typedef struct snack
{
	int locx;
	int locy;
	int dir;//����1~4��ʾ�ҡ��¡�����
	struct snack *next;
} snack;

extern int map[3][240][320];//�����������ͼ��һ������治��
extern int map2[1][240][320];
extern snack *head,*last_tail;
extern int length;
extern int game_start;
extern int need_restart;
extern int food_x;
extern int food_y;
extern int lastfood_x;
extern int lastfood_y;
extern int if_eaten;
extern int score;
extern int which_map;
extern int speed;
extern int if_map;
extern int if_speed;

extern void snack_init(void);
extern void snack_update(void);
extern void map_init(void);
extern void speed_choose(void);
extern void map_choose(void);

#endif