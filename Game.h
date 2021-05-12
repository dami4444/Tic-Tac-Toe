#pragma once
#include "Board.h"
#include "Turn.h"
#include "Ai.h"
//#include "MainMenu.h"
//#define NDEBUG
//#include <cassert>

class Game
{
	Turn gameTurn;			//Determines the turn. 1 - O turn / -1 - X turn. Chosed randomly in the Turn class constructor

	int numRows;
	int numCols;
	int gameMode;		//Determines game mode. 0-both players controlled by mouse/ 1- one player controlled by ai, the other by mouse / 2-both players contolled by ai
	int winningStreak;	//Determines how many symbols in a row are needed for a victory.

	sf::Image* icon;
	sf::RenderWindow* window;
	sf::RenderWindow* menuWindow;
	Board* board;
public:
	Game(int gameMode, int numRows, int numCols, int winningStreak, sf::RenderWindow* menuW);
	~Game();
	Board GetBoard();
	void EndGame(int boardState);
	void Start();
};