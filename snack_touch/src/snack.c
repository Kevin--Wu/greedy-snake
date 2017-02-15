#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "def.h"
#include "option.h"
#include "2440addr.h"
#include "2440lib.h"
#include "2440slib.h"
#include "mmu.h"
#include "profile.h"
#include "memtest.h"
#include "snack.h"
#include "lcd.h"
#include "uart.h"



/*******************
����Ϊȫ�ֱ���
*******************/
snack *head;//̰ʳ��ͷ���
snack *last_tail;//��һ�ε�β�ڵ㣬�������֮ǰ��LCD��ʾ
int length;//̰ʳ�߳���
int game_start;//��ʶ��Ϸ�Ƿ�ʼ
int need_restart;//��ʶ�Ƿ�Ҫ���¿�ʼ��Ϸ
int score;//��Ϸ����
int which_map=2;//����ѡ������ͼ
int speed;//��Ϸ�ٶ�
//ʳ�������
int food_x;
int food_y;
int lastfood_x;
int lastfood_y;
//�ķ���ͼ
int map[3][240][320];
int map2[1][240][320];
//��ʶĳ��̰ʳ��ǰ��ʱ����û�гԵ�ʳ��
int if_eaten;
//����Ƿ�ѡ�����ٶ����ͼ
int if_speed=1;
int if_map=1;



/*******************
����Ϊ�ֲ�����
*******************/
int step[][2]={{0,1},{1,0},{0,-1},{-1,0}};
int if_crash[][2]={{0,1},{1,0},{0,-1},{-1,0},{-1,-1},{-1,1},{1,-1},{1,1}};

/*��ʼ��̰ʳ�ߵ����ݣ�һ��ʼΪ�ĸ񳤶�*/
void snack_init()
{
	int i,food_ok=1;
	snack *temp,*pre;
	
	//��ʼ��һЩȫ�ֱ���
	length=40;
	if_eaten=0;
	score=200;//ÿһ��5��
	
	//��ʼ����������ӣ���������ʳ������
	srand(1);//ò�Ʋ�����srand(time(0));
	
	//���õ�һ��ʳ������ĵ�λ��
	//ʳ�����ĵ��λ���ڣ�5��5������235��237��
	//����ʳ��ʱ��Ҫ�Ӹ��жϣ�����ʳ�������ǽ�ϻ����ϰ���
	while(1)
	{
		food_ok=1;
		food_x=(rand()%231)+5;
		food_y=(rand()%233)+5;
		for(i=0;i<8;++i)
			if(map[which_map][food_x+if_crash[i][0]][food_y+if_crash[i][1]])
				food_ok=0;
		if(food_ok)break;
	}
	lastfood_x=-1;
	lastfood_y=-1;
	
	//ͷ�������
	head=(snack*)malloc(sizeof(snack));
	head->locx=4;
	head->locy=150;
	head->dir=1;
	
	last_tail=(snack*)malloc(sizeof(snack));
	
	pre=head;
	//��ֵ֮��������ڵ�
	for(i=1;i<=length;++i)
	{
		temp=(snack*)malloc(sizeof(snack));
		temp->locx=pre->locx;
		temp->locy=pre->locy-3;
		temp->dir=pre->dir;
		pre->next=temp;
		pre=temp;
	}
	pre->next=NULL;//���һ����nextΪ��
}

/********************

һ��4�ŵ�ͼ��������ȫ����ʼ��

********************/
void map_init(void)
{
	int i,j;
	
	/*������Ϸ�߿�*/
	for(j=0;j<3;++j)
	{
		for(i=0;i<240;++i)
			map[j][i][0]=map[j][i][1]=map[j][i][2]=map[j][i][317]=map[j][i][318]=map[j][i][319]=1;
		for(i=0;i<320;++i)
			map[j][0][i]=map[j][1][i]=map[j][2][i]=map[j][237][i]=map[j][238][i]=map[j][239][i]=1;
	}
		
	//�����Ʒ��������ý���
	for(j=0;j<3;++j)
	{
		for(i=0;i<240;++i)
			map[j][i][239]=map[j][i][240]=map[j][i][241]=1;
	}
	
	/************************
	�ֱ����ò�ͬ�ĵ�ͼ���ϰ�
	************************/
	//�������õ�һ�ŵ�ͼ
	for(i=35;i<=197;++i)
		for(j=-1;j<2;++j)
			map[0][80+j][i]=map[0][160+j][i]=1;
			
	//�������õڶ��ŵ�ͼ
	for(i=0;i<70;++i)
		for(j=-1;j<2;++j)
			map[1][120+j][30+i]=map[1][120+j][138+i]=1;
	for(i=0;i<70;++i)
		for(j=-1;j<2;++j)
			map[1][20+i][119+j]=map[1][146+i][119+j]=1;
			
	//�������õ����ŵ�ͼ
	for(i=0;i<80;++i)
		for(j=-1;j<2;++j)
		{
			map[2][120+i][119+j]=map[2][120-i][119+j]=1;
			map[2][120+i][198+j]=map[2][120-i][40+j]=1;
			map[2][120+j][119+i]=map[2][120+j][119-i]=1;
			map[2][41+j][119+i]=map[2][199+j][119-i]=1;
		}
		
	//�����ŵ�ͼ�����ѡ���˴�ģʽ���򽫵����ŵ�ͼ���Ϊ���ϰ��ĵ�ͼ
}
	

