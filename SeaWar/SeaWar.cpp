// SeaWar.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <cstring>
#include <conio.h>
#include <windows.h> 
#include <cstdlib>
using namespace std;

struct ship
{
	int l;
	int v = 1;
	int x1 = 0, x2 = 0;
	int y1 = 0, y2 = 0;
};

ship p1[10];

int me, opponent;
char myboard[12][12], board[12][12], comp_board[12][12];
bool game_over;
int shot_x = 5, shot_y = 5;
char comand;
char past_board;
bool popal;

bool start_game = 0;
int cursor = 1;
int dif = 1;
//две переменные для логики компа уровня выше
char mas_comp_logic[12][12];
int comp_check_complete = 0;
int comp_x_last = 0, comp_y_last = 0;

char mas1[12][12];

void Set();
void Draw();
void Logic();

void comp_logic();

void Set_sea(char matr[12][12]);
void put_ships(char matr[12][12]);
void show_sea(char matr[12][12]);
void put_ships_computer(char matr[12][12]);
void put_ship_computer_gor(char matr[12][12], int x, int y, int i);
void put_ship_computer_vert(char matr[12][12], int x, int y, int i);
void range_gor(char matr[12][12], int x, int y, int &k);
void range_vert(char matr[12][12], int x, int y, int &k);
void preset_board();
void set_board();
void fire_board();
void perevod();

void my_menu(int &cursor);
void button_menu(int &cursor, int &dif, bool &start_game);

void dif_menu(int &dif);
void botton_dif(int &dif);

void set_ship(char mas[12][12], ship p1[10]);


int main()
{

	Set();
	while (!game_over) {
		Logic();
		Draw();
	}

	system("pause");
	return 0;
}

void Set() {
	game_over = false;
	Set_sea(myboard);
	Set_sea(board);
	Set_sea(comp_board);
	p1[0].l = 4;
	p1[1].l = 3; p1[2].l = 3;
	p1[3].l = 2; p1[4].l = 2; p1[5].l = 2;
	p1[6].l = 1; p1[7].l = 1; p1[8].l = 1; p1[9].l = 1;

	Set_sea(mas1);
	my_menu(cursor);
	button_menu(cursor, dif, start_game);
	system("cls"); Sleep(1000);

	set_ship(mas1, p1);
	put_ships_computer(comp_board);
	perevod();

	me = 20;
	opponent = 20;
}

void Draw() {
	system("cls");
	show_sea(myboard);
	cout << endl << endl;
	show_sea(board);
	cout << endl;
	cout << "You ships: " << me << "         //  press 'w' 'a' 's' 'd' for move  // " << endl;
	cout << "Opponent ships: " << opponent << "  //  press 'e' for fire!!! press 'x' for EXIT //" << endl;
	if (opponent == 0) {
		system("cls");
		system("cls"); 	HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD cd;
		cd.X = 40;
		cd.Y = 15;
		SetConsoleCursorPosition(hd, cd);
		cout << "You Win!!!" << endl;
		game_over = true;
	}
	if (me == 0) {
		system("cls");
		system("cls"); 	HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD cd;
		cd.X = 40;
		cd.Y = 15;
		SetConsoleCursorPosition(hd, cd);
		cout << "Opponent Win" << endl;
		game_over = true;
	}
}


void Logic() {
	do {
		popal = false;
		comand = ' ';
		if (_kbhit()) {
			comand = _getch();
			switch (comand)
			{
			case 'w':
				if (shot_x > 1) {
					preset_board();
					shot_x--;
					set_board();
				}
				break;
			case 's':
				if (shot_x < 10) {
					preset_board();
					shot_x++;
					set_board();
				}
				break;
			case 'a':
				if (shot_y > 1) {
					preset_board();
					shot_y--;
					set_board();
				}
				break;
			case 'd':
				if (shot_y < 10) {
					preset_board();
					shot_y++;
					set_board();
				}
				break;
			case 'e':
				fire_board();
				break;
			case 'x':
				game_over = 1;
				return;
				break;
			default:
				break;
			}
			Draw();
		}
		Sleep(100);
	} while (comand != 'e' || popal);
	comp_logic();
}

