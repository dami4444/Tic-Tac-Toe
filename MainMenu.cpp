#include "MainMenu.h"
#include "Game.h"

MainMenu::MainMenu()
{
    desktopMode = sf::VideoMode::getDesktopMode();

    numRows = 15;
    numCols = 15;
    gameMode = 1;
    winningStreak = 5;

    startButton.setSize(sf::Vector2f(255.f, 70.f));
    startButton.setPosition(272.5f, 500.0f);
    
    icon = new sf::Image;
    if (!(*icon).loadFromFile("icon.png"))
    {
        sf::err() << "Error! Window icon could not be loaded!";
    }

    buttonTexture = new sf::Texture;
    if (!(*buttonTexture).loadFromFile("button_start-game.bmp"))
    {
        sf::err() << "Error! Cell textures could not be loaded!";
    }
    startButton.setTexture(buttonTexture);

    arrowLeftTexture = new sf::Texture;
    if (!(*arrowLeftTexture).loadFromFile("button_arrow_left.bmp"))
    {
        sf::err() << "Error! Cell textures could not be loaded!";
    }

    arrowRightTexture = new sf::Texture;
    if (!(*arrowRightTexture).loadFromFile("button_arrow_right.bmp"))
    {
        sf::err() << "Error! Cell textures could not be loaded!";
    }

    arrowGameModeLeft = CreateArrow(520, 120, arrowLeftTexture);
    arrowNumRowsLeft = CreateArrow(520, 220, arrowLeftTexture);
    arrowNumColsLeft = CreateArrow(520, 320, arrowLeftTexture);
    arrowWinningStreakLeft = CreateArrow(520, 420, arrowLeftTexture);

    arrowGameModeRight = CreateArrow(720, 120, arrowRightTexture);
    arrowNumRowsRight = CreateArrow(720, 220, arrowRightTexture);
    arrowNumColsRight = CreateArrow(720, 320, arrowRightTexture);
    arrowWinningStreakRight = CreateArrow(720, 420, arrowRightTexture);
}

void MainMenu::Display()
{
    sf::RenderWindow menuWindow(sf::VideoMode(800, 600), "Tic Tac Toe - Main Menu",sf::Style::Close);

    menuWindow.setFramerateLimit(60);
    menuWindow.setIcon(icon->getSize().x, icon->getSize().y, icon->getPixelsPtr());
    while (menuWindow.isOpen())
    {
        sf::Event event;
        while (menuWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                menuWindow.close();
        }

        menuWindow.clear();
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            sf::Vector2f mousePos;
            mousePos.x = sf::Mouse::getPosition(menuWindow).x;
            mousePos.y = sf::Mouse::getPosition(menuWindow).y;

            onClick(mousePos, menuWindow);
        }
        Draw(menuWindow);
        menuWindow.display();
        
    }
}

void MainMenu::Draw(sf::RenderWindow& menuWindow)
{
    
    DrawText(350, 50, "Settings:", menuWindow);

    DrawText(40, 120, "Game Mode:", menuWindow);
    DrawText(40, 220, "Number of Rows:", menuWindow);
    DrawText(40, 320, "Number of Cols:", menuWindow);
    DrawText(40, 420, "Winning number of symbols in a row:", menuWindow);

    switch (gameMode)
    {
    case 0:
        DrawText(588, 120, "Plr vs Plr", menuWindow);
        break;
    case 1:
        DrawText(588, 120, "Plr vs Ai", menuWindow);
        break;
    case 2:
        DrawText(592, 120, "Ai vs Ai", menuWindow);
        break;
    }
    //DrawText(640, 120, std::to_string(gameMode), menuWindow);
    
    DrawText(640, 220, std::to_string(numRows), menuWindow);
    DrawText(640, 320, std::to_string(numCols), menuWindow);
    DrawText(640, 420, std::to_string(winningStreak), menuWindow);

    menuWindow.draw(arrowNumRowsLeft);
    menuWindow.draw(arrowNumRowsRight);
    menuWindow.draw(arrowNumColsLeft);
    menuWindow.draw(arrowNumColsRight);
    menuWindow.draw(arrowGameModeLeft);
    menuWindow.draw(arrowGameModeRight);
    menuWindow.draw(arrowWinningStreakLeft);
    menuWindow.draw(arrowWinningStreakRight);

    menuWindow.draw(startButton);
}

void MainMenu::DrawText(float posX, float posY, sf::String s, sf::RenderWindow& menuWindow)
{
    sf::Font font;
    if (!font.loadFromFile("TimesNewRoman.ttf"))
    {
        // error...
        sf::err() << "Error! Font couldn't be loaded!";
    }

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(30);
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.setPosition(posX, posY);
    text.setString(s);
    //text.setString("X won!");
    text.setFillColor(sf::Color::Red);

    menuWindow.draw(text);
}

sf::RectangleShape MainMenu::CreateArrow(float posX, float posY, sf::Texture* t)
{
    sf::RectangleShape shape(sf::Vector2f(43.f, 43.f));
    shape.setPosition(posX, posY);
    shape.setTexture(t);
    //menuWindow.draw(shape);
    return shape;
}

bool MainMenu::isButtonClicked(sf::Vector2f mousePos, sf::RectangleShape button)
{
    if (button.getGlobalBounds().contains(mousePos)) return true;
    else return false;
}

void MainMenu::onClick(sf::Vector2f mousePos, sf::RenderWindow& menuWindow)
{
    if (isButtonClicked(mousePos, this->startButton))
    {
        Game game(gameMode, numRows, numCols, winningStreak, &menuWindow);
        //menuWindow.close();
        menuWindow.setVisible(false);
        game.Start();
    }

    //gameMode changing
    if (isButtonClicked(mousePos, arrowGameModeLeft))
    {
        if (gameMode > 0) gameMode--;
        sf::sleep(sf::milliseconds(100));
    }

    if (isButtonClicked(mousePos, arrowGameModeRight))
    {
        if (gameMode < 2) gameMode++;
        sf::sleep(sf::milliseconds(100));
    }

    //numRows changing
    if (isButtonClicked(mousePos, arrowNumRowsLeft))
    {
        if(numRows > winningStreak && winningStreak) numRows--;
        sf::sleep(sf::milliseconds(100));
    }

    if (isButtonClicked(mousePos, arrowNumRowsRight))
    {

        if (numRows < (int(desktopMode.height)/30) - 3 && winningStreak > 3) numRows++;
        sf::sleep(sf::milliseconds(100));
    }

    //numCols changing
    if (isButtonClicked(mousePos, arrowNumColsLeft))
    {
        if (numCols > winningStreak) numCols--;
        sf::sleep(sf::milliseconds(100));
    }

    if (isButtonClicked(mousePos, arrowNumColsRight))
    {
        if (numCols < (int(desktopMode.width) / 30) - 1 && winningStreak > 3) numCols++;
        sf::sleep(sf::milliseconds(100));
    }

    //winningStreak changing
    if (isButtonClicked(mousePos, arrowWinningStreakLeft))
    {
        if(winningStreak > 3) winningStreak--;
        if (winningStreak == 3)
        {
            numRows = 3;
            numCols = 3;
        }
        sf::sleep(sf::milliseconds(100));
    }

    if (isButtonClicked(mousePos, arrowWinningStreakRight))
    {
        if (winningStreak < 10) winningStreak++;
        if (numRows < winningStreak) numRows++;
        if (numCols < winningStreak) numCols++;
        if (winningStreak == 5)
        {
            if (numRows < 15) numRows = 15;
            if (numCols < 15) numCols = 15;
        }
        sf::sleep(sf::milliseconds(100));
    }
}

MainMenu::~MainMenu()
{
    delete buttonTexture;
    delete arrowLeftTexture;
    delete arrowRightTexture;
    delete icon;
}
