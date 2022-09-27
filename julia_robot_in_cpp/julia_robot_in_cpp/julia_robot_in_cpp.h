﻿#include <iostream>
#include <fstream>
#include <string.h>
#include <Windows.h>


enum Area {
	empty = 0,
	marker
};

struct AREA_DATA {
	Area data = empty;
	bool border_nord = false;
	bool border_ost = false;
	bool border_sud = false;
	bool border_west = false;
};

enum HorizonSide
{
	Nord = 0,
	Ost,
	Sud,
	West
};

struct coordinates {
	int x = 0;
	int y = 0;
};
class Robot {
public:
	bool dead = false;
private:
	coordinates coord;
	int area_x = 3; int area_y = 3;
	AREA_DATA area[100][100];
	void kill() {
		std::cout << "u r dead";
		dead = true;
	}
	coordinates choose(HorizonSide diraction) {
		coordinates res;
		res.x = 0; res.y = 0;
		switch (diraction) {
		case Nord: res.y++; break;
		case Sud:res.y--; break;
		case Ost: res.x++; break;
		case West: res.x--; break;
		}
		return res;
	}
	HorizonSide flip(HorizonSide rotation) {
		int a = ((int)rotation + 2) % 4;
		HorizonSide res = (HorizonSide)a;
		//std::cout << a << " " << res;
		return res;
	}
	void put_border_utilized(int x, int y, HorizonSide diraction) {
		switch (diraction) {
		case Nord: area[x][y].border_nord = true; break;
		case Sud:area[x][y].border_sud = true; break;
		case Ost: area[x][y].border_ost = true; break;
		case West:area[x][y].border_west = true; break;
		}
	}
	void create_borders(int area_x, int area_y, int aa, int bb) {
		for (int i = aa; i < area_x; i++) {
			for (int ii = bb; ii < area_y; ii++) {
				if ((i == aa || i == area_x - 1 && (ii > bb && ii < area_y)) || (ii == bb || ii == area_y - 1 && (i > aa && i < area_x))) {
					for (int iii = 0; iii < 4; iii++) {
						put_border(i, ii, (HorizonSide)iii);
					}
				}
			}
		}
	}
	void create_marker(int x, int y) {
		area[x][y].data = marker;
	}
	HorizonSide denormalise(HorizonSide direction) {
		HorizonSide res;
		switch (direction)
		{
		case Nord:res = West;
			break;
		case Ost: res = Nord;
			break;
		case Sud: res = Ost;
			break;
		case West: res = Sud;
			break;
		}
		return res;
	}
public:
	bool isborder(HorizonSide diraction, bool need_denormalise) {
		if (need_denormalise)
			diraction = denormalise(diraction);
		bool res = false;
		switch (diraction) {
		case Nord: res = area[coord.x][coord.y].border_nord; break;
		case Sud:res = area[coord.x][coord.y].border_sud; break;
		case Ost: res = area[coord.x][coord.y].border_ost; break;
		case West:res = area[coord.x][coord.y].border_west; break;
		}
		return res;
	}
	bool ismarker() {
		bool res = false;
		if (area[coord.x][coord.y].data == marker) {
			res = true;
		}
		return res;
	}
	void move(HorizonSide diraction) {
		diraction = denormalise(diraction);
		if (!dead) {
			if (!isborder(diraction,false)) {
				coordinates a = choose(diraction);
				coord.x += a.x; coord.y += a.y;
			}
			else {
				kill();
			}
		}
		draw();
	}
	void putmarker() {
		if (!dead)
			area[coord.x][coord.y].data = marker;
		draw();
	}
	void put_border(int x, int y, HorizonSide diraction) {
		put_border_utilized(x, y, diraction);
		coordinates utilese = choose(diraction);
		HorizonSide fliped_rot = flip(diraction);
		put_border_utilized(x + utilese.x, y + utilese.y, fliped_rot);
	}
	void draw() {
		if (!dead) {
			system("cls");
			for (int x = 0; x < area_x; x++) {
				std::string f_row; std::string s_row; std::string t_row;
				for (int y = 0; y < area_y; y++) {
					if (area[x][y].border_west) {
						f_row += "###";
					}
					else {
						f_row += "   ";
					}
					if (area[x][y].border_sud) {
						s_row += "|";
					}
					else {
						s_row += " ";
					}
					if (x == coord.x && y == coord.y) {
						s_row += "0";
					}
					else {
						if (area[x][y].data == empty) {
							s_row += ".";
						}
						else {
							s_row += "M";
						}
					}
					if (area[x][y].border_nord) {
						s_row += "|";
					}
					else {
						s_row += " ";
					}
					if (area[x][y].border_ost) {
						t_row += "###";
					}
					else {
						t_row += "   ";
					}
				}
				std::cout << f_row << std::endl << s_row << std::endl << t_row << std::endl;
			}
		}
	}