void preset_board() {
	board[shot_x][shot_y] = past_board;
}

void set_board() {
	past_board = board[shot_x][shot_y];
	if (board[shot_x][shot_y] != 'X')       //исправлен баг Васи, благодаря которому модно было выйграть компьютер после нахождение его первой клетки, просто стреляя по 1 месту
		board[shot_x][shot_y] = 'E';
}

void fire_board() {
	if (comp_board[shot_x][shot_y] == '0') {
		if (board[shot_x][shot_y] != 'X')
			opponent--;
		board[shot_x][shot_y] = 'X';
		past_board = 'X';
		popal = true;
	}
	else {
		board[shot_x][shot_y] = '*';
		past_board = '*';
	}
}

void Set_sea(char matr[12][12]) {
	matr[0][0] = ' ';
	for (int j = 1; j < 11; ++j) {
		matr[0][j] = (char)(47 + j);
	}
	matr[0][11] = ' ';
	for (int i = 1; i < 11; ++i) {
		for (int j = 0; j < 12; ++j) {
			if (j == 0 || j == 11) {
				matr[i][j] = (char)(47 + i);
			}
			else {
				matr[i][j] = ' ';
			}
		}
	}
	matr[11][0] = ' ';
	for (int j = 1; j < 11; ++j) {
		matr[11][j] = (char)(47 + j);
	}
	matr[11][11] = ' ';
}

void put_ships(char matr[12][12]) {
	system("cls");
	int x, y;
	bool approve;
	char comand;
	int number_ships[4] = { 4,3,2,1 };
	for (int i = 0; i < 20; ++i) {
		approve = true;
		while (approve) {
			system("cls");
			approve = false;
			cout << "Enter x coordinate: ";
			cin >> x;
			cout << "Enter y coordinate: ";
			cin >> y;
			while (x < 0 || x>10 || y < 0 || y>10) {
				cout << "Enter x coordinate: ";
				cin >> x;
				cout << "Enter y coordinate: ";
				cin >> y;
			}
			matr[y + 1][x + 1] = '0';
			show_sea(matr);
			cout << "If you want to change press E: ";
			cin >> comand;
			if (comand == 'e') {
				approve = true;
				matr[y + 1][x + 1] = ' ';
			}
			else if (comand == 'E') {
				approve = true;
				matr[y + 1][x + 1] = ' ';
			}
		}
	}
}


void put_ships_computer(char matr[12][12]) {
	char mess_matr[12][12];
	Set_sea(mess_matr);
	int x, y, k, naprav;
	int number_ships[4] = { 4,3,2,1 };
	int i = 4;
	while (number_ships[0] != 0) {
		do {
			x = rand() % 11 + 1;
			y = rand() % 11 + 1;
			naprav = rand() % 2;
			if (naprav == 1) {
				range_gor(mess_matr, x, y, k);
			}
			else
			{
				range_vert(mess_matr, x, y, k);
			}
		} while (i >= k);
		if (naprav == 1) {
			put_ship_computer_gor(mess_matr, x, y, i);
		}
		else {
			put_ship_computer_vert(mess_matr, x, y, i);
		}
		number_ships[i - 1]--;
		if (number_ships[i - 1] == 0) {
			i--;
		}
	}
	for (int i = 0; i < 12; ++i) {
		for (int j = 0; j < 12; ++j) {
			if (mess_matr[i][j] == '0') {
				matr[i][j] = '0';
			}
		}
	}
}

void put_ship_computer_gor(char matr[12][12], int x, int y, int i) {
	if (matr[x][y + 1] == ' ') {
		matr[x][y + 1] = '*';
	}
	if (matr[x + 1][y + 1] == ' ') {
		matr[x + 1][y + 1] = '*';
	}
	if (matr[x - 1][y + 1] == ' ') {
		matr[x - 1][y + 1] = '*';
	}
	while (i != 0) {
		matr[x][y] = '0';
		if (matr[x - 1][y] == ' ') {
			matr[x - 1][y] = '*';
		}
		if (matr[x + 1][y] == ' ') {
			matr[x + 1][y] = '*';
		}
		y--;
		i--;
	}
	if (matr[x][y] == ' ') {
		matr[x][y] = '*';
	}
	if (matr[x - 1][y] == ' ') {
		matr[x - 1][y] = '*';
	}
	if (matr[x + 1][y] == ' ') {
		matr[x + 1][y] = '*';
	}
}

