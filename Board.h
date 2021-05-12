#pragma once
#include "Cell.h"
#include "Turn.h"
#include <SFML/Graphics.hpp>
#include <vector>

typedef std::vector<Cell*> cellRow;
typedef std::vector<cellRow> cellArray;

class Board
{
	Turn* gameTurn;			//Points to the gameTurn paramater of the Game class

	cellArray cellArr;
	int numRows;
	int numCols;
	int winningStreak;

	friend class Ai;

	int state;		//Determines the state of the board. 0 - game in progress / 1 - O won / -1 - X won / 2 - draw
	sf::Image* cellTextures;
	sf::RectangleShape* shape;
	sf::RenderWindow* window;
public:
	Board(int cols, int rows, sf::RenderWindow* w, int wS, Turn* t);
	void SetWindow(sf::RenderWindow* w);
	void SetPosition(float posX, float posY);
	void Draw();
	void onClick();
	bool isMovePossible();
	void DirectionCheckNextIter(int checkingDirection, int* x, int* y);
	int CheckState();	//Checks and returns the current state of the board and determines if the game should end
	int GetState();
	~Board();

};