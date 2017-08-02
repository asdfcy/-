#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>
#include<windows.h>
using namespace std;

typedef struct { int x, y; }坐标;

char 地图[22][22];                        //定义一个22*22的地图（含边界）
坐标 蛇[400], 食物, 下一步蛇头的位置;            //定义蛇、食物、下一步蛇头的位置
int 蛇头, 蛇尾;                            //用于储存蛇头和蛇尾的下标
int 等级, 蛇长度, 刷新时间;            //游戏等级、蛇长、自动前进所需时间
char 前进方向;                            //前进方向

										   //定位光标
void 光标(int x, int y)
{
	HANDLE hConsoleOutput;
	COORD dwCursorPosition;
	dwCursorPosition.X = x;
	dwCursorPosition.Y = y;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsoleOutput, dwCursorPosition);
}

//用inline定义内联函数节省程序运行时的调用开销
//刷新地图
inline void Update(char 地图[][22], int 等级, int 蛇长度, int 刷新时间)
{
	//system("cls");                        //清屏
	光标(0, 0);
	int i, j;
	printf("\n");
	for (i = 0; i < 22; i++)
	{
		printf("\t");
		for (j = 0; j < 22; j++)
			printf("%c ", 地图[i][j]);
		if (i == 0)
			printf("\t等级为：%d", 等级);
		if (i == 2)
			printf("\t长度为：%d", 蛇长度);
		if (i == 6)
			printf("\t自动前进时间");
		if (i == 8)
			printf("\t间隔为：%d ms", 刷新时间);
		printf("\n");
	}
}

//欢迎界面
inline void 欢迎界面()
{
	puts("\n\n\n\t\t\t贪吃蛇游戏即将开始！");                        //准备开始
	double start;
	for (int i = 3; i >= 0; i--)
	{
		start = (double)clock() / CLOCKS_PER_SEC;                //得到程序目前为止运行的时间
		while ((double)clock() / CLOCKS_PER_SEC - start <= 1);    //经过1秒之后
		if (i > 0)
		{
			system("cls");                                        //清屏
			printf("\n\n\n\t\t\t进入倒计时：%d\n", i);                //倒计时
		}
		else
			Update(地图, 等级, 蛇长度, 刷新时间);                //刷新地图
	}
}

//随机生成食物位置
inline void f()
{
	srand(int(time(0)));                                        //调用种子函数
	do {
		食物.x = rand() % 20 + 1;
		食物.y = rand() % 20 + 1;
	} while (地图[食物.x][食物.y] != ' ');
	地图[食物.x][食物.y] = '!';                                    //食物为“!”
}

//初始化
inline void init()
{
	int i, j;
	for (i = 1; i <= 20; i++)
		for (j = 1; j <= 20; j++)
			地图[i][j] = ' ';
	for (i = 0; i <= 21; i++)
		地图[0][i] = 地图[21][i] = 地图[i][0] = 地图[i][21] = '*';    //边界
	地图[1][1] = 地图[1][2] = 'O';                                //蛇身（含蛇尾）
	地图[1][3] = '@';                                            //蛇头
	蛇头 = 2; 蛇尾 = 0;                                            //开始时头和尾的下标
	蛇[蛇头].x = 1; 蛇[蛇头].y = 3;                        //开始时蛇头在地图上的下标
	蛇[蛇尾].x = 1; 蛇[蛇尾].y = 1;                        //开始时蛇尾在地图上的下标
	蛇[1].x = 1; 蛇[1].y = 2;                                //开始时蛇身在地图上的下标
	f();                                                        //随机生成食物位置
	等级 = 1; 蛇长度 = 3; 刷新时间 = 500;                        //开始的等级、长度、自动前进时间
	前进方向 = 77;                                                //初始的运动方向向右
}

