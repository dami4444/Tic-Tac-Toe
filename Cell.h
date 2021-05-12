#pragma once
#include <SFML/Graphics.hpp>
#include "Turn.h"

class Cell
{
	int colNum;
	int rowNum;
	int state;
	sf::RectangleShape* shape;
	sf::RenderWindow* window;
	sf::Image* cellTextures;
	sf::Texture texture;
public:
	Cell(int col, int row, sf::RenderWindow* w, sf::Image* cellTextures, sf::Vector2f boardPos);
	void SetPosition(float posX, float posY);
	sf::RectangleShape GetShape();
	void Draw();
	void SetState(int s);
	int GetState();
	int GetCol();
	int GetRow();
	void SetTexture();
	bool isClicked(sf::Vector2f mousePos);
	void onClick(Turn* gameTurn);
	~Cell();


};
