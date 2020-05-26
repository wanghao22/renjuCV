#pragma once
#ifndef AI_H
#define AI_H
#include "renju.h"
#include <vector>
#include <math.h>

//λ�ú��������������ƶ����Ӳ��ж��Ƿ�Խ��
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

//λ�ú��������������ƶ����Ӳ��ж��Ƿ�Խ��
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

//λ�ú������������������ƶ����Ӳ��ж��Ƿ�Խ��
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

//λ�ú������������������ƶ����Ӳ��ж��Ƿ�Խ��
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
	__int64 evaluate(position pos, state color, position(*pf)(position, bool));//��������λ�úͷ����ƶ����������ظ�����λ���ڸ÷����ϵ�����

	__int64 point(position pos, state color);//����һ������λ�ã����ظ����ӵĵ÷�

	void whole_points(__int64 points[][g_length], state color);//������ɫ ����¼����ɫ��������ÿһ���ĵ÷�

	__int64 best_posits(const __int64 points[][g_length], position p_s[], int& count); //�����������飬�ҳ����ֵ��Ӧ��λ�ã����ܲ�ֹһ���������ط������ֵ

	chess ms;
	const chessboard *p_bd;
};

#endif
