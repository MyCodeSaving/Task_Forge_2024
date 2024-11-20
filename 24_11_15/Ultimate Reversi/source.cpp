#include<easyx.h>
#include<graphics.h>
#include<strstream>
#include<iostream>
#include<fstream>
#include<time.h>
#include<ctime>
#include<stdlib.h>
#include<stdio.h>
#include<conio.h>
#include<algorithm>
#include<math.h>
#include <map>
#include <string>

using namespace std;

#define WINDOWS_X 1200	//窗口大小X
#define WINDOWS_Y 650	//窗口大小Y

#define PX 280			//棋盘偏移量X
#define PY 80			//棋盘偏移量Y
#define BBLACK 70		//空格大小

#define CHESSSIZE 20	//棋子大小
#define SIZE 8			//棋盘格数

#define ESC 27
#define ESCEXIT (_kbhit()&&_getch()==ESC)

#pragma comment (lib,"ws2_32.lib")						// 引用 Windows Multimedia API
#pragma comment(lib,"Winmm.lib")

const int black = 1;	//黑棋
const int white = 0;	//白棋
int step = 0;  //回合数
int diff = 1; //难度
int whitecount = 2;  //黑白棋得分
int blackcount = 2;
bool single = true; //单人模式
const int timeout_turn = 3000;		//每步棋的思考时间
clock_t start;					//AI开始思考的时间点
bool stopThink = false;			//AI是否停止思考
const int MAX_DEPTH = 20, MIN_DEPTH = 4;

int mapp[SIZE][SIZE] = {};		//棋盘矩阵存储
int historymap[100][SIZE][SIZE] = {};
const int MOVE[8][2] = { { -1, 0 },{ 1, 0 },{ 0, -1 },{ 0, 1 },{ -1, -1 },{ 1, -1 },{ 1, 1 },{ -1, 1 } };	//方位
const int MAPPOINTCOUNT[8][8] =												//棋盘各点权值估值
{
	{ 90,-60,10,10,10,10,-60,90 },
	{ -60,-80,5,5,5,5,-80,-60 },
	{ 10,5,1,1,1,1,5,10 },
	{ 10,5,1,1,1,1,5,10 },
	{ 10,5,1,1,1,1,5,10 },
	{ 10,5,1,1,1,1,5,10 },
	{ -60,-80,5,5,5,5,-80,-60 },
	{ 90,-60,10,10,10,10,-60,90 }
};
IMAGE MAPIMAGE[SIZE][SIZE];
IMAGE COUNT[3];

int expect[SIZE][SIZE] = {};

SOCKET sockSer;
SOCKET sockConn;

int MYCOLOR = 1;							//我的颜色 1黑色 0白色 -1其他
int NOWCOLOR = 1;							//当前执子颜色
bool TIANEYES = false;							//天眼模式

class POINT2
{
public:
	void WIN2MAP(POINT2 &MAP)			//建立棋盘与矩阵的映射关系
	{
		MAP.x = (x - PX) / BBLACK;
		MAP.y = (y - PY) / BBLACK;
	}
	void MAP2WIN(POINT2 &WIN)			//建立矩阵与棋盘的映射关系
	{
		WIN.x = PX + x*BBLACK + BBLACK / 2;
		WIN.y = PY + y*BBLACK + BBLACK / 2;
	}
	void INIT(int x, int y)				//插入元素
	{
		this->x = x;
		this->y = y;
	}
	void ADD(int x, int y)				//计算和
	{
		this->x += x;
		this->y += y;
	}
	int MAP2WINX()
	{
		return PX + x*BBLACK + BBLACK / 2;
	}
	int MAP2WINY()
	{
		return PY + y*BBLACK + BBLACK / 2;
	}
	int WIN2MAPX()
	{
		return (x - PX) / BBLACK;
	}
	int WIN2MAPY()
	{
		return (y - PY) / BBLACK;
	}
	int x, y;
};

POINT2 LASTCH;								//上一步棋子方位

void gameStart();						//函数声明
void STARTVS(int YOURCOLOR, POINT2 P1(), POINT2 P2());

void mappadd(int x, int y, int color, int MAP[SIZE][SIZE])		//向地图中添加棋子
{
	POINT2 WINDOWS2, MAP2;
	WINDOWS2.INIT(x, y);
	WINDOWS2.WIN2MAP(MAP2);
	MAP[MAP2.x][MAP2.y] = color ? 1 : -1;
}

void printcircle(int x, int y, int color, int MAP[SIZE][SIZE])				//绘制棋子
{
	mappadd(x, y, color, MAP);
	setfillcolor(color ? BLACK : WHITE);					//从中间放大变换
	for (int i = 0; i <= CHESSSIZE; ++i)
	{
		solidcircle(x, y, i);
		Sleep(1);
	}
}

void init()															//绘制棋盘
{
	memset(mapp, 0, sizeof(mapp));									//初始化
	memset(historymap, 0, sizeof(historymap));
	memset(expect, 0, sizeof(expect));
	TIANEYES = false;												//关闭天眼模式
	LASTCH.INIT(0, 0);
	step = 1;
	whitecount = 2;
	blackcount = 2;
	NOWCOLOR = 1;

	settextstyle(15, 0, "宋体");
	loadimage(NULL, "Picture/1.png");										//背景图片
	for (int x = PX; x < PX + BBLACK*(SIZE + 1); x += BBLACK)		//绘制棋盘 横线
	{
		if ((x / BBLACK % 2) == 0)setlinecolor(BLACK);
		else setlinecolor(RED);
		line(x, PY, x, PY + BBLACK*SIZE);
		line(x-1, PY, x-1, PY + BBLACK*SIZE);

	}
	for (int y = PY; y <PY + BBLACK*(SIZE + 1); y += BBLACK)		//绘制棋盘 竖线
	{
		if (y / BBLACK % 2 == 0)setlinecolor(BLACK);
		else setlinecolor(RED);
		line(PX, y, PX + BBLACK*SIZE, y);
		line(PX, y-1, PX + BBLACK*SIZE, y-1);
	}
	for (int i = PX; i <= PX + BBLACK*SIZE; i += BBLACK)			//获取每部分图片保存在图片指针中
	{
		for (int j = PY; j <= PY + BBLACK*SIZE; j += BBLACK)
		{
			POINT2 DATA;
			DATA.INIT(i, j);
			DATA.WIN2MAP(DATA);
			getimage(&MAPIMAGE[DATA.x][DATA.y], i, j, BBLACK, BBLACK);
		}
	}

	printcircle(PX + (SIZE / 2 - 1)* BBLACK + BBLACK / 2, PY + (SIZE / 2 - 1) * BBLACK + BBLACK / 2, white, mapp);		//初始的四枚棋子
	printcircle(PX + (SIZE / 2 - 1) * BBLACK + BBLACK / 2, PY + (SIZE / 2) * BBLACK + BBLACK / 2, black, mapp);
	printcircle(PX + (SIZE / 2) * BBLACK + BBLACK / 2, PY + (SIZE / 2) * BBLACK + BBLACK / 2, white, mapp);
	printcircle(PX + (SIZE / 2) * BBLACK + BBLACK / 2, PY + (SIZE / 2 - 1) * BBLACK + BBLACK / 2, black, mapp);


	getimage(COUNT, WINDOWS_X / 30, WINDOWS_Y - WINDOWS_Y / 6, 230, 40);											//成绩所在区域背景图片指针
	getimage(COUNT + 1, WINDOWS_X - WINDOWS_X / 5, WINDOWS_Y - WINDOWS_Y / 6, 230, 40);
	getimage(COUNT + 2, BBLACK / 4 + BBLACK * 2, BBLACK / 4 + 2 * BBLACK, 100, 40);   //回合数

	LOGFONT f;
	gettextstyle(&f);												// 获取字体样式
	f.lfHeight = 35;												// 设置字体高度
	strcpy_s(f.lfFaceName, _T("方正姚体"));							// 设置字体
	f.lfQuality = ANTIALIASED_QUALITY;								// 设置输出效果为抗锯齿
	settextstyle(&f);												// 设置字体样式

	settextcolor(BLACK);
	outtextxy(BBLACK / 4, BBLACK / 2, "你所执子");
	outtextxy(BBLACK / 4, BBLACK / 4 + BBLACK, "当前执子");
	outtextxy(BBLACK / 4, BBLACK / 4 + 2 * BBLACK, "当前回合");

	settextstyle(25, 0, "黑体");
	rectangle((WINDOWS_X - WINDOWS_X / 4 - 10), (0.5*BBLACK - 10), (WINDOWS_X - WINDOWS_X / 4 + 1.5*BBLACK), BBLACK);
	outtextxy(WINDOWS_X - WINDOWS_X / 4, 0.5*BBLACK, "保存游戏");          //保存，读取。悔棋
	rectangle((WINDOWS_X - WINDOWS_X / 4 + 2 * BBLACK), (0.5 * BBLACK - 10), (WINDOWS_X - WINDOWS_X / 4 + 3.5 * BBLACK + 10), BBLACK);
	outtextxy(WINDOWS_X - WINDOWS_X / 4 + 2 * BBLACK + 10, 0.5*BBLACK, "读取游戏");          //保存，读取。悔棋

	settextstyle(30, 0, "方正姚体");
	rectangle(WINDOWS_X - WINDOWS_X / 4 - 10,  1.5*BBLACK, WINDOWS_X - WINDOWS_X / 4 + BBLACK, 2 * BBLACK);
	outtextxy(WINDOWS_X - WINDOWS_X / 4,  1.5 * BBLACK, "悔棋");          //悔棋

}