void snack_update(void)//����̰ʳ�ߵ�λ�����ݣ���ǰǰ��һ��
{
	int food_ok;
	snack *temp=head->next,*pre;
	snack *p;//������³Ե�ʳ������ָ��ָ�����ɵ����һ���ڵ�
	int tempdir,i;
	
	//��¼ͷ���֮ǰ��λ��
	pre=(snack*)malloc(sizeof(snack));
	pre->locx=head->locx;
	pre->locy=head->locy;
	pre->dir=head->dir;
	
	head->locx+=(step[(head->dir)-1][0]*3);//dir��1��ʼ������Ҫ��1
	head->locy+=(step[(head->dir)-1][1]*3);//����ԭ�еķ��򣬸���̰ʳ��λ��
	//����˴�ͷ�����ʳ�����С��3�����ʾ�Ե������ʳ��
	if((abs((head->locx)-food_x)<=2)&&(abs((head->locy)-food_y)<=2))if_eaten=1;
	
	//����˴�ײ���߽�����ϰ�������ʾ��Ϸ����
	for(i=0;i<8;++i)
	{
		if(map[which_map][(head->locx+if_crash[i][0])][(head->locy+if_crash[i][1])])
		{
			game_start=0;
		}
	}
	
	while(temp)
	{
		//���һ�£�����ˢ��LCD��ʾʱ�����Խ�֮ǰ����ʾ�ۼ����
		if(temp->next==NULL)
		{
			p=temp;
			last_tail->locx=temp->locx;
			last_tail->locy=temp->locy;
			last_tail->dir=temp->dir;
		}
		//��¼����ǰ���ڵ��λ��
		pre->locx=temp->locx;
		pre->locy=temp->locy;
		
		temp->locx+=(step[(temp->dir)-1][0]*3);
		temp->locy+=(step[(temp->dir)-1][1]*3);
		if(abs((head->locx)-(temp->locx))<3&&abs((head->locy)-(temp->locy))<3)game_start=0;
		
		//��¼��֮ǰ��ǰ������
		tempdir=temp->dir;
		temp->dir=pre->dir;
		pre->dir=tempdir;
		
		
		temp=temp->next;
	}
	
	//����Ե���ʳ��
	if(if_eaten)
	{
		//������ʳ��
		lastfood_x=food_x;
		lastfood_y=food_y;
		
		while(1)
		{
			food_ok=1;
			food_x=(rand()%231)+5;
			food_y=(rand()%233)+5;
			for(i=0;i<8;++i)
				if(map[which_map][food_x+if_crash[i][0]][food_y+if_crash[i][1]])
					food_ok=0;
			if(food_ok)break;
		}
		
		//����β��һ���ڵ�
		temp=(snack*)malloc(sizeof(snack));
		p->next=temp;
		temp->locx=last_tail->locx;
		temp->locy=last_tail->locy;
		temp->dir=last_tail->dir;
		temp->next=NULL;
	}
	if(game_start==0&&pause==0)
	{
		lcd_showGameover();
	}
}

void speed_choose()
{
	if_speed=0;
	while(!if_speed)
	{
		delay(5000);
		//Uart_Printf("waiting speed choose\n");
	}
}

void map_choose()
{
	int i,j;
	if_map=0;
	while(!if_map)
	{
		delay(5000);
		//Uart_Printf("waiting map choose\n");
	}
	if(which_map==3)//�����ѡ�����ͼ���򽫵����ŵ�ͼ���ϰ�ȫ�������Ϊ����ͼ
	{
		which_map-=1;
		for(i=3;i<237;++i)
			for(j=3;j<239;++j)
				map[which_map][i][j]=0;
	}
}