void put_ship_computer_vert(char matr[12][12], int x, int y, int i) {
	if (matr[x + 1][y] == ' ') {
		matr[x + 1][y] = '*';
	}
	if (matr[x + 1][y + 1] == ' ') {
		matr[x + 1][y + 1] = '*';
	}
	if (matr[x + 1][y - 1] == ' ') {
		matr[x + 1][y - 1] = '*';
	}
	while (i != 0) {
		matr[x][y] = '0';
		if (matr[x][y + 1] == ' ') {
			matr[x][y + 1] = '*';
		}
		if (matr[x][y - 1] == ' ') {
			matr[x][y - 1] = '*';
		}
		x--;
		i--;
	}
	if (matr[x][y] == ' ') {
		matr[x][y] = '*';
	}
	if (matr[x][y + 1] == ' ')
	{
		matr[x][y + 1] = '*';
	}
	if (matr[x][y - 1] == ' ') {
		matr[x][y - 1] = '*';
	}
}

void range_gor(char matr[12][12], int x, int y, int &k) {
	k = 0;
	char symvol;
	symvol = matr[x][y];
	while (symvol == ' ') {
		k++;
		y--;
		symvol = matr[x][y];
	}
}

void range_vert(char matr[12][12], int x, int y, int &k) {
	k = 0;
	char symvol;
	symvol = matr[x][y];
	while (symvol == ' ') {
		k++;
		x--;
		symvol = matr[x][y];
	}
}

void show_sea(char matr[12][12]) {
	for (int i = 0; i < 12; ++i) {
		for (int j = 0; j < 12; ++j) {
			cout << matr[i][j] << ' ';
		}
		cout << endl;
	}
}

void comp_logic() {
	bool comp_popal = true;
	int comp_x = rand() % 10 + 1;
	int comp_y = rand() % 10 + 1;
	while (comp_popal) {
		comp_popal = false;
		while (!(myboard[comp_y][comp_x] == ' ' || myboard[comp_y][comp_x] == '0' || myboard[comp_y][comp_x] == '*')) {
			comp_x = rand() % 10 + 1;
			comp_y = rand() % 10 + 1;
		}
		if (myboard[comp_y][comp_x] == ' ') {
			myboard[comp_y][comp_x] = '*';
		}
		if (myboard[comp_y][comp_x] == '0') {
			myboard[comp_y][comp_x] = 'X';
			me--;
			comp_popal = true;
		}
	}
}

void button_menu(int &cursor, int &dif, bool &start_game)
{
	setlocale(0, "");
	char button;
	while (true)
	{
		if (_kbhit()) {
			button = _getch();

			if ((button == 'w' || button == 'W') && (cursor < 5 && cursor > 1)) {
				cursor--;
				my_menu(cursor);
			}
			else if ((button == 's' || button == 'S') && (cursor < 4 && cursor > 0)) {
				cursor++;
				my_menu(cursor);
			}
			if (button == 't' || button == 't')
			{
				if (cursor == 1)
				{
					dif_menu(dif);
					botton_dif(dif); if (start_game)
					{
						system("cls");
						int i = 0;
						for (i; i < 11; i++)
						{
							HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
							COORD cd;
							cd.X = 30;
							cd.Y = 15;
							SetConsoleCursorPosition(hd, cd);

							cout << "\tgame is starting";
							for (int j = 0; j < i; j++)
								cout << "...";
							cout << i * 10 << "%";
							Sleep(100);
							system("cls");
						}


						return;
					}
				}

			}

		}
		Sleep(150);
	}

}