void printmapp()         //画出棋盘上的棋子
{
	int i, j;
	POINT2 WINDOWS2, MAP2;
	for (i = 0; i < SIZE; ++i)                                     //画出棋盘上的棋子
		for (j = 0; j < SIZE; ++j) {
			if (mapp[i][j] == 1 || mapp[i][j] == -1) {
				MAP2.INIT(i, j);
				MAP2.MAP2WIN(WINDOWS2);
				setfillcolor(mapp[i][j] == 1 ? BLACK : WHITE);					//从中间放大变换
				for (int i = 0; i <= CHESSSIZE; ++i)
				{
					solidcircle(WINDOWS2.x, WINDOWS2.y, i);
				}
			}
		}
}


int Judge(int x, int y, int color, int MAP[SIZE][SIZE])									//预判当前位置能否下子
{
	if (MAP[x][y])return 0;																//如果当前位置已经有棋子
	int me = color ? 1 : -1;															//准备落棋棋子颜色
	POINT2 star;
	int count = 0, flag;																//count为该位置可以转换对手棋子个数
	for (int i = 0; i < SIZE; ++i)														//搜索
	{
		flag = 0;
		star.INIT(x + MOVE[i][0], y + MOVE[i][1]);
		while (star.x >= 0 && star.x < SIZE&&star.y >= 0 && star.y < SIZE&&MAP[star.x][star.y])
		{
			if (MAP[star.x][star.y] == -me)flag++;
			else
			{
				count += flag;
				break;
			}
			star.ADD(MOVE[i][0], MOVE[i][1]);
		}
	}
	return count;																		//返回该点转换对方棋子个数
}

void Change(POINT2 NOW, int MAP[SIZE][SIZE], bool F)												//落子后改变棋盘状态 F为是否输出到屏幕
{
	int me = MAP[NOW.x][NOW.y];																	//当前落子棋子颜色
	bool flag;
	POINT2 a, b;
	for (int i = 0; i<SIZE; ++i)																//搜索
	{
		flag = false;
		a.INIT(NOW.x + MOVE[i][0], NOW.y + MOVE[i][1]);
		while (a.x >= 0 && a.x<SIZE&&a.y >= 0 && a.y<SIZE&&MAP[a.x][a.y])
		{
			if (MAP[a.x][a.y] == -me)flag = true;
			else
			{
				if (flag)
				{
					a.ADD(-MOVE[i][0], -MOVE[i][1]);
					b.INIT(NOW.x + MOVE[i][0], NOW.y + MOVE[i][1]);
					while (((NOW.x <= b.x && b.x <= a.x) || (a.x <= b.x && b.x <= NOW.x)) && ((NOW.y <= b.y && b.y <= a.y) || (a.y <= b.y && b.y <= NOW.y)))
					{
						if (F)printcircle(b.MAP2WINX(), b.MAP2WINY(), (me == 1) ? black : white, MAP);	//改变棋子
						if (!F)mappadd(b.MAP2WINX(), b.MAP2WINY(), (me == 1) ? black : white, MAP);		//如果不输出到屏幕，改变地图数组
						b.ADD(MOVE[i][0], MOVE[i][1]);
					}
				}
				break;
			}
			a.ADD(MOVE[i][0], MOVE[i][1]);
		}
	}
}

int Statistics(int color)														//预判每个位置可以转化对手棋子个数
{
	int NOWEXPECT = 0;															//总的转化棋子个数
	for (int i = 0; i < SIZE; ++i)												//遍历
		for (int j = 0; j < SIZE; ++j)
		{
			expect[i][j] = Judge(i, j, color, mapp);									//存储该位置可以转化棋子个数
			if (expect[i][j])
			{
				++NOWEXPECT;

				POINT2 a;
				a.INIT(i, j);
				if (!single || color == MYCOLOR) {
					setfillcolor(RGB(96, 96, 96));
					circle(a.MAP2WINX(), a.MAP2WINY(), CHESSSIZE / 4);
					circle(a.MAP2WINX(), a.MAP2WINY(), CHESSSIZE / 4 - 1);
				}

				if (TIANEYES)														//如果开启天眼模式
				{
					settextstyle(15, 0, "宋体");
					TCHAR s[20];													//天眼模式
					_stprintf_s(s, _T("%d"), expect[i][j]);
					outtextxy(a.MAP2WINX(), a.MAP2WINY() + 10, s);
				}
			}
		}
	return NOWEXPECT;
}

void CleanStatistics()										//清除期望点提示
{
	for (int i = 0; i < SIZE; ++i)
	{
		for (int j = 0; j < SIZE; ++j)
		{
			if (expect[i][j] && !mapp[i][j])				//如果当前点没有棋子或者有期望
			{
				POINT2 a;
				a.INIT(i, j);								//记录坐标
				putimage(a.MAP2WINX() - BBLACK / 2, a.MAP2WINY() - BBLACK / 2, &MAPIMAGE[i][j]);	//输出局部背景
			}
		}
	}
}

string INTTOCHI(int num, int color)											//当前局势成绩输出汉字
{
	string number[10] = { "","一","二","三","四","五","六","七","八","九" };
	string data = "";
	if (num >= 10)
	{
		data += number[num / 10 % 10];
		data += "十";
	}
	data += number[num % 10];
	//return (color ? "黑棋：" : "白棋：") + data;							//num>=0&&num<=64
	return (color ? "黑棋：" : "白棋：") + std::to_string(num);
}

void Printcount(int balckcount, int whitecount, int nowcolor)		//输出当前分数
{
	settextcolor(BLACK);											//更改字体颜色
	settextstyle(35, 0, "方正姚体");

	putimage(WINDOWS_X / 30, WINDOWS_Y - WINDOWS_Y / 6, COUNT);		//擦出原来痕迹
	putimage(WINDOWS_X - WINDOWS_X / 5, WINDOWS_Y - WINDOWS_Y / 6, COUNT + 1);
	putimage(BBLACK / 4 + BBLACK * 2, BBLACK / 4 + 2 * BBLACK, COUNT + 2);

	outtextxy(WINDOWS_X / 30, WINDOWS_Y - WINDOWS_Y / 6, INTTOCHI(whitecount, white).data());	//输出当前成绩
	outtextxy(WINDOWS_X - WINDOWS_X / 5, WINDOWS_Y - WINDOWS_Y / 6, INTTOCHI(balckcount, black).data());

	char s[5] = {};
	_itoa_s(step, s, 3, 10);
	settextstyle(55, 0, "Kristen ITC");
	outtextxy(BBLACK / 4 + BBLACK * 2, BBLACK / 4 + 2 * BBLACK - 10, s);

	setfillcolor(MYCOLOR == 1 ? BLACK : MYCOLOR == 0 ? WHITE : LIGHTCYAN);						//从中间放大变换
	solidcircle(BBLACK * 2 + 20, BBLACK * 3 / 4, CHESSSIZE * 3 / 4);
	setfillcolor((!nowcolor || balckcount + whitecount == 4) ? BLACK : WHITE);
	NOWCOLOR = (!nowcolor || balckcount + whitecount == 4) ? black : white;						//记录当前执子
	for (int i = 0; i <= CHESSSIZE * 3 / 4; ++i)
	{
		solidcircle(BBLACK * 2 + 20, BBLACK * 3 / 2, i);
		Sleep(1);
	}
}

void WIN(int YOURCOLOR, int balckcount, int whitecount)			//判断输赢
{
	HWND wnd = GetHWnd();										//获取窗口句柄
	if (balckcount>whitecount)
	{
		if (YOURCOLOR == black) mciSendString("play Music/胜利笑声.wav", NULL, 0, NULL);
		else  mciSendString("play Music/失败.wav", NULL, 0, NULL);
		MessageBox(wnd, YOURCOLOR == black ? "恭喜你，胜利啦" : YOURCOLOR == white ? "失败，再接再厉！" : "黑方得胜~", "Result", MB_OK);
	}
	else if (balckcount<whitecount)
	{
		if (YOURCOLOR == white) mciSendString("play Music/胜利笑声.wav", NULL, 0, NULL);
		else  mciSendString("play Music/失败.wav", NULL, 0, NULL);
		MessageBox(wnd, YOURCOLOR == white ? "恭喜你，胜利啦" : YOURCOLOR == black ? "失败，再接再厉！" : "白方得胜", "Result", MB_OK);
	}
	else
	{
		mciSendString("play Music/平局.wav", NULL, 0, NULL);
		MessageBox(wnd, "平局！", "Result", MB_OK);
	}
}

