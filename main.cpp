#include <iostream>
#include <math.h>
#include <random>
#include <time.h>
#include <stdlib.h>
#include "container.h"
using namespace std;


/*
T = unknown tile
x = x vector
y = y vector
ax, ay = current tile
bx, by = next state to compare tile
0 = UP
1 = DOWN
2 = LEFT
3 = RIGHT
*/

// TO CHANGE NUMBER OF TILES, CHANGE APPROPRIATE FIELDS
#define NUM_ROWS 3
#define NUM_TILES_IN_ROW 3 
#define NUM_TILES NUM_ROWS * NUM_TILES_IN_ROW



// DO NOT CHANGE THESE VALUES
#define XY 2			// number of coordinates, x and y
#define X 0				// position of x coordinate
#define Y 1				// position of y coordinate
#define CLEAR 0			// hardcoded blank tile
#define MAX_MOVES 4		// max number of moves 4: up/down/left/right
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

// puzzle arrays
// solved[TILE NUMBER][COORDINATE_VECTOR]
int solved[NUM_TILES][XY] = {
	{1,1}, // 0
	{0,0}, // 1
	{1,0}, // 2
	{2,0}, // 3
	{2,1},	// 4
	{2,2}, // 5
	{1,2},	// 6
	{0,2},	// 7
	{0,1}	// 8

};


// valid moves calculate for current position of
// blank tile
int moves[MAX_MOVES];


void print(Container this_contaier);

bool adj(int ax, int ay, int bx, int by);

void find_moves(int T, Container this_contaier);

void move(int T, int ax, int ay, int bx, int by, Container &this_contaier);

void get_user_puzzle(Container &this_contaier);

int choose_path(int T, Container this_contaier);

int manhattan(int T, int x, int y, Container this_contaier);

bool puzzle_solved(Container this_container);

void solve_puzzle(Container this_container);

void resolve_path_coordinates(int direction, int T, int &ax, int &ay, int &bx, int &by, Container this_container);


int main()
{
	Container cont;
	LL *temp;
	int path, ax, ay, bx, by;


	get_user_puzzle(cont);
	
	print(cont);

	while (!puzzle_solved(cont))
	{
		find_moves(0, cont);
		path = choose_path(0, cont);
		resolve_path_coordinates(path, 0, ax, ay, bx, by, cont);
		move(0, ax, ay, bx, by, cont);
		cout << endl;
		print(cont);	
	}
	
	
	

	
	return 0;
}

// prits puzzle in 3x3 form
// adapted to LL from 2D array
// tested and works ok
void print(Container this_contaier)
{
	
	LL * temp_struct;			// temporary pointer to struct holding info
	int temp_arr[NUM_TILES];	// to lienar array
	int col_pos;				// position in column 
	int x, y;					// temporary x y coordinates
	for (int i = 0; i < NUM_TILES; i++)
	{
		temp_struct = this_contaier.find(i);
		x = temp_struct->x;
		y = temp_struct->y;
		// to linearize formula is y_coordinate * number_of_rown + x_coordinate 
		// X + (NUM_ROW * Y) 
		// EXAMPLE 
		// 8 is (0,1) in vector form. To linear: 0 + (1 * 3) = 3
		col_pos = (y * NUM_ROWS) + x;		
		temp_arr[col_pos] = i;
		
	}
	for (int i = 0; i < NUM_TILES; i++)
	{
		// if i is greater then 0 and is 3 or 6 print endl
		if ((i > 0) && (i % NUM_TILES_IN_ROW == 0))
		{
			cout << endl;
		}
		
		cout << temp_arr[i];
	}
}


