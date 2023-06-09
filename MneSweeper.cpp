#define _CRT_SECURE_NO_WARNINGS 1
/*
	扫雷小游戏
	1.绘制地图 ，二维数组 雷 ，数字
	2.贴图 ，把数字转化成图片 ，加密，标记
	3.鼠标操作，点击打开格子（把鼠标坐标 ，转化成数组下标）
	4.点到空格之后，打开周围的格子（递归）
	5.判断游戏的输赢
	6.分数 ，音乐
*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<graphics.h>
#include<conio.h>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")

#define SIZE 60      //窗口尺寸
#define ROW 10          //雷区行数
#define COL 10          //雷区列数
#define BOOM 50         //雷的个数
#define LOCK 20

int map[ROW][COL];  //全局变量自动初始化为0；
int count;        //定义剩余的安全格子
int isok;         //存储是否点击 确定 取消
IMAGE img[12];

//记录游戏时间结构体
struct {
	unsigned int start;
	unsigned int end;
}score;
//游戏初始化
void GameInit()
{
	setbkcolor(WHITE);
	cleardevice();
	//设置随机数种子
	srand((unsigned int)time(NULL));
	//初始化数组
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			map[i][j] = 0;
		}
	}
	//埋雷， -1表示雷 boom个雷
	for (int i = 0; i < BOOM; )
	{
		int r = rand() % ROW;
		int c = rand() % COL;
		if (map[r][c] == 0)
		{
			map[r][c] = -1;
			i++;
		}
	}
	//把以雷为中心的九宫格数据+1；
	for (int i=0; i < ROW; i++)
	{
		for (int j=0; j < COL; j++)
		{
			//判断是否是雷
			if (map[i][j] == -1)
			{
				for (int m = 0; m < 3; m++)
				{
					for (int n = 0; n < 3; n++)
					{
						if (i - 1 + m >= 0 && i - 1 + m < ROW && j - 1 + n >= 0 && j - 1 + n < COL && map[i - 1 + m][j - 1 + n] != -1)
						{
							//雷周边不是雷的格子数字+1；
							map[i - 1 + m][j - 1 + n] ++;
						}
					}
				}
			}
		}
	}
	//初始化安全格子个数
	count = ROW * COL - BOOM;
	//加载图片
	char file[50] = "";
	for (int i = 0; i < 12; i++)
	{
		sprintf(file, "./image/%d.jpg", i);
		loadimage(&img[i], file , SIZE , SIZE);
	}
	//加密格子
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			map[i][j] += LOCK;
		}
	}
}




//绘制界面
void GameDraw()
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			if (map[i][j] == -1)
			{
				putimage(j * SIZE, i * SIZE, &img[11]);
			}
			else if(map[i][j]>=0 && map[i][j] <= 8)
			{
				putimage(j * SIZE, i * SIZE, &img[map[i][j]]);
			}
			else if(map[i][j]>=19 &&map[i][j]<=28)
			{
				putimage(j * SIZE, i * SIZE, &img[9]);
			}
			else if (map[i][j] >= LOCK*2)
			{
				putimage(j * SIZE, i * SIZE, &img[10]);
			}
		}
	}
}
//处理消息
void dealmsg(int m, int n)
{
	if (m < 0 || n < 0 || m >= ROW || n >= COL)
	{
		return;
	}
	if (map[m][n] == LOCK)
	{
		count--;
		map[m][n] -= LOCK;
		dealmsg(m - 1, n - 1);
		dealmsg(m - 1, n );
		dealmsg(m - 1, n + 1);
		dealmsg(m , n - 1);
		dealmsg(m , n + 1);
		dealmsg(m + 1, n - 1);
		dealmsg(m + 1, n );
		dealmsg(m + 1, n + 1);
	}
	else if (map[m][n] > LOCK)
	{
		count--;
		map[m][n] -= LOCK;
	}
	else if (map[m][n] == LOCK - 1)
	{
		for (int i = 0; i < ROW; i++)
		{
			for (int j = 0; j < COL; j++)
			{
				if (map[i][j] == LOCK - 1)
				{
					map[i][j] -= LOCK;
				}
			}
		}
	}
	
}
//鼠标操作
void MouseCtl()
{
	if (MouseHit()) //判断有无鼠标消息
	{
		MOUSEMSG msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 0 && msg.x <= SIZE * COL && msg.y >= 0 && msg.y <= SIZE * ROW)
		{
			int m = msg.y / SIZE;
			int n = msg.x / SIZE;
			dealmsg(m, n);
			if(count==0)
			{
				for (int i = 0; i < ROW; i++)
				{
					for (int j = 0; j < COL; j++)
					{
						if (map[i][j] != LOCK - 1)
						{
							map[i][j] += LOCK * 2;
						}
					}
				}
			}
		}
	}
}
void iswinner()
{
	if (count == 0)
	{
		score.end = time(NULL);
		
		HWND hnd = GetHWnd();
		char str[50];
		sprintf(str, "恭喜您通关啦\n您的成绩是：%d秒", score.end - score.start);
		isok = MessageBox(hnd, str , "通关成功", MB_OKCANCEL);
	}
	else
	{
		for (int i = 0; i < ROW; i++)
		{
			for (int j = 0; j < COL; j++)
			{
				if (map[i][j] == -1)
				{
					HWND hnd = GetHWnd();
					isok = MessageBox(hnd, "game over \n 您真不幸 ，被炸死啦！", "通关失败", MB_OKCANCEL);
					return;
				}
			}
		}
	}
}
void BGM()
{
	mciSendString("open ./BGM/Leadfoot.mp3 alias BGM", 0, 0, 0);
	mciSendString("play BGM", 0, 0, 0);
}
void show()
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			printf("%2d ", map[i][j]);
		}
		printf("\n");
	}
}

int main()
{
	//创建窗口
	initgraph(SIZE * ROW, SIZE * COL);
gamestart:
	BGM();
	isok = -1;
	score.start = time(NULL);
	GameInit();
	//show();
	while(1)
	{
		GameDraw();
		iswinner();
		MouseCtl();
		if (isok == IDOK)
		{
			goto
				gamestart;
		}
		else if (isok == IDCANCEL)
		{
			break;
		}
	}
	
	return 0;
}