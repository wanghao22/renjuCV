#include "renju.h"

position chess::drop_point()const
{
	return point;
}

//����Ƿ����ʤ������
int check_five(chessboard bd, chess ch)
{
	position ori = ch.drop_point();
	int count = 1;//��������ͳ��ͬɫ����
	int sum[4] = { 0 };
	bool locked = 0;//�߼����������������Ƿ������˷�ͬɫ�ڵ�
					//ˮƽ������
	for (int i = 0, locked = 0; i<5 && ((ori.x - i) >= 0) && (!locked); i++)//��ֹѭ��������ͬɫ����4�����������̱߽��������ͬɫ�ڵ�
		if (ch.get_color() == bd.viewboard(ori.x - i, ori.y))
			count++;
		else
			if (i != 0)
				locked = 1;
	//sum[0]=count-1;
	for (int i = 0, locked = 0; i<5 && ((ori.x + i) <= g_length - 1) && (!locked); i++)//��ֹѭ��������ͬɫ����4�����������̱߽��������ͬɫ�ڵ�
		if (ch.get_color() == bd.viewboard(ori.x + i, ori.y))
			count++;
		else
			if (i != 0)
				locked = 1;
	//sum[1]=count-sum[0]-2;
	sum[0] = count;
	if (count >= 5)
		return count;
	//��ֱ������
	count = 1;
	for (int i = 0, locked = 0; i<5 && ((ori.y - i) >= 0) && (!locked); i++)//��ֹѭ��������ͬɫ����4�����������̱߽��������ͬɫ�ڵ�
		if (ch.get_color() == bd.viewboard(ori.x, ori.y - i))
			count++;
		else
			if (i != 0)
				locked = 1;
	//sum[2]=count-1;
	for (int i = 0, locked = 0; i<5 && ((ori.y + i) <= g_length - 1) && (!locked); i++)//��ֹѭ��������ͬɫ����4�����������̱߽��������ͬɫ�ڵ�
		if (ch.get_color() == bd.viewboard(ori.x, ori.y + i))
			count++;
		else
			if (i != 0)
				locked = 1;
	//sum[3]=count-sum[2]-2;
	sum[1] = count;
	if (count >= 5)
		return count;
	//���ϵ�����б����
	count = 1;
	for (int i = 0, locked = 0; i<5 && ((ori.y - i) >= 0) && ((ori.x - i) >= 0) && (!locked); i++)//��ֹѭ��������ͬɫ����4�����������̱߽��������ͬɫ�ڵ�
		if (ch.get_color() == bd.viewboard(ori.x - i, ori.y - i))
			count++;
		else
			if (i != 0)
				locked = 1;
	//sum[4]=count-1;
	for (int i = 0, locked = 0; i<5 && ((ori.x + i) <= g_length - 1) && ((ori.y + i) <= g_length - 1) && (!locked); i++)//��ֹѭ��������ͬɫ����4�����������̱߽��������ͬɫ�ڵ�
		if (ch.get_color() == bd.viewboard(ori.x + i, ori.y + i))
			count++;
		else
			if (i != 0)
				locked = 1;
	//sum[5]=count-sum[4]-2;
	sum[2] = count;
	if (count >= 5)
		return count;
	//���µ�����б����
	count = 1;
	for (int i = 0, locked = 0; i<5 && ((ori.y + i) <= g_length - 1) && ((ori.x - i) >= 0) && (!locked); i++)//��ֹѭ��������ͬɫ����4�����������̱߽��������ͬɫ�ڵ�
		if (ch.get_color() == bd.viewboard(ori.x - i, ori.y + i))
			count++;
		else
			if (i != 0)
				locked = 1;
	//sum[6]=count-1;
	for (int i = 0, locked = 0; i<5 && ((ori.x + i) <= g_length - 1) && ((ori.y - i) >= 0) && (!locked); i++)//��ֹѭ��������ͬɫ����4�����������̱߽��������ͬɫ�ڵ�
		if (ch.get_color() == bd.viewboard(ori.x + i, ori.y - i))
			count++;
		else
			if (i != 0)
				locked = 1;
	//sum[7]=count-sum[6]-2;
	sum[3] = count;
	if (count >= 5)
		return count;
	return _MAX(sum, 4);
}

void chess::set_point()
{
	if (color == black)
	{
		setpos(110, 1);
		cout << "��ڷ����������������������ո������";
		cin >> point.x >> point.y;
		while (point.x<1 || point.x>g_length)
		{
			setpos(110, 1);
			cout << "�������볬����Χ������������1~"<< g_length <<"֮������      ";
			cin >> point.x;
		}
		while (point.y<1 || point.y>g_length)
		{
			setpos(110, 2);
			cout << "�������볬����Χ������������1~" << g_length << "֮������    ";
			cin >> point.y;
		}
	}
	else if (color == white)
	{
		setpos(110, 1);
		cout << "��׷����������������������ո������";
		cin >> point.x >> point.y;
		while (point.x<1 || point.x>g_length)
		{
			setpos(110, 1);
			cout << "�������볬����Χ������������1~" << g_length << "֮������      ";
			cin >> point.x;
		}
		while (point.y<1 || point.y>g_length)
		{
			setpos(110, 2);
			cout << "�������볬����Χ������������1~" << g_length << "֮������    ";
			cin >> point.y;
		}
	}
	point.x--;
	point.y--;
}

state chessboard::viewboard(position p_c) const
{
	return square[p_c.x][p_c.y];
}

state chessboard::viewboard(int x, int y) const
{
	return square[x][y];
}
void chessboard::update(chess ch)
{
	position pos = ch.drop_point();
	square[pos.x][pos.y] = ch.get_color();
}


void chessboard::display()const
{
	system("cls");


	for (int i = 0; i<g_length; i++)//��ӡ������˵��
	{
		gps(0, 6 + i*hor, i + 1);
	}
	for (int i = 0; i<g_length; i++)//��ӡ������˵��
	{
		gps(16 * ver, 6 + i*hor, i + 1);
	}
	for (int i = 0; i<g_length; i++)//��ӡ������˵��
	{
		gps(3 + i*ver, 1, i + 1);
	}
	for (int i = 0; i<g_length; i++)//��ӡ������˵��
	{
		gps(3 + i*ver, 1 + 16 * hor, i + 1);
	}
	for (int i = 0, j = 0; i<g_length; i++)
	{
		for (j = 0; j<g_length; j++)
			tab(1 + i*ver, 3 + hor*j, square[j][i]);
	}
	cout << endl;
}

result chessboard::judge(chess set)const
{
	bool full = 1;
	if (viewboard(set.drop_point()) != blank)
	{
		return error_;
	}
	int x = 0, y = 0;
	set.get_point(x, y);
	if (x < 0 || x >= g_length || y < 0 || y >= g_length)
		return error_;


	if (check_five(*this, set) >= 5 && (set.get_color() == black))
		return black_win;


	if (check_five(*this, set) >= 5 && (set.get_color() == white))
		return white_win;


	for (int i = 0; i < g_length; i++)
	{
		for (int j = 0; j < g_length; j++)
		{
			if (square[i][j] == blank)
				full = 0;
		}
	}
	if (full == 1)
		return draw;
	else
		return go_on;
}