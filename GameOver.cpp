#include "GameOver.h"

GameOver::GameOver() : gameOver(false), gameWon(false)
{
    if (!font.loadFromFile("C:/Users/trey4/Clion Projects/Sudokugame/Fonts/OpenSans-VariableFont_wdth,wght.ttf"))
        {
        std::cerr << "Error loading font from: C:/Users/trey4/Clion Projects/Sudokugame/Fonts/OpenSans-VariableFont_wdth,wght.ttf" << std::endl;
    }
}

void GameOver::displayMessage(sf::RenderWindow &window)
{
    if (gameOver || gameWon)
        {
        sf::Text messageText;
        messageText.setFont(font);
        messageText.setCharacterSize(36);
        messageText.setFillColor(gameWon ? sf::Color::Green : sf::Color::Red);

        if (gameWon)
            {
            messageText.setString("CONGRATULATIONS! YOU WIN!\nPress R to Restart");
        } else
            {
            messageText.setString("YOU LOSE!\nPress R to Restart");
        }

        // Center the text
        sf::FloatRect textRect = messageText.getLocalBounds();
        messageText.setOrigin(textRect.width / 2, textRect.height / 2);
        messageText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

        window.draw(messageText);
    }
}


void GameOver::resetGame()
{
    gameOver = false;
    gameWon = false;
}

bool GameOver::isGameOver() const
{
    return gameOver;
}

void GameOver::setGameOver(bool state)
{
    gameOver = state;
}

bool GameOver::isGameWon() const
{
    return gameWon;
}

void GameOver::setGameWon(bool state)
{
    gameWon = state;
}
