#include "julia_robot_in_cpp.h"
#include <time.h>
#include <conio.h>


const int razmer_massive = 100;
const int spawn_coords = 50;
Robot rod(1);
enum type_cell {
	NUL = 0,
	EMPTY
};
struct coord {
	int x = 0;
	int y = 0;
};
struct some_border_data {
	int min_y = 0;
	int max_y = 0;
	int min_x = 0;
	int max_x = 0;
};
struct cell_data {
	bool border[4] = { 0 };
	type_cell type = NUL;
	bool need_to_marker = false;
};

HorizonSide change_rotation(HorizonSide rotation, int i);
bool border_check();

some_border_data border_data;
cell_data data [razmer_massive][razmer_massive];
coord cord;
coord spawn;
bool arr[razmer_massive][razmer_massive] = {0};
coord coord_in_move(HorizonSide direction, coord cord) {
	switch (direction)
	{
	case Nord:
		cord.y++;
		break;
	case Ost:
		cord.x++;
		break;
	case Sud:
		cord.y--;
		break;
	case West:
		cord.x--;
		break;
	}
	return cord;
}
void move_in_coord(HorizonSide direction) {
	if (!rod.isborder(direction, 1)) {
		rod.move(direction);
		cord = coord_in_move(direction, cord);
	}
}
void save_data() {
	data[cord.x][cord.y].type = EMPTY;
	for (int i = 0; i < 4; i++) {
		data[cord.x][cord.y].border[i] = rod.isborder((HorizonSide)i, 1);
	}
}
void recurse_for_2_and_5() {
	arr[cord.x][cord.y] = true;
	if (border_check()) {
		rod.putmarker();
	}
	for (int i = 0; i < 4; i++) {
		HorizonSide side = (HorizonSide)i;
		coord a = cord;
		a = coord_in_move(side, a);
		if (!rod.isborder(side, 1) && !arr[a.x][a.y]) {
			move_in_coord(side);
			recurse_for_2_and_5();
			move_in_coord(change_rotation(side, 2));
		}
	}
}
void recurse_get_data() {
	arr[cord.x][cord.y] = true;
	for (int i = 0; i < 4; i++) {
		HorizonSide side = (HorizonSide)i;
		coord a = cord;
		a = coord_in_move(side, a);
		if (!rod.isborder(side, 1) && !arr[a.x][a.y]) {
			move_in_coord(side);
			save_data();
			recurse_get_data();
			move_in_coord(change_rotation(side, 2));
		}
	}
}
void recursive_put_all_markers() {
	arr[cord.x][cord.y] = true;
	if (data[cord.x][cord.y].need_to_marker) {
		rod.putmarker();
	}
	for (int i = 0; i < 4; i++) {
		HorizonSide side = (HorizonSide)i;
		coord a = cord;
		a = coord_in_move(side, a);
		if (!rod.isborder(side, 1) && !arr[a.x][a.y]) {
			move_in_coord(side);
			recursive_put_all_markers();
			move_in_coord(change_rotation(side, 2));
		}
	}
}
void get_data() {
	cord.x = spawn_coords;
	cord.y = spawn_coords;
	HorizonSide direction = Nord;
	save_data();
	recurse_get_data();
	for (int i = 0; i < razmer_massive; i++) {
		for (int ii = 0; ii < razmer_massive; ii++) {
			arr[i][ii] = false;
		}
	}
}
void put_all_markers() {
	recursive_put_all_markers();
}

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
void zad2() {
	get_data();
	for (int x = 1; x < razmer_massive - 1; x++) {
		for (int y = 1; y < razmer_massive - 1; y++) {
			if (data[x][y].type == NUL) {
				for (int i = -1; i < 2; i++) {
					for (int ii = -1; ii < 2; ii++) {
						data[x + i][y + ii].need_to_marker = true;
					}
				}
			}
		}
	}
	put_all_markers();
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
	recurse_for_2_and_5();
}


