#pragma once
#include <SFML/Graphics.hpp>
#include <sstream>

class MainMenu
{
	sf::VideoMode desktopMode;

	int numRows;
	int numCols;
	int gameMode;		//Determines game mode. 0-both players controlled by mouse/ 1- one player controlled by ai, the other by mouse / 2-both players contolled by ai
	int winningStreak;	//Determines how many symbols in a row are needed for a victory.

	sf::Image* icon;
	sf::Texture* buttonTexture;
	sf::Texture* arrowLeftTexture;
	sf::Texture* arrowRightTexture;

	sf::RectangleShape startButton;
	sf::RectangleShape arrowNumRowsLeft;
	sf::RectangleShape arrowNumRowsRight;
	sf::RectangleShape arrowNumColsLeft;
	sf::RectangleShape arrowNumColsRight;
	sf::RectangleShape arrowGameModeLeft;
	sf::RectangleShape arrowGameModeRight;
	sf::RectangleShape arrowWinningStreakLeft;
	sf::RectangleShape arrowWinningStreakRight;
public:
	MainMenu();
	void Display();
	void Draw(sf::RenderWindow& menuWindow);	//Draws the contents of the main menu window
	void DrawText(float posX, float posY, sf::String s, sf::RenderWindow& menuWindow);
	sf::RectangleShape CreateArrow(float posX, float posY, sf::Texture* t);
	bool isButtonClicked(sf::Vector2f mousePos, sf::RectangleShape button);
	void onClick(sf::Vector2f mousePos, sf::RenderWindow& menuWindow);
	~MainMenu();
};