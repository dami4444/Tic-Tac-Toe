#pragma once
#include <stdlib.h>

class Turn
{
	int turn;	//Determines the turn. 1 - O turn / -1 - X turn. Chosed randomly in Constructor
public:
	Turn();
	void Next();
	int GetTurn();
};