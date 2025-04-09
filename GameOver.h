#ifndef GAMEOVER_H
#define GAMEOVER_H

#include <SFML/Graphics.hpp>
#include <iostream>

class GameOver
{
public:
    GameOver(); // Constructor

    void displayMessage(sf::RenderWindow &window); // Display win/lose message
    void resetGame(); // Reset both win and lose states

    bool isGameOver() const; // Check if the game is over
    void setGameOver(bool state); // Set the game over state

    bool isGameWon() const; // Check if the game is won
    void setGameWon(bool state); // Set the game won state

private:
    bool gameOver; // Flag for losing
    bool gameWon;  // Flag for winning
    sf::Font font; // Load font once
};

#endif // GAMEOVER_H
