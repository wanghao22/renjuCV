#include "Ai.h"

//确定落子
chess Ai::set_chess()
{
	__int64 points_b[g_length][g_length];       //记录黑棋各落点分数
	__int64 points_w[g_length][g_length];       //记录白棋各落点分数
	position best_b[20];        //记录黑棋最大分数对应的落点位置
	position best_w[20];        //记录白棋最大分数对应的落点位置
	__int64 s_black = 0, s_white = 0;       //记录黑白棋分别的最大分数值
	int count_b = 0, count_w = 0;            //记录黑白棋最大分数对应的落点位置个数

	whole_points(points_b, black);
	whole_points(points_w, white);
	s_white = best_posits(points_w, best_w, count_w);
	s_black = best_posits(points_b, best_b, count_b);

	if (count_w >= 19 || count_b >= 19)
	{
		//MessageBox(NULL, L"游戏即将崩溃", L"游戏结束", MB_OKCANCEL);
		throw std::exception("平局");
	}

	if (s_black > s_white)     //黑棋最高分高过白棋，在黑棋最高分对应的位置中选出白棋分数最大的位置落子
	{
	sb: __int64 a[20];
		for (int i = 0; i < count_b; i++)
		{
			a[i] = point(best_b[i], white);
		}
		__int64 max_b = _MAX(a, count_b);
		for (int i = 0; i < count_b; i++)
		{
			if (a[i] == max_b)
			{
				ms.set_point(best_b[i]);
				return ms;
			}
		}
	}
	if (s_black < s_white)     //白棋最高分高过黑棋，在白棋最高分对应的位置中选出黑棋分数最大的位置落子
	{
	sw: __int64 a[20];
		for (int i = 0; i < count_w; i++)
		{
			a[i] = point(best_w[i], black);
		}
		__int64 max_w = _MAX(a, count_w);
		for (int i = 0; i < count_w; i++)
		{
			if (a[i] == max_w)
			{
				ms.set_point(best_w[i]);
				return ms;
			}
		}
	}
	if (s_black == s_white)
	{
		if (ms.get_color() == white)
			goto sw;
		if (ms.get_color() == black)
			goto sb;
	}
	return ms;
}

//给出分数数组，找出最大值对应的位置（可能不止一个），返回分数最大值
__int64 Ai::best_posits(const __int64 points[][g_length], position p_s[], int& count)
{
	__int64 max_row[g_length];
	__int64 max_all;
	for (int i = 0; i < g_length; i++)
		max_row[i] = _MAX(points[i], g_length);
	max_all = _MAX(max_row, g_length);
	cout << "maxall" << max_all;
	count = 0;
	for (int i = 0; i < g_length; i++)
	{
		for (int j = 0; j < g_length; j++)
		{
			if (points[i][j] == max_all)
			{
				position x(i, j);
				p_s[count] = x;
				count++;
			}
			if(count>=19)
				return max_all;
		}
	}
	return max_all;
}

//给定颜色 ，记录该颜色棋子下在每一处的得分
void Ai::whole_points(__int64 points[][g_length], state color)
{
	for (int i = 0; i < g_length; i++)
	{
		for (int j = 0; j < g_length; j++)
		{
			position temp(i, j);
			points[i][j] = point(temp, color);
		}
	}
}

__int64 Ai::evaluate(position pos, state color, position(*pf)(position, bool))
{
	int sum = 0;
	position p_i = pos;
	int count = 0, mc = 1;
	bool flag = true;
	int c_blank = 0;
	state judge_t;

	try
	{
		do
		{
			p_i = pf(p_i, flag);
			judge_t = p_bd->viewboard(p_i);
			if (judge_t == color)
			{
				if (c_blank == 1)
				{
					count += 1;
				}
				else
				{
					mc++;
					if (mc == 5)
						return 100000000000;
					count += 2;
				}
			}
			else
			{
				if (judge_t == blank)
				{
					if (c_blank >= 1)
						flag = false;
					else
					{
						c_blank++;
					}
				}
				else
				{
					count -= 2;
					flag = false;
				}
			}
		} while (flag);
	}
	catch (int)
	{
		flag = false;
		if (c_blank == 0)count -= 2;
	}

	p_i = pos;
	int b_blank = 0;//记录另一半的空白格子
	try
	{
		do
		{
			p_i = pf(p_i, flag);
			judge_t = p_bd->viewboard(p_i);
			if (judge_t == color)
			{
				if (b_blank == 1)
				{
					count += 1;
				}
				else
				{
					if (c_blank == 0 && b_blank == 0)
						mc++;
					if (mc == 5)
						return 100000000000;
					count += 2;
				}
			}
			else
			{
				if (judge_t == blank)
				{
					if (b_blank >= 1)
						flag = true;
					else
					{
						b_blank++;
					}
				}
				else
				{
					count -= 2;
					flag = true;
				}
			}
		} while (!flag);
	}
	catch (int)
	{
		if (b_blank == 0)
			count -= 2;
		__int64 result = static_cast<__int64>(pow(10, count));
		return result;
	}
	__int64 result = static_cast<__int64>(pow(10, count));
	return result;
}

//给出一个落子位置，返回该落子的得分
__int64 Ai::point(position pos, state color)
{
	if (p_bd->viewboard(pos) != blank)
	{
		return 0;
	}

	position(*p_f)(position, bool) = NULL;
	__int64 sum = 0;

	p_f = up;
	sum += evaluate(pos, color, p_f);
	p_f = left;
	sum += evaluate(pos, color, p_f);
	p_f = left_up;
	sum += evaluate(pos, color, p_f);
	p_f = right_up;
	sum += evaluate(pos, color, p_f);

	return sum;
}