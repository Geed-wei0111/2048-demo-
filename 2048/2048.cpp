#include <iostream>
#include <fstream>
#include <random>
#include <io.h>
#include <graphics.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>

IMAGE _0, _2, _4, _8, _16, _32, _64, _128, _256, _512, _1024, _2048;
int temparr[4][4];
enum { COLUMN, ROW };
class g_2048
{
public:
	g_2048()
	{
		memset(data, 0, sizeof(data));
		int x, y;
		int t_x = 0, t_y = 0;
		int count = 0;
		srand(time(NULL));
		while (count != 2)
		{
			x = rand() % 4;
			y = rand() % 4;
			if (x != t_x && y != t_y)
			{
				data[x][y] = 2;
				count++;
				t_x = x;
				t_y = y;
			}
			if (count == 1 && x == t_x && y == t_y)
			{
				continue;
			}
		}
	}
	void display()
	{
		int i, j;
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
			{
				if (data[i][j] == 0)
					putimage(j * 60, i * 60, &_0);
				if (data[i][j] == 2)
					putimage(j * 60, i * 60, &_2);
				if (data[i][j] == 4)
					putimage(j * 60, i * 60, &_4);
				if (data[i][j] == 8)
					putimage(j * 60, i * 60, &_8);
				if (data[i][j] == 16)
					putimage(j * 60, i * 60, &_16);
				if (data[i][j] == 32)
					putimage(j * 60, i * 60, &_32);
				if (data[i][j] == 64)
					putimage(j * 60, i * 60, &_64);
				if (data[i][j] == 128)
					putimage(j * 60, i * 60, &_128);
				if (data[i][j] == 256)
					putimage(j * 60, i * 60, &_256);
				if (data[i][j] == 512)
					putimage(j * 60, i * 60, &_512);
				if (data[i][j] == 1024)
					putimage(j * 60, i * 60, &_1024);
				if (data[i][j] == 2048)
					putimage(j * 60, i * 60, &_2048);
			}
		}
	}
	void play()
	{
		char ch;
		while (true)
		{
			this->display();
			this->check();
			ch = _getch();
			switch (ch)
			{
			case 'w': this->savetemp(); this->move_up();break;
			case 's':this->savetemp(); this->move_down(); break;
			case 'a': this->savetemp();this->move_left(); break;
			case 'd': this->savetemp();this->move_right(); break;
			case 'r':this->loadtemp(); break;
			case 27:
				savemap();
				closegraph();
				return;
			default:break;
			}
		}
	}
	void check()
	{
		int i, j;
		int x, y;
		int include_0 = 0;
		int dead = 0;
		int win = 0;
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
			{
				if (data[i][j] == 2048)
					win = 1;
			}
		}
		if (win == 1)
		{
			HWND hwnd = GetHWnd();
			MessageBox(hwnd, "恭喜你获得胜利", "You Win", MB_OK);
			remove("./pics/map.txt");
			closegraph();
			exit(0);
		}
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
			{
				if (data[i][j] == 0)
				{
					include_0 = 1;
				}
			}
		}
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 3; j++)
			{
				if (data[i][j] == data[i][j + 1])
					break;
			}
			if (j != 3)
				if (data[i][j] == data[i][j + 1])
					break;
		}
		for (x = 0; x < 4; x++)
		{
			for (y = 0; y < 3; y++)
			{
				if (data[y + 1][x] == data[y][x])
					break;
			}
			if (y != 3)
				if (data[y + 1][x] == data[y][x])
					break;
		}
		if (i == 4 && j == 3 && x == 4 && y == 3)
		{
			dead = 1;
		}
		if (dead == 1 && include_0 != 1)
		{
			HWND hwnd = GetHWnd();
			MessageBox(hwnd, "抱歉，游戏结束", "You Lose!", MB_OK);
			remove("./pics/map.txt");
			closegraph();
			exit(0);
		}
	}
	int count_nums(int pos, int FLAG)
	{
		int i;
		int temp = 0;
		for (i = 0; i < 4; i++)
		{
			if (FLAG == COLUMN)
			{
				if (data[i][pos] != 0)
				{
					temp++;
				}
			}
			if (FLAG == ROW)
			{
				if (data[pos][i] != 0)
				{
					temp++;
				}
			}
		}
		return temp;
	}
	void savemap()
	{
		std::ofstream out;
		out.open("./pics/map.txt");
		for (int i=0;i<4;i++)
			for (int j = 0; j < 4; j++)
			{
				out << data[i][j] << ' ';
			}
		out.close();
		return;
	}
	void initmap()
	{
		_finddata_t fileInfo;
		long HANDLE;
		if ((HANDLE = _findfirst("./pics/map.txt", &fileInfo) != -1L))
		{
			int i, j;
			_findclose(HANDLE);
			std::ifstream in;
			in.open("./pics/map.txt");
			for (i = 0; i < 4; i++)
				for (j = 0; j < 4; j++)
				{
					in >> data[i][j];
				}
			in.close();
		}
		else
			return;
	}
	void savetemp()
	{
		for (int i=0;i<4;i++)
			for (int j = 0; j < 4; j++)
			{
				temparr[i][j] = data[i][j];
			}
	}
	void loadtemp()
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
			{
				data[i][j] = temparr[i][j];
			}
	}
	void move_up()
	{
		int i;
		int process = 0;
		int flag = 0;
		int flag2 = 0;
		int change = 0;
		while (process != 4)
		{
			flag = this->count_nums(process, COLUMN);
			switch (flag)
			{
			case 0:change++; break;
			case 1:
				//如果只有一个就直接移动到第一格
				if (data[0][process] != 0)
				{
					change++;
					break;
				}
				for (i = 1; i < 4; i++)
				{
					if (data[i][process] != 0)
					{
						data[0][process] = data[i][process];
						data[i][process] = 0;
						break;
					}
				}
				break;
			case 2:	
				//先把两个放到第一第二格
				for (i = 0; i < 2; i++)
				{
					if (data[i][process] != 0)
					{
						flag2++;
					}
					else
						break;
				}
				if (flag2 == 2 && data[0][process] != data[1][process])
				{
					change++;
					flag2 = 0;
					break;
				}
				i = flag2;
				while (flag2 != 2 && i <= 3)
				{
					if (data[i][process] != 0)
					{
						data[flag2][process] = data[i][process];
						data[i][process] = 0;
						flag2++;
					}
					i++;
				}
				//如果这两个相等就合并
				if (data[0][process] == data[1][process])
				{
					data[0][process] += data[1][process];
					data[1][process] = 0;
				}
				flag2 = 0;
				break;

			case 3:
				//先把三个数放在最前面
				for (i = 0; i < 3; i++)
				{
					if (data[i][process] != 0)
					{
						flag2++;
					}
					else
						break;
				}
				if (flag2 == 3 && data[0][process] != data[1][process] && data[1][process] != data[2][process])
				{
					change++;
					flag2 = 0;
					break;
				}
				i = flag2;
				while (flag2 != 3 && i <= 3)
				{
					if (data[i][process] != 0)
					{
						data[flag2][process] = data[i][process];
						data[i][process] = 0;
						flag2++;
					}
					i++;
				}
				//如果有相同的就合并
				if (data[0][process] == data[1][process])
				{
					data[0][process] += data[1][process];
					data[1][process] = data[2][process];
					data[2][process] = 0;
				}
				else if (data[1][process] == data[2][process])
				{
					data[1][process] += data[2][process];
					data[2][process] = 0;
				}
				flag2 = 0;
				break;
			case 4:
				if (data[0][process] != data[1][process] && data[1][process] != data[2][process] && data[2][process] != data[3][process])
				{
					change++;
					flag2 = 0;
					break;
				}
				if (data[0][process] == data[1][process] && data[2][process] == data[3][process])
				{//如果第一第二相等并且第二第三也相等
					data[0][process] += data[1][process];
					data[1][process] = data[2][process] * 2;
					data[2][process] = data[3][process] = 0;
				}
				else if (data[0][process] == data[1][process] && data[2][process] != data[3][process])
				{//如果只有第一第二相等
					data[0][process] += data[1][process];
					for (i = 1; i < 2; i++)
					{
						data[i][process] = data[i + 1][process];
					}
					data[3][process] = 0;
				}
				else if (data[1][process] == data[2][process])
				{//如果只有第二第三相等
					data[1][process] += data[2][process];
					data[2][process] = data[3][process];
					data[3][process] = 0;
				}
				else if (data[2][process] == data[3][process] && data[0][process] != data[1][process])
				{//如果只有第三第四相等
					data[2][process] += data[3][process];
					data[3][process] = 0;
				}
				flag2 = 0;
				break;
			default:
				break;
			}
			process++;
		}
		if (change == 4)
		{
			return;
		}
		//生成新的数字添加到图里
		int x, y;
		while (true)
		{
			x = rand() % 4;
			y = rand() % 4;
			if (data[x][y] != 0)
			{
				continue;
			}
			else
			{
				data[x][y] = 2;
				break;
			}
		}
	}
	void move_down()
	{
		int i;
		int process = 0;
		int flag = 0;
		int flag2 = 0;
		int change = 0;
		while (process != 4)
		{
			flag = this->count_nums(process, COLUMN);
			switch (flag)
			{
			case 0:change++; break;
			case 1:
				if (data[3][process] != 0)
				{
					change++;
					break;
				}
				//如果只有一个就直接移动到第一格
				for (i = 2; i >= 0; i--)
				{
					if (data[i][process] != 0)
					{
						data[3][process] = data[i][process];
						data[i][process] = 0;
						break;
					}
				}
				break;
			case 2:
				//先把两个放到第一第二格
				for (i = 3; i >= 2; i--)
				{
					if (data[i][process] != 0)
					{
						flag2++;
					}
					else
						break;
				}
				if (flag2 == 2 && data[3][process] != data[2][process])
				{
					change++;
					flag2 = 0;
					break;
				}
				i = 3 - flag2;
				while (flag2 != 2 && i >= 0)
				{
					if (data[i][process] != 0)
					{
						data[3 - flag2][process] = data[i][process];
						data[i][process] = 0;
						flag2++;
					}
					i--;
				}
				//如果这两个相等就合并
				if (data[2][process] == data[3][process])
				{
					data[3][process] += data[2][process];
					data[2][process] = 0;
				}
				flag2 = 0;
				break;

			case 3:
				//先把三个数放在最前面
				for (i = 3; i >= 1; i--)
				{
					if (data[i][process] != 0)
					{
						flag2++;
					}
					else
						break;
				}
				if (flag2 == 3 && data[3][process] != data[2][process] && data[2][process] != data[1][process])
				{
					change++;
					flag2 = 0;
					break;
				}
				i = 3 - flag2;
				while (flag2 != 3 && i >= 0)
				{
					if (data[i][process] != 0)
					{
						data[3 - flag2][process] = data[i][process];
						data[i][process] = 0;
						flag2++;
					}
					i--;
				}
				//如果有相同的就合并
				if (data[3][process] == data[2][process])
				{
					data[3][process] += data[2][process];
					data[2][process] = data[1][process];
					data[1][process] = 0;
				}
				else if (data[2][process] == data[1][process])
				{
					data[2][process] += data[1][process];
					data[1][process] = 0;
				}
				flag2 = 0;
				break;
			case 4:
				if (data[0][process] != data[1][process] && data[1][process] != data[2][process] && data[2][process] != data[3][process])
				{
					change++;
					flag2 = 0;
					break;
				}
				if (data[0][process] == data[1][process] && data[2][process] == data[3][process])
				{//如果第一第二相等并且第二第三也相等
					data[3][process] += data[2][process];
					data[2][process] = data[1][process] * 2;
					data[0][process] = data[1][process] = 0;
				}
				else if (data[2][process] == data[3][process] && data[0][process] != data[1][process])
				{//如果只有第三第四相等
					data[3][process] += data[2][process];
					data[2][process] = data[1][process];
					data[1][process] = data[0][process];
					data[0][process] = 0;
				}
				else if (data[1][process] == data[2][process])
				{//如果只有第二第三相等
					data[2][process] += data[1][process];
					data[1][process] = data[0][process];
					data[0][process] = 0;
				}
				else if (data[0][process] == data[1][process] && data[2][process] != data[3][process])
				{//如果只有第一第二相等
					data[1][process] += data[0][process];
					data[0][process] = 0;
				}
				flag2 = 0;
				break;
			default:
				break;
			}
			process++;
		}
		if (change == 4)
			return;
		//生成新的数字添加到图里
		int x, y;
		while (true)
		{
			x = rand() % 4;
			y = rand() % 4;
			if (data[x][y] != 0)
			{
				continue;
			}
			else
			{
				data[x][y] = 2;
				break;
			}
		}
	}
	void move_left()
	{
		int i;
		int process = 0;
		int flag = 0;
		int flag2 = 0;
		int change = 0;
		while (process != 4)
		{
			flag = this->count_nums(process, ROW);
			switch (flag)
			{
			case 0:change++; break;
			case 1:
				if (data[process][0] != 0)
				{
					change++;
					break;
				}
				for (i = 1; i < 4; i++)
				{
					if (data[process][i] != 0)
					{
						data[process][0] = data[process][i];
						data[process][i] = 0;
						break;
					}
				}
				break;
			case 2:
				for (i = 0; i < 4; i++)
				{
					if (data[process][i] == 0)
						break;
					if (data[process][i] != 0)
						flag2++;
				}
				if (flag2 == 2 && data[process][0] != data[process][1])
				{
					change++;
					flag2 = 0;
					break;
				}
				i = flag2;
				while (flag2 != 2 && i <= 3)
				{
					if (data[process][i] != 0)
					{
						data[process][flag2] = data[process][i];
						data[process][i] = 0;
						flag2++;
					}
					i++;
				}
				if (data[process][0] == data[process][1])
				{
					data[process][0] += data[process][1];
					data[process][1] = 0;
				}
				flag2 = 0;
				break;
			case 3:
				for (i = 0; i < 4; i++)
				{
					if (data[process][i] == 0)
						break;
					if (data[process][i] != 0)
						flag2++;
				}
				if (flag2 == 3 && data[process][0] != data[process][1] && data[process][1] != data[process][2])
				{
					change++;
					flag2 = 0;
					break;
				}
				i = flag2;
				while (flag2 != 3 && i <= 3)
				{
					if (data[process][i] != 0)
					{
						data[process][flag2] = data[process][i];
						data[process][i] = 0;
						flag2++;
					}
					i++;
				}
				if (data[process][0] == data[process][1])
				{
					data[process][0] += data[process][1];
					data[process][1] = data[process][2];
					data[process][2] = 0;
				}
				else if (data[process][1] == data[process][2])
				{
					data[process][1] += data[process][2];
					data[process][2] = 0;
				}
				flag2 = 0;
				break;
			case 4:
				if (data[process][0] != data[process][1] && data[process][1] != data[process][2] && data[process][2] != data[process][3])
				{
					change++;
					flag2 = 0;
					break;
				}
				if (data[process][0] == data[process][1] && data[process][2] == data[process][3])
				{
					data[process][0] += data[process][1];
					data[process][1] = data[process][2] * 2;
					data[process][2] = data[process][3] = 0;
				}
				else if (data[process][0] == data[process][1] && data[process][2] != data[process][3])
				{
					data[process][0] += data[process][1];
					data[process][1] = data[process][2];
					data[process][2] = data[process][3];
					data[process][3] = 0;
				}
				else if (data[process][1] == data[process][2])
				{
					data[process][1] += data[process][2];
					data[process][2] = data[process][3];
					data[process][3] = 0;
				}
				else if (data[process][2] == data[process][3] && data[process][0] != data[process][1])
				{
					data[process][2] += data[process][3];
					data[process][3] = 0;
				}
				flag2 = 0;
				break;
			}
			process++;
		}
		if (change == 4)
			return;
		//生成新的数字添加到图里
		int x, y;
		while (true)
		{
			x = rand() % 4;
			y = rand() % 4;
			if (data[x][y] != 0)
			{
				continue;
			}
			else
			{
				data[x][y] = 2;
				break;
			}
		}
	}
	void move_right()
	{
		int i;
		int process = 0;
		int flag = 0;
		int flag2 = 0;
		int change = 0;
		while (process != 4)
		{
			flag = this->count_nums(process, ROW);
			switch (flag)
			{
			case 0:change++; break;
			case 1:
				if (data[process][3] != 0)
				{
					change++;
					break;
				}
				for (i = 2; i >= 0; i--)
				{
					if (data[process][i] != 0)
					{
						data[process][3] = data[process][i];
						data[process][i] = 0;
					}
				}
				break;
			case 2:
				for (i = 3; i >= 0; i--)
				{
					if (data[process][i] == 0)
						break;
					if (data[process][i] != 0)
						flag2++;
				}
				if (flag2 == 2 && data[process][3] != data[process][2])
				{
					change++;
					flag2 = 0;
					break;
				}
				i = 3 - flag2;
				while (flag2 != 2 && i >= 0)
				{
					if (data[process][i] != 0)
					{
						data[process][3 - flag2] = data[process][i];
						data[process][i] = 0;
						flag2++;
					}
					i--;
				}
				if (data[process][2] == data[process][3])
				{
					data[process][3] += data[process][2];
					data[process][2] = 0;
				}
				flag2 = 0;
				break;
			case 3:
				for (i = 3; i >= 0; i--)
				{
					if (data[process][i] == 0)
						break;
					if (data[process][i] != 0)
						flag2++;
				}
				if (flag2 == 3 && data[process][3] != data[process][2] && data[process][2] != data[process][1])
				{
					change++;
					flag2 = 0;
					break;
				}
				i = 3 - flag2;
				while (flag2 != 3 && i >= 0)
				{
					if (data[process][i] != 0)
					{
						data[process][3 - flag2] = data[process][i];
						data[process][i] = 0;
						flag2++;
					}
					i--;
				}
				if (data[process][3] == data[process][2])
				{
					data[process][3] += data[process][2];
					data[process][2] = data[process][1];
					data[process][1] = 0;
				}
				else if (data[process][2] == data[process][1])
				{
					data[process][2] += data[process][1];
					data[process][1] = 0;
				}
				flag2 = 0;
				break;
			case 4:
				if (data[process][3] != data[process][2] && data[process][2] != data[process][1] && data[process][1] != data[process][0])
				{
					change++;
					flag2 = 0;
					break;
				}
				if (data[process][0] == data[process][1] && data[process][2] == data[process][3])
				{
					data[process][3] += data[process][2];
					data[process][2] = data[process][1] * 2;
					data[process][1] = data[process][0] = 0;
				}
				else if (data[process][3] == data[process][2] && data[process][1] != data[process][0])
				{
					data[process][3] += data[process][2];
					data[process][2] = data[process][1];
					data[process][1] = data[process][0];
					data[process][0] = 0;
				}
				else if (data[process][1] == data[process][2])
				{
					data[process][2] += data[process][1];
					data[process][1] = data[process][0];
					data[process][0] = 0;
				}
				else if (data[process][0] == data[process][1] && data[process][2] != data[process][3])
				{
					data[process][1] += data[process][0];
					data[process][0] = 0;
				}
				flag2 = 0;
				break;
			}
			process++;
		}
		if (change == 4)
			return;
		//生成新的数字添加到图里
		int x, y;
		while (true)
		{
			x = rand() % 4;
			y = rand() % 4;
			if (data[x][y] != 0)
			{
				continue;
			}
			else
			{
				data[x][y] = 2;
				break;
			}
		}
	}
private:
	int data[4][4];
};

int main()
{
	getimage(&_2048, 0, 0, 60, 60);
	loadimage(&_0, "./pics/0.png");
	loadimage(&_2, "./pics/2.png");
	loadimage(&_4, "./pics/4.png");
	loadimage(&_8, "./pics/8.png");
	loadimage(&_16, "./pics/16.png");
	loadimage(&_32, "./pics/32.png");
	loadimage(&_64, "./pics/64.png");
	loadimage(&_128, "./pics/128.png");
	loadimage(&_256, "./pics/256.png");
	loadimage(&_512, "./pics/512.png");
	loadimage(&_1024, "./pics/1024.png");
	initgraph(240, 240);
	setbkmode(TRANSPARENT);
	g_2048 game;
	game.initmap();
	game.play();
	return 0;
}