void find_border_coord() {
	for (int y = 0; y < razmer_massive; y++) {
		bool found = false;
		for (int x = 0; x < razmer_massive; x++) {
			if (data[x][y].type != NUL) {
				border_data.min_y = y;
				found = true;
			}
		}
		if (found) {
			break;
		}
	}
	for (int y = razmer_massive - 1; y >= 0; y--) {
		bool found = false;
		for (int x = 0; x < razmer_massive; x++) {
			if (data[x][y].type != NUL) {
				border_data.max_y = y;
				found = true;
			}
		}
		if (found) {
			break;
		}
	}
	for (int x = 0; x < razmer_massive; x++) {
		bool found = false;
		for (int y = 0; y < razmer_massive; y++) {
			if (data[x][y].type != NUL) {
				border_data.min_x = x;
				found = true;
			}
		}
		if (found) {
			break;
		}
	}
	for (int x = razmer_massive - 1; x >= 0; x--) {
		bool found = false;
		for (int y = 0; y < razmer_massive; y++) {
			if (data[x][y].type != NUL) {
				border_data.max_x = x;
				found = true;
			}
		}
		if (found) {
			break;
		}
	}
}

/*bool is_border(HorizonSide direction, int x, int y) {
	return data[x][y].border[(int)direction];
}

coord move(HorizonSide direction, int x, int y) {
	switch (direction)
	{
	case Nord:
		y++;
		break;
	case Ost:
		x++;
		break;
	case Sud:
		y--;
		break;
	case West:
		x--;
		break;
	}
	coord res;
	res.x = x;
	res.y = y;
	return res;
}

bool check_for_markers() {
	coord ptr_cord;
	ptr_cord.x = border_data.min_x;
	ptr_cord.y = border_data.min_y;
	HorizonSide direction = Ost;
	while (ptr_cord.x != border_data.max_x && !is_border(Sud,ptr_cord.x,ptr_cord.y)) {
		if (!is_border(Sud, ptr_cord.x, ptr_cord.y)) {
			ptr_cord = move(Sud, ptr_cord.x, ptr_cord.y);
		} else
			if (!is_border(Ost, ptr_cord.x, ptr_cord.y)) {
				ptr_cord = move(Ost, ptr_cord.x, ptr_cord.y);
			}

	}
	
	
}*/
void recursive_put_markers(int x, int y, int min_x, int max_x, int min_y, int max_y) {
	data[x][y].need_to_marker = true;
	if (data[x][y].type == NUL) {
		if (x > min_x ) if(!data[x - 1][y].need_to_marker) {
			recursive_put_markers(x - 1, y, min_x, max_x,  min_y,  max_y);
		}
		if (x< max_x - 1) if (!data[x+1][y].need_to_marker) {
			recursive_put_markers(x + 1, y, min_x, max_x, min_y, max_y);
		}
		if (y > min_y) if (!data[x][y-1].need_to_marker) {
			recursive_put_markers(x, y - 1, min_x, max_x, min_y, max_y);
		}
		if (y < max_y - 1) if (!data[x][y+1].need_to_marker) {
			recursive_put_markers(x , y + 1, min_x, max_x, min_y, max_y);
		}
	}
}

void put_marks() {
	recursive_put_markers(border_data.min_x - 1, border_data.min_y - 1, border_data.min_x - 1, border_data.max_x + 2, border_data.min_y - 1, border_data.max_y + 2);
}

void zad6() {
	get_data();
	find_border_coord();
	put_marks();
	put_all_markers();
}

