#include "Ai.h"

//ȷ������
chess Ai::set_chess()
{
	__int64 points_b[g_length][g_length];       //��¼�����������
	__int64 points_w[g_length][g_length];       //��¼�����������
	position best_b[20];        //��¼������������Ӧ�����λ��
	position best_w[20];        //��¼������������Ӧ�����λ��
	__int64 s_black = 0, s_white = 0;       //��¼�ڰ���ֱ��������ֵ
	int count_b = 0, count_w = 0;            //��¼�ڰ�����������Ӧ�����λ�ø���

	whole_points(points_b, black);
	whole_points(points_w, white);
	s_white = best_posits(points_w, best_w, count_w);
	s_black = best_posits(points_b, best_b, count_b);

	if (count_w >= 19 || count_b >= 19)
	{
		//MessageBox(NULL, L"��Ϸ��������", L"��Ϸ����", MB_OKCANCEL);
		throw std::exception("ƽ��");
	}

	if (s_black > s_white)     //������߷ָ߹����壬�ں�����߷ֶ�Ӧ��λ����ѡ�������������λ������
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
	if (s_black < s_white)     //������߷ָ߹����壬�ڰ�����߷ֶ�Ӧ��λ����ѡ�������������λ������
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

//�����������飬�ҳ����ֵ��Ӧ��λ�ã����ܲ�ֹһ���������ط������ֵ
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

//������ɫ ����¼����ɫ��������ÿһ���ĵ÷�
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
	int b_blank = 0;//��¼��һ��Ŀհ׸���
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

//����һ������λ�ã����ظ����ӵĵ÷�
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