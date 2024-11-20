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

#define WINDOWS_X 1200	//���ڴ�СX
#define WINDOWS_Y 650	//���ڴ�СY

#define PX 280			//����ƫ����X
#define PY 80			//����ƫ����Y
#define BBLACK 70		//�ո��С

#define CHESSSIZE 20	//���Ӵ�С
#define SIZE 8			//���̸���

#define ESC 27
#define ESCEXIT (_kbhit()&&_getch()==ESC)

#pragma comment (lib,"ws2_32.lib")						// ���� Windows Multimedia API
#pragma comment(lib,"Winmm.lib")

const int black = 1;	//����
const int white = 0;	//����
int step = 0;  //�غ���
int diff = 1; //�Ѷ�
int whitecount = 2;  //�ڰ���÷�
int blackcount = 2;
bool single = true; //����ģʽ
const int timeout_turn = 3000;		//ÿ�����˼��ʱ��
clock_t start;					//AI��ʼ˼����ʱ���
bool stopThink = false;			//AI�Ƿ�ֹͣ˼��
const int MAX_DEPTH = 20, MIN_DEPTH = 4;

int mapp[SIZE][SIZE] = {};		//���̾���洢
int historymap[100][SIZE][SIZE] = {};
const int MOVE[8][2] = { { -1, 0 },{ 1, 0 },{ 0, -1 },{ 0, 1 },{ -1, -1 },{ 1, -1 },{ 1, 1 },{ -1, 1 } };	//��λ
const int MAPPOINTCOUNT[8][8] =												//���̸���Ȩֵ��ֵ
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

int MYCOLOR = 1;							//�ҵ���ɫ 1��ɫ 0��ɫ -1����
int NOWCOLOR = 1;							//��ǰִ����ɫ
bool TIANEYES = false;							//����ģʽ

class POINT2
{
public:
	void WIN2MAP(POINT2 &MAP)			//��������������ӳ���ϵ
	{
		MAP.x = (x - PX) / BBLACK;
		MAP.y = (y - PY) / BBLACK;
	}
	void MAP2WIN(POINT2 &WIN)			//�������������̵�ӳ���ϵ
	{
		WIN.x = PX + x*BBLACK + BBLACK / 2;
		WIN.y = PY + y*BBLACK + BBLACK / 2;
	}
	void INIT(int x, int y)				//����Ԫ��
	{
		this->x = x;
		this->y = y;
	}
	void ADD(int x, int y)				//�����
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

POINT2 LASTCH;								//��һ�����ӷ�λ

void gameStart();						//��������
void STARTVS(int YOURCOLOR, POINT2 P1(), POINT2 P2());

void mappadd(int x, int y, int color, int MAP[SIZE][SIZE])		//���ͼ���������
{
	POINT2 WINDOWS2, MAP2;
	WINDOWS2.INIT(x, y);
	WINDOWS2.WIN2MAP(MAP2);
	MAP[MAP2.x][MAP2.y] = color ? 1 : -1;
}

void printcircle(int x, int y, int color, int MAP[SIZE][SIZE])				//��������
{
	mappadd(x, y, color, MAP);
	setfillcolor(color ? BLACK : WHITE);					//���м�Ŵ�任
	for (int i = 0; i <= CHESSSIZE; ++i)
	{
		solidcircle(x, y, i);
		Sleep(1);
	}
}

void init()															//��������
{
	memset(mapp, 0, sizeof(mapp));									//��ʼ��
	memset(historymap, 0, sizeof(historymap));
	memset(expect, 0, sizeof(expect));
	TIANEYES = false;												//�ر�����ģʽ
	LASTCH.INIT(0, 0);
	step = 1;
	whitecount = 2;
	blackcount = 2;
	NOWCOLOR = 1;

	settextstyle(15, 0, "����");
	loadimage(NULL, "Picture/1.png");										//����ͼƬ
	for (int x = PX; x < PX + BBLACK*(SIZE + 1); x += BBLACK)		//�������� ����
	{
		if ((x / BBLACK % 2) == 0)setlinecolor(BLACK);
		else setlinecolor(RED);
		line(x, PY, x, PY + BBLACK*SIZE);
		line(x-1, PY, x-1, PY + BBLACK*SIZE);

	}
	for (int y = PY; y <PY + BBLACK*(SIZE + 1); y += BBLACK)		//�������� ����
	{
		if (y / BBLACK % 2 == 0)setlinecolor(BLACK);
		else setlinecolor(RED);
		line(PX, y, PX + BBLACK*SIZE, y);
		line(PX, y-1, PX + BBLACK*SIZE, y-1);
	}
	for (int i = PX; i <= PX + BBLACK*SIZE; i += BBLACK)			//��ȡÿ����ͼƬ������ͼƬָ����
	{
		for (int j = PY; j <= PY + BBLACK*SIZE; j += BBLACK)
		{
			POINT2 DATA;
			DATA.INIT(i, j);
			DATA.WIN2MAP(DATA);
			getimage(&MAPIMAGE[DATA.x][DATA.y], i, j, BBLACK, BBLACK);
		}
	}

	printcircle(PX + (SIZE / 2 - 1)* BBLACK + BBLACK / 2, PY + (SIZE / 2 - 1) * BBLACK + BBLACK / 2, white, mapp);		//��ʼ����ö����
	printcircle(PX + (SIZE / 2 - 1) * BBLACK + BBLACK / 2, PY + (SIZE / 2) * BBLACK + BBLACK / 2, black, mapp);
	printcircle(PX + (SIZE / 2) * BBLACK + BBLACK / 2, PY + (SIZE / 2) * BBLACK + BBLACK / 2, white, mapp);
	printcircle(PX + (SIZE / 2) * BBLACK + BBLACK / 2, PY + (SIZE / 2 - 1) * BBLACK + BBLACK / 2, black, mapp);


	getimage(COUNT, WINDOWS_X / 30, WINDOWS_Y - WINDOWS_Y / 6, 230, 40);											//�ɼ��������򱳾�ͼƬָ��
	getimage(COUNT + 1, WINDOWS_X - WINDOWS_X / 5, WINDOWS_Y - WINDOWS_Y / 6, 230, 40);
	getimage(COUNT + 2, BBLACK / 4 + BBLACK * 2, BBLACK / 4 + 2 * BBLACK, 100, 40);   //�غ���

	LOGFONT f;
	gettextstyle(&f);												// ��ȡ������ʽ
	f.lfHeight = 35;												// ��������߶�
	strcpy_s(f.lfFaceName, _T("����Ҧ��"));							// ��������
	f.lfQuality = ANTIALIASED_QUALITY;								// �������Ч��Ϊ�����
	settextstyle(&f);												// ����������ʽ

	settextcolor(BLACK);
	outtextxy(BBLACK / 4, BBLACK / 2, "����ִ��");
	outtextxy(BBLACK / 4, BBLACK / 4 + BBLACK, "��ǰִ��");
	outtextxy(BBLACK / 4, BBLACK / 4 + 2 * BBLACK, "��ǰ�غ�");

	settextstyle(25, 0, "����");
	rectangle((WINDOWS_X - WINDOWS_X / 4 - 10), (0.5*BBLACK - 10), (WINDOWS_X - WINDOWS_X / 4 + 1.5*BBLACK), BBLACK);
	outtextxy(WINDOWS_X - WINDOWS_X / 4, 0.5*BBLACK, "������Ϸ");          //���棬��ȡ������
	rectangle((WINDOWS_X - WINDOWS_X / 4 + 2 * BBLACK), (0.5 * BBLACK - 10), (WINDOWS_X - WINDOWS_X / 4 + 3.5 * BBLACK + 10), BBLACK);
	outtextxy(WINDOWS_X - WINDOWS_X / 4 + 2 * BBLACK + 10, 0.5*BBLACK, "��ȡ��Ϸ");          //���棬��ȡ������

	settextstyle(30, 0, "����Ҧ��");
	rectangle(WINDOWS_X - WINDOWS_X / 4 - 10,  1.5*BBLACK, WINDOWS_X - WINDOWS_X / 4 + BBLACK, 2 * BBLACK);
	outtextxy(WINDOWS_X - WINDOWS_X / 4,  1.5 * BBLACK, "����");          //����

}

void printmapp()         //���������ϵ�����
{
	int i, j;
	POINT2 WINDOWS2, MAP2;
	for (i = 0; i < SIZE; ++i)                                     //���������ϵ�����
		for (j = 0; j < SIZE; ++j) {
			if (mapp[i][j] == 1 || mapp[i][j] == -1) {
				MAP2.INIT(i, j);
				MAP2.MAP2WIN(WINDOWS2);
				setfillcolor(mapp[i][j] == 1 ? BLACK : WHITE);					//���м�Ŵ�任
				for (int i = 0; i <= CHESSSIZE; ++i)
				{
					solidcircle(WINDOWS2.x, WINDOWS2.y, i);
				}
			}
		}
}


int Judge(int x, int y, int color, int MAP[SIZE][SIZE])									//Ԥ�е�ǰλ���ܷ�����
{
	if (MAP[x][y])return 0;																//�����ǰλ���Ѿ�������
	int me = color ? 1 : -1;															//׼������������ɫ
	POINT2 star;
	int count = 0, flag;																//countΪ��λ�ÿ���ת���������Ӹ���
	for (int i = 0; i < SIZE; ++i)														//����
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
	return count;																		//���ظõ�ת���Է����Ӹ���
}

void Change(POINT2 NOW, int MAP[SIZE][SIZE], bool F)												//���Ӻ�ı�����״̬ FΪ�Ƿ��������Ļ
{
	int me = MAP[NOW.x][NOW.y];																	//��ǰ����������ɫ
	bool flag;
	POINT2 a, b;
	for (int i = 0; i<SIZE; ++i)																//����
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
						if (F)printcircle(b.MAP2WINX(), b.MAP2WINY(), (me == 1) ? black : white, MAP);	//�ı�����
						if (!F)mappadd(b.MAP2WINX(), b.MAP2WINY(), (me == 1) ? black : white, MAP);		//������������Ļ���ı��ͼ����
						b.ADD(MOVE[i][0], MOVE[i][1]);
					}
				}
				break;
			}
			a.ADD(MOVE[i][0], MOVE[i][1]);
		}
	}
}