	Robot(int y) {
		char a = ' ';
		std::cout << "do you want to load previous tschine? (y/n)" << std::endl;
		while(!(a == 'y' || a == 'n'))
		std::cin >> a;
		std::ifstream g("save.txt");
		switch (a) {
		case 'y':
			if (g.is_open()) {
				for (int i = 0; i < 100; i++) {
					for (int ii = 0; ii < 100; ii++) {
						int c;
						g >> area[i][ii].border_nord >> area[i][ii].border_ost >> area[i][ii].border_sud >> area[i][ii].border_west >> c;
						area[i][ii].data = (Area)c;
					}
				}
				g >> coord.x >> coord.y;
				g >> area_x >> area_y;
				draw();
			}
			else {
				std::cout << "\n save error";
			}
			break;
		case 'n':
			std::cout << "before start open full consol" << std::endl << "lets make size of window first" << std::endl << "put size x (from 1 to 60 ) and y (from 1 to 19)" << std::endl;
			int border_x = 0;
			int border_y = 3;
			std::cin >> border_y >> border_x;
			create_borders(border_x + 1, border_y + 1, 1, 1);
			area_x = border_x + 1;
			area_y = border_y + 1;
			int border_pos_x = 0;
			int border_pos_y = 0;
			draw();
			std::cout << "input 0 0 to skip" << std::endl;
			while (!border_x == 0 && !border_y == 0) {
				std::cout << std::endl << "enter obstacle position or input 0 0" << std::endl << "position counts from lift up corner" << std::endl;
				std::cin >> border_pos_y >> border_pos_x;
				std::cout << std::endl << "enter obstacle size like you do before before or input 0 0" << std::endl;
				std::cin >> border_y >> border_x;
				create_borders(border_x + border_pos_x, border_y + border_pos_y, border_pos_x, border_pos_y);
				draw();
			}
			border_pos_x = 1;
			border_pos_y = 1;
			while (!border_pos_x == 0 && !border_pos_y == 0) {
				std::cout << std::endl << "enter marker position or input 0 0" << std::endl << "position counts from lift up corner" << std::endl;
				std::cin >> border_pos_y >> border_pos_x;
				create_marker(border_pos_x, border_pos_y);
				draw();
			}
			std::cout << "input robot start position" << std::endl << "after that you willl start" << std::endl << "put size x (from 2 to 59 ) and y (from 2 to 18)" << std::endl;
			std::cin >> coord.y >> coord.x;
			draw();
			std::cout << std::endl << "\nsavethis tschine? (y/n)\n";
			char b = ' ';
			while (!(b == 'y' || b == 'n')) {
				std::cin >> b;
			}
			if (b == 'y') {
				std::fstream f("save.txt", std::ios_base::out | std::ios_base::trunc);
				if (f.is_open()) {
					for (int i = 0; i < 100; i++) {
						for (int ii = 0; ii < 100; ii++) {
							f << area[i][ii].border_nord << " " << area[i][ii].border_ost << " " << area[i][ii].border_sud << " "<< area[i][ii].border_west << " " <<area[i][ii].data << std::endl;
						}
					}
					f << coord.x << " " << coord.y << std::endl;
					f << area_x << " " << area_y;
					f.close();
				}
				else {
					std::cout << "\n save error";
				}
			}
			break;
		}
		g.close();
	}

};
