#ifndef _CONTAINER_H_
#define _CONTAINER_H_
#include <string>
using namespace std;
struct LL
{
	string status;
	int tile;
	int x;
	int y;
	LL *next;
};

class Container
{
private:
	LL *head;
	LL *curr;
	LL *temp;
	int num_elements;
public:
	Container();
	int get_num_elements();
	void insert(string, int, int, int);
	bool del(int);
	void print();
	LL * find(string, int);
	LL * find(int);
	LL * find(int, int);

};




#endif