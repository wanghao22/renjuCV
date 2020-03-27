#pragma once
#ifndef RENJU_H
#define RENJU_H
#include <iostream>
#include <windows.h>
#include <string>
#define hor 7
#define ver 4
using namespace std;

const int g_length = 13;


//���ڼ�¼����
struct  position
{
	int x;
	int y;
	position()
	{
		x = 0;
		y = 0;
	}
	position(int a, int b)
	{
		x = a;
		y = b;
	}
};


//���ڼ�¼������ɫ�ͽڵ�״̬
enum state
{
	blank = 0, 
	black = 1, 
	white = 2
};


//���ڴ洢��ַ�����Ϣ:δ���������棬ƽ�֣��ڷ�ʤ���׷�ʤ
enum result
{
	go_on, error_, draw, black_win, white_win
};


// ���ù��
inline void setpos(COORD a)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(out, a);
}


// ���ù��
inline void setpos(int i, int j)
{
	COORD pos = { i, j };
	setpos(pos);
}


//��ͼ������������ָ���������ָ���ַ�
inline void gps(int x, int y, char c)
{
	setpos(y, x);
	cout << c;
}


//��ͼ������������ָ�������������
inline void  gps(int x, int y, int i)
{
	setpos(y, x);
	if (i >= 10)
		cout << i;
	else
		cout << 0 << i;
}


//��ͼ������������ָ����������ַ���
inline void  gps(int x, int y, string s)
{
	setpos(y, x);
	cout << s;
}


//��ͼ�����������ڸ��������ӡ�����е�һ��
inline void tab(int x, int y, state str)
{
	string s;
	switch (str)
	{
	case blank:
		s = "  ";
		break;
	case black:
		s = "��";
		break;
	case white:
		s = "��";
		break;
	default:
		break;
	}
	setpos(y, x);
	cout << " ------ ";
	setpos(y, x + 1);
	cout << "|      |";
	setpos(y, x + 2);
	cout << "|  " << s << "  |";
	setpos(y, x + 3);
	cout << "|      |";
	setpos(y, x + 4);
	cout << " ------ ";
}


//�������ֵ
inline int _MAX(const int *a, int n)
{
	int max = a[0];
	for (int i = 1; i < n; i++)
	{
		if (a[i] > max)
			max = a[i];
	}
	return max;
}

class chess
{
public:
	inline chess(int x = 0, int y = 0, state c = blank)
	{
		point.x = x, point.y = y;
		color = c;
	};
	inline chess(chess &ch)
	{
		point = ch.drop_point();
		color = ch.get_color();
	}
	~chess() {};
	struct position drop_point()const;//�������ⲿ�ṩ����λ��
	inline state get_color() const { return color; }//�ṩ������ɫ��Ϣ
	void set_point();//���ڴ��ⲿ��ȡ����λ��
	void set_point(int x, int y) { point.x = x, point.y = y; }
	void set_point(position p) { point.x = p.x, point.y = p.y; }
	void set_color(state c) { color = c; }//����������ɫ
	void get_point(int &x, int &y) { x = point.x, y = point.y; }
private:
	position point;
	enum state color;
};

class chessboard
{
public:
	chessboard()
	{
		for (int i = 0; i<g_length; i++)
			for (int j = 0; j<g_length; j++)
			{
				square[i][j] = blank;
			}
	}
	chessboard(chessboard *cb)
	{
		for (int i = 0; i<g_length; i++)
			for (int j = 0; j<g_length; j++)
			{
				square[i][j] = cb->viewboard(i, j);
			}
	}
	inline state viewboard(position p_c) const;//�������꣬���ظ�λ�õ�״̬
	inline state viewboard(int x, int y) const;//�����������꣬���ظ�λ�õ�״̬
	void update(chess ch);//���������ӣ���������״̬
	void display()const;//�����������״̬
	result judge(chess ch)const;//���������ж������Ƿ񷸹����Ӯ
	void clear()//�������
	{
		for (int i = 0; i<g_length; i++)
			for (int j = 0; j<g_length; j++)
			{
				square[i][j] = blank;
			}
	}
private:
	state square[g_length][g_length];
};

extern int check_five(chessboard bd, chess ch);

#endif