//预前进
inline int GO()
{
	bool timeover = true;
	double start = (double)clock() / CLOCKS_PER_SEC;            //得到程序目前为止运行的时间

L:
	//自动经过1秒或者等待1秒内的键盘输入
	while ((timeover = ((double)clock() / CLOCKS_PER_SEC - start <= 刷新时间 / 1000.0)) && !_kbhit());
	//键盘输入
	if (timeover)
	{
		//_getch();
		char d = _getch();                                    //获取方向
		if (d != 72 && d != 80 && d != 75 && d != 77 || 前进方向 == 72 && d == 80 || 前进方向 == 80 && d == 72 || 前进方向 == 75 && d == 77 || 前进方向 == 77 && d == 75)
			goto L;
		else
			前进方向 = d;
	}
	switch (前进方向)
	{
	case 72:
		下一步蛇头的位置.x = 蛇[蛇头].x - 1; 下一步蛇头的位置.y = 蛇[蛇头].y;        //向上
		break;
	case 80:
		下一步蛇头的位置.x = 蛇[蛇头].x + 1; 下一步蛇头的位置.y = 蛇[蛇头].y;        //向下
		break;
	case 75:
		下一步蛇头的位置.x = 蛇[蛇头].x; 下一步蛇头的位置.y = 蛇[蛇头].y - 1;        //向左
		break;
	case 77:
		下一步蛇头的位置.x = 蛇[蛇头].x; 下一步蛇头的位置.y = 蛇[蛇头].y + 1;        //向右
		break;
	default:
		puts("\tGame over!");                                    //按下非方向键游戏失败
		return 0;
	}
	if (下一步蛇头的位置.x == 0 || 下一步蛇头的位置.x == 21 || 下一步蛇头的位置.y == 0 || 下一步蛇头的位置.y == 21)            //撞到边界
	{
		puts("\tGame over!");
		return 0;
	}
	if (地图[下一步蛇头的位置.x][下一步蛇头的位置.y] != ' ' && !(下一步蛇头的位置.x == 食物.x&&下一步蛇头的位置.y == 食物.y))    //吃到自己
	{
		puts("\tGame over!");
		return 0;
	}
	if (蛇长度 == 400)                                                        //最长蛇长
	{
		puts("\tGood game!");
		return 0;
	}
	return 1;
}

//吃到食物
inline void EAT()
{
	蛇长度++;                                        //长度增加1
	int _等级 = 蛇长度 / 10 + 1;                    //计算等级
	if (_等级 != 等级)
	{
		等级 = _等级;
		if (刷新时间 >= 100)
			刷新时间 = 550 - 等级 * 50;            //增加一级自动时间减短50毫秒
	}
	地图[下一步蛇头的位置.x][下一步蛇头的位置.y] = '@';                        //蛇头位置变化
	地图[蛇[蛇头].x][蛇[蛇头].y] = 'O';        //原蛇头位置变化为蛇身
	蛇头 = (蛇头 + 1) % 400;                        //蛇头下标加1
	蛇[蛇头].x = 下一步蛇头的位置.x; 蛇[蛇头].y = 下一步蛇头的位置.y;    //蛇头下标变化
	f();                                            //随机生成食物位置
	Update(地图, 等级, 蛇长度, 刷新时间);            //刷新地图
}

//没吃到食物
inline void FAILURE()
{
	地图[蛇[蛇尾].x][蛇[蛇尾].y] = ' ';        //蛇尾原来的位置变成“ ”
	蛇尾 = (蛇尾 + 1) % 400;                        //蛇尾下标加1
	地图[下一步蛇头的位置.x][下一步蛇头的位置.y] = '@';                        //蛇头位置变化
	地图[蛇[蛇头].x][蛇[蛇头].y] = 'O';        //原蛇头位置变化为蛇身
	蛇头 = (蛇头 + 1) % 400;                        //蛇头下标加1
	蛇[蛇头].x = 下一步蛇头的位置.x;                            //蛇头下标变化
	蛇[蛇头].y = 下一步蛇头的位置.y;
	Update(地图, 等级, 蛇长度, 刷新时间);            //刷新地图
}

//main函数
int main()
{
	system("color F0");
	init();                    //初始化
	欢迎界面();                //欢迎界面
	while (1)
	{
		if (GO())            //预前进
		{
			if (下一步蛇头的位置.x == 食物.x&&下一步蛇头的位置.y == 食物.y)
				EAT();        //吃到食物
			else
				FAILURE();    //没吃到食物
		}
		else
			return 0;        //失败或者胜利，游戏结束
	}
	return 0;
}
//