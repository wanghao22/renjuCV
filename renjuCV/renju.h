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


//用于记录坐标
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


//用于记录棋子颜色和节点状态
enum state
{
	blank = 0, 
	black = 1, 
	white = 2
};


//用于存储棋局分析信息:未完赛，犯规，平局，黑方胜，白方胜
enum result
{
	go_on, error_, draw, black_win, white_win
};


// 设置光标
inline void setpos(COORD a)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(out, a);
}


// 设置光标
inline void setpos(int i, int j)
{
	COORD pos = { i, j };
	setpos(pos);
}


//绘图函数，用于在指定坐标输出指定字符
inline void gps(int x, int y, char c)
{
	setpos(y, x);
	cout << c;
}


//绘图函数，用于在指定坐标输出整数
inline void  gps(int x, int y, int i)
{
	setpos(y, x);
	if (i >= 10)
		cout << i;
	else
		cout << 0 << i;
}


//绘图函数，用于在指定坐标输出字符串
inline void  gps(int x, int y, string s)
{
	setpos(y, x);
	cout << s;
}


//绘图函数，用于在给定坐标打印棋盘中的一格
inline void tab(int x, int y, state str)
{
	string s;
	switch (str)
	{
	case blank:
		s = "  ";
		break;
	case black:
		s = "黑";
		break;
	case white:
		s = "白";
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


//查找最大值
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
	struct position drop_point()const;//用于向外部提供棋子位置
	inline state get_color() const { return color; }//提供棋子颜色信息
	void set_point();//用于从外部获取棋子位置
	void set_point(int x, int y) { point.x = x, point.y = y; }
	void set_point(position p) { point.x = p.x, point.y = p.y; }
	void set_color(state c) { color = c; }//设置棋子颜色
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
	inline state viewboard(position p_c) const;//接收坐标，返回该位置的状态
	inline state viewboard(int x, int y) const;//接收整数坐标，返回该位置的状态
	void update(chess ch);//接收新棋子，更新棋盘状态
	void display()const;//向外输出棋盘状态
	result judge(chess ch)const;//规则函数，判断走棋是否犯规和输赢
	void clear()//清空棋盘
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