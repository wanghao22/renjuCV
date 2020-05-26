#pragma once
#ifndef AI_H
#define AI_H
#include "renju.h"
#include <vector>
#include <math.h>

//位置函数，用于上下移动棋子并判断是否越界
inline position up(position pos, bool dir)
{
	position r;
	if (dir)
	{
		while (pos.y > 0)
		{
			r.x = pos.x;
			r.y = pos.y - 1;
			return r;
		}
		throw 0;
	}
	else
	{
		while (pos.y < g_length - 1)
		{
			r.x = pos.x;
			r.y = pos.y + 1;
			return r;
		}
		throw 0;
	}
}

//位置函数，用于左右移动棋子并判断是否越界
inline position left(position pos, bool dir)
{
	position r;
	if (dir)
	{
		while (pos.x > 0)
		{
			r.x = pos.x - 1;
			r.y = pos.y;
			return r;
		}
		throw 0;
	}
	else
	{
		while (pos.x < g_length - 1)
		{
			r.x = pos.x + 1;
			r.y = pos.y;
			return r;
		}
		throw 0;
	}
}

//位置函数，用于左上右下移动棋子并判断是否越界
inline position left_up(position pos, bool dir)
{
	position r;
	if (dir)
	{
		while (pos.x > 0 && pos.y > 0)
		{
			r.x = pos.x - 1;
			r.y = pos.y - 1;
			return r;
		}
		throw 0;
	}
	else
	{
		while (pos.x < g_length - 1 && pos.y < g_length - 1)
		{
			r.x = pos.x + 1;
			r.y = pos.y + 1;
			return r;
		}
		throw 0;
	}
}

//位置函数，用于右上左下移动棋子并判断是否越界
inline position right_up(position pos, bool dir)
{
	position r;
	if (dir)
	{
		while (pos.x < g_length - 1 && pos.y > 0)
		{
			r.x = pos.x + 1;
			r.y = pos.y - 1;
			return r;
		}
		throw 0;
	}
	else
	{
		while (pos.x > 0 && pos.y < g_length - 1)
		{
			r.x = pos.x - 1;
			r.y = pos.y + 1;
			return r;
		}
		throw 0;
	}
}

inline position around(position pos)
{
	position r;
	if (pos.x == g_length - 1)
	{
		r.x = pos.x - 1;
		r.y = pos.y;
		return r;
	}
	else if (pos.x == 0)
	{
		r.x = pos.x + 1;
		r.y = pos.y;
		return r;
	}
	else
	{
		while (pos.x < g_length - 1)
		{
			r.x = pos.x + 1;
			r.y = pos.y;
			return r;
		}
	}
	return r;
}

class Ai
{
public:
	Ai(chessboard &bd, state hm)
	{
		ms.set_color(hm);
		this->p_bd = &bd;
	}
	chess set_chess();

private:
	__int64 evaluate(position pos, state color, position(*pf)(position, bool));//给出落子位置和方向移动函数，返回该落子位置在该方向上的评分

	__int64 point(position pos, state color);//给出一个落子位置，返回该落子的得分

	void whole_points(__int64 points[][g_length], state color);//给定颜色 ，记录该颜色棋子下在每一处的得分

	__int64 best_posits(const __int64 points[][g_length], position p_s[], int& count); //给出分数数组，找出最大值对应的位置（可能不止一个），返回分数最大值

	chess ms;
	const chessboard *p_bd;
};

#endif