int Statistics(int color)														//Ԥ��ÿ��λ�ÿ���ת���������Ӹ���
{
	int NOWEXPECT = 0;															//�ܵ�ת�����Ӹ���
	for (int i = 0; i < SIZE; ++i)												//����
		for (int j = 0; j < SIZE; ++j)
		{
			expect[i][j] = Judge(i, j, color, mapp);									//�洢��λ�ÿ���ת�����Ӹ���
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

				if (TIANEYES)														//�����������ģʽ
				{
					settextstyle(15, 0, "����");
					TCHAR s[20];													//����ģʽ
					_stprintf_s(s, _T("%d"), expect[i][j]);
					outtextxy(a.MAP2WINX(), a.MAP2WINY() + 10, s);
				}
			}
		}
	return NOWEXPECT;
}

void CleanStatistics()										//�����������ʾ
{
	for (int i = 0; i < SIZE; ++i)
	{
		for (int j = 0; j < SIZE; ++j)
		{
			if (expect[i][j] && !mapp[i][j])				//�����ǰ��û�����ӻ���������
			{
				POINT2 a;
				a.INIT(i, j);								//��¼����
				putimage(a.MAP2WINX() - BBLACK / 2, a.MAP2WINY() - BBLACK / 2, &MAPIMAGE[i][j]);	//����ֲ�����
			}
		}
	}
}

string INTTOCHI(int num, int color)											//��ǰ���Ƴɼ��������
{
	string number[10] = { "","һ","��","��","��","��","��","��","��","��" };
	string data = "";
	if (num >= 10)
	{
		data += number[num / 10 % 10];
		data += "ʮ";
	}
	data += number[num % 10];
	//return (color ? "���壺" : "���壺") + data;							//num>=0&&num<=64
	return (color ? "���壺" : "���壺") + std::to_string(num);
}

void Printcount(int balckcount, int whitecount, int nowcolor)		//�����ǰ����
{
	settextcolor(BLACK);											//����������ɫ
	settextstyle(35, 0, "����Ҧ��");

	putimage(WINDOWS_X / 30, WINDOWS_Y - WINDOWS_Y / 6, COUNT);		//����ԭ���ۼ�
	putimage(WINDOWS_X - WINDOWS_X / 5, WINDOWS_Y - WINDOWS_Y / 6, COUNT + 1);
	putimage(BBLACK / 4 + BBLACK * 2, BBLACK / 4 + 2 * BBLACK, COUNT + 2);

	outtextxy(WINDOWS_X / 30, WINDOWS_Y - WINDOWS_Y / 6, INTTOCHI(whitecount, white).data());	//�����ǰ�ɼ�
	outtextxy(WINDOWS_X - WINDOWS_X / 5, WINDOWS_Y - WINDOWS_Y / 6, INTTOCHI(balckcount, black).data());

	char s[5] = {};
	_itoa_s(step, s, 3, 10);
	settextstyle(55, 0, "Kristen ITC");
	outtextxy(BBLACK / 4 + BBLACK * 2, BBLACK / 4 + 2 * BBLACK - 10, s);

	setfillcolor(MYCOLOR == 1 ? BLACK : MYCOLOR == 0 ? WHITE : LIGHTCYAN);						//���м�Ŵ�任
	solidcircle(BBLACK * 2 + 20, BBLACK * 3 / 4, CHESSSIZE * 3 / 4);
	setfillcolor((!nowcolor || balckcount + whitecount == 4) ? BLACK : WHITE);
	NOWCOLOR = (!nowcolor || balckcount + whitecount == 4) ? black : white;						//��¼��ǰִ��
	for (int i = 0; i <= CHESSSIZE * 3 / 4; ++i)
	{
		solidcircle(BBLACK * 2 + 20, BBLACK * 3 / 2, i);
		Sleep(1);
	}
}

void WIN(int YOURCOLOR, int balckcount, int whitecount)			//�ж���Ӯ
{
	HWND wnd = GetHWnd();										//��ȡ���ھ��
	if (balckcount>whitecount)
	{
		if (YOURCOLOR == black) mciSendString("play Music/ʤ��Ц��.wav", NULL, 0, NULL);
		else  mciSendString("play Music/ʧ��.wav", NULL, 0, NULL);
		MessageBox(wnd, YOURCOLOR == black ? "��ϲ�㣬ʤ����" : YOURCOLOR == white ? "ʧ�ܣ��ٽ�������" : "�ڷ���ʤ~", "Result", MB_OK);
	}
	else if (balckcount<whitecount)
	{
		if (YOURCOLOR == white) mciSendString("play Music/ʤ��Ц��.wav", NULL, 0, NULL);
		else  mciSendString("play Music/ʧ��.wav", NULL, 0, NULL);
		MessageBox(wnd, YOURCOLOR == white ? "��ϲ�㣬ʤ����" : YOURCOLOR == black ? "ʧ�ܣ��ٽ�������" : "�׷���ʤ", "Result", MB_OK);
	}
	else
	{
		mciSendString("play Music/ƽ��.wav", NULL, 0, NULL);
		MessageBox(wnd, "ƽ�֣�", "Result", MB_OK);
	}
}