void HL(int NOWWJ)
{
	if (NOWWJ != -1)
	{
		HWND wnd = GetHWnd();										//获取窗口句柄
		MessageBox(wnd, NOWWJ == MYCOLOR ? "你没有可以下的子！" : "对方已无子可下！", "回合跳过", MB_OK);
	}
}

POINT2 Easy()										//人机对战简单AI
{
	POINT2 MAX;										//定义以及初始化最优解
	MAX.INIT(0, 0);
	int maxx = 0;
	for (int i = 0; i < SIZE; ++i)
		for (int j = 0; j < SIZE; ++j)
		{
			if (expect[i][j] >= maxx)				//寻找可以转化棋子最多的点作为最优解
			{
				maxx = expect[i][j];
				MAX.INIT(i, j);
			}
		}
	if (ESCEXIT)gameStart();
	Sleep(200);										//间歇
	return MAX;										//返回最优解
}

void copymap(int one[SIZE][SIZE], int last[SIZE][SIZE])						//拷贝地图
{
	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
			one[i][j] = last[i][j];
}


void save_game() {
	ofstream outfile("document.dat", ios::binary);//以二进制形式保存
	if (!outfile) {
		cerr << "保存失败！" << endl;
		HWND wnd = GetHWnd();										//获取窗口句柄
		MessageBox(wnd, "保存失败！", "保存游戏", MB_OK);
		abort();
	}
	outfile.write((char*)&mapp[0], sizeof(mapp));//保存棋盘
	outfile.write((char*)&step, sizeof(step));//步数
	outfile.write((char*)&MYCOLOR, sizeof(MYCOLOR));//玩家棋子颜色
	outfile.write((char*)&NOWCOLOR, sizeof(NOWCOLOR));//当前棋子颜色
	outfile.write((char*)&whitecount, sizeof(whitecount));//白棋得分
	outfile.write((char*)&blackcount, sizeof(blackcount));//黑棋得分
	outfile.write((char*)&diff, sizeof(diff));//难度
	outfile.write((char*)&single, sizeof(single));//单人模式
	outfile.write((char*)&historymap[0][0], sizeof(historymap));//历史棋盘
	outfile.close();

	HWND wnd = GetHWnd();										//获取窗口句柄
	MessageBox(wnd, "保存成功", "保存游戏", MB_OK);
}

void load_game() {
	ifstream infile("document.dat", ios::binary);//把文档以2进制形式保存
	if (!infile) {
		cerr << "读取失败！" << endl;
		HWND wnd = GetHWnd();										//获取窗口句柄
		MessageBox(wnd, "读取失败！", "读取游戏", MB_OK);
		abort();
	}
	init();
	infile.read((char*)&mapp[0], sizeof(mapp));//读取棋盘
	infile.read((char*)&step, sizeof(step));//步数
	infile.read((char*)&MYCOLOR, sizeof(MYCOLOR));//玩家棋子颜色
	infile.read((char*)&NOWCOLOR, sizeof(NOWCOLOR));//玩家棋子颜色
	infile.read((char*)&whitecount, sizeof(whitecount));//电脑棋子颜色
	infile.read((char*)&blackcount, sizeof(blackcount));//白棋得分
	infile.read((char*)&diff, sizeof(diff));//难度
	infile.read((char*)&single, sizeof(single));//单人模式
	infile.read((char*)&historymap[0][0], sizeof(historymap));//历史棋盘
	infile.close();

	printmapp();
	Printcount(blackcount, whitecount, MYCOLOR);					//当前形势
	Statistics(MYCOLOR);
	HWND wnd = GetHWnd();										//获取窗口句柄
	MessageBox(wnd, "载入游戏成功", "确定", MB_OK);
}

void hui_qi() {
	if (step < 2) {
		HWND wnd = GetHWnd();										//获取窗口句柄
		MessageBox(wnd, "无法悔棋", "悔棋", MB_OK);
		return;
	}
	bool flag = false;//用于判断这一把能否悔棋后下棋
	for (step -= 1; step > 0; step--) {//从上一回开始计算
		for (int i = 1; i <= 8; i++) {
			for (int j = 1; j <= 8; j++)
				if (historymap[step][i][j] != 0) {//若历史棋盘在某一位置不为0，则说明这个棋盘可用，因为保存棋盘时已经有能下棋的判断
					flag = true;//这个棋盘可用
					break;
				}
			if (flag)
				break;
		}
		if (flag) {//备份这个棋盘
			copymap(mapp, historymap[step]);
			break;
		}
	}
	int i, j;
	int tstep = step, tMYCOLOR = MYCOLOR, tdiff = diff, tNOWCOLOR = NOWCOLOR;
	int tmapp[SIZE][SIZE], thistorymap[100][SIZE][SIZE];
	bool tsingle = single;
	copymap(tmapp, mapp);
	for (i = 0; i <= tstep; i++) copymap(thistorymap[i], historymap[i]);
	init();
	step = tstep, MYCOLOR = tMYCOLOR, diff = tdiff, single = tsingle, NOWCOLOR = 1-tNOWCOLOR;
	copymap(mapp, tmapp);
	for (i = 0; i <= tstep; i++) copymap(historymap[i], thistorymap[i]);
	whitecount = blackcount = 0;

	for (i = 0; i < SIZE; i++)
		for (j = 0; j < SIZE; j++) {
			if (mapp[i][j] == 1) blackcount++;
			else if (mapp[i][j] == -1) whitecount++;
		}

	printmapp();
	Printcount(blackcount, whitecount,NOWCOLOR);					//当前形势
	Statistics(MYCOLOR);

	HWND wnd = GetHWnd();										//获取窗口句柄
	MessageBox(wnd, "悔棋成功", "悔棋", MB_OK);
}

POINT2 MOUSE()										//鼠标事件
{
	MOUSEMSG m;
	while (true)
	{
		m = GetMouseMsg();							//获取鼠标信息
		switch (m.uMsg)
		{
		case(WM_LBUTTONDOWN):						//当鼠标左键按下时
		{
			POINT2 NOWMOUSE;
			NOWMOUSE.INIT(m.x, m.y);
			return NOWMOUSE;						//返回鼠标坐标
			break;
		}
		case(WM_MOUSEMOVE):								//调试 输出鼠标位置
		{
			if (ESCEXIT)gameStart();
			break;
		}
		case(WM_RBUTTONDOWN):								//如果鼠标右键点下时
		{
			TIANEYES = !TIANEYES;							//开启OR关闭天眼模式
			break;
		}
		}
	}
}

bool putmouse(POINT2 &m)									//重定向鼠标坐标
{
	bool flag = true;
	int mouseinx[SIZE + 1], mouseiny[SIZE + 1];
	for (int i = 0; i < SIZE + 1; ++i)						//精确坐标打表
	{
		mouseinx[i] = PX + i*BBLACK;
		mouseiny[i] = PY + i*BBLACK;
	}
	if (m.x > WINDOWS_X - WINDOWS_X / 4 - 10 && m.x< WINDOWS_X - WINDOWS_X / 4 + 1.5*BBLACK && m.y > 0.5*BBLACK - 10 && m.y < BBLACK)  //保存游戏
	{
		mciSendString("play Music/点击.wav", NULL, 0, NULL);
		save_game();
	}
	else if (m.x > WINDOWS_X - WINDOWS_X / 4 + 2 * BBLACK && m.x< WINDOWS_X - WINDOWS_X / 4 + 3.5 * BBLACK + 10 && m.y > 0.5*BBLACK - 10 && m.y < BBLACK)  //读取存档
	{
		mciSendString("play Music/点击.wav", NULL, 0, NULL);
		load_game();
		if (!single) STARTVS(MYCOLOR, MOUSE, MOUSE);
		else {
			if (diff == 1) STARTVS(MYCOLOR, MOUSE, Easy);

		}
	}
	else if (m.x > WINDOWS_X - WINDOWS_X / 4 - 10 && m.x< WINDOWS_X - WINDOWS_X / 4 + BBLACK && m.y > 1.5*BBLACK && m.y < 2 * BBLACK)  //悔棋
	{
		mciSendString("play Music/悔棋.wav", NULL, 0, NULL);
		if (single) hui_qi();
		else {
			HWND wnd = GetHWnd();										//获取窗口句柄
			MessageBox(wnd, "双人游戏不允许悔棋", "悔棋", MB_OK);
		}
	}
	if (m.x < PX || m.x>PX + SIZE*BBLACK || m.y < PY || m.y>PY + SIZE*BBLACK)flag = false;	//如果点击在棋盘外
	else
	{
		mciSendString("play Music/下棋.wav", NULL, 0, NULL);
		for (int i = 0; i<SIZE; ++i)
		{
			if (m.x >= mouseinx[i] && m.x <= mouseinx[i + 1])
			{
				if (m.x - mouseinx[i]>BBLACK / 8 && mouseinx[i + 1] - m.x>BBLACK / 8)		//重定向X
				{
					m.x = (mouseinx[i] + mouseinx[i + 1]) / 2;
				}
				else flag = false;
			}
		}
		for (int i = 0; i<SIZE; ++i)
		{
			if (m.y >= mouseiny[i] && m.y <= mouseiny[i + 1])
			{
				if (m.y - mouseiny[i]>BBLACK / 8 && mouseiny[i + 1] - m.y > BBLACK / 8)		//重定向Y
				{
					m.y = (mouseiny[i] + mouseiny[i + 1]) / 2;
				}
				else flag = false;
			}
		}
	}
	return flag;											//返回当前位置能否落子
}

