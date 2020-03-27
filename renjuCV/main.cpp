#include <iostream>
#include <opencv2\opencv.hpp>
#include "Ai.h"
#include "renju.h"
#include <Windows.h>

using namespace std;
using namespace cv;

//小格子间距
const int c_w = 50;
const cv::Scalar g_cwhite(255, 255, 255);
const cv::Scalar g_cblack(0, 0, 0);
//棋子半径
const int g_cwidth = 20;

//创建棋盘格
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

void 双人对战()
{
	chessboard bd;
	chess now;
	result finalr;
	cv::namedWindow("五子棋对战");
	Mat src;
	cvSetMouseCallback("五子棋对战", mousecallback_mat3, (void*)&src);
	src = creatChessBody(g_length);
	now.set_color(black);//执黑先行
	bd.clear();
	cout << "对局开始，黑旗先行";
	//循环判断上一次落子结果，收集下一次输入，直到棋局出结果
	while (true)
	{
		finalr = go_on;
		if (g_falg)
		{
			now.set_point(g_x - 1, g_y - 1);
			finalr = bd.judge(now);
			//判断是否违规
			if (finalr == error_)//犯规输入则重新输入
			{
				MessageBox(NULL, L"不能下这里", L"警告", MB_OKCANCEL);
				g_falg = false;
				continue;
			}
			putChress(src, g_x, g_y, now.get_color());
			//正确落子后更新棋盘
			bd.update(now);
			//反转下一步走棋的颜色
			if (now.get_color() == black)
				now.set_color(white);
			else
				now.set_color(black);
			g_falg = false;
		}
		imshow("五子棋对战", src);
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
		MessageBox(NULL, L"平局", L"游戏结束", MB_OKCANCEL);
		break;
	case black_win:
		MessageBox(NULL, L"黑旗获胜", L"游戏结束", MB_OKCANCEL);
		break;
	case white_win:
		MessageBox(NULL, L"白旗获胜", L"游戏结束", MB_OKCANCEL);
		break;
	case go_on:
	case error_:
	default:
		break;
	}
}

void AI后手()
{
	chessboard bd;
	chess now;
	chess ai_ch;
	result finalr;
	bool init_flag = true;
	cv::namedWindow("五子棋对战");
	Mat src;
	cvSetMouseCallback("五子棋对战", mousecallback_mat3, (void*)&src);
	src = creatChessBody(g_length);
	now.set_color(black);//执黑先行
	bd.clear();
	Ai afago(bd, white);
	ai_ch.set_color(white);
	cout << "对局开始，请您落子";

	//循环判断上一次落子结果，收集下一次输入，直到棋局出结果
	while (true)
	{
		finalr = go_on;
		if (init_flag&&g_falg)
		{
			now.set_point(g_x - 1, g_y - 1);
			if (finalr == error_)//犯规输入则重新输入
			{
				MessageBox(NULL, L"不能下这里", L"警告", MB_OKCANCEL);
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
			now.set_point(g_x - 1, g_y - 1);//输入
			finalr = bd.judge(now);
			//判断是否违规
			if (finalr == error_)//犯规输入则重新输入
			{
				MessageBox(NULL, L"不能下这里", L"警告", MB_OKCANCEL);
				g_falg = false;
				continue;
			}
			putChress(src, g_x, g_y, black);
			//正确落子后更新棋盘
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
		imshow("五子棋对战", src);
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
		MessageBox(NULL, L"平局", L"游戏结束", MB_OKCANCEL);
		break;
	case black_win:
		MessageBox(NULL, L"恭喜您棋艺高超，战胜了AI", L"游戏结束", MB_OKCANCEL);
		break;
	case white_win:
		MessageBox(NULL, L"电脑获胜，请继续努力提高自己", L"游戏结束", MB_OKCANCEL);
		break;
	case go_on:
	case error_:
	default:
		break;
	}
}

void AI先手()
{
	chessboard bd;
	chess now;
	chess ai_ch;
	result finalr;
	cv::namedWindow("五子棋对战");
	Mat src;
	cvSetMouseCallback("五子棋对战", mousecallback_mat3, (void*)&src);
	src = creatChessBody(g_length);
	ai_ch.set_color(black);//AI执黑先行
	now.set_color(white);
	bd.clear();
	Ai afago(bd, black);
	ai_ch.set_point(g_length / 2, g_length / 2);
	putChress(src, g_length / 2 + 1, g_length / 2 + 1, black);
	bd.update(ai_ch);
	cout << "对局开始，请您落子";
	//循环判断上一次落子结果，收集下一次输入，直到棋局出结果
	while (true)
	{
		finalr = go_on;
		if (g_falg)
		{
			now.set_point(g_x - 1, g_y - 1);//输入
			finalr = bd.judge(now);
			//判断是否违规
			if (finalr == error_)//犯规输入则重新输入
			{
				MessageBox(NULL, L"不能下这里", L"警告", MB_OKCANCEL);
				g_falg = false;
				continue;
			}
			putChress(src, g_x, g_y, white);
			//正确落子后更新棋盘
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
		imshow("五子棋对战", src);
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
		MessageBox(NULL, L"平局", L"游戏结束", MB_OKCANCEL);
		break;
	case white_win:
		MessageBox(NULL, L"恭喜您棋艺高超，战胜了AI", L"游戏结束", MB_OKCANCEL);
		break;
	case black_win:
		MessageBox(NULL, L"电脑获胜，请继续努力提高自己", L"游戏结束", MB_OKCANCEL);
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
		//双人对战();
		AI后手();
		//AI先手();
		g_falg = false;
		system("cls");
		cout << "是否继续下一局？Y/N" << endl;
		char flag;
		cin >> flag;
		if (flag == 'Y' || flag == 'y')
			continue;
		else
			break;
	}
	return 0;
}