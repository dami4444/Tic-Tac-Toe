#include "Board.h"

Board::Board(int nrOfCols, int nrOfRows, sf::RenderWindow* w, int wS, Turn* t)
{
	state = 0;
	gameTurn = t;

	numCols = nrOfCols;
	numRows = nrOfRows;
	winningStreak = wS;
	float width = 30.f + numCols * 30;
	float height = 30.f + numRows * 30;
	shape = new sf::RectangleShape((sf::Vector2f(width, height)));
	shape->setFillColor(sf::Color(100, 100, 100));
	window = w;

	sf::Vector2u windowSize = window->getSize();
	SetPosition((windowSize.x-width) / 2, (windowSize.y - height) / 2);

	//load cell textures file
	cellTextures = new sf::Image;
	if (!(*cellTextures).loadFromFile("CellTextures.bmp"))
	{
		// error...
		sf::err() << "Error! Cell textures could not be loaded!";
	}

	// Add rows
	for (int i = 0; i < numRows; i++) {
		cellArr.push_back(cellRow());
	}

	// Create cells
	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j < numCols; j++) {
			cellArr[i].push_back(new Cell(j, i, window, cellTextures, shape->getPosition()));
		}
	}

}

void Board::SetWindow(sf::RenderWindow* w)
{
	window = w;
}

void Board::SetPosition(float posX, float posY)
{
	shape->setPosition(posX, posY);
}

void Board::Draw()
{
	window->draw(*shape);

	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j < numCols; j++) {
			cellArr[i][j]->Draw();
		}
	}
}

void Board::onClick()
{
	//const sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
	sf::Vector2f mousePos;
	mousePos.x = sf::Mouse::getPosition(*window).x;
	mousePos.y = sf::Mouse::getPosition(*window).y;


	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j < numCols; j++) {
			if (cellArr[i][j]->isClicked(mousePos))
			{
				cellArr[i][j]->onClick(gameTurn);
			}
		}
	}

	state=CheckState();
	//if(state!=0) EndGame();
}

bool Board::isMovePossible()
{
	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j < numCols; j++) {
			if (cellArr[i][j]->GetState() == 0) return true;
		}
	}
	return false;
}

void Board::DirectionCheckNextIter(int checkingDirection, int* x, int* y)
{
	switch (checkingDirection)
	{
	case 0:
		(*y)--;
		break;
	case 1:
		(*x)++;
		(*y)--;
		break;
	case 2:
		(*x)++;
		break;
	case 3:
		(*y)++;
		(*x)++;
		break;
	case 4:
		(*y)++;
		break;
	case 5:
		(*x)--;
		(*y)++;
		break;
	case 6:
		(*x)--;
		break;
	case 7:
		(*x)--;
		(*y)--;
		break;
	}
}

int Board::CheckState()
{

	int cellState = 0;
	int streakCounter = 0;		//Counts how many O or X are in a row. If it reaches 4 the game ends.
	//int checkingDirection = 0;	//The checkingDirection variable determines the direction of the checked row. Values from 0 to 7 clockwise shifting directions. 0 is up, 1 is up-right, 7 up-left etc.

	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j < numCols; j++) {
			cellState = cellArr[i][j]->GetState();	//Checking the state of every cell
			if (cellState != 0)						//If it's not an empty cell
			{
				for (int checkingDirection = 0; checkingDirection <= 7; checkingDirection++)	//Checking for a row of (winningStreak-1) same state cells in every direction
				{
					int* y = new int(i);
					int* x = new int(j);
					DirectionCheckNextIter(checkingDirection, x, y);	//while preincrementation
					streakCounter = 0;		//setting the streak counter to 0 for each checked direction
					//for ((*x),(*y); (*x >= 0) && (*x <= numCols) && (*y >= 0) && (*y < numRows);DirectionCheckNextIter(checkingDirection,x,y))
					while((*x >= 0) && (*x < numCols) && (*y >= 0) && (*y < numRows))
					{
						if (cellArr[*y][*x]->GetState() == cellState)
						{
							streakCounter++;
							if (streakCounter == this->winningStreak-1) return cellState;	//Returns 1 or -1 if either O or X wins
						}
						else
						{
							//streakCounter = 0;
							break;
						}
						//while increment
						DirectionCheckNextIter(checkingDirection, x, y);	//while preincrementation (for the next iteration)
					}
					delete x, y;
				}
			}
		}
	}
	if (!isMovePossible()) return 2;		//Return 2 if no moves can be made (the game is a draw)
	return 0;	//Return 0 if the board state doesn't change;
	//delete cellState, streakCounter, checkingDirection;
}

int Board::GetState()
{
	return this->state;
}

//
//void Board::EndGame()		//temporary before creating the Game class
//{
//	if (state == 0) return;
//	else
//	{
//		sf::Font font;
//		if (!font.loadFromFile("TimesNewRoman.ttf"))
//		{
//			// error...
//			sf::err() << "Error! Font couldn' be loaded!";
//		}
//
//		sf::Text text;
//		text.setFont(font);
//		text.setCharacterSize(48);
//		switch (state)
//		{
//		case -1:
//			text.setString("X won!");
//			text.setFillColor(sf::Color::Red);
//			break;
//		case 1:
//			text.setString("O won!");
//			text.setFillColor(sf::Color::Blue);
//			break;
//		case 2:
//			text.setString("Draw!");
//			text.setFillColor(sf::Color::Color(180,180,180));
//			break;
//		}
//		sf::FloatRect textRect = text.getLocalBounds();
//		text.setOrigin(textRect.left + textRect.width / 2.0f,textRect.top + textRect.height / 2.0f);
//
//		sf::RenderWindow resultWindow(sf::VideoMode(300, 200), "Game Over!");
//		while (resultWindow.isOpen())
//		{
//			sf::Event event;
//			while (resultWindow.pollEvent(event))
//			{
//				if (event.type == sf::Event::Closed)
//				{
//					resultWindow.close();
//					this->window->close();
//				}
//					
//
//			}
//
//			resultWindow.clear();
//			//text.setPosition(resultWindow.getPosition().x / 2.f, resultWindow.getPosition().y / 2.f);
//			text.setPosition(150.f,100.f);
//			resultWindow.draw(text);
//			resultWindow.display();
//		}
//	}
//}

Board::~Board()
{
	//delete[] cellArray;
	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j < numCols; j++) {
			delete cellArr[i][j];
		}
	}
	cellArr.clear();
	
	delete shape;
	delete cellTextures;
	//delete gameTurn; //It's allocated automaticaly in the game class 
}
