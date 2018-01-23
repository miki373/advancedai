#include <iostream>
#include <math.h>
#include <random>
#include <time.h>
#include "container.h"
using namespace std;


/*
T = unknown tile
x = x vector
y = y vector
ax, ay = current tile
bx, by = next state to compare tile
*/

// TO CHANGE NUMBER OF TILES, CHANGE APROPTIATE FIELDS
#define NUM_TILES 9
#define NUM_ROWS 3
#define NUM_TILES_IN_ROW 3 
// DO NOT CHANGE THERE VALUES
#define XY 2			// number of coordinates, x and y
#define X 0				// position of x coordinate
#define Y 1				// position of y coordinate
#define CLEAR 0			// hardcoded blank
#define MAX_MOVES 4		// max number of moves 4: up down left right
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

// user entered unsolved puzzle
int puzzle[NUM_TILES][XY];

// valid moves calculate for current position of
// blank tile
int moves[MAX_MOVES];

void print(Container this_contaier);
bool is_on(int T, int x, int y, Container this_contaier);
bool clear(int x, int y, Container this_contaier);
bool adj(int ax, int ay, int bx, int by);
void find_moves(int T, Container this_contaier);
bool move(int T, int ax, int ay, int bx, int by);

int main()
{
	Container cont;
	LL *temp;
	
	
	cont.insert("clear", 0, 1, 1);
	cont.insert("on", 1, 0, 0);
	cont.insert("on", 2, 1, 0);
	cont.insert("on", 3, 2, 0);
	cont.insert("on", 4, 2, 1);
	cont.insert("on", 5, 2, 2);
	cont.insert("on", 6, 1, 2);
	cont.insert("on", 7, 0, 2);
	cont.insert("on", 8, 0, 1);

	find_moves(6, cont);

	for (int i = 0; i < MAX_MOVES; i++)
	{
		cout << moves[i] << endl;
	}

	print(cont);
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

// returns true or false if T is on x and y
bool is_on(int T, int x, int y, Container this_contaier)
{
	LL * temp_struct;
	temp_struct = this_contaier.find(T);
	if (temp_struct->x == x && temp_struct->y == y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// checks if value on x,y is 0
bool is_clear(int x, int y, Container this_contaier)
{
	LL * temp_struct;
	temp_struct = this_contaier.find("clear",0);
	if (temp_struct->x == x && temp_struct->y == y)
	{
		return true;
	}
	else
	{
		return false;
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

	// clear moves array
	for (int i = 0; i < MAX_MOVES; i++)
	{
		moves[i] = 0;
	}
	
	if (status == "on")
	{
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