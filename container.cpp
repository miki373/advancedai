#include "container.h"

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

Container::Container()
{
	head = NULL;
	curr = NULL;
	temp = NULL;
	num_elements = 0;
}


void Container::insert(string status, int tile, int x, int y)
{	
	LL* newNode = new LL;
	newNode->status = status;
	newNode->tile = tile;
	newNode->x = x;
	newNode->y = y;
	newNode->next = NULL;
	num_elements++;

	if (head == NULL)
	{
		head = newNode;
	}
	else
	{
		curr = head;
		temp = NULL;
		while (curr != NULL)
		{
			temp = curr;
			curr = curr->next;
		}
		if (temp == NULL)
		{
			head = newNode;
			newNode->next = curr;
		}
		else if (curr == NULL)
		{
			temp->next = newNode;
		}
		else
		{
			temp->next = newNode;
			newNode->next = curr;
		}

	}
}

bool Container::del(int tile)
{
	LL* delTile;
	curr = head;
	bool found = false;
	temp = NULL;

	if (head->tile == tile)
	{
		delTile = head;
		head = head->next;
		found = true;
	}
	else
	{
		while (curr != NULL && curr->tile != tile)
		{
			temp = curr;
			curr = curr->next;
		}

		if (curr == NULL)
		{
			return false;
		}
		else
		{
			temp->next = curr->next;
			delTile = curr;
			found = true;
		}
	}

	if (found)
	{
		delete delTile;
	}





	/*
	while (curr != NULL && curr->tile != tile)
	{
		temp = curr;
		curr = curr->next;
	}
	if (temp == NULL)
	{
		delTile = head;
		head = head->next;
		delete delTile;
		num_elements--;
		return true;
	}
	else if (curr == NULL)
	{
		return false;
	}
	else
	{
		delTile = curr;
		curr = curr->next;
		temp->next = curr->next;
		delete delTile;
		num_elements--;
		return true;
	}
	temp = curr = delTile = NULL;
	*/
}

LL * Container::find(int tile)
{
	curr = head;
	while (curr != NULL && curr->tile != tile)
	{
		curr = curr->next;
	}
	if (curr != NULL)
	{
		return curr;
	}
	else
	{
		return NULL; 
	}
}

LL * Container::find(int x, int y)
{
	curr = head;

	while (curr != NULL)
	{
		if ((curr->x == x) && (curr->y == y))
		{
			break;
		}
		curr = curr->next;
	}
	if (curr != NULL)
	{
		return curr;
	}
	else
	{
		return NULL;
	}
}

LL* Container::find(string status, int tile)
{
	curr = head;
	while (curr != NULL && curr->status != status && curr->tile != tile)
	{
		curr = curr->next;
	}
	if (curr != NULL)
	{
		return curr;
	}
	else
		return NULL;
}

int Container::get_num_elements()
{
	return num_elements;
}

// FOR DEBUG ONLY
// prints every element
void Container::print()
{
	for (curr = head; curr != NULL; curr = curr->next)
	{
		cout << "Status: " << curr->status << " ,Tile: " << curr->tile << " ,X: " << curr->x <<" Y: " <<curr->y << endl;
	}

}