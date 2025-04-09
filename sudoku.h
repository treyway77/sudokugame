#ifndef SUDOKU_H
#define SUDOKU_H

#include <SFML/Graphics.hpp>
#include <array>
#include "GameOver.h" // Include the GameOver header

class Sudoku
{
public:
    Sudoku(); // Constructor

    void draw(sf::RenderWindow &window); // Draw the Sudoku grid and numbers
    void selectCell(int row, int col); // Select a cell
    void setCell(int value); // Set a value in the selected cell
    void handleClick(int x, int y); // Handle mouse clicks
    void handleKeyPress(sf::Keyboard::Key key); // Handle key presses
    void generatePuzzle(int emptyCells); // Generate the Sudoku puzzle
    void update(float deltaTime); // Update the flashing effect
    bool isGameOver() const; // Check if the game is over
    void resetGameOver(); // Reset the game over state
    int getHintsRemaining() const; // Get remaining hints
    void provideHint(); // Provide a hint to the player

    bool checkWinCondition() const; // Check if the player has won
    void setGameWon(bool state); // Set game won state
    bool isGameWon() const; // Check if the game is won

private:
    static const int SIZE = 9; // Size of the grid
    static const int CELL_SIZE = 60; // Size of each cell
    static const int GRID_SIZE = SIZE * CELL_SIZE; // Total size of the grid

    int grid[SIZE][SIZE]; // 2D array to hold Sudoku numbers
    bool hidden[SIZE][SIZE]; // 2D array to track hidden cells
    int selectedRow; // Currently selected row
    int selectedCol; // Currently selected column
    sf::Font font; // Font for displaying numbers

    GameOver gameOverState; // GameOver object
    int incorrectInputCount; // Counter for incorrect inputs
    const int maxIncorrectInputs = 3; // Maximum allowed incorrect inputs

    // Flashing effect variables
    bool flashRed; // Flag to indicate if the screen should flash red
    float flashDuration; // Duration of the flash
    float flashTimer; // Timer to track flash duration

    // Hint system variables
    int hintsUsed; // Number of hints used
    const int maxHints = 3; // Maximum allowed hints

    bool isSafe(int grid[SIZE][SIZE], int row, int col, int num); // Check if a number can be placed
    bool solveSudoku(int grid[SIZE][SIZE]); // Solve the Sudoku puzzle
};

#endif // SUDOKU_H