void analize_7() {
	for (int y = border_data.min_y; y <= border_data.max_y; y++) {
		if (data[spawn_coords][y].type == EMPTY) {
			data[spawn_coords][y].need_to_marker = true;
			break;
		}
	}
	for (int y = border_data.max_y; y >= border_data.min_y; y--) {
		if (data[spawn_coords][y].type == EMPTY) {
			data[spawn_coords][y].need_to_marker = true;
			break;
		}
	}
	for (int x = border_data.min_x; x <= border_data.max_x; x++) {
		if (data[x][spawn_coords].type == EMPTY) {
			data[x][spawn_coords].need_to_marker = true;
			break;
		}
	}
	for (int x = border_data.max_x; x >= border_data.min_x; x--) {
		if (data[x][spawn_coords].type == EMPTY) {
			data[x][spawn_coords].need_to_marker = true;
			break;
		}
	}
}
void zad7() {
	get_data();
	find_border_coord();
	analize_7();
	put_all_markers();
}

void try_to_find_hole(int steps_left,HorizonSide direction, bool& found){
	if (steps_left > 0) {
		if (rod.isborder(Nord, 1)) {
			rod.move(direction);
			try_to_find_hole(steps_left - 1, direction, found);
			if (!found) {
				rod.move(change_rotation(direction, 2));
			}
		}
		else {
			found = true;
		}
	}
}

void zad8() {
	int steps_amount = 1;
	bool found = false;
	while (!found) {
		try_to_find_hole(steps_amount, Ost, found);
		try_to_find_hole(steps_amount, West, found);
		steps_amount++;
	}
}

/*void recursive_find(int steps_left, bool& found, bool arr[razmer_massive][razmer_massive]) {
	arr[cord.x][cord.y] = true;
	if (rod.ismarker()) {
		found = true;
	}
	if (steps_left > 0 && !found) {
		for (int i = 0; i < 4; i++) {
			coord a = coord_in_move((HorizonSide)i, cord);
			if (!arr[a.x][a.y]) {
				rod.move((HorizonSide)i);
				recursive_find(steps_left - 1, found, arr);
				rod.move(change_rotation((HorizonSide)i,2));
			}
		}
	}
	arr[cord.x][cord.y] = false;
}
void zad9() {
	int steps_amount = 1;
	bool found = false;
	while (!found) {
		{
			bool bool_arr[razmer_massive][razmer_massive] = { 0 };
			recursive_find(steps_amount, found, bool_arr); 
		}
		steps_amount++;
	}
}*/

void zad9() {
	int step = 1;
	int timer = 0;
	HorizonSide direction = Nord;
	bool found = false;
	while (!found) {
		for (int i = 0; i < step; i++) {
			rod.move(direction);
			if (rod.ismarker()) {
				found = true;
				break;
			}
		}
		direction = change_rotation(direction, 1);
		if (timer == 1) {
			step++;
			timer = 0;
		}
		else {
			timer++;
		}
	}
}

void zad10() {
	get_data();
	for (int i = 0; i < razmer_massive; i++) {
		for (int ii = 0; ii < razmer_massive; ii++) {
			if (i % 2 == ii % 2)
			data[i][ii].need_to_marker = true;
		}
	}
	put_all_markers();
}

void zad11(int n) {
	get_data();
	find_border_coord();
	for (int i = border_data.min_x; i < razmer_massive; i++) {
		for (int ii = border_data.min_y; ii < razmer_massive; ii++) {
			if (((i - border_data.min_x) / n) % 2 == ((ii - border_data.min_y) / n) % 2) {
				data[i][ii].need_to_marker = true;
			}
		}
	}
	put_all_markers();
}

int main() {
	std::cout << "choose zadanie(1,2,3,4,5,6,,7,8,9,10,11)" << std::endl;
	int zad = 0;
	std::cin >> zad;
	switch (zad) {
	default:
		break;
	case 1:
		zad1();
		break;
	case 2:
		zad2();
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
	case 6:
		zad6();
		break;
	case 7:
		zad7();
		break;
	case 8:
		zad8();
		break;
	case 9:
		zad9();
		break;
	case 10:
		zad10();
		break;
	case 11:
		int n = 1;
		std::cout << std::endl << "input n" << std::endl << "N = ";
		std::cin >> n;
		zad11(n);
		break;

	}
	system("pause");
}