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
以下为全局变量
*******************/
snack *head;//贪食蛇头结点
snack *last_tail;//上一次的尾节点，用来清除之前的LCD显示
int length;//贪食蛇长度
int game_start;//标识游戏是否开始
int need_restart;//标识是否要重新开始游戏
int score;//游戏分数
int which_map=2;//标明选择哪张图
int speed;//游戏速度
//食物的坐标
int food_x;
int food_y;
int lastfood_x;
int lastfood_y;
//四幅地图
int map[3][240][320];
int map2[1][240][320];
//标识某次贪食蛇前进时，有没有吃到食物
int if_eaten;
//标记是否选择了速度与地图
int if_speed=1;
int if_map=1;



/*******************
以下为局部变量
*******************/
int step[][2]={{0,1},{1,0},{0,-1},{-1,0}};
int if_crash[][2]={{0,1},{1,0},{0,-1},{-1,0},{-1,-1},{-1,1},{1,-1},{1,1}};

/*初始化贪食蛇的数据，一开始为四格长度*/
void snack_init()
{
	int i,food_ok=1;
	snack *temp,*pre;
	
	//初始化一些全局变量
	length=40;
	if_eaten=0;
	score=200;//每一节5分
	
	//初始化随机数种子，用来生成食物坐标
	srand(1);//貌似不能用srand(time(0));
	
	//设置第一个食物的中心点位置
	//食物中心点的位置在（5，5）到（235，237）
	//生成食物时，要加个判断，以免食物出现在墙上或者障碍上
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
	
	//头结点设置
	head=(snack*)malloc(sizeof(snack));
	head->locx=4;
	head->locy=150;
	head->dir=1;
	
	last_tail=(snack*)malloc(sizeof(snack));
	
	pre=head;
	//赋值之后的三个节点
	for(i=1;i<=length;++i)
	{
		temp=(snack*)malloc(sizeof(snack));
		temp->locx=pre->locx;
		temp->locy=pre->locy-3;
		temp->dir=pre->dir;
		pre->next=temp;
		pre=temp;
	}
	pre->next=NULL;//最后一个的next为空
}

/********************

一共4张地图，在这里全部初始化

********************/
void map_init(void)
{
	int i,j;
	
	/*设置游戏边框*/
	for(j=0;j<3;++j)
	{
		for(i=0;i<240;++i)
			map[j][i][0]=map[j][i][1]=map[j][i][2]=map[j][i][317]=map[j][i][318]=map[j][i][319]=1;
		for(i=0;i<320;++i)
			map[j][0][i]=map[j][1][i]=map[j][2][i]=map[j][237][i]=map[j][238][i]=map[j][239][i]=1;
	}
		
	//划出计分栏与设置界面
	for(j=0;j<3;++j)
	{
		for(i=0;i<240;++i)
			map[j][i][239]=map[j][i][240]=map[j][i][241]=1;
	}
	
	/************************
	分别设置不同的地图的障碍
	************************/
	//下面设置第一张地图
	for(i=35;i<=197;++i)
		for(j=-1;j<2;++j)
			map[0][80+j][i]=map[0][160+j][i]=1;
			
	//下面设置第二张地图
	for(i=0;i<70;++i)
		for(j=-1;j<2;++j)
			map[1][120+j][30+i]=map[1][120+j][138+i]=1;
	for(i=0;i<70;++i)
		for(j=-1;j<2;++j)
			map[1][20+i][119+j]=map[1][146+i][119+j]=1;
			
	//下面设置第三张地图
	for(i=0;i<80;++i)
		for(j=-1;j<2;++j)
		{
			map[2][120+i][119+j]=map[2][120-i][119+j]=1;
			map[2][120+i][198+j]=map[2][120-i][40+j]=1;
			map[2][120+j][119+i]=map[2][120+j][119-i]=1;
			map[2][41+j][119+i]=map[2][199+j][119-i]=1;
		}
		
	//第四张地图：如果选择了此模式，则将第三号地图替代为无障碍的地图
}
	

void snack_update(void)//更新贪食蛇的位置数据，向前前进一格
{
	int food_ok;
	snack *temp=head->next,*pre;
	snack *p;//如果蛇新吃到食物，则这个指针指向倒数旧的最后一个节点
	int tempdir,i;
	
	//记录头结点之前的位置
	pre=(snack*)malloc(sizeof(snack));
	pre->locx=head->locx;
	pre->locy=head->locy;
	pre->dir=head->dir;
	
	head->locx+=(step[(head->dir)-1][0]*3);//dir从1开始，所以要减1
	head->locy+=(step[(head->dir)-1][1]*3);//根据原有的方向，更新贪食蛇位置
	//如果此次头结点与食物相距小于3，则表示吃到了这个食物
	if((abs((head->locx)-food_x)<=2)&&(abs((head->locy)-food_y)<=2))if_eaten=1;
	
	//如果此次撞到边界或者障碍，则显示游戏结束
	for(i=0;i<8;++i)
	{
		if(map[which_map][(head->locx+if_crash[i][0])][(head->locy+if_crash[i][1])])
		{
			game_start=0;
		}
	}
	
	while(temp)
	{
		//标记一下，待会刷新LCD显示时，可以将之前的显示痕迹清除
		if(temp->next==NULL)
		{
			p=temp;
			last_tail->locx=temp->locx;
			last_tail->locy=temp->locy;
			last_tail->dir=temp->dir;
		}
		//记录更新前，节点的位置
		pre->locx=temp->locx;
		pre->locy=temp->locy;
		
		temp->locx+=(step[(temp->dir)-1][0]*3);
		temp->locy+=(step[(temp->dir)-1][1]*3);
		if(abs((head->locx)-(temp->locx))<3&&abs((head->locy)-(temp->locy))<3)game_start=0;
		
		//记录下之前的前进方向
		tempdir=temp->dir;
		temp->dir=pre->dir;
		pre->dir=tempdir;
		
		
		temp=temp->next;
	}
	
	//如果吃到了食物
	if(if_eaten)
	{
		//生成新食物
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
		
		//在蛇尾加一个节点
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
	if(which_map==3)//如果是选择测试图，则将第三号地图的障碍全清除，做为测试图
	{
		which_map-=1;
		for(i=3;i<237;++i)
			for(j=3;j<239;++j)
				map[which_map][i][j]=0;
	}
}







