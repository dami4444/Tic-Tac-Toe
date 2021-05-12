#include "Game.h"

Game::Game(int gameMode, int numRows, int numCols, int winningStreak, sf::RenderWindow* menuW)
{
	this->numRows = numRows;
	this->numCols = numCols;
	this->gameMode = gameMode;
	this->winningStreak = winningStreak;
    
	icon = new sf::Image;
	if (!(*icon).loadFromFile("icon.png"))
	{
		sf::err() << "Error! Window icon could not be loaded!";
	}

	menuWindow = menuW;
    window = nullptr;
    board = nullptr;
}

Game::~Game()
{
	delete board;
    delete window;
	delete icon;
}

void Game::Start()
{
    window = new sf::RenderWindow (sf::VideoMode((numCols +1)*30, (numRows + 1) * 30), "Tic Tac Toe", sf::Style::Close);
	//window->setPosition(sf::Vector2i(0, 0));
    board = new Board(numCols, numRows, window, winningStreak, &gameTurn);

	window->setFramerateLimit(60);
	window->setIcon(icon->getSize().x, icon->getSize().y, icon->getPixelsPtr());
    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
			if (event.type == sf::Event::Closed)
			{
				window->close();
				menuWindow->close();
			}
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
				{
					window->close();
					menuWindow->setVisible(true);

				}
			}
        }

        window->clear();
		if (gameMode == 0)
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				board->onClick();
				sf::sleep(sf::milliseconds(150));
			}
		}
		else if (gameMode == 1)
		{
			Ai gameAi(board, -1);

			if (gameTurn.GetTurn() == 1)
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					board->onClick();
					sf::sleep(sf::milliseconds(150));
				}
			}
			else if (gameTurn.GetTurn() == -1)
			{
				gameAi.Start();
			}
		}
		else if (gameMode == 2)
		{
			Ai gameAi1(board, 1);
			Ai gameAi2(board, -1);

			if (gameTurn.GetTurn() == 1)
			{
				gameAi1.Start();
			}
			else if (gameTurn.GetTurn() == -1)
			{
				gameAi2.Start();
			}
			
		}

        board->Draw();
        window->display();

		int boardState = board->GetState();
		EndGame(boardState);
		//menuWindow->setVisible(true);		//In the EndGame method when the game and result windows are closed

    }
}

Board Game::GetBoard()
{
	return *board;
}


void Game::EndGame(int boardState)
{
	if (boardState == 0) return;
	else
	{
		sf::Font font;
		if (!font.loadFromFile("TimesNewRoman.ttf"))
		{
			sf::err() << "Error! Font couldn' be loaded!";
		}

		sf::Text text;
		text.setFont(font);
		text.setCharacterSize(48);
		switch (boardState)
		{
		case -1:
			text.setString("X won!");
			text.setFillColor(sf::Color::Red);
			break;
		case 1:
			text.setString("O won!");
			text.setFillColor(sf::Color::Blue);
			break;
		case 2:
			text.setString("Draw!");
			text.setFillColor(sf::Color::Color(180, 180, 180));
			break;
		}
		sf::FloatRect textRect = text.getLocalBounds();
		text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

		sf::RenderWindow resultWindow(sf::VideoMode(300, 200), "Game Over!", sf::Style::Close);
		resultWindow.setFramerateLimit(60);
		while (resultWindow.isOpen())
		{
			sf::Event event;
			while (resultWindow.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					resultWindow.close();
					this->window->close();
					menuWindow->setVisible(true);
				}

			}

			resultWindow.clear();
			text.setPosition(150.f, 100.f);
			resultWindow.draw(text);
			resultWindow.display();

		}
	}
}