void CleanLast(POINT2 WINDOWS2, int F)						//记录上一步走棋位置
{
	if (LASTCH.x > SIZE&&LASTCH.y > SIZE)					//以下取消上一步填充
	{
		setfillcolor(getpixel(LASTCH.x, LASTCH.y));			//获取原来棋子颜色
		putimage(LASTCH.x - BBLACK / 2, LASTCH.y - BBLACK / 2, &MAPIMAGE[LASTCH.WIN2MAPX()][LASTCH.WIN2MAPY()]);
		solidcircle(LASTCH.x, LASTCH.y, CHESSSIZE);
	}

	setfillcolor(RGB(49, 153, 182));						//以下为填充当前走棋
	LASTCH.INIT(WINDOWS2.x, WINDOWS2.y);

	solidrectangle(WINDOWS2.x - BBLACK / 2 + 2, WINDOWS2.y - BBLACK / 2 + 2, WINDOWS2.x + BBLACK / 2 - 2, WINDOWS2.y + BBLACK / 2 - 2);		//背景矩形
	setfillcolor(F ? BLACK : WHITE);
	solidcircle(WINDOWS2.x, WINDOWS2.y, CHESSSIZE);			//画棋子
}

int Playchess(int F, POINT2 WINDOWS2, int &balckcount, int &whitecount)	//开始
{
	//F 黑方为1  白方为0
	POINT2 MAP2;

	if (WINDOWS2.x < SIZE&&WINDOWS2.y < SIZE)					//如果传入的坐标为矩阵坐标
	{
		MAP2 = WINDOWS2;
		WINDOWS2.MAP2WIN(WINDOWS2);								//将其转换成实际展示坐标
	}
	else
	{
		if (!putmouse(WINDOWS2))return 0;						//鼠标输入坐标重定向
		WINDOWS2.WIN2MAP(MAP2);									//存储重定向之后的矩阵坐标
	}
	if (expect[MAP2.x][MAP2.y])									//有位置可行
	{
		CleanStatistics();										//清除屏幕提示
		if (F)													//判断如果为黑棋得分
		{
			balckcount += expect[MAP2.x][MAP2.y] + 1;
			whitecount -= expect[MAP2.x][MAP2.y];
		}
		else
		{
			whitecount += expect[MAP2.x][MAP2.y] + 1;
			balckcount -= expect[MAP2.x][MAP2.y];
		}

		printcircle(WINDOWS2.x, WINDOWS2.y, F, mapp);			//画棋子 mapp为输入数组
		CleanLast(WINDOWS2, F);									//当前走棋棋子提示
		Change(MAP2, mapp, true);									//翻转棋子 true为显示在屏幕
		Printcount(balckcount, whitecount, F);	//打印分数

		if (balckcount + whitecount >= SIZE*SIZE || !balckcount || !whitecount)return 3;	//如果胜负已分
		if (!Statistics(!F))									//如果对方无法走棋
		{
			if (Statistics(F))									//判断自己是否可以走棋
			{
				HL(!F);											//自己可走棋输出对方无法走棋信息
				Printcount(balckcount, whitecount, !F);			//因为前面已经改变了状态，这里用于还原
				return 2;
			}
			else return 3;										//双方都无法走棋
		}
		return 1;
	}
	return 0;
}

void STARTVS(int YOURCOLOR, POINT2 P1(), POINT2 P2())			//开始对战   (mycolor,mouse,easy)
{
	mciSendString("stop Music/背景音乐.mp3", NULL, 0, NULL);
	mciSendString("play Music/棋局.mp3", NULL, 0, NULL);
	time_t timestart , timeend ;
	int music_no = 1;
	timestart= time(NULL);
	MYCOLOR = YOURCOLOR;
	Printcount(blackcount, whitecount, NOWCOLOR);					//当前形势
	Statistics(NOWCOLOR);
	if (MYCOLOR == white && step == 1) goto CX2;
	if (NOWCOLOR == white && !single) goto CX2;
	while (true)
	{
		cout << "hello" << endl;
	CX1:														//本回合忽略
		timeend = time(NULL);
		if (music_no == 1 && difftime(timeend, timestart) > 170) {
			mciSendString("stop Music/背景音乐.mp3", NULL, 0, NULL);
			mciSendString("stop Music/棋局.mp3", NULL, 0, NULL);
			mciSendString("play Music/棋局2.mp3", NULL, 0, NULL);
			timestart = time(NULL);
			music_no = 2;
		}
		if (music_no == 2 && difftime(timeend, timestart) > 285) {
			mciSendString("stop Music/背景音乐.mp3", NULL, 0, NULL);
			mciSendString("stop Music/棋局2.mp3", NULL, 0, NULL);
			mciSendString("play Music/棋局.mp3", NULL, 0, NULL);
			timestart = time(NULL);
			music_no = 1;
		}
		copymap(historymap[step], mapp);
		int PD = Playchess(MYCOLOR, (*P1)(), blackcount, whitecount);
		if (single && MYCOLOR == white) step++;
		switch (PD)
		{
		case 0:
			if (MYCOLOR == white) step--;
			goto CX1;											//输入失误或无输入
			break;
		case 1:
			break;												//正常结束
		case 2:
			if (MYCOLOR == white) step++;
			goto CX1;											//忽略对方
			break;
		case 3:
			goto ED;											//棋局结束
			break;
		}
	CX2:                                     //本回合忽略
		if (!single) step++;
		if (single && MYCOLOR == black) step++;
		PD = Playchess(!MYCOLOR, (*P2)(), blackcount, whitecount);
		switch (PD)
		{
		case 0:
			goto CX2;
			break;
		case 1:
			break;
		case 2:
			goto CX2;
			break;
		case 3:
			goto ED;
			break;
		}
	}
ED:																	//结束
	WIN(YOURCOLOR, blackcount, whitecount);
	_getch();
	gameStart();
}