void HL(int NOWWJ)
{
	if (NOWWJ != -1)
	{
		HWND wnd = GetHWnd();										//��ȡ���ھ��
		MessageBox(wnd, NOWWJ == MYCOLOR ? "��û�п����µ��ӣ�" : "�Է������ӿ��£�", "�غ�����", MB_OK);
	}
}

POINT2 Easy()										//�˻���ս��AI
{
	POINT2 MAX;										//�����Լ���ʼ�����Ž�
	MAX.INIT(0, 0);
	int maxx = 0;
	for (int i = 0; i < SIZE; ++i)
		for (int j = 0; j < SIZE; ++j)
		{
			if (expect[i][j] >= maxx)				//Ѱ�ҿ���ת���������ĵ���Ϊ���Ž�
			{
				maxx = expect[i][j];
				MAX.INIT(i, j);
			}
		}
	if (ESCEXIT)gameStart();
	Sleep(200);										//��Ъ
	return MAX;										//�������Ž�
}

void copymap(int one[SIZE][SIZE], int last[SIZE][SIZE])						//������ͼ
{
	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
			one[i][j] = last[i][j];
}


void save_game() {
	ofstream outfile("document.dat", ios::binary);//�Զ�������ʽ����
	if (!outfile) {
		cerr << "����ʧ�ܣ�" << endl;
		HWND wnd = GetHWnd();										//��ȡ���ھ��
		MessageBox(wnd, "����ʧ�ܣ�", "������Ϸ", MB_OK);
		abort();
	}
	outfile.write((char*)&mapp[0], sizeof(mapp));//��������
	outfile.write((char*)&step, sizeof(step));//����
	outfile.write((char*)&MYCOLOR, sizeof(MYCOLOR));//���������ɫ
	outfile.write((char*)&NOWCOLOR, sizeof(NOWCOLOR));//��ǰ������ɫ
	outfile.write((char*)&whitecount, sizeof(whitecount));//����÷�
	outfile.write((char*)&blackcount, sizeof(blackcount));//����÷�
	outfile.write((char*)&diff, sizeof(diff));//�Ѷ�
	outfile.write((char*)&single, sizeof(single));//����ģʽ
	outfile.write((char*)&historymap[0][0], sizeof(historymap));//��ʷ����
	outfile.close();

	HWND wnd = GetHWnd();										//��ȡ���ھ��
	MessageBox(wnd, "����ɹ�", "������Ϸ", MB_OK);
}

void load_game() {
	ifstream infile("document.dat", ios::binary);//���ĵ���2������ʽ����
	if (!infile) {
		cerr << "��ȡʧ�ܣ�" << endl;
		HWND wnd = GetHWnd();										//��ȡ���ھ��
		MessageBox(wnd, "��ȡʧ�ܣ�", "��ȡ��Ϸ", MB_OK);
		abort();
	}
	init();
	infile.read((char*)&mapp[0], sizeof(mapp));//��ȡ����
	infile.read((char*)&step, sizeof(step));//����
	infile.read((char*)&MYCOLOR, sizeof(MYCOLOR));//���������ɫ
	infile.read((char*)&NOWCOLOR, sizeof(NOWCOLOR));//���������ɫ
	infile.read((char*)&whitecount, sizeof(whitecount));//����������ɫ
	infile.read((char*)&blackcount, sizeof(blackcount));//����÷�
	infile.read((char*)&diff, sizeof(diff));//�Ѷ�
	infile.read((char*)&single, sizeof(single));//����ģʽ
	infile.read((char*)&historymap[0][0], sizeof(historymap));//��ʷ����
	infile.close();

	printmapp();
	Printcount(blackcount, whitecount, MYCOLOR);					//��ǰ����
	Statistics(MYCOLOR);
	HWND wnd = GetHWnd();										//��ȡ���ھ��
	MessageBox(wnd, "������Ϸ�ɹ�", "ȷ��", MB_OK);
}