// check if ax and ay are adjacent to bx and by
bool adj(int ax, int ay, int bx, int by)
{
	/* 
	8 tile valid move rules:
	Move at most one position
	on diagonal (x) or vertical (y) axis.
	Algorithm:
	1. difX holds absolute value of difference between first x and second. difY is same as X but for Y
	2. if at least one of difX or difY is a zero then we can deduce that we are only moving in one direction ie. moving vertical or horizontal
	3. if difrence difX or didY is exactly 1 we know that the move is exactly 1 tile away. 
	4. If inner if statment is true, move is valid. Else moves are invalid. 
	*/
	int difX, difY;
	difX = abs((ax - bx));
	difY = abs((ay - by));
	if (difX == 0 || difY == 0)
	{
		if (difX == 1 || difY == 1)
		{
			return true;
		}
	}

	return false;
}

// find moves available to tile T
// clears and populates global moves array
void find_moves(int T, Container this_contaier)
{

	int x, y;
	string status;
	LL * temp_struct;
	temp_struct = this_contaier.find(T);
	x = temp_struct->x;
	y = temp_struct->y;
	status = temp_struct->status;

	// reset the moves golobal array. 
	// make all elements 0
	for (int i = 0; i < MAX_MOVES; i++)
	{
		moves[i] = 0;
	}
	// NON CLEAR tile test
	// will set 
	if (status == "on")
	{
		// find where clear tile is located
		LL * temp_blank = this_contaier.find("clear",0);
		int blank_x = temp_blank->x;
		int blank_y = temp_blank->y;
		if (adj(x, y, blank_x, blank_y))
		{
			if (x > blank_x)
			{
				moves[LEFT] = 1;
			}
			else if (x < blank_x)
			{
				moves[RIGHT] = 1;
			}
			else if (y > blank_y)
			{
				moves[UP] = 1;
			}
			else
			{
				moves[DOWN] = 1;
			}
		}


	}
	// CLEAR TILE test
	// tests where clear tile is located. 
	// IF center (1,1) then moves: UP, DOWN, LEFT, RIGHT
	// IF endge & middle (0,1),(1,0),(1,2)... then moves: LEFT, RIGHT, DOWN/UP
	// IF edge (0,0),(0,2),(2,0) ... then moves: LEFT/RIGHT, DOWN/UP
	else
	{
		// begin testing moves
		if (y > 0)
		{
			// OK to go up
			moves[UP] = 1;
		}
		if (y < NUM_ROWS - 1)	// NUM_ROWS is absolute number of rows, need to sub 1 because computer science and reasons
		{
			// OK to go down
			moves[DOWN] = 1;
		}
		if (x < NUM_TILES_IN_ROW - 1)	// same as NUM_ROWS
		{
			// OK to go right
			moves[RIGHT] = 1;
		}
		if (x > 0)
		{
			// OK to go left
			moves[LEFT] = 1;
		}
		// end test moves
	}
}

// gets user to input puzzle
void get_user_puzzle(Container &this_contaier)
{
	int x, y, tile_num;
	string status;

	cout << "Enter puzzle:\nFormat (x,y) = tile number" << endl;
	for (int i = 0; i < NUM_TILES; i++)
	{
		x = i % 3;
		y = i / 3;
		cout << '(' << x << ',' << y << ") : ";
		cin >> tile_num;
		if (tile_num == 0)
		{
			status = "clear";
		}
		else
		{
			status = "on";
		}
		this_contaier.insert(status,tile_num,x,y);
	}
	cout << "Data accepted..." << endl;
}

int choose_path(int T, Container this_contaier)
{
	int direction = 0; 
	int curr_distance = 0;
	static int last_direction = INT_MAX;
	int shortest_distance = INT_MAX;
	LL * temp;
	int x, y;
	temp = this_contaier.find(T);
	x = temp->x;
	y = temp->y;
	srand(time(NULL));


	if (moves[UP] && last_direction != DOWN)
	{
		curr_distance = manhattan(T, x, y - 1, this_contaier);
		if (curr_distance < shortest_distance)
		{
			shortest_distance = curr_distance;
			direction = UP;
			
		}
	}
	if (moves[DOWN] && last_direction != UP)
	{
		curr_distance = manhattan(T, x, y + 1, this_contaier);
		if (curr_distance < shortest_distance)
		{
			shortest_distance = curr_distance;
			direction = DOWN;
		}
	}
	if (moves[RIGHT] && last_direction != LEFT)
	{
		curr_distance = manhattan(T, x + 1, y, this_contaier);
		if (curr_distance < shortest_distance)
		{
			shortest_distance = curr_distance;
			direction = RIGHT;
		}
	}
	if (moves[LEFT] && last_direction != RIGHT)
	{
		curr_distance = manhattan(T, x - 1, y, this_contaier);
		if (curr_distance < shortest_distance)
		{
			shortest_distance = curr_distance;
			direction = LEFT;
		}
	}
	last_direction = direction;

	return direction;
}

