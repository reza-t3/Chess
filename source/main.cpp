#include "Board.h"

#include <iostream>
using namespace std;

int main()
{
	Board b;
	string s;
	b.setBoard();
	while (b.playGame())
        continue;
	return 0;
}