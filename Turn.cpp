#include "Turn.h"

Turn::Turn()
{
	if (rand() % 2 == 1) turn = 1;
	else turn = -1;
}

void Turn::Next()
{
	turn *= -1;
}

int Turn::GetTurn()
{
	return turn;
}
