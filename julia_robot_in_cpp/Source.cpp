#include "julia_robot_in_cpp.h"
#include <time.h>
#include <conio.h>

Robot rod(1);

HorizonSide change_rotation(HorizonSide rotation, int i) {
	int a = ((int)rotation + i + 4) % 4;
	return (HorizonSide)a;
}

bool border_check() {
	for (int i = 0; i < 4; i++) {
		if (rod.isborder((HorizonSide)i,true)) {
			return true;
		}
	}
	return false;
}

void move_to_1(HorizonSide diraction) {
	while (!rod.isborder(diraction,true)) {
		rod.move(diraction);
		rod.putmarker();
	}
	diraction = change_rotation(diraction,2);
	while (rod.ismarker()) {
		rod.move(diraction);
	}
}
void moving(HorizonSide diraction) {
	if (!rod.isborder(diraction,true)) {
		rod.move(diraction);
		if (!rod.isborder(change_rotation(diraction, 1),true))
			rod.move(change_rotation(diraction, 1));
	}
}
void moving_2(HorizonSide diraction) {
	if (!rod.isborder(diraction, true)) {
		rod.move(diraction);
	}
}

void move_to_2(HorizonSide diraction) {
	int x = 0;
	int y = 0;
	while (!rod.isborder(diraction,true) && !rod.isborder(change_rotation(diraction, 1),true)) {
		moving(diraction);
		rod.putmarker();
		x++; y++;
	}
	diraction = change_rotation(diraction, 2);
	for (int i = 0; i < y; i++) {
		rod.move(diraction);
	}
	diraction = change_rotation(diraction, 1);
	for (int i = 0; i < x; i++) {
		rod.move(diraction);
	}
}

void move_to_3(HorizonSide direction) {
	while(!rod.isborder(direction,true))
		moving_2(direction);
	HorizonSide a = change_rotation(direction, 1);
	while (!rod.isborder(change_rotation(direction, 2), true)) {
		while (!rod.isborder(a, true)) {
			moving_2(a);
			if (border_check()) {
				rod.putmarker();
			}
		}
		a = change_rotation(a, 2);
		moving_2(change_rotation(direction,2));
		if (border_check()) {
			rod.putmarker();
		}
	}
	direction = change_rotation(direction, 2);
	a = change_rotation(direction, 1);
	bool end = false;
	while (!rod.isborder(change_rotation(direction, 2), true) && ! end) {
		while (!rod.isborder(a, true) && ! end) {
			moving_2(a);
			if (!border_check() && rod.ismarker()) {
				rod.putmarker();
				end = true;
			}
		}
		if (!end) {
			a = change_rotation(a, 2);
			moving_2(change_rotation(direction, 2));
		}
	}
}
void zad1() {
	HorizonSide diraction = Nord;
	for (int i = 0; i < 4; i++) {
		move_to_1(diraction);
		diraction = change_rotation(diraction, 1);
	}
	rod.putmarker();
}
void zad3() {
	rod.putmarker();
	int y_pos = 0; int x_pos = 0;
	while (!rod.isborder(Nord, 1)) {
		y_pos++;
		rod.move(Nord);
	}
	while (!rod.isborder(West, 1)) {
		x_pos++;
		rod.move(West);
	}
	HorizonSide direction = Sud;
	while (!rod.isborder(Ost, 1) || !rod.isborder(direction, 1)) {
		rod.putmarker();
		rod.move(direction);
		rod.putmarker();
		if (rod.isborder(direction, 1) && !rod.isborder(Ost, 1)) {
			rod.move(Ost);
			direction = change_rotation(direction, 2);
		}
	}
	while (!rod.isborder(Nord, 1)) {
		rod.move(Nord);
	}
	while (!rod.isborder(West, 1)) {
		rod.move(West);
	}
	for (int i = 0; i < x_pos; i++) {
		rod.move(Ost);
	}
	for (int i = 0; i < y_pos; i++) {
		rod.move(Sud);
	}
}

void zad4() {
	HorizonSide diraction = Nord;
	for (int i = 0; i < 4; i++) {
		move_to_2(diraction);
		diraction = change_rotation(diraction, 1);
	}
	rod.putmarker();
}
void zad5() {
	HorizonSide diraction = Nord;
	rod.putmarker();
	for (int i = 0; i < 4; i++) {
		move_to_3(diraction);
		diraction = change_rotation(diraction, 1);
	}
	rod.putmarker();
}

int main() {
	std::cout << "choose zadanie(1,2,3,4,5)" << std::endl;
	int zad = 0;
	std::cin >> zad;
	switch (zad) {
	case 1:
		zad1();
		break;
	case 2:
		zad5();
		break;
	case 3:
		zad3();
		break;
	case 4:
		zad4();
		break;
	case 5:
		zad5();
		break;
	}
	system("pause");
}