void gameStart()
{
	mciSendString("stop Music/棋局.mp3", NULL, 0, NULL);
	mciSendString("play Music/背景音乐.mp3", NULL, 0, NULL);
	time_t timestart, timeend;
	timestart = time(NULL);

	IMAGE MM[11] = { 0 }, MB[3] = { 0 }, MC[3] = { 0 };
	initgraph(WINDOWS_X, WINDOWS_Y);
	setbkmode(TRANSPARENT);					//透明字体

	HWND hwnd = GetHWnd();					// 设置窗口标题文字
	SetWindowText(hwnd, "翻转棋阵");
	loadimage(NULL, "Picture/0.png", 1200, 800);

	const int bblack = 10;

	LOGFONT f;
	gettextstyle(&f);												// 获取字体样式
	f.lfHeight = BBLACK;												// 设置字体高度
	strcpy_s(f.lfFaceName, _T("方正姚体"));								// 设置字体
	f.lfQuality = ANTIALIASED_QUALITY;								// 设置输出效果为抗锯齿
	settextstyle(&f);												// 设置字体样式
	RECT r1 = { 0, 0, WINDOWS_X, WINDOWS_Y / 3 };
	drawtext("黑 白 棋", &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	settextstyle(BBLACK / 3, 0, "方正姚体");
	RECT r2 = { WINDOWS_X / 2 - BBLACK,WINDOWS_Y / 3,WINDOWS_X / 2 + BBLACK ,WINDOWS_Y / 3 + BBLACK / 2 };
	rectangle(WINDOWS_X / 2 - BBLACK, WINDOWS_Y / 3, WINDOWS_X / 2 + BBLACK, WINDOWS_Y / 3 + BBLACK / 2);
	drawtext("单人模式", &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT r3 = { WINDOWS_X / 2 - BBLACK,WINDOWS_Y / 3 + BBLACK / 2 + bblack,WINDOWS_X / 2 + BBLACK,WINDOWS_Y / 3 + BBLACK + bblack };
	rectangle(WINDOWS_X / 2 - BBLACK, WINDOWS_Y / 3 + BBLACK / 2 + bblack, WINDOWS_X / 2 + BBLACK, WINDOWS_Y / 3 + BBLACK + bblack);
	drawtext("双人模式", &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT r4 = { WINDOWS_X / 2 - BBLACK,WINDOWS_Y / 3 + BBLACK + 2 * bblack,WINDOWS_X / 2 + BBLACK,(int)(WINDOWS_Y / 3 + BBLACK*1.5 + 2 * bblack) };
	rectangle(WINDOWS_X / 2 - BBLACK, WINDOWS_Y / 3 + BBLACK + 2 * bblack, WINDOWS_X / 2 + BBLACK, (int)(WINDOWS_Y / 3 + BBLACK*1.5 + 2 * bblack));
	drawtext("载入存档", &r4, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT r5 = { WINDOWS_X / 2 - BBLACK,(int)(WINDOWS_Y / 3 + BBLACK*1.5 + 3 * bblack),WINDOWS_X / 2 + BBLACK,WINDOWS_Y / 3 + BBLACK * 2 + 3 * bblack };
	rectangle(WINDOWS_X / 2 - BBLACK, (int)(WINDOWS_Y / 3 + BBLACK*1.5 + 3 * bblack), WINDOWS_X / 2 + BBLACK, WINDOWS_Y / 3 + BBLACK * 2 + 3 * bblack);
	drawtext("联机对战", &r5, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT r6 = { WINDOWS_X / 2 - BBLACK,WINDOWS_Y / 3 + BBLACK * 2 + 4 * bblack,WINDOWS_X / 2 + BBLACK,(int)(WINDOWS_Y / 3 + BBLACK * 2.5 + 4 * bblack) };
	rectangle(WINDOWS_X / 2 - BBLACK, WINDOWS_Y / 3 + BBLACK * 2 + 4 * bblack, WINDOWS_X / 2 + BBLACK, (int)(WINDOWS_Y / 3 + BBLACK * 2.5 + 4 * bblack));
	drawtext("观战模式", &r6, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT r7 = { WINDOWS_X / 2 - BBLACK,(int)(WINDOWS_Y / 3 + BBLACK * 2.5 + 5 * bblack),WINDOWS_X / 2 + BBLACK,WINDOWS_Y / 3 + BBLACK * 3 + 5 * bblack };
	rectangle(WINDOWS_X / 2 - BBLACK, (int)(WINDOWS_Y / 3 + BBLACK * 2.5 + 5 * bblack), WINDOWS_X / 2 + BBLACK, WINDOWS_Y / 3 + BBLACK * 3 + 5 * bblack);
	drawtext("游戏介绍", &r7, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT r8 = { WINDOWS_X / 2 - BBLACK,WINDOWS_Y / 3 + BBLACK * 3 + 6 * bblack,WINDOWS_X / 2 + BBLACK,(int)(WINDOWS_Y / 3 + BBLACK * 3.5 + 6 * bblack) };
	rectangle(WINDOWS_X / 2 - BBLACK, WINDOWS_Y / 3 + BBLACK * 3 + 6 * bblack, WINDOWS_X / 2 + BBLACK, (int)(WINDOWS_Y / 3 + BBLACK * 3.5 + 6 * bblack));
	drawtext("操作说明", &r8, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT r9 = { WINDOWS_X / 2 - BBLACK,(int)(WINDOWS_Y / 3 + BBLACK * 3.5 + 7 * bblack),WINDOWS_X / 2 + BBLACK,WINDOWS_Y / 3 + BBLACK * 4 + 7 * bblack };
	rectangle(WINDOWS_X / 2 - BBLACK, (int)(WINDOWS_Y / 3 + BBLACK * 3.5 + 7 * bblack), WINDOWS_X / 2 + BBLACK, WINDOWS_Y / 3 + BBLACK * 4 + 7 * bblack);
	drawtext("关    于", &r9, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT r10 = { WINDOWS_X / 2 - BBLACK,(int)(WINDOWS_Y / 3 + BBLACK * 4 + 8 * bblack),WINDOWS_X / 2 + BBLACK,(int)(WINDOWS_Y / 3 + BBLACK * 4.5 + 8 * bblack) };
	rectangle(WINDOWS_X / 2 - BBLACK, (int)(WINDOWS_Y / 3 + BBLACK * 4 + 8 * bblack), WINDOWS_X / 2 + BBLACK, (int)(WINDOWS_Y / 3 + BBLACK * 4.5 + 8 * bblack));
	drawtext("退出游戏", &r10, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	for (int i = 0; i <= 8; i++)																						//保存按钮图片
		getimage(MM + i, WINDOWS_X / 2 - BBLACK, WINDOWS_Y / 3 + i*bblack + i*BBLACK / 2, 2 * BBLACK, BBLACK / 2);
	getimage(MM + 9, WINDOWS_X - BBLACK, WINDOWS_Y - BBLACK / 2, BBLACK - 11, BBLACK / 2 - 11);

	bool _HOME = true, _INTRODUCTION = false, _OPERATION = false, _ABOUT = false, _TOINTERNET = false, _DRMS = false, _SELECTDIFF = false;			//TRUE表示处于当前页面,selectdiff表示已选择难度，该选择棋子颜色
	MOUSEMSG m;
	while (_HOME)
	{
		timeend = time(NULL);
		if (difftime(timeend, timestart) > 257) {
			mciSendString("stop Music/背景音乐.mp3", NULL, 0, NULL);
			mciSendString("stop Music/棋局.mp3", NULL, 0, NULL);
			mciSendString("stop Music/棋局2.mp3", NULL, 0, NULL);
			mciSendString("play Music/背景音乐.mp3", NULL, 0, NULL);
			timestart = time(NULL);
		}
		BeginBatchDraw();
		m = GetMouseMsg();
		switch (m.uMsg)
		{
		case WM_LBUTTONDOWN:												//当鼠标左键击下时
			EndBatchDraw();
			mciSendString("stop Music/棋局.mp3", NULL, 0, NULL);
			mciSendString("play Music/背景音乐.mp3", NULL, 0, NULL);
			if (m.x>WINDOWS_X / 2 - BBLACK && m.x<WINDOWS_X / 2 + BBLACK && m.y>WINDOWS_Y / 3 && m.y<WINDOWS_Y / 3 + BBLACK / 2 && _HOME && !_INTRODUCTION && !_OPERATION && !_ABOUT && !_TOINTERNET && !_DRMS && !_SELECTDIFF)//单人模式
			{
				mciSendString("play Music/按键.wav", NULL, 0, NULL);
				single = true;
				_DRMS = true;									//离开HOME界面
				cleardevice();
				loadimage(NULL, "Picture/0.png");				//背景
				rectangle(BBLACK, BBLACK, WINDOWS_X - BBLACK, WINDOWS_Y - BBLACK);

				settextstyle(BBLACK / 2, 0, "方正姚体");
				RECT r0 = { 0, 0, WINDOWS_X, WINDOWS_Y / 3 };
				drawtext("选择游戏难度", &r0, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				settextstyle(BBLACK / 3, 0, "方正姚体");
				RECT r1 = { WINDOWS_X / 2 - BBLACK,WINDOWS_Y / 3,WINDOWS_X / 2 + BBLACK ,WINDOWS_Y / 3 + BBLACK / 2 };
				rectangle(WINDOWS_X / 2 - BBLACK, WINDOWS_Y / 3, WINDOWS_X / 2 + BBLACK, WINDOWS_Y / 3 + BBLACK / 2);
				drawtext("简    单", &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				RECT r2 = { WINDOWS_X / 2 - BBLACK,WINDOWS_Y / 3 + BBLACK + 2 * bblack,WINDOWS_X / 2 + BBLACK,(int)(WINDOWS_Y / 3 + BBLACK*1.5 + 2 * bblack) };
				rectangle(WINDOWS_X / 2 - BBLACK, WINDOWS_Y / 3 + BBLACK + 2 * bblack, WINDOWS_X / 2 + BBLACK, (int)(WINDOWS_Y / 3 + BBLACK*1.5 + 2 * bblack));
				drawtext("中    等", &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				RECT r3 = { WINDOWS_X / 2 - BBLACK,WINDOWS_Y / 3 + BBLACK * 2 + 4 * bblack,WINDOWS_X / 2 + BBLACK,(int)(WINDOWS_Y / 3 + BBLACK * 2.5 + 4 * bblack) };
				rectangle(WINDOWS_X / 2 - BBLACK, WINDOWS_Y / 3 + BBLACK * 2 + 4 * bblack, WINDOWS_X / 2 + BBLACK, (int)(WINDOWS_Y / 3 + BBLACK * 2.5 + 4 * bblack));
				drawtext("困    难", &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				for (int i = 0; i < 3; i++)
				{
					getimage(MB + i, WINDOWS_X / 2 - BBLACK, WINDOWS_Y / 3 + BBLACK*i + 2 * i * bblack, 2 * BBLACK, BBLACK / 2);
				}

				RECT R = { WINDOWS_X - BBLACK,WINDOWS_Y - BBLACK / 2,WINDOWS_X - 10,WINDOWS_Y - 10 };
				rectangle(WINDOWS_X - BBLACK, WINDOWS_Y - BBLACK / 2, WINDOWS_X - 10, WINDOWS_Y - 10);
				drawtext("返回", &R, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				break;
			}
			else if (m.x>WINDOWS_X / 2 - BBLACK && m.x<WINDOWS_X / 2 + BBLACK && m.y>WINDOWS_Y / 3 && m.y<WINDOWS_Y / 3 + BBLACK / 2 && _DRMS && !_SELECTDIFF)			//简单
			{
				mciSendString("play Music/按键.wav", NULL, 0, NULL);
				_SELECTDIFF = true;
				diff = 1;         //单人简单模式

				cleardevice();
				loadimage(NULL, "Picture/0.png");				//背景
				rectangle(BBLACK, BBLACK, WINDOWS_X - BBLACK, WINDOWS_Y - BBLACK);

				settextstyle(BBLACK / 2, 0, "方正姚体");
				r1 = { 0, 0, WINDOWS_X, WINDOWS_Y / 3 };
				drawtext("选择先手还是后手", &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				settextstyle(BBLACK / 3, 0, "方正姚体");
				r2 = { WINDOWS_X / 2 - 2 * BBLACK,WINDOWS_Y / 3,WINDOWS_X / 2   ,WINDOWS_Y / 3 + BBLACK / 2 };
				rectangle(WINDOWS_X / 2 - 2 * BBLACK, WINDOWS_Y / 3, WINDOWS_X / 2 + BBLACK, WINDOWS_Y / 3 + BBLACK / 2);
				drawtext("先   手", &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				setfillcolor(BLACK);
				solidcircle((int)(WINDOWS_X / 2 + BBLACK*0.5), (int)(WINDOWS_Y / 3 + BBLACK / 4), int(BBLACK / 6));

				r3 = { (int)WINDOWS_X / 2 - 2 * BBLACK,(int)WINDOWS_Y / 3 + BBLACK + 2 * bblack,(int)WINDOWS_X / 2  ,(int)(WINDOWS_Y / 3 + BBLACK*1.5 + 2 * bblack) };
				rectangle((int)WINDOWS_X / 2 - 2 * BBLACK, (int)WINDOWS_Y / 3 + BBLACK + 2 * bblack, (int)WINDOWS_X / 2 + BBLACK, (int)(WINDOWS_Y / 3 + BBLACK*1.5 + 2 * bblack));
				drawtext("后   手", &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				setfillcolor(WHITE);
				solidcircle((int)(WINDOWS_X / 2 + BBLACK *0.5), (int)(WINDOWS_Y / 3 + BBLACK*1.25 + 2 * bblack), int(BBLACK / 6));

				for (int i = 0; i < 2; i++)
				{
					getimage(MC + i, WINDOWS_X / 2 - 2 * BBLACK, WINDOWS_Y / 3 + BBLACK*i + 2 * i * bblack, 2 * BBLACK, BBLACK / 2);
				}

				RECT R = { WINDOWS_X - BBLACK,WINDOWS_Y - BBLACK / 2,WINDOWS_X - 10,WINDOWS_Y - 10 };
				rectangle(WINDOWS_X - BBLACK, WINDOWS_Y - BBLACK / 2, WINDOWS_X - 10, WINDOWS_Y - 10);
				drawtext("返回", &R, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				break;
			}
			else if (m.x>WINDOWS_X / 2 - BBLACK && m.x<WINDOWS_X / 2 + BBLACK && m.y>WINDOWS_Y / 3 + BBLACK + 2 * bblack && m.y<(int)(WINDOWS_Y / 3 + BBLACK*1.5 + 2 * bblack) && _DRMS && !_SELECTDIFF)			//中等
			{
				mciSendString("play Music/按键.wav", NULL, 0, NULL);
				_SELECTDIFF = true;            //单人中等模式
				diff = 2;
				cleardevice();
				loadimage(NULL, "Picture/0.png");				//背景
				rectangle(BBLACK, BBLACK, WINDOWS_X - BBLACK, WINDOWS_Y - BBLACK);

				settextstyle(BBLACK / 2, 0, "方正姚体");
				r1 = { 0, 0, WINDOWS_X, WINDOWS_Y / 3 };
				drawtext("选择先手还是后手", &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				settextstyle(BBLACK / 3, 0, "方正姚体");
				r2 = { WINDOWS_X / 2 - 2 * BBLACK,WINDOWS_Y / 3,WINDOWS_X / 2  ,WINDOWS_Y / 3 + BBLACK / 2 };
				rectangle(WINDOWS_X / 2 - 2 * BBLACK, WINDOWS_Y / 3, WINDOWS_X / 2 + BBLACK, WINDOWS_Y / 3 + BBLACK / 2);
				drawtext("先   手", &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				setfillcolor(BLACK);
				solidcircle(WINDOWS_X / 2 + BBLACK / 2, WINDOWS_Y / 3 + BBLACK / 4, int(BBLACK / 6));

				r3 = { WINDOWS_X / 2 - 2 * BBLACK,WINDOWS_Y / 3 + BBLACK + 2 * bblack,WINDOWS_X / 2 ,(int)(WINDOWS_Y / 3 + BBLACK*1.5 + 2 * bblack) };
				rectangle(WINDOWS_X / 2 - 2 * BBLACK, WINDOWS_Y / 3 + BBLACK + 2 * bblack, WINDOWS_X / 2 + BBLACK, (int)(WINDOWS_Y / 3 + BBLACK*1.5 + 2 * bblack));
				drawtext("后   手", &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				setfillcolor(WHITE);
				solidcircle((int)(WINDOWS_X / 2 + BBLACK / 2), (int)(WINDOWS_Y / 3 + BBLACK*1.25 + 2 * bblack), int(BBLACK / 6));

				for (int i = 0; i < 2; i++)
				{
					getimage(MC + i, WINDOWS_X / 2 - 2 * BBLACK, WINDOWS_Y / 3 + BBLACK*i + 2 * i * bblack, 2 * BBLACK, BBLACK / 2);
				}

				RECT R = { WINDOWS_X - BBLACK,WINDOWS_Y - BBLACK / 2,WINDOWS_X - 10,WINDOWS_Y - 10 };
				rectangle(WINDOWS_X - BBLACK, WINDOWS_Y - BBLACK / 2, WINDOWS_X - 10, WINDOWS_Y - 10);
				drawtext("返回", &R, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				break;
			}
			else if (m.x>WINDOWS_X / 2 - BBLACK && m.x<WINDOWS_X / 2 + BBLACK && m.y> WINDOWS_Y / 3 + BBLACK * 2 + 4 * bblack && m.y<(int)(WINDOWS_Y / 3 + BBLACK * 2.5 + 4 * bblack) && _DRMS && !_SELECTDIFF)			//困难
			{
				mciSendString("play Music/按键.wav", NULL, 0, NULL);
				_SELECTDIFF = true;
				diff = 3;            //单人困难模式
				// 待实现
				break;
			}
			else if (m.x>WINDOWS_X / 2 - 2 * BBLACK && m.x<WINDOWS_X / 2 + BBLACK && m.y>WINDOWS_Y / 3 && m.y<(int)(WINDOWS_Y / 3 + BBLACK / 2) && _DRMS && _SELECTDIFF)			//选择先手
			{
				mciSendString("play Music/按键.wav", NULL, 0, NULL);
				MYCOLOR = black;
				_HOME = false;
				cleardevice();
				init();
				if (diff == 1) STARTVS(black, MOUSE, Easy);

				break;
			}
			else if (m.x>WINDOWS_X / 2 - 2 * BBLACK && m.x<WINDOWS_X / 2 + BBLACK && m.y>WINDOWS_Y / 3 + BBLACK + 2 * bblack && m.y<(int)(WINDOWS_Y / 3 + BBLACK*1.5 + 2 * bblack) && _DRMS && _SELECTDIFF)			//选择后手
			{
				mciSendString("play Music/按键.wav", NULL, 0, NULL);
				MYCOLOR = white;
				_HOME = false;
				cleardevice();
				init();
				if (diff == 1) STARTVS(white, MOUSE, Easy);

				break;
			}
			else if (m.x>WINDOWS_X / 2 - BBLACK && m.x<WINDOWS_X / 2 + BBLACK && m.y>WINDOWS_Y / 3 + BBLACK / 2 + bblack && m.y<WINDOWS_Y / 3 + BBLACK + bblack && _HOME && !_INTRODUCTION && !_OPERATION && !_ABOUT && !_TOINTERNET && !_DRMS && !_SELECTDIFF)//双人模式
			{
				mciSendString("play Music/按键.wav", NULL, 0, NULL);
				single = false;
				_HOME = false;									//离开HOME界面
				init();
				STARTVS(1, MOUSE, MOUSE);							//双人模式
				break;
			}
			else if (m.x>WINDOWS_X / 2 - BBLACK && m.x<WINDOWS_X / 2 + BBLACK && m.y>WINDOWS_Y / 3 + BBLACK + 2 * bblack && m.y<(int)(WINDOWS_Y / 3 + BBLACK*1.5 + 2 * bblack) && _HOME && !_OPERATION && !_ABOUT && !_INTRODUCTION && !_TOINTERNET && !_DRMS && !_SELECTDIFF)   //载入游戏
			{
				mciSendString("play Music/按键.wav", NULL, 0, NULL);
				load_game();
				_HOME = false;
				if (!single) STARTVS(MYCOLOR, MOUSE, MOUSE);
				else {
					if (diff == 1) STARTVS(MYCOLOR, MOUSE, Easy);

				}
				break;
			}
			else if (m.x>WINDOWS_X / 2 - BBLACK && m.x<WINDOWS_X / 2 + BBLACK && m.y>(int)(WINDOWS_Y / 3 + BBLACK*1.5 + 3 * bblack) && m.y<WINDOWS_Y / 3 + BBLACK * 2 + 3 * bblack && _HOME && !_OPERATION && !_ABOUT && !_INTRODUCTION && !_TOINTERNET && !_DRMS && !_SELECTDIFF)//联机对战
			{
				mciSendString("play Music/按键.wav", NULL, 0, NULL);
				single = false;
				_TOINTERNET = true;
				cleardevice();

				break;
			}
			else if (m.x>WINDOWS_X / 4 && m.x<WINDOWS_X / 4 + 2 * BBLACK  && m.y>WINDOWS_Y / 2 + BBLACK && m.y<WINDOWS_Y / 2 + 2 * BBLACK && _TOINTERNET)//我要创建
			{
				mciSendString("play Music/按键.wav", NULL, 0, NULL);

				cleardevice();

				break;
			}
			else if (m.x>WINDOWS_X - WINDOWS_X / 4 - 2 * BBLACK && m.x<WINDOWS_X - WINDOWS_X / 4 && m.y>WINDOWS_Y / 2 + BBLACK && m.y<WINDOWS_Y / 2 + 2 * BBLACK && _TOINTERNET)//我要连接
			{
				mciSendString("play Music/按键.wav", NULL, 0, NULL);

				cleardevice();

				break;
			}
			else if (m.x>WINDOWS_X / 2 - BBLACK && m.x<WINDOWS_X / 2 + BBLACK && m.y> WINDOWS_Y / 3 + BBLACK * 2 + 4 * bblack && m.y<(int)(WINDOWS_Y / 3 + BBLACK * 2.5 + 4 * bblack) && _HOME && !_OPERATION && !_ABOUT && !_INTRODUCTION && !_TOINTERNET && !_DRMS && !_SELECTDIFF)		//观战模式
			{
				mciSendString("play Music/按键.wav", NULL, 0, NULL);
				single = false;
				_HOME = false;										//离开HOME界面
				//观战模式

				break;
			}
			else if (m.x>WINDOWS_X / 2 - BBLACK && m.x<WINDOWS_X / 2 + BBLACK && m.y>(int)(WINDOWS_Y / 3 + BBLACK * 2.5 + 5 * bblack) && m.y<WINDOWS_Y / 3 + BBLACK * 3 + 5 * bblack && _HOME && !_INTRODUCTION && !_ABOUT && !_OPERATION && !_TOINTERNET && !_DRMS && !_SELECTDIFF)	//游戏介绍
			{
				mciSendString("play Music/按键.wav", NULL, 0, NULL);
				_INTRODUCTION = true;
				cleardevice();
				loadimage(NULL, "Picture/0.png");				//背景
				string data[16] = {
					"游戏介绍：" ,
					"模拟黑白棋游戏，实现了人机模式和双人对战模式。",
					""	// 可添加文本
				};

				rectangle(BBLACK, BBLACK, WINDOWS_X - BBLACK, WINDOWS_Y - BBLACK);
				settextstyle(BBLACK / 2, 0, "方正姚体");
				settextcolor(RGB(0, 255, 255));
				outtextxy(WINDOWS_X / 3 - 100, 90, data[0].data());
				settextstyle(BBLACK / 3, 0, "隶书");
				settextcolor(WHITE);
				int LEFT, TOP = 115;
				for (int i = 1; i < 16; i++)
				{
					LEFT = WINDOWS_X / 3;
					//if (i == 5 || i >= 10)LEFT -= 25;
					//if (i == 5 || i == 10 || i == 6)TOP += 30;
					outtextxy(LEFT, TOP + 35 * i, data[i].data());
				}
				RECT R1 = { WINDOWS_X - BBLACK,WINDOWS_Y - BBLACK / 2,WINDOWS_X - 10,WINDOWS_Y - 10 };
				rectangle(WINDOWS_X - BBLACK, WINDOWS_Y - BBLACK / 2, WINDOWS_X - 10, WINDOWS_Y - 10);
				drawtext("返回", &R1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				break;
			}
			else if (m.x>WINDOWS_X / 2 - BBLACK && m.x<WINDOWS_X / 2 + BBLACK && m.y>WINDOWS_Y / 3 + BBLACK * 3 + 6 * bblack && m.y < (int)(WINDOWS_Y / 3 + BBLACK * 3.5 + 6 * bblack) && _HOME && !_INTRODUCTION && !_OPERATION && !_ABOUT && !_TOINTERNET && !_DRMS && !_SELECTDIFF)	//操作说明
			{
				mciSendString("play Music/按键.wav", NULL, 0, NULL);
				_OPERATION = true;
				cleardevice();
				loadimage(NULL, "Picture/0.png");				//背景
				string data[16] = {
					"操作说明：" ,
					"黑白棋的棋盘是一-个有8*8方格的棋盘。",
					"下棋时将棋下在空格中间，而不是像围棋一样下在交叉点上。",
					"开始时在棋盘正中有两白两黑四个棋子交叉放置,黑棋总是先下子。",
					"把自己颜色的棋子放在棋盘的空格上，而当自己放下的棋子",
					"在横、竖、斜八个方向内有一-个自己的棋子,则被夹在中间的全部翻转会成为",
					"自己的棋子。",

				};

				rectangle(BBLACK, BBLACK, WINDOWS_X - BBLACK, WINDOWS_Y - BBLACK);
				settextstyle(BBLACK / 2, 0, "方正姚体");
				settextcolor(RGB(0, 255, 255));
				outtextxy(WINDOWS_X / 8 - 50, 90, data[0].data());
				settextstyle(BBLACK / 3, 0, "方正姚体");
				settextcolor(WHITE);
				int LEFT, TOP = 115;
				for (int i = 1; i < 16; i++)
				{
					LEFT = WINDOWS_X / 8;
					//if (i == 1)LEFT -= 50;
					//if (i == 6 || i == 10)LEFT -= 25;
					//if (i == 12)TOP += 30;
					outtextxy(LEFT, TOP + 35 * i, data[i].data());
				}
				RECT R3 = { WINDOWS_X - BBLACK,WINDOWS_Y - BBLACK / 2,WINDOWS_X - 10,WINDOWS_Y - 10 };
				rectangle(WINDOWS_X - BBLACK, WINDOWS_Y - BBLACK / 2, WINDOWS_X - 10, WINDOWS_Y - 10);
				drawtext("返回", &R3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				break;
			}
			else if (m.x > WINDOWS_X / 2 - BBLACK && m.x<WINDOWS_X / 2 + BBLACK && m.y>(int)(WINDOWS_Y / 3 + BBLACK * 3.5 + 7 * bblack) && m.y < (int)(WINDOWS_Y / 3 + BBLACK * 4 + 7 * bblack) && _HOME && !_INTRODUCTION && !_OPERATION && !_ABOUT && !_TOINTERNET && !_DRMS && !_SELECTDIFF)//关于
			{
				mciSendString("play Music/按键.wav", NULL, 0, NULL);
				_ABOUT = true;
				cleardevice();
				loadimage(NULL, "Picture/0.png");				//背景
				string data[16] = {
					"关于:" ,
					"本游戏基于EasyX图形化界面库完成。"	// 可添加文本
				};

				rectangle(BBLACK, BBLACK, WINDOWS_X - BBLACK, WINDOWS_Y - BBLACK);
				settextstyle(BBLACK / 2, 0, "方正姚体");
				settextcolor(RGB(0, 255, 255));
				outtextxy(WINDOWS_X / 3 - 100, 90, data[0].data());
				settextstyle(BBLACK / 3, 0, "隶书");
				settextcolor(WHITE);
				int LEFT, TOP = 115;
				for (int i = 1; i < 12; i++)
				{
					LEFT = WINDOWS_X / 4;
					//if (i == 1 || i == 9)LEFT -= 50;
					//else if (i == 2 || i == 8)LEFT -= 25;
					//if (i == 9)TOP += 30;
					outtextxy(LEFT, TOP + 35 * i, data[i].data());
				}
				RECT R3 = { WINDOWS_X - BBLACK,WINDOWS_Y - BBLACK / 2,WINDOWS_X - 10,WINDOWS_Y - 10 };
				rectangle(WINDOWS_X - BBLACK, WINDOWS_Y - BBLACK / 2, WINDOWS_X - 10, WINDOWS_Y - 10);
				drawtext("返回", &R3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				settextstyle(BBLACK / 4, 0, "微软雅黑");
				settextstyle(BBLACK / 4, 0, "方正姚体");
				break;
			}
			else if (m.x>WINDOWS_X - BBLACK && m.x<WINDOWS_X - 10 && m.y>WINDOWS_Y - BBLACK / 2 && m.y<WINDOWS_Y - 10 && (_INTRODUCTION || _OPERATION || _ABOUT || _TOINTERNET || _DRMS))					//返回
			{
				cleardevice();
				_HOME = false, _INTRODUCTION = false, _OPERATION = false, _ABOUT = false, _TOINTERNET = false, _DRMS = false;
				gameStart();
			}
			else if (m.x>WINDOWS_X / 2 - BBLACK && m.x<WINDOWS_X / 2 + BBLACK && m.y>(int)(WINDOWS_Y / 3 + BBLACK * 4 + 8 * bblack) && m.y<WINDOWS_Y / 3 + BBLACK * 4.5 + 8 * bblack && _HOME && !_INTRODUCTION && !_OPERATION && !_ABOUT && !_TOINTERNET && !_DRMS && !_SELECTDIFF)//退出游戏
			{
				exit(0);
			}
			else break;
		case WM_MOUSEMOVE:									//移动鼠标
			RECT r;
			if (_INTRODUCTION || _OPERATION || _ABOUT || _TOINTERNET || _DRMS)				//如果当前处于游戏介绍 操作说明 或者关于界面 或者联机对战界面 或者单人模式界面
			{
				if (ESCEXIT)gameStart();							//部分界面按ESC退出
				if (m.x>WINDOWS_X - BBLACK && m.x<WINDOWS_X - 10 && m.y>WINDOWS_Y - BBLACK / 2 && m.y<WINDOWS_Y - 10)
				{
					r.left = WINDOWS_X - BBLACK;
					r.top = WINDOWS_Y - BBLACK / 2;
					r.right = WINDOWS_X - 10;
					r.bottom = WINDOWS_Y - 10;
					POINT points[8] = { { r.left,r.top },{ r.right,r.top },{ r.right,r.bottom },{ r.left,r.bottom } };
					setfillcolor(RED);
					fillpolygon(points, 4);
					setbkmode(TRANSPARENT);
					drawtext("返回", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				}
				else
				{
					if (getpixel(WINDOWS_X - BBLACK + 1, WINDOWS_Y - BBLACK / 2 + 1) == RED)
					{
						putimage(WINDOWS_X - BBLACK + 1, WINDOWS_Y - BBLACK / 2 + 1, MM + 8);
						setbkmode(TRANSPARENT);
						drawtext("返回", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					}
				}
				if (_DRMS && !_SELECTDIFF)
				{
					for (int i = 0; i < 3; i++)
					{
						if (m.x > WINDOWS_X / 2 - BBLACK && m.x<WINDOWS_X / 2 + BBLACK && m.y>WINDOWS_Y / 3 + BBLACK*i + 2 * i * bblack && m.y < WINDOWS_Y / 3 + BBLACK*i + 2 * i * bblack + BBLACK / 2)
						{
							r.left = WINDOWS_X / 2 - BBLACK;
							r.top = WINDOWS_Y / 3 + BBLACK*i + 2 * i * bblack;
							r.right = WINDOWS_X / 2 + BBLACK;
							r.bottom = WINDOWS_Y / 3 + BBLACK*i + 2 * i * bblack + BBLACK / 2;
							POINT points[8] = { { r.left,r.top },{ r.right,r.top },{ r.right,r.bottom },{ r.left,r.bottom } };
							setfillcolor(RED);
							fillpolygon(points, 4);
							setbkmode(TRANSPARENT);
							switch (i)
							{
							case 0:
								drawtext("简    单", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
								break;
							case 1:
								drawtext("中    等", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
								break;
							case 2:
								drawtext("困    难", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
								break;
							}
						}
						else
						{
							if (getpixel(WINDOWS_X / 2 - BBLACK + 1, WINDOWS_Y / 3 + BBLACK*i + 2 * i * bblack + 1) == RED)
							{
								putimage(WINDOWS_X / 2 - BBLACK, WINDOWS_Y / 3 + BBLACK*i + 2 * i * bblack, MB + i);	//输出原来图片
							}
						}
					}
				}
				if (_SELECTDIFF)
				{
					for (int i = 0; i < 2; i++)
					{
						if (m.x > WINDOWS_X / 2 - 2 * BBLACK && m.x<WINDOWS_X / 2 + BBLACK && m.y>WINDOWS_Y / 3 + BBLACK*i + 2 * i * bblack && m.y < WINDOWS_Y / 3 + BBLACK*i + 2 * i * bblack + BBLACK / 2)
						{
							r.left = WINDOWS_X / 2 - 2 * BBLACK;
							r.top = WINDOWS_Y / 3 + BBLACK*i + 2 * i * bblack;
							r.right = WINDOWS_X / 2;
							r.bottom = WINDOWS_Y / 3 + BBLACK*i + 2 * i * bblack + BBLACK / 2;
							POINT points[8] = { { r.left,r.top },{ r.right,r.top },{ r.right,r.bottom },{ r.left,r.bottom } };
							setfillcolor(RED);
							fillpolygon(points, 4);
							setbkmode(TRANSPARENT);
							switch (i)
							{
							case 0:
								drawtext("先    手", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
								break;
							case 1:
								drawtext("后    手", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
								break;

							}
						}
						else
						{
							if (getpixel(WINDOWS_X / 2 - 2 * BBLACK + 1, WINDOWS_Y / 3 + BBLACK*i + 2 * i * bblack + 1) == RED)
							{
								putimage(WINDOWS_X / 2 - 2 * BBLACK, WINDOWS_Y / 3 + BBLACK*i + 2 * i * bblack, MC + i);	//输出原来图片

							}
						}
					}
				}
			}
			else
			{
				for (int i = 0; i <= 8; i++)
				{
					if (m.x>WINDOWS_X / 2 - BBLACK && m.x<WINDOWS_X / 2 + BBLACK && m.y>WINDOWS_Y / 3 + i*bblack + i*BBLACK / 2 && m.y<WINDOWS_Y / 3 + i*bblack + i*BBLACK / 2 + BBLACK / 2)
					{
						r.left = WINDOWS_X / 2 - BBLACK;
						r.top = WINDOWS_Y / 3 + i*bblack + i*BBLACK / 2;
						r.right = WINDOWS_X / 2 + BBLACK;
						r.bottom = WINDOWS_Y / 3 + i*bblack + i*BBLACK / 2 + BBLACK / 2;
						POINT points[8] = { { r.left,r.top },{ r.right,r.top },{ r.right,r.bottom },{ r.left,r.bottom } };
						setfillcolor(RED);
						fillpolygon(points, 4);
						setbkmode(TRANSPARENT);
						switch (i)
						{
						case 0:
							drawtext("单人模式", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 1:
							drawtext("双人模式", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 2:
							drawtext("载入存档", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 3:
							drawtext("联机对战", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 4:
							drawtext("观战模式", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 5:
							drawtext("游戏介绍", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 6:
							drawtext("操作说明", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 7:
							drawtext("关    于", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 8:
							drawtext("退出游戏", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						}
					}
					else
					{
						if (getpixel(WINDOWS_X / 2 - BBLACK + 1, WINDOWS_Y / 3 + i*bblack + i*BBLACK / 2 + 1) == RED)
						{
							putimage(WINDOWS_X / 2 - BBLACK, WINDOWS_Y / 3 + i*bblack + i*BBLACK / 2, MM + i);	//输出原来图片
						}
					}
				}
			}
			FlushBatchDraw();
			break;
		default: break;
		}
	}
	gameStart();
}

int main()
{
	gameStart();					//主界面
	closegraph();					//关闭图形化界面
	return 0;
}