int manhattan(int T, int x, int y, Container this_contaier)
{
	int tempX, tempY, difX, difY, solvedX, solvedY, unsolvedX, unsolvedY;
	int absoluteDif = 0;
	LL * temp;
	LL * current_position = this_contaier.find(T);
	LL * destination_position = this_contaier.find(x, y);

	/*
	cout << endl;
	cout << "(" << x << "," << y << ")" << endl;
	cout << "Current tile" << current_position->tile << "(" << current_position->x << current_position->y << ")" << endl;
	cout << "Dest tile " << destination_position->tile << "(" << destination_position->x << destination_position->y << ")" << endl;
	cout << endl;
	*/

	// SWAP
	tempX = current_position->x;
	tempY = current_position->y;
	current_position->x = destination_position->x;
	current_position->y = destination_position->y;
	destination_position->x = tempX;
	destination_position->y = tempY;
	// END SWAP
	

	cout << endl;
	for (int i = 0; i < NUM_TILES; i++)
	{
		temp = this_contaier.find(i);
		unsolvedX = temp->x;
		unsolvedY = temp->y;
		solvedX = solved[i][X];
		solvedY = solved[i][Y];
		difX = abs((solvedX - unsolvedX));
		difY = abs((solvedY - unsolvedY));
		absoluteDif += difX + difY;
		/*
		cout << "Unsolved " << i << " " << "(" << unsolvedX << "," << unsolvedY << ")" << endl;
		cout << "Snsolved " << i << " " << "(" << solvedX << "," << solvedY << ")" << endl;
		cout << absoluteDif;
		cout << endl;
		*/
	}

	// SWAP BACK 
	tempX = current_position->x;
	tempY = current_position->y;
	current_position->x = destination_position->x;
	current_position->y = destination_position->y;
	destination_position->x = tempX;
	destination_position->y = tempY;
	// END SWAP
	
	return absoluteDif;
}

bool puzzle_solved(Container this_container)
{
	LL * temp;
	int solvedX, solvedY, currX, currY;

	for (int i = 0; i < NUM_TILES; i++)
	{
		temp = this_container.find(i);
		currX = temp->x;
		currY = temp->y;
		solvedX = solved[i][X];
		solvedY = solved[i][Y];
		if ((solvedX != currX) || (solvedY != currY))
		{
			return false;
		}
	}
	
	return true;
}

void resolve_path_coordinates(int direction,int T, int &ax, int &ay, int &bx, int &by, Container this_container)
{
	LL* current = this_container.find(T);
	ax = current->x;
	ay = current->y;
	

	if (direction == UP)
	{
		bx = ax;
		by = ay - 1;
	}
	else if (direction == DOWN)
	{
		bx = ax;
		by = ay + 1;
	}
	else if (direction == LEFT)
	{
		bx = ax - 1;
		by = ay;
	}
	else if (direction == RIGHT)
	{
		bx = ax + 1;
		by = ay;
	}
	else
	{
		exit(1);
	}
}

void move(int T, int ax, int ay, int bx, int by, Container &this_contaier)
{
	LL * temp;
	int destT;
	
	temp = this_contaier.find(bx, by);
	destT = temp->tile;
	// DELETE
	this_contaier.del(T);
	this_contaier.del(destT);
	
	// INSERT
	this_contaier.insert("clear", 0, bx, by);
	this_contaier.insert("on", destT, ax, ay);
	

}