void hui_qi() {
	if (step < 2) {
		HWND wnd = GetHWnd();										//��ȡ���ھ��
		MessageBox(wnd, "�޷�����", "����", MB_OK);
		return;
	}
	bool flag = false;//�����ж���һ���ܷ���������
	for (step -= 1; step > 0; step--) {//����һ�ؿ�ʼ����
		for (int i = 1; i <= 8; i++) {
			for (int j = 1; j <= 8; j++)
				if (historymap[step][i][j] != 0) {//����ʷ������ĳһλ�ò�Ϊ0����˵��������̿��ã���Ϊ��������ʱ�Ѿ�����������ж�
					flag = true;//������̿���
					break;
				}
			if (flag)
				break;
		}
		if (flag) {//�����������
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
	Printcount(blackcount, whitecount,NOWCOLOR);					//��ǰ����
	Statistics(MYCOLOR);

	HWND wnd = GetHWnd();										//��ȡ���ھ��
	MessageBox(wnd, "����ɹ�", "����", MB_OK);
}

POINT2 MOUSE()										//����¼�
{
	MOUSEMSG m;
	while (true)
	{
		m = GetMouseMsg();							//��ȡ�����Ϣ
		switch (m.uMsg)
		{
		case(WM_LBUTTONDOWN):						//������������ʱ
		{
			POINT2 NOWMOUSE;
			NOWMOUSE.INIT(m.x, m.y);
			return NOWMOUSE;						//�����������
			break;
		}
		case(WM_MOUSEMOVE):								//���� ������λ��
		{
			if (ESCEXIT)gameStart();
			break;
		}
		case(WM_RBUTTONDOWN):								//�������Ҽ�����ʱ
		{
			TIANEYES = !TIANEYES;							//����OR�ر�����ģʽ
			break;
		}
		}
	}
}

bool putmouse(POINT2 &m)									//�ض����������
{
	bool flag = true;
	int mouseinx[SIZE + 1], mouseiny[SIZE + 1];
	for (int i = 0; i < SIZE + 1; ++i)						//��ȷ������
	{
		mouseinx[i] = PX + i*BBLACK;
		mouseiny[i] = PY + i*BBLACK;
	}
	if (m.x > WINDOWS_X - WINDOWS_X / 4 - 10 && m.x< WINDOWS_X - WINDOWS_X / 4 + 1.5*BBLACK && m.y > 0.5*BBLACK - 10 && m.y < BBLACK)  //������Ϸ
	{
		mciSendString("play Music/���.wav", NULL, 0, NULL);
		save_game();
	}
	else if (m.x > WINDOWS_X - WINDOWS_X / 4 + 2 * BBLACK && m.x< WINDOWS_X - WINDOWS_X / 4 + 3.5 * BBLACK + 10 && m.y > 0.5*BBLACK - 10 && m.y < BBLACK)  //��ȡ�浵
	{
		mciSendString("play Music/���.wav", NULL, 0, NULL);
		load_game();
		if (!single) STARTVS(MYCOLOR, MOUSE, MOUSE);
		else {
			if (diff == 1) STARTVS(MYCOLOR, MOUSE, Easy);

		}
	}
	else if (m.x > WINDOWS_X - WINDOWS_X / 4 - 10 && m.x< WINDOWS_X - WINDOWS_X / 4 + BBLACK && m.y > 1.5*BBLACK && m.y < 2 * BBLACK)  //����
	{
		mciSendString("play Music/����.wav", NULL, 0, NULL);
		if (single) hui_qi();
		else {
			HWND wnd = GetHWnd();										//��ȡ���ھ��
			MessageBox(wnd, "˫����Ϸ���������", "����", MB_OK);
		}
	}
	if (m.x < PX || m.x>PX + SIZE*BBLACK || m.y < PY || m.y>PY + SIZE*BBLACK)flag = false;	//��������������
	else
	{
		mciSendString("play Music/����.wav", NULL, 0, NULL);
		for (int i = 0; i<SIZE; ++i)
		{
			if (m.x >= mouseinx[i] && m.x <= mouseinx[i + 1])
			{
				if (m.x - mouseinx[i]>BBLACK / 8 && mouseinx[i + 1] - m.x>BBLACK / 8)		//�ض���X
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
				if (m.y - mouseiny[i]>BBLACK / 8 && mouseiny[i + 1] - m.y > BBLACK / 8)		//�ض���Y
				{
					m.y = (mouseiny[i] + mouseiny[i + 1]) / 2;
				}
				else flag = false;
			}
		}
	}
	return flag;											//���ص�ǰλ���ܷ�����
}

void CleanLast(POINT2 WINDOWS2, int F)						//��¼��һ������λ��
{
	if (LASTCH.x > SIZE&&LASTCH.y > SIZE)					//����ȡ����һ�����
	{
		setfillcolor(getpixel(LASTCH.x, LASTCH.y));			//��ȡԭ��������ɫ
		putimage(LASTCH.x - BBLACK / 2, LASTCH.y - BBLACK / 2, &MAPIMAGE[LASTCH.WIN2MAPX()][LASTCH.WIN2MAPY()]);
		solidcircle(LASTCH.x, LASTCH.y, CHESSSIZE);
	}

	setfillcolor(RGB(49, 153, 182));						//����Ϊ��䵱ǰ����
	LASTCH.INIT(WINDOWS2.x, WINDOWS2.y);

	solidrectangle(WINDOWS2.x - BBLACK / 2 + 2, WINDOWS2.y - BBLACK / 2 + 2, WINDOWS2.x + BBLACK / 2 - 2, WINDOWS2.y + BBLACK / 2 - 2);		//��������
	setfillcolor(F ? BLACK : WHITE);
	solidcircle(WINDOWS2.x, WINDOWS2.y, CHESSSIZE);			//������
}

int Playchess(int F, POINT2 WINDOWS2, int &balckcount, int &whitecount)	//��ʼ
{
	//F �ڷ�Ϊ1  �׷�Ϊ0
	POINT2 MAP2;

	if (WINDOWS2.x < SIZE&&WINDOWS2.y < SIZE)					//������������Ϊ��������
	{
		MAP2 = WINDOWS2;
		WINDOWS2.MAP2WIN(WINDOWS2);								//����ת����ʵ��չʾ����
	}
	else
	{
		if (!putmouse(WINDOWS2))return 0;						//������������ض���
		WINDOWS2.WIN2MAP(MAP2);									//�洢�ض���֮��ľ�������
	}
	if (expect[MAP2.x][MAP2.y])									//��λ�ÿ���
	{
		CleanStatistics();										//�����Ļ��ʾ
		if (F)													//�ж����Ϊ����÷�
		{
			balckcount += expect[MAP2.x][MAP2.y] + 1;
			whitecount -= expect[MAP2.x][MAP2.y];
		}
		else
		{
			whitecount += expect[MAP2.x][MAP2.y] + 1;
			balckcount -= expect[MAP2.x][MAP2.y];
		}

		printcircle(WINDOWS2.x, WINDOWS2.y, F, mapp);			//������ mappΪ��������
		CleanLast(WINDOWS2, F);									//��ǰ����������ʾ
		Change(MAP2, mapp, true);									//��ת���� trueΪ��ʾ����Ļ
		Printcount(balckcount, whitecount, F);	//��ӡ����

		if (balckcount + whitecount >= SIZE*SIZE || !balckcount || !whitecount)return 3;	//���ʤ���ѷ�
		if (!Statistics(!F))									//����Է��޷�����
		{
			if (Statistics(F))									//�ж��Լ��Ƿ��������
			{
				HL(!F);											//�Լ�����������Է��޷�������Ϣ
				Printcount(balckcount, whitecount, !F);			//��Ϊǰ���Ѿ��ı���״̬���������ڻ�ԭ
				return 2;
			}
			else return 3;										//˫�����޷�����
		}
		return 1;
	}
	return 0;
}

void STARTVS(int YOURCOLOR, POINT2 P1(), POINT2 P2())			//��ʼ��ս   (mycolor,mouse,easy)
{
	mciSendString("stop Music/��������.mp3", NULL, 0, NULL);
	mciSendString("play Music/���.mp3", NULL, 0, NULL);
	time_t timestart , timeend ;
	int music_no = 1;
	timestart= time(NULL);
	MYCOLOR = YOURCOLOR;
	Printcount(blackcount, whitecount, NOWCOLOR);					//��ǰ����
	Statistics(NOWCOLOR);
	if (MYCOLOR == white && step == 1) goto CX2;
	if (NOWCOLOR == white && !single) goto CX2;
	while (true)
	{
		cout << "hello" << endl;
	CX1:														//���غϺ���
		timeend = time(NULL);
		if (music_no == 1 && difftime(timeend, timestart) > 170) {
			mciSendString("stop Music/��������.mp3", NULL, 0, NULL);
			mciSendString("stop Music/���.mp3", NULL, 0, NULL);
			mciSendString("play Music/���2.mp3", NULL, 0, NULL);
			timestart = time(NULL);
			music_no = 2;
		}
		if (music_no == 2 && difftime(timeend, timestart) > 285) {
			mciSendString("stop Music/��������.mp3", NULL, 0, NULL);
			mciSendString("stop Music/���2.mp3", NULL, 0, NULL);
			mciSendString("play Music/���.mp3", NULL, 0, NULL);
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
			goto CX1;											//����ʧ���������
			break;
		case 1:
			break;												//��������
		case 2:
			if (MYCOLOR == white) step++;
			goto CX1;											//���ԶԷ�
			break;
		case 3:
			goto ED;											//��ֽ���
			break;
		}
	CX2:                                     //���غϺ���
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
ED:																	//����
	WIN(YOURCOLOR, blackcount, whitecount);
	_getch();
	gameStart();
}



void gameStart()
{
	mciSendString("stop Music/���.mp3", NULL, 0, NULL);
	mciSendString("play Music/��������.mp3", NULL, 0, NULL);
	time_t timestart, timeend;
	timestart = time(NULL);

	IMAGE MM[11] = { 0 }, MB[3] = { 0 }, MC[3] = { 0 };
	initgraph(WINDOWS_X, WINDOWS_Y);
	setbkmode(TRANSPARENT);					//͸������

	HWND hwnd = GetHWnd();					// ���ô��ڱ�������
	SetWindowText(hwnd, "��ת����");
	loadimage(NULL, "Picture/0.png", 1200, 800);

	const int bblack = 10;

	LOGFONT f;
	gettextstyle(&f);												// ��ȡ������ʽ
	f.lfHeight = BBLACK;												// ��������߶�
	strcpy_s(f.lfFaceName, _T("����Ҧ��"));								// ��������
	f.lfQuality = ANTIALIASED_QUALITY;								// �������Ч��Ϊ�����
	settextstyle(&f);												// ����������ʽ
	RECT r1 = { 0, 0, WINDOWS_X, WINDOWS_Y / 3 };
	drawtext("�� �� ��", &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	settextstyle(BBLACK / 3, 0, "����Ҧ��");
	RECT r2 = { WINDOWS_X / 2 - BBLACK,WINDOWS_Y / 3,WINDOWS_X / 2 + BBLACK ,WINDOWS_Y / 3 + BBLACK / 2 };
	rectangle(WINDOWS_X / 2 - BBLACK, WINDOWS_Y / 3, WINDOWS_X / 2 + BBLACK, WINDOWS_Y / 3 + BBLACK / 2);
	drawtext("����ģʽ", &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT r3 = { WINDOWS_X / 2 - BBLACK,WINDOWS_Y / 3 + BBLACK / 2 + bblack,WINDOWS_X / 2 + BBLACK,WINDOWS_Y / 3 + BBLACK + bblack };
	rectangle(WINDOWS_X / 2 - BBLACK, WINDOWS_Y / 3 + BBLACK / 2 + bblack, WINDOWS_X / 2 + BBLACK, WINDOWS_Y / 3 + BBLACK + bblack);
	drawtext("˫��ģʽ", &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT r4 = { WINDOWS_X / 2 - BBLACK,WINDOWS_Y / 3 + BBLACK + 2 * bblack,WINDOWS_X / 2 + BBLACK,(int)(WINDOWS_Y / 3 + BBLACK*1.5 + 2 * bblack) };
	rectangle(WINDOWS_X / 2 - BBLACK, WINDOWS_Y / 3 + BBLACK + 2 * bblack, WINDOWS_X / 2 + BBLACK, (int)(WINDOWS_Y / 3 + BBLACK*1.5 + 2 * bblack));
	drawtext("����浵", &r4, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT r5 = { WINDOWS_X / 2 - BBLACK,(int)(WINDOWS_Y / 3 + BBLACK*1.5 + 3 * bblack),WINDOWS_X / 2 + BBLACK,WINDOWS_Y / 3 + BBLACK * 2 + 3 * bblack };
	rectangle(WINDOWS_X / 2 - BBLACK, (int)(WINDOWS_Y / 3 + BBLACK*1.5 + 3 * bblack), WINDOWS_X / 2 + BBLACK, WINDOWS_Y / 3 + BBLACK * 2 + 3 * bblack);
	drawtext("������ս", &r5, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT r6 = { WINDOWS_X / 2 - BBLACK,WINDOWS_Y / 3 + BBLACK * 2 + 4 * bblack,WINDOWS_X / 2 + BBLACK,(int)(WINDOWS_Y / 3 + BBLACK * 2.5 + 4 * bblack) };
	rectangle(WINDOWS_X / 2 - BBLACK, WINDOWS_Y / 3 + BBLACK * 2 + 4 * bblack, WINDOWS_X / 2 + BBLACK, (int)(WINDOWS_Y / 3 + BBLACK * 2.5 + 4 * bblack));
	drawtext("��սģʽ", &r6, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT r7 = { WINDOWS_X / 2 - BBLACK,(int)(WINDOWS_Y / 3 + BBLACK * 2.5 + 5 * bblack),WINDOWS_X / 2 + BBLACK,WINDOWS_Y / 3 + BBLACK * 3 + 5 * bblack };
	rectangle(WINDOWS_X / 2 - BBLACK, (int)(WINDOWS_Y / 3 + BBLACK * 2.5 + 5 * bblack), WINDOWS_X / 2 + BBLACK, WINDOWS_Y / 3 + BBLACK * 3 + 5 * bblack);
	drawtext("��Ϸ����", &r7, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT r8 = { WINDOWS_X / 2 - BBLACK,WINDOWS_Y / 3 + BBLACK * 3 + 6 * bblack,WINDOWS_X / 2 + BBLACK,(int)(WINDOWS_Y / 3 + BBLACK * 3.5 + 6 * bblack) };
	rectangle(WINDOWS_X / 2 - BBLACK, WINDOWS_Y / 3 + BBLACK * 3 + 6 * bblack, WINDOWS_X / 2 + BBLACK, (int)(WINDOWS_Y / 3 + BBLACK * 3.5 + 6 * bblack));
	drawtext("����˵��", &r8, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT r9 = { WINDOWS_X / 2 - BBLACK,(int)(WINDOWS_Y / 3 + BBLACK * 3.5 + 7 * bblack),WINDOWS_X / 2 + BBLACK,WINDOWS_Y / 3 + BBLACK * 4 + 7 * bblack };
	rectangle(WINDOWS_X / 2 - BBLACK, (int)(WINDOWS_Y / 3 + BBLACK * 3.5 + 7 * bblack), WINDOWS_X / 2 + BBLACK, WINDOWS_Y / 3 + BBLACK * 4 + 7 * bblack);
	drawtext("��    ��", &r9, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT r10 = { WINDOWS_X / 2 - BBLACK,(int)(WINDOWS_Y / 3 + BBLACK * 4 + 8 * bblack),WINDOWS_X / 2 + BBLACK,(int)(WINDOWS_Y / 3 + BBLACK * 4.5 + 8 * bblack) };
	rectangle(WINDOWS_X / 2 - BBLACK, (int)(WINDOWS_Y / 3 + BBLACK * 4 + 8 * bblack), WINDOWS_X / 2 + BBLACK, (int)(WINDOWS_Y / 3 + BBLACK * 4.5 + 8 * bblack));
	drawtext("�˳���Ϸ", &r10, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	for (int i = 0; i <= 8; i++)																						//���水ťͼƬ
		getimage(MM + i, WINDOWS_X / 2 - BBLACK, WINDOWS_Y / 3 + i*bblack + i*BBLACK / 2, 2 * BBLACK, BBLACK / 2);
	getimage(MM + 9, WINDOWS_X - BBLACK, WINDOWS_Y - BBLACK / 2, BBLACK - 11, BBLACK / 2 - 11);

	bool _HOME = true, _INTRODUCTION = false, _OPERATION = false, _ABOUT = false, _TOINTERNET = false, _DRMS = false, _SELECTDIFF = false;			//TRUE��ʾ���ڵ�ǰҳ��,selectdiff��ʾ��ѡ���Ѷȣ���ѡ��������ɫ
	MOUSEMSG m;
	while (_HOME)
	{
		timeend = time(NULL);
		if (difftime(timeend, timestart) > 257) {
			mciSendString("stop Music/��������.mp3", NULL, 0, NULL);
			mciSendString("stop Music/���.mp3", NULL, 0, NULL);
			mciSendString("stop Music/���2.mp3", NULL, 0, NULL);
			mciSendString("play Music/��������.mp3", NULL, 0, NULL);
			timestart = time(NULL);
		}
		BeginBatchDraw();
		m = GetMouseMsg();
		switch (m.uMsg)
		{
		case WM_LBUTTONDOWN:												//������������ʱ
			EndBatchDraw();
			mciSendString("stop Music/���.mp3", NULL, 0, NULL);
			mciSendString("play Music/��������.mp3", NULL, 0, NULL);
			if (m.x>WINDOWS_X / 2 - BBLACK && m.x<WINDOWS_X / 2 + BBLACK && m.y>WINDOWS_Y / 3 && m.y<WINDOWS_Y / 3 + BBLACK / 2 && _HOME && !_INTRODUCTION && !_OPERATION && !_ABOUT && !_TOINTERNET && !_DRMS && !_SELECTDIFF)//����ģʽ
			{
				mciSendString("play Music/����.wav", NULL, 0, NULL);
				single = true;
				_DRMS = true;									//�뿪HOME����
				cleardevice();
				loadimage(NULL, "Picture/0.png");				//����
				rectangle(BBLACK, BBLACK, WINDOWS_X - BBLACK, WINDOWS_Y - BBLACK);

				settextstyle(BBLACK / 2, 0, "����Ҧ��");
				RECT r0 = { 0, 0, WINDOWS_X, WINDOWS_Y / 3 };
				drawtext("ѡ����Ϸ�Ѷ�", &r0, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				settextstyle(BBLACK / 3, 0, "����Ҧ��");
				RECT r1 = { WINDOWS_X / 2 - BBLACK,WINDOWS_Y / 3,WINDOWS_X / 2 + BBLACK ,WINDOWS_Y / 3 + BBLACK / 2 };
				rectangle(WINDOWS_X / 2 - BBLACK, WINDOWS_Y / 3, WINDOWS_X / 2 + BBLACK, WINDOWS_Y / 3 + BBLACK / 2);
				drawtext("��    ��", &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				RECT r2 = { WINDOWS_X / 2 - BBLACK,WINDOWS_Y / 3 + BBLACK + 2 * bblack,WINDOWS_X / 2 + BBLACK,(int)(WINDOWS_Y / 3 + BBLACK*1.5 + 2 * bblack) };
				rectangle(WINDOWS_X / 2 - BBLACK, WINDOWS_Y / 3 + BBLACK + 2 * bblack, WINDOWS_X / 2 + BBLACK, (int)(WINDOWS_Y / 3 + BBLACK*1.5 + 2 * bblack));
				drawtext("��    ��", &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				RECT r3 = { WINDOWS_X / 2 - BBLACK,WINDOWS_Y / 3 + BBLACK * 2 + 4 * bblack,WINDOWS_X / 2 + BBLACK,(int)(WINDOWS_Y / 3 + BBLACK * 2.5 + 4 * bblack) };
				rectangle(WINDOWS_X / 2 - BBLACK, WINDOWS_Y / 3 + BBLACK * 2 + 4 * bblack, WINDOWS_X / 2 + BBLACK, (int)(WINDOWS_Y / 3 + BBLACK * 2.5 + 4 * bblack));
				drawtext("��    ��", &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				for (int i = 0; i < 3; i++)
				{
					getimage(MB + i, WINDOWS_X / 2 - BBLACK, WINDOWS_Y / 3 + BBLACK*i + 2 * i * bblack, 2 * BBLACK, BBLACK / 2);
				}

				RECT R = { WINDOWS_X - BBLACK,WINDOWS_Y - BBLACK / 2,WINDOWS_X - 10,WINDOWS_Y - 10 };
				rectangle(WINDOWS_X - BBLACK, WINDOWS_Y - BBLACK / 2, WINDOWS_X - 10, WINDOWS_Y - 10);
				drawtext("����", &R, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				break;
			}
			else if (m.x>WINDOWS_X / 2 - BBLACK && m.x<WINDOWS_X / 2 + BBLACK && m.y>WINDOWS_Y / 3 && m.y<WINDOWS_Y / 3 + BBLACK / 2 && _DRMS && !_SELECTDIFF)			//��
			{
				mciSendString("play Music/����.wav", NULL, 0, NULL);
				_SELECTDIFF = true;
				diff = 1;         //���˼�ģʽ

				cleardevice();
				loadimage(NULL, "Picture/0.png");				//����
				rectangle(BBLACK, BBLACK, WINDOWS_X - BBLACK, WINDOWS_Y - BBLACK);

				settextstyle(BBLACK / 2, 0, "����Ҧ��");
				r1 = { 0, 0, WINDOWS_X, WINDOWS_Y / 3 };
				drawtext("ѡ�����ֻ��Ǻ���", &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				settextstyle(BBLACK / 3, 0, "����Ҧ��");
				r2 = { WINDOWS_X / 2 - 2 * BBLACK,WINDOWS_Y / 3,WINDOWS_X / 2   ,WINDOWS_Y / 3 + BBLACK / 2 };
				rectangle(WINDOWS_X / 2 - 2 * BBLACK, WINDOWS_Y / 3, WINDOWS_X / 2 + BBLACK, WINDOWS_Y / 3 + BBLACK / 2);
				drawtext("��   ��", &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				setfillcolor(BLACK);
				solidcircle((int)(WINDOWS_X / 2 + BBLACK*0.5), (int)(WINDOWS_Y / 3 + BBLACK / 4), int(BBLACK / 6));

				r3 = { (int)WINDOWS_X / 2 - 2 * BBLACK,(int)WINDOWS_Y / 3 + BBLACK + 2 * bblack,(int)WINDOWS_X / 2  ,(int)(WINDOWS_Y / 3 + BBLACK*1.5 + 2 * bblack) };
				rectangle((int)WINDOWS_X / 2 - 2 * BBLACK, (int)WINDOWS_Y / 3 + BBLACK + 2 * bblack, (int)WINDOWS_X / 2 + BBLACK, (int)(WINDOWS_Y / 3 + BBLACK*1.5 + 2 * bblack));
				drawtext("��   ��", &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				setfillcolor(WHITE);
				solidcircle((int)(WINDOWS_X / 2 + BBLACK *0.5), (int)(WINDOWS_Y / 3 + BBLACK*1.25 + 2 * bblack), int(BBLACK / 6));

				for (int i = 0; i < 2; i++)
				{
					getimage(MC + i, WINDOWS_X / 2 - 2 * BBLACK, WINDOWS_Y / 3 + BBLACK*i + 2 * i * bblack, 2 * BBLACK, BBLACK / 2);
				}

				RECT R = { WINDOWS_X - BBLACK,WINDOWS_Y - BBLACK / 2,WINDOWS_X - 10,WINDOWS_Y - 10 };
				rectangle(WINDOWS_X - BBLACK, WINDOWS_Y - BBLACK / 2, WINDOWS_X - 10, WINDOWS_Y - 10);
				drawtext("����", &R, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				break;
			}
			else if (m.x>WINDOWS_X / 2 - BBLACK && m.x<WINDOWS_X / 2 + BBLACK && m.y>WINDOWS_Y / 3 + BBLACK + 2 * bblack && m.y<(int)(WINDOWS_Y / 3 + BBLACK*1.5 + 2 * bblack) && _DRMS && !_SELECTDIFF)			//�е�
			{
				mciSendString("play Music/����.wav", NULL, 0, NULL);
				_SELECTDIFF = true;            //�����е�ģʽ
				diff = 2;
				cleardevice();
				loadimage(NULL, "Picture/0.png");				//����
				rectangle(BBLACK, BBLACK, WINDOWS_X - BBLACK, WINDOWS_Y - BBLACK);

				settextstyle(BBLACK / 2, 0, "����Ҧ��");
				r1 = { 0, 0, WINDOWS_X, WINDOWS_Y / 3 };
				drawtext("ѡ�����ֻ��Ǻ���", &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				settextstyle(BBLACK / 3, 0, "����Ҧ��");
				r2 = { WINDOWS_X / 2 - 2 * BBLACK,WINDOWS_Y / 3,WINDOWS_X / 2  ,WINDOWS_Y / 3 + BBLACK / 2 };
				rectangle(WINDOWS_X / 2 - 2 * BBLACK, WINDOWS_Y / 3, WINDOWS_X / 2 + BBLACK, WINDOWS_Y / 3 + BBLACK / 2);
				drawtext("��   ��", &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				setfillcolor(BLACK);
				solidcircle(WINDOWS_X / 2 + BBLACK / 2, WINDOWS_Y / 3 + BBLACK / 4, int(BBLACK / 6));

				r3 = { WINDOWS_X / 2 - 2 * BBLACK,WINDOWS_Y / 3 + BBLACK + 2 * bblack,WINDOWS_X / 2 ,(int)(WINDOWS_Y / 3 + BBLACK*1.5 + 2 * bblack) };
				rectangle(WINDOWS_X / 2 - 2 * BBLACK, WINDOWS_Y / 3 + BBLACK + 2 * bblack, WINDOWS_X / 2 + BBLACK, (int)(WINDOWS_Y / 3 + BBLACK*1.5 + 2 * bblack));
				drawtext("��   ��", &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				setfillcolor(WHITE);
				solidcircle((int)(WINDOWS_X / 2 + BBLACK / 2), (int)(WINDOWS_Y / 3 + BBLACK*1.25 + 2 * bblack), int(BBLACK / 6));

				for (int i = 0; i < 2; i++)
				{
					getimage(MC + i, WINDOWS_X / 2 - 2 * BBLACK, WINDOWS_Y / 3 + BBLACK*i + 2 * i * bblack, 2 * BBLACK, BBLACK / 2);
				}

				RECT R = { WINDOWS_X - BBLACK,WINDOWS_Y - BBLACK / 2,WINDOWS_X - 10,WINDOWS_Y - 10 };
				rectangle(WINDOWS_X - BBLACK, WINDOWS_Y - BBLACK / 2, WINDOWS_X - 10, WINDOWS_Y - 10);
				drawtext("����", &R, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				break;
			}
			else if (m.x>WINDOWS_X / 2 - BBLACK && m.x<WINDOWS_X / 2 + BBLACK && m.y> WINDOWS_Y / 3 + BBLACK * 2 + 4 * bblack && m.y<(int)(WINDOWS_Y / 3 + BBLACK * 2.5 + 4 * bblack) && _DRMS && !_SELECTDIFF)			//����
			{
				mciSendString("play Music/����.wav", NULL, 0, NULL);
				_SELECTDIFF = true;
				diff = 3;            //��������ģʽ
				// ��ʵ��
				break;
			}
			else if (m.x>WINDOWS_X / 2 - 2 * BBLACK && m.x<WINDOWS_X / 2 + BBLACK && m.y>WINDOWS_Y / 3 && m.y<(int)(WINDOWS_Y / 3 + BBLACK / 2) && _DRMS && _SELECTDIFF)			//ѡ������
			{
				mciSendString("play Music/����.wav", NULL, 0, NULL);
				MYCOLOR = black;
				_HOME = false;
				cleardevice();
				init();
				if (diff == 1) STARTVS(black, MOUSE, Easy);

				break;
			}
			else if (m.x>WINDOWS_X / 2 - 2 * BBLACK && m.x<WINDOWS_X / 2 + BBLACK && m.y>WINDOWS_Y / 3 + BBLACK + 2 * bblack && m.y<(int)(WINDOWS_Y / 3 + BBLACK*1.5 + 2 * bblack) && _DRMS && _SELECTDIFF)			//ѡ�����
			{
				mciSendString("play Music/����.wav", NULL, 0, NULL);
				MYCOLOR = white;
				_HOME = false;
				cleardevice();
				init();
				if (diff == 1) STARTVS(white, MOUSE, Easy);

				break;
			}
			else if (m.x>WINDOWS_X / 2 - BBLACK && m.x<WINDOWS_X / 2 + BBLACK && m.y>WINDOWS_Y / 3 + BBLACK / 2 + bblack && m.y<WINDOWS_Y / 3 + BBLACK + bblack && _HOME && !_INTRODUCTION && !_OPERATION && !_ABOUT && !_TOINTERNET && !_DRMS && !_SELECTDIFF)//˫��ģʽ
			{
				mciSendString("play Music/����.wav", NULL, 0, NULL);
				single = false;
				_HOME = false;									//�뿪HOME����
				init();
				STARTVS(1, MOUSE, MOUSE);							//˫��ģʽ
				break;
			}
			else if (m.x>WINDOWS_X / 2 - BBLACK && m.x<WINDOWS_X / 2 + BBLACK && m.y>WINDOWS_Y / 3 + BBLACK + 2 * bblack && m.y<(int)(WINDOWS_Y / 3 + BBLACK*1.5 + 2 * bblack) && _HOME && !_OPERATION && !_ABOUT && !_INTRODUCTION && !_TOINTERNET && !_DRMS && !_SELECTDIFF)   //������Ϸ
			{
				mciSendString("play Music/����.wav", NULL, 0, NULL);
				load_game();
				_HOME = false;
				if (!single) STARTVS(MYCOLOR, MOUSE, MOUSE);
				else {
					if (diff == 1) STARTVS(MYCOLOR, MOUSE, Easy);

				}
				break;
			}
			else if (m.x>WINDOWS_X / 2 - BBLACK && m.x<WINDOWS_X / 2 + BBLACK && m.y>(int)(WINDOWS_Y / 3 + BBLACK*1.5 + 3 * bblack) && m.y<WINDOWS_Y / 3 + BBLACK * 2 + 3 * bblack && _HOME && !_OPERATION && !_ABOUT && !_INTRODUCTION && !_TOINTERNET && !_DRMS && !_SELECTDIFF)//������ս
			{
				mciSendString("play Music/����.wav", NULL, 0, NULL);
				single = false;
				_TOINTERNET = true;
				cleardevice();

				break;
			}
			else if (m.x>WINDOWS_X / 4 && m.x<WINDOWS_X / 4 + 2 * BBLACK  && m.y>WINDOWS_Y / 2 + BBLACK && m.y<WINDOWS_Y / 2 + 2 * BBLACK && _TOINTERNET)//��Ҫ����
			{
				mciSendString("play Music/����.wav", NULL, 0, NULL);

				cleardevice();

				break;
			}
			else if (m.x>WINDOWS_X - WINDOWS_X / 4 - 2 * BBLACK && m.x<WINDOWS_X - WINDOWS_X / 4 && m.y>WINDOWS_Y / 2 + BBLACK && m.y<WINDOWS_Y / 2 + 2 * BBLACK && _TOINTERNET)//��Ҫ����
			{
				mciSendString("play Music/����.wav", NULL, 0, NULL);

				cleardevice();

				break;
			}
			else if (m.x>WINDOWS_X / 2 - BBLACK && m.x<WINDOWS_X / 2 + BBLACK && m.y> WINDOWS_Y / 3 + BBLACK * 2 + 4 * bblack && m.y<(int)(WINDOWS_Y / 3 + BBLACK * 2.5 + 4 * bblack) && _HOME && !_OPERATION && !_ABOUT && !_INTRODUCTION && !_TOINTERNET && !_DRMS && !_SELECTDIFF)		//��սģʽ
			{
				mciSendString("play Music/����.wav", NULL, 0, NULL);
				single = false;
				_HOME = false;										//�뿪HOME����
				//��սģʽ

				break;
			}
			else if (m.x>WINDOWS_X / 2 - BBLACK && m.x<WINDOWS_X / 2 + BBLACK && m.y>(int)(WINDOWS_Y / 3 + BBLACK * 2.5 + 5 * bblack) && m.y<WINDOWS_Y / 3 + BBLACK * 3 + 5 * bblack && _HOME && !_INTRODUCTION && !_ABOUT && !_OPERATION && !_TOINTERNET && !_DRMS && !_SELECTDIFF)	//��Ϸ����
			{
				mciSendString("play Music/����.wav", NULL, 0, NULL);
				_INTRODUCTION = true;
				cleardevice();
				loadimage(NULL, "Picture/0.png");				//����
				string data[16] = {
					"��Ϸ���ܣ�" ,
					"ģ��ڰ�����Ϸ��ʵ�����˻�ģʽ��˫�˶�սģʽ��",
					""	// ������ı�
				};

				rectangle(BBLACK, BBLACK, WINDOWS_X - BBLACK, WINDOWS_Y - BBLACK);
				settextstyle(BBLACK / 2, 0, "����Ҧ��");
				settextcolor(RGB(0, 255, 255));
				outtextxy(WINDOWS_X / 3 - 100, 90, data[0].data());
				settextstyle(BBLACK / 3, 0, "����");
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
				drawtext("����", &R1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				break;
			}
			else if (m.x>WINDOWS_X / 2 - BBLACK && m.x<WINDOWS_X / 2 + BBLACK && m.y>WINDOWS_Y / 3 + BBLACK * 3 + 6 * bblack && m.y < (int)(WINDOWS_Y / 3 + BBLACK * 3.5 + 6 * bblack) && _HOME && !_INTRODUCTION && !_OPERATION && !_ABOUT && !_TOINTERNET && !_DRMS && !_SELECTDIFF)	//����˵��
			{
				mciSendString("play Music/����.wav", NULL, 0, NULL);
				_OPERATION = true;
				cleardevice();
				loadimage(NULL, "Picture/0.png");				//����
				string data[16] = {
					"����˵����" ,
					"�ڰ����������һ-����8*8��������̡�",
					"����ʱ�������ڿո��м䣬��������Χ��һ�����ڽ�����ϡ�",
					"��ʼʱ���������������������ĸ����ӽ������,�������������ӡ�",
					"���Լ���ɫ�����ӷ������̵Ŀո��ϣ������Լ����µ�����",
					"�ںᡢ����б�˸���������һ-���Լ�������,�򱻼����м��ȫ����ת���Ϊ",
					"�Լ������ӡ�",

				};

				rectangle(BBLACK, BBLACK, WINDOWS_X - BBLACK, WINDOWS_Y - BBLACK);
				settextstyle(BBLACK / 2, 0, "����Ҧ��");
				settextcolor(RGB(0, 255, 255));
				outtextxy(WINDOWS_X / 8 - 50, 90, data[0].data());
				settextstyle(BBLACK / 3, 0, "����Ҧ��");
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
				drawtext("����", &R3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				break;
			}
			else if (m.x > WINDOWS_X / 2 - BBLACK && m.x<WINDOWS_X / 2 + BBLACK && m.y>(int)(WINDOWS_Y / 3 + BBLACK * 3.5 + 7 * bblack) && m.y < (int)(WINDOWS_Y / 3 + BBLACK * 4 + 7 * bblack) && _HOME && !_INTRODUCTION && !_OPERATION && !_ABOUT && !_TOINTERNET && !_DRMS && !_SELECTDIFF)//����
			{
				mciSendString("play Music/����.wav", NULL, 0, NULL);
				_ABOUT = true;
				cleardevice();
				loadimage(NULL, "Picture/0.png");				//����
				string data[16] = {
					"����:" ,
					"����Ϸ����EasyXͼ�λ��������ɡ�"	// ������ı�
				};

				rectangle(BBLACK, BBLACK, WINDOWS_X - BBLACK, WINDOWS_Y - BBLACK);
				settextstyle(BBLACK / 2, 0, "����Ҧ��");
				settextcolor(RGB(0, 255, 255));
				outtextxy(WINDOWS_X / 3 - 100, 90, data[0].data());
				settextstyle(BBLACK / 3, 0, "����");
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
				drawtext("����", &R3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				settextstyle(BBLACK / 4, 0, "΢���ź�");
				settextstyle(BBLACK / 4, 0, "����Ҧ��");
				break;
			}
			else if (m.x>WINDOWS_X - BBLACK && m.x<WINDOWS_X - 10 && m.y>WINDOWS_Y - BBLACK / 2 && m.y<WINDOWS_Y - 10 && (_INTRODUCTION || _OPERATION || _ABOUT || _TOINTERNET || _DRMS))					//����
			{
				cleardevice();
				_HOME = false, _INTRODUCTION = false, _OPERATION = false, _ABOUT = false, _TOINTERNET = false, _DRMS = false;
				gameStart();
			}
			else if (m.x>WINDOWS_X / 2 - BBLACK && m.x<WINDOWS_X / 2 + BBLACK && m.y>(int)(WINDOWS_Y / 3 + BBLACK * 4 + 8 * bblack) && m.y<WINDOWS_Y / 3 + BBLACK * 4.5 + 8 * bblack && _HOME && !_INTRODUCTION && !_OPERATION && !_ABOUT && !_TOINTERNET && !_DRMS && !_SELECTDIFF)//�˳���Ϸ
			{
				exit(0);
			}
			else break;
		case WM_MOUSEMOVE:									//�ƶ����
			RECT r;
			if (_INTRODUCTION || _OPERATION || _ABOUT || _TOINTERNET || _DRMS)				//�����ǰ������Ϸ���� ����˵�� ���߹��ڽ��� ����������ս���� ���ߵ���ģʽ����
			{
				if (ESCEXIT)gameStart();							//���ֽ��水ESC�˳�
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
					drawtext("����", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				}
				else
				{
					if (getpixel(WINDOWS_X - BBLACK + 1, WINDOWS_Y - BBLACK / 2 + 1) == RED)
					{
						putimage(WINDOWS_X - BBLACK + 1, WINDOWS_Y - BBLACK / 2 + 1, MM + 8);
						setbkmode(TRANSPARENT);
						drawtext("����", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
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
								drawtext("��    ��", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
								break;
							case 1:
								drawtext("��    ��", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
								break;
							case 2:
								drawtext("��    ��", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
								break;
							}
						}
						else
						{
							if (getpixel(WINDOWS_X / 2 - BBLACK + 1, WINDOWS_Y / 3 + BBLACK*i + 2 * i * bblack + 1) == RED)
							{
								putimage(WINDOWS_X / 2 - BBLACK, WINDOWS_Y / 3 + BBLACK*i + 2 * i * bblack, MB + i);	//���ԭ��ͼƬ
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
								drawtext("��    ��", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
								break;
							case 1:
								drawtext("��    ��", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
								break;

							}
						}
						else
						{
							if (getpixel(WINDOWS_X / 2 - 2 * BBLACK + 1, WINDOWS_Y / 3 + BBLACK*i + 2 * i * bblack + 1) == RED)
							{
								putimage(WINDOWS_X / 2 - 2 * BBLACK, WINDOWS_Y / 3 + BBLACK*i + 2 * i * bblack, MC + i);	//���ԭ��ͼƬ

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
							drawtext("����ģʽ", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 1:
							drawtext("˫��ģʽ", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 2:
							drawtext("����浵", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 3:
							drawtext("������ս", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 4:
							drawtext("��սģʽ", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 5:
							drawtext("��Ϸ����", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 6:
							drawtext("����˵��", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 7:
							drawtext("��    ��", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						case 8:
							drawtext("�˳���Ϸ", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
							break;
						}
					}
					else
					{
						if (getpixel(WINDOWS_X / 2 - BBLACK + 1, WINDOWS_Y / 3 + i*bblack + i*BBLACK / 2 + 1) == RED)
						{
							putimage(WINDOWS_X / 2 - BBLACK, WINDOWS_Y / 3 + i*bblack + i*BBLACK / 2, MM + i);	//���ԭ��ͼƬ
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
	gameStart();					//������
	closegraph();					//�ر�ͼ�λ�����
	return 0;
}
