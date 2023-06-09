#define _CRT_SECURE_NO_WARNINGS 1
/*
	ɨ��С��Ϸ
	1.���Ƶ�ͼ ����ά���� �� ������
	2.��ͼ ��������ת����ͼƬ �����ܣ����
	3.������������򿪸��ӣ���������� ��ת���������±꣩
	4.�㵽�ո�֮�󣬴���Χ�ĸ��ӣ��ݹ飩
	5.�ж���Ϸ����Ӯ
	6.���� ������
*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<graphics.h>
#include<conio.h>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")

#define SIZE 60      //���ڳߴ�
#define ROW 10          //��������
#define COL 10          //��������
#define BOOM 50         //�׵ĸ���
#define LOCK 20

int map[ROW][COL];  //ȫ�ֱ����Զ���ʼ��Ϊ0��
int count;        //����ʣ��İ�ȫ����
int isok;         //�洢�Ƿ��� ȷ�� ȡ��
IMAGE img[12];

//��¼��Ϸʱ��ṹ��
struct {
	unsigned int start;
	unsigned int end;
}score;
//��Ϸ��ʼ��
void GameInit()
{
	setbkcolor(WHITE);
	cleardevice();
	//�������������
	srand((unsigned int)time(NULL));
	//��ʼ������
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			map[i][j] = 0;
		}
	}
	//���ף� -1��ʾ�� boom����
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
	//������Ϊ���ĵľŹ�������+1��
	for (int i=0; i < ROW; i++)
	{
		for (int j=0; j < COL; j++)
		{
			//�ж��Ƿ�����
			if (map[i][j] == -1)
			{
				for (int m = 0; m < 3; m++)
				{
					for (int n = 0; n < 3; n++)
					{
						if (i - 1 + m >= 0 && i - 1 + m < ROW && j - 1 + n >= 0 && j - 1 + n < COL && map[i - 1 + m][j - 1 + n] != -1)
						{
							//���ܱ߲����׵ĸ�������+1��
							map[i - 1 + m][j - 1 + n] ++;
						}
					}
				}
			}
		}
	}
	//��ʼ����ȫ���Ӹ���
	count = ROW * COL - BOOM;
	//����ͼƬ
	char file[50] = "";
	for (int i = 0; i < 12; i++)
	{
		sprintf(file, "./image/%d.jpg", i);
		loadimage(&img[i], file , SIZE , SIZE);
	}
	//���ܸ���
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			map[i][j] += LOCK;
		}
	}
}




//���ƽ���
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
//������Ϣ
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
//������
void MouseCtl()
{
	if (MouseHit()) //�ж����������Ϣ
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
		sprintf(str, "��ϲ��ͨ����\n���ĳɼ��ǣ�%d��", score.end - score.start);
		isok = MessageBox(hnd, str , "ͨ�سɹ�", MB_OKCANCEL);
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
					isok = MessageBox(hnd, "game over \n ���治�� ����ը������", "ͨ��ʧ��", MB_OKCANCEL);
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
	//��������
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