void my_menu(int &cursor)
{
	system("cls");
	for (int i = 0; i < 6; ++i)
	{
		char right, left;
		if (i == cursor)
			right = '>', left = '<';
		else  right = ' ', left = ' ';
		switch (i)
		{
		case 0: cout << "________________________\n"; break;
		case 1:cout << right << " new game " << left << endl; break;
		case 2:cout << right << "    2     " << left << endl; break;
		case 3:cout << right << "    3     " << left << endl; break;
		case 4:cout << right << "    4     " << left << endl; break;
		case 5: cout << "________________________\n"; break;
		}
		cout << endl;
	}
	cout << "cursor=" << cursor << " " << "w-up; s-down; t-enter;" << endl;

}

void dif_menu(int &dif)
{
	system("cls");
	for (int i = 0; i < 5; ++i)
	{
		char right, left;
		if (i == dif)
			right = '>', left = '<';
		else  right = ' ', left = ' ';
		switch (i)
		{
		case 0: cout << "________________________\n"; break;
		case 1:cout << right << "  easy  " << left << endl; break;
		case 2:cout << right << " medium " << left << "- not use" << endl; break;
		case 3:cout << right << "  hard  " << left << "- not use" << endl; break;
		case 4: cout << "________________________\n"; break;
		}
		cout << endl;
	}
	cout << "cursor=" << dif << " " << "w-up; s-down; t-enter;" << endl;

}

void botton_dif(int &dif)
{
	while (true)
	{
		char button;
		if (_kbhit()) {
			button = _getch();

			if ((button == 'w' || button == 'W') && (dif < 4 && dif > 1)) {
				dif--;
				dif_menu(dif);
			}
			else if ((button == 's' || button == 'S') && (dif < 3 && dif > 0)) {
				dif++;
				dif_menu(dif);
			}
			if (button == 't' || button == 'T')
			{
				start_game = 1; return;
			}

		}
		Sleep(150);
	}
}


void set_ship(char mas[12][12], ship p1[10])
{
	{
		system("cls"); 	HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD cd;
		cd.X = 30;
		cd.Y = 15;
		SetConsoleCursorPosition(hd, cd);
		cout << "\t\t\tset your ships"; Sleep(2000);
		system("cls");

	}
	for (int i = 0; i < 10; i++)																////поменять 1 на 10
	{
		int x = 1, y = 1;

		//постановка координат на уловии
		//-------------------------------------------------

		//-------------------------------------------------
		//Постановка кораблей в числовой массив на основе x1x2 y1y2;

		bool set = 0;//переменная постановки корабля клавишами

		show_sea(mas1);

		while (!set)
		{

			char button;
			if (_kbhit())
			{
				if (p1[i].v == 1)
				{
					for (int g = p1[i].y1; g <= p1[i].y2; g++)
						for (int j = p1[i].x1; j <= p1[i].x2; j++)
						{
							if (j == p1[i].x1 + 1 && g > p1[i].y1 && g < p1[i].y2)
								if (mas[g][j] == 'o')
									mas[g][j] = ' ';
						}
				}
				else
				{
					for (int g = p1[i].y1; g <= p1[i].y2; g++)
						for (int j = p1[i].x1; j <= p1[i].x2; j++)
						{
							if (g == p1[i].y1 + 1 && j > p1[i].x1 && j < p1[i].x2)
								if (mas[g][j] == 'o')
									mas[g][j] = ' ';
						}
				}


				button = _getch();
				switch (button)
				{
				case 'w':
					if (y > 1) {
						y--;
					}
					break;
					//вывод сетки кораблей----------------
				case 's':
					if (p1[i].v == 1) {
						if ((y + p1[i].l) < 11)
						{
							y++;
						}
					}
					else
					{
						if (y < 10)
						{
							y++;
						}
					}
					break;
					//вывод сетки кораблей----------------
				case 'a':
					if (x > 1)
					{
						x--;
					}
					break;
					//вывод сетки кораблей----------------
				case 'd':
					if (p1[i].v == 2)
					{
						if ((x + p1[i].l) < 10)
						{
							x++;
						}
					}
					else
					{
						if (x < 10)
						{
							x++;
						}
					}
					break;
					//вывод сетки кораблей----------------
				case 'r':
					if (p1[i].v == 1 && ((x + p1[i].l) < 12))
						p1[i].v = 2; //поменяли ориентацию
					else if (p1[i].v == 2 && ((y + p1[i].l) < 12))
						p1[i].v = 1;
					//идет проверка на свободное пространство

					//вывод сетки кораблей----------------

					break;
				case 't':
				{
					set = 1;
					if (p1[i].v == 1)
					{
						for (int g = p1[i].y1; g <= p1[i].y2; g++)
							for (int j = p1[i].x1; j <= p1[i].x2; j++)
							{
								if (j == p1[i].x1 + 1 && g > p1[i].y1 && g < p1[i].y2)
								{
									mas[0][1] = '1'; mas[1][0] = '1';					 //иначе нельзя, пришлось прибегнуть к хитростям из-за парной разработки кода
									if (mas[g][j] == '0')
										set = 0;
									mas[0][1] = '0'; mas[1][0] = '0';					 //0 которые есть в массиве не давали поставить корабль на 1-1
								}
								else
								{
									mas[0][1] = '1'; mas[1][0] = '1';
									if (mas[g][j] == '0')
										set = 0;
									mas[0][1] = '0'; mas[1][0] = '0';
								}
							}
					}
					else
					{
						for (int g = p1[i].y1; g <= p1[i].y2; g++)
							for (int j = p1[i].x1; j <= p1[i].x2; j++)
							{
								if (g == p1[i].y1 + 1 && j > p1[i].x1 && j < p1[i].x2)
								{
									mas[0][1] = '1'; mas[1][0] = '1'; mas[11][1] = '1'; mas[1][11] = '1';
									if (mas[g][j] == '0')
										set = 0;
									mas[0][1] = '0'; mas[1][0] = '0'; mas[11][1] = '0'; mas[1][11] = '0';
								}
								else
								{
									mas[0][1] = '1'; mas[1][0] = '1'; mas[11][1] = '1'; mas[1][11] = '1';
									if (mas[g][j] == '0')
										set = 0;
									mas[0][1] = '0'; mas[1][0] = '0'; mas[11][1] = '0'; mas[1][11] = '0';
								}
							}
					}




				}

				}
				//-------------------------------------------------
				p1[i].x1 = x - 1; p1[i].y1 = y - 1;
				if (p1[i].v == 1) { p1[i].x2 = x + 1; p1[i].y2 = y + p1[i].l; } //button_ship в каждом условии case
				else { p1[i].x2 = x + p1[i].l; p1[i].y2 = y + 1; }
				//-------------------------------------------------
				if (p1[i].v == 1)
				{
					for (int g = p1[i].y1; g <= p1[i].y2; g++)
						for (int j = p1[i].x1; j <= p1[i].x2; j++)
						{
							if (j == p1[i].x1 + 1 && g > p1[i].y1 && g < p1[i].y2)
							{
								if (set)
									mas[g][j] = '0';
								else
									if (mas[g][j] != '0')
									{
										cout << mas[g][j] << endl;
										mas[g][j] = 'o';
									}
							}
						}
				}
				else
				{
					for (int g = p1[i].y1; g <= p1[i].y2; g++)
						for (int j = p1[i].x1; j <= p1[i].x2; j++)
						{
							if (g == p1[i].y1 + 1 && j > p1[i].x1 && j < p1[i].x2)
							{
								if (set)
									mas[g][j] = '0';
								else
									if (mas[g][j] != '0')
									{
										cout << mas[g][j] << endl;
										mas[g][j] = 'o';
									}
							}
						}
				}
				system("cls");
				show_sea(mas1);
			}
			Sleep(10);
		}
	}
	cout << endl << "                                                    WAITING FOR START";
	Sleep(4000);
}

void perevod() {
	for (int i = 0; i < 12; ++i) {
		for (int j = 0; j < 12; ++j) {
			myboard[i][j] = mas1[i][j];
		}
	}
}


