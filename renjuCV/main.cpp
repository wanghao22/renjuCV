#include <iostream>
#include <opencv2\opencv.hpp>
#include "Ai.h"
#include "renju.h"
#include <Windows.h>

using namespace std;
using namespace cv;

//С���Ӽ��
const int c_w = 50;
const cv::Scalar g_cwhite(255, 255, 255);
const cv::Scalar g_cblack(0, 0, 0);
//���Ӱ뾶
const int g_cwidth = 20;

//�������̸�
cv::Mat creatChessBody(int x)
{
	int width = x * c_w;
	int b_min = 0 + c_w / 2 - 1;
	int b_max = width - c_w / 2 - 1;
	cv::Mat result(width, width, CV_8UC3, Scalar(149, 179, 209));
	for (int i = 0; i < x; i++)
	{
		int t = i*c_w + b_min;
		if (x % 2 == 1 && i == x / 2)
		{
			line(result, Point(b_min, t), Point(b_max, t), Scalar(0, 0, 0), 2);
			line(result, Point(t, b_min), Point(t, b_max), Scalar(0, 0, 0), 2);
		}
		else
		{
			line(result, Point(b_min, t), Point(b_max, t), Scalar(0, 0, 0), 1);
			line(result, Point(t, b_min), Point(t, b_max), Scalar(0, 0, 0), 1);
		}
	}
	return result;
}

void putChress(cv::Mat &src, int x, int y, int color)
{
	if (x >= 1 && x <= g_length&&y >= 1 && y <= g_length)
	{
		Point pt;
		pt.x = (x - 1)*c_w + c_w / 2 - 1;
		pt.y = (y - 1)*c_w + c_w / 2 - 1;
		switch (color)
		{
		case(black):
			circle(src, pt, g_cwidth, g_cblack, FILLED);
			break;
		case(white):
			circle(src, pt, g_cwidth, g_cwhite, FILLED);
			break;
		default:
			break;
		}
	}
}

bool putChress1(int &x, int &y)
{
	bool x_flag = false, y_flag = false;
	int col = 0, row = 0;
	for (int i = 0; i < g_length; i++)
	{
		int t = i*c_w + c_w / 2 - 1;
		if (fabs(t - x) <= g_cwidth)
		{
			x = i + 1;
			x_flag = true;
		}
		if (fabs(t - y) <= g_cwidth)
		{
			y = i + 1;
			y_flag = true;
		}
	}
	return x_flag&&y_flag;
}

int g_x = 8, g_y = 8;
bool g_falg = false;
void mousecallback_mat3(int event, int x, int y, int flag, void *param)
{
	Mat *img = (Mat*)param;
	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN:
		if (!g_falg)
		{
			g_x = x, g_y = y;
			if (putChress1(g_x, g_y))
				g_falg = true;
		}
		break;
	default:
		break;
	}
}

void ˫�˶�ս()
{
	chessboard bd;
	chess now;
	result finalr;
	cv::namedWindow("�������ս");
	Mat src;
	cvSetMouseCallback("�������ս", mousecallback_mat3, (void*)&src);
	src = creatChessBody(g_length);
	now.set_color(black);//ִ������
	bd.clear();
	cout << "�Ծֿ�ʼ����������";
	//ѭ���ж���һ�����ӽ�����ռ���һ�����룬ֱ����ֳ����
	while (true)
	{
		finalr = go_on;
		if (g_falg)
		{
			now.set_point(g_x - 1, g_y - 1);
			finalr = bd.judge(now);
			//�ж��Ƿ�Υ��
			if (finalr == error_)//������������������
			{
				MessageBox(NULL, L"����������", L"����", MB_OKCANCEL);
				g_falg = false;
				continue;
			}
			putChress(src, g_x, g_y, now.get_color());
			//��ȷ���Ӻ��������
			bd.update(now);
			//��ת��һ���������ɫ
			if (now.get_color() == black)
				now.set_color(white);
			else
				now.set_color(black);
			g_falg = false;
		}
		imshow("�������ս", src);
		char c = waitKey(30);
		if (c == 27 || c == 'q' || c == 'Q')
			break;
		else if (c == 'W' || c == 'w')
			imwrite("111.png", src);
		if (finalr != go_on)
			break;
	}
	switch (finalr)
	{
	case draw:
		MessageBox(NULL, L"ƽ��", L"��Ϸ����", MB_OKCANCEL);
		break;
	case black_win:
		MessageBox(NULL, L"�����ʤ", L"��Ϸ����", MB_OKCANCEL);
		break;
	case white_win:
		MessageBox(NULL, L"�����ʤ", L"��Ϸ����", MB_OKCANCEL);
		break;
	case go_on:
	case error_:
	default:
		break;
	}
}

void AI����()
{
	chessboard bd;
	chess now;
	chess ai_ch;
	result finalr;
	bool init_flag = true;
	cv::namedWindow("�������ս");
	Mat src;
	cvSetMouseCallback("�������ս", mousecallback_mat3, (void*)&src);
	src = creatChessBody(g_length);
	now.set_color(black);//ִ������
	bd.clear();
	Ai afago(bd, white);
	ai_ch.set_color(white);
	cout << "�Ծֿ�ʼ����������";

	//ѭ���ж���һ�����ӽ�����ռ���һ�����룬ֱ����ֳ����
	while (true)
	{
		finalr = go_on;
		if (init_flag&&g_falg)
		{
			now.set_point(g_x - 1, g_y - 1);
			if (finalr == error_)//������������������
			{
				MessageBox(NULL, L"����������", L"����", MB_OKCANCEL);
				init_flag = true;
				g_falg = false;
				continue;
			}
			putChress(src, g_x, g_y, black);
			bd.update(now);
			ai_ch.set_point(around(now.drop_point()));
			int x = 0, y = 0;
			ai_ch.get_point(x, y);
			putChress(src, x + 1, y + 1, white);
			bd.update(ai_ch);
			init_flag = false;
			g_falg = false;
			system("cls");
			bd.display();
			cout << x + 1 << " " << y + 1 << endl;
		}
		else if (!init_flag&&g_falg)
		{
			now.set_point(g_x - 1, g_y - 1);//����
			finalr = bd.judge(now);
			//�ж��Ƿ�Υ��
			if (finalr == error_)//������������������
			{
				MessageBox(NULL, L"����������", L"����", MB_OKCANCEL);
				g_falg = false;
				continue;
			}
			putChress(src, g_x, g_y, black);
			//��ȷ���Ӻ��������
			bd.update(now);
			int x = 0, y = 0;
			if (finalr == go_on)
			{
				try
				{
					ai_ch = afago.set_chess();
				}
				catch (const std::exception& e)
				{
					finalr = draw;
					break;
				}
				finalr = bd.judge(ai_ch);
				bd.update(ai_ch);
				ai_ch.get_point(x, y);
				putChress(src, x + 1, y + 1, white);
			}
			g_falg = false;
			system("cls");
			bd.display();
			cout << x + 1 << " " << y + 1 << endl;
		}
		imshow("�������ս", src);
		char c = waitKey(30);
		if (c == 27 || c == 'q' || c == 'Q')
			break;
		else if (c == 'W' || c == 'w')
			imwrite("111.png", src);
		if (finalr != go_on)
			break;
	}
	switch (finalr)
	{
	case draw:
		MessageBox(NULL, L"ƽ��", L"��Ϸ����", MB_OKCANCEL);
		break;
	case black_win:
		MessageBox(NULL, L"��ϲ�����ո߳���սʤ��AI", L"��Ϸ����", MB_OKCANCEL);
		break;
	case white_win:
		MessageBox(NULL, L"���Ի�ʤ�������Ŭ������Լ�", L"��Ϸ����", MB_OKCANCEL);
		break;
	case go_on:
	case error_:
	default:
		break;
	}
}

void AI����()
{
	chessboard bd;
	chess now;
	chess ai_ch;
	result finalr;
	cv::namedWindow("�������ս");
	Mat src;
	cvSetMouseCallback("�������ս", mousecallback_mat3, (void*)&src);
	src = creatChessBody(g_length);
	ai_ch.set_color(black);//AIִ������
	now.set_color(white);
	bd.clear();
	Ai afago(bd, black);
	ai_ch.set_point(g_length / 2, g_length / 2);
	putChress(src, g_length / 2 + 1, g_length / 2 + 1, black);
	bd.update(ai_ch);
	cout << "�Ծֿ�ʼ����������";
	//ѭ���ж���һ�����ӽ�����ռ���һ�����룬ֱ����ֳ����
	while (true)
	{
		finalr = go_on;
		if (g_falg)
		{
			now.set_point(g_x - 1, g_y - 1);//����
			finalr = bd.judge(now);
			//�ж��Ƿ�Υ��
			if (finalr == error_)//������������������
			{
				MessageBox(NULL, L"����������", L"����", MB_OKCANCEL);
				g_falg = false;
				continue;
			}
			putChress(src, g_x, g_y, white);
			//��ȷ���Ӻ��������
			bd.update(now);
			int x = 0, y = 0;
			if (finalr == go_on)
			{
				try
				{
					ai_ch = afago.set_chess();
				}
				catch (const std::exception& e)
				{
					finalr = draw;
					break;
				}
				finalr = bd.judge(ai_ch);
				bd.update(ai_ch);
				ai_ch.get_point(x, y);
				putChress(src, x + 1, y + 1, black);
			}
			g_falg = false;
			system("cls");
			bd.display();
			cout << x + 1 << " " << y + 1 << endl;
		}
		imshow("�������ս", src);
		char c = waitKey(30);
		if (c == 27 || c == 'q' || c == 'Q')
			break;
		else if (c == 'W' || c == 'w')
			imwrite("111.png", src);
		if (finalr != go_on)
			break;
	}
	switch (finalr)
	{
	case draw:
		MessageBox(NULL, L"ƽ��", L"��Ϸ����", MB_OKCANCEL);
		break;
	case white_win:
		MessageBox(NULL, L"��ϲ�����ո߳���սʤ��AI", L"��Ϸ����", MB_OKCANCEL);
		break;
	case black_win:
		MessageBox(NULL, L"���Ի�ʤ�������Ŭ������Լ�", L"��Ϸ����", MB_OKCANCEL);
		break;
	case go_on:
	case error_:
	default:
		break;
	}
}

int main()
{
	while (true)
	{
		//˫�˶�ս();
		AI����();
		//AI����();
		g_falg = false;
		system("cls");
		cout << "�Ƿ������һ�֣�Y/N" << endl;
		char flag;
		cin >> flag;
		if (flag == 'Y' || flag == 'y')
			continue;
		else
			break;
	}
	return 0;
}