#include "Cell.h"

Cell::Cell(int nrOfCol, int nrOfRow, sf::RenderWindow* w, sf::Image* cellTextures, sf::Vector2f boardPos)
{
	shape = new sf::RectangleShape((sf::Vector2f(25.f, 25.f)));
	//shape->setFillColor(sf::Color::Transparent);

	colNum = nrOfCol;
	rowNum = nrOfRow;

	this->cellTextures = cellTextures;
	SetState(0);

	window = w;
	SetPosition(float(boardPos.x + 17.5f + nrOfCol * 30.f), float(boardPos.y + 17.5f + nrOfRow * 30.f));

}

void Cell::SetPosition(float posX, float posY)
{
	shape->setPosition(posX, posY);
}

sf::RectangleShape Cell::GetShape()
{
	return *shape;
}

void Cell::Draw()
{
	window->draw(*shape);
}

void Cell::SetState(int s)
{
	state = s;
	SetTexture();	//The state of a cell determines what texture it displays
}

int Cell::GetState()
{
	return state;
}

int Cell::GetCol()
{
	return colNum;
}

int Cell::GetRow()
{
	return rowNum;
}

void Cell::SetTexture()
{
	if (state == 0) texture.loadFromImage(*cellTextures, sf::IntRect(1, 0, 25, 25));
	else
	{
		if (state == 1 ) texture.loadFromImage(*cellTextures, sf::IntRect(27, 0, 25, 25));
		if (state == -1 ) texture.loadFromImage(*cellTextures, sf::IntRect(52, 0, 25, 25));
	}

	shape->setTexture(&texture);
}

bool Cell::isClicked(sf::Vector2f mousePos)
{
	if (shape->getGlobalBounds().contains(mousePos)) return true;
	else return false;
}

void Cell::onClick(Turn* gameTurn)
{
	if (state == 0)
	{
		SetState(gameTurn->GetTurn());
		gameTurn->Next();
	}
}

Cell::~Cell()
{
	delete shape;
}


