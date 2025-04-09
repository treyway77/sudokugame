#include "Sudoku.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <algorithm> // Required for std::shuffle

#define SIZE 9
#define CELL_SIZE 60 // Size of each cell
#define GRID_SIZE (SIZE * CELL_SIZE) // Total size of the grid

Sudoku::Sudoku() : selectedRow(-1), selectedCol(-1), grid{}, hidden{}, incorrectInputCount(0),
                   flashRed(false), flashDuration(0.5f), flashTimer(0.0f), hintsUsed(0) // Initialize members
{
    // Load the font
    if (!font.loadFromFile("C:/Users/trey4/Clion Projects/Sudokugame/Fonts/OpenSans-VariableFont_wdth,wght.ttf"))
        {
        std::cerr << "Error loading font" << std::endl; // Print error if font fails to load
    }

    generatePuzzle(30); // Generate a puzzle with 30 cells hidden
}

void Sudoku::draw(sf::RenderWindow &window)
{
    // Draw flashing effect
    if (flashRed)
        {
        window.clear(sf::Color::Red); // Clear the window with red color
    } else
        {
        window.clear(sf::Color::Black); // Clear the window with black color
    }

    // Draw numbers and grid
    sf::Text numberText;
    numberText.setFont(font);
    numberText.setCharacterSize(24);
    numberText.setFillColor(sf::Color::White); // Set number color to white

    for (int row = 0; row < SIZE; ++row)
        {
        for (int col = 0; col < SIZE; ++col)
            {
            sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
            cell.setPosition(col * CELL_SIZE, row * CELL_SIZE);
            cell.setFillColor((row == selectedRow && col == selectedCol) ? sf::Color(50, 50, 50) : sf::Color::Black);
            window.draw(cell); // Draw the cell

            // Draw the number if it exists in the grid
            if (grid[row][col] != 0)
                {
                numberText.setString(std::to_string(grid[row][col])); // Convert number to string
                // Center the number within the cell
                numberText.setPosition(col * CELL_SIZE + (CELL_SIZE / 4), row * CELL_SIZE + (CELL_SIZE / 4)); // Adjust position
                window.draw(numberText); // Draw number
            }
        }
    }

    // Draw grid lines
    sf::RectangleShape line;
    line.setFillColor(sf::Color::White);

    for (int i = 0; i <= SIZE; ++i)
        {
        line.setSize(sf::Vector2f(GRID_SIZE, (i % 3 == 0) ? 3 : 1)); // Thicker lines for 3x3 boxes
        line.setPosition(0, i * CELL_SIZE);
        window.draw(line);

        line.setSize(sf::Vector2f((i % 3 == 0) ? 3 : 1, GRID_SIZE)); // Vertical lines
        line.setPosition(i * CELL_SIZE, 0);
        window.draw(line);
    }

    // Display game over or win message if the game is over
    if (gameOverState.isGameOver() || gameOverState.isGameWon())
        {
        gameOverState.displayMessage(window);
    }

    // Draw remaining hints counter
    sf::Text hintText;
    hintText.setFont(font);
    hintText.setCharacterSize(20);
    hintText.setFillColor(sf::Color::Green);
    hintText.setString("Hints left: " + std::to_string(getHintsRemaining()));
    hintText.setPosition(10, GRID_SIZE + 5); // Slightly below the grid
    window.draw(hintText);
}

void Sudoku::selectCell(int row, int col)
{
    if (row >= 0 && row < SIZE && col >= 0 && col < SIZE)
        {
        selectedRow = row; // Update the selected row
        selectedCol = col; // Update the selected column
        std::cout << "Selected cell: (" << selectedRow << ", " << selectedCol << ")" << std::endl; // Debug output
    }
}

void Sudoku::setCell(int value)
{
    // Allow setting value only for hidden cells
    if (selectedRow != -1 && selectedCol != -1 && hidden[selectedRow][selectedCol])
        {
        if (isSafe(grid, selectedRow, selectedCol, value))
            {
            grid[selectedRow][selectedCol] = value; // Set the selected cell to the entered value
            std::cout << "Set cell: (" << selectedRow << ", " << selectedCol << ") to " << value << std::endl; // Debug output
            incorrectInputCount = 0; // Reset the counter on a valid input
            flashRed = false; // Reset flash state

            // Check for win condition after setting the cell
            if (checkWinCondition())
                {
                gameOverState.setGameWon(true); // Set game won state
                std::cout << "Player has won the game!" << std::endl; // Debug output
            }
        } else
            {
            // Increment the incorrect input counter
            incorrectInputCount++;
            std::cout << "Invalid move: " << value << " cannot be placed at (" << selectedRow << ", " << selectedCol << ")" << std::endl; // Debug output

            // Check if the max number of incorrect inputs has been reached
            if (incorrectInputCount >= maxIncorrectInputs) {
                std::cout << "Game Over! Too many invalid moves." << std::endl; // Debug output
                gameOverState.setGameOver(true); // Set game over state
            } else
                {
                flashRed = true; // Trigger the flash effect
                flashTimer = 0.0f; // Reset the flash timer
            }
        }
    }
}


bool Sudoku::isSafe(int grid[SIZE][SIZE], int row, int col, int num)
{
    for (int x = 0; x < SIZE; ++x)
        {
        if (grid[row][x] == num || grid[x][col] == num ||
            grid[row - row % 3 + x / 3][col - col % 3 + x % 3] == num)
            {
            return false; // Number already exists in the row, column, or box
        }
    }
    return true; // The move is valid
}

bool Sudoku::solveSudoku(int grid[SIZE][SIZE])
{
    for (int row = 0; row < SIZE; ++row)
        {
        for (int col = 0; col < SIZE; ++col)
            {
            if (grid[row][col] == 0) { // Find an empty cell
                std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9};

                // Initialize random number generator
                std::random_device rd; // Obtain a random number from hardware
                std::mt19937 eng(rd()); // Seed the generator
                std::shuffle(numbers.begin(), numbers.end(), eng); // Shuffle the numbers

                for (int num : numbers) { // Try numbers in random order
                    if (isSafe(grid, row, col, num)) {
                        grid[row][col] = num; // Assign num to the cell
                        if (solveSudoku(grid)) {
                            return true; // Recursively call solveSudoku
                        }
                        grid[row][col] = 0; // Reset if failed
                    }
                }
                return false; // Return false if no number can be placed
            }
        }
    }
    return true; // Solved
}

void Sudoku::generatePuzzle(int emptyCells)
{
    // Step 1: Create full solution
    solveSudoku(grid);

    // Step 2: Hide numbers for difficulty
    int count = emptyCells;

    // Reset hidden array
    for (int row = 0; row < SIZE; ++row)
        {
        for (int col = 0; col < SIZE; ++col)
            {
            hidden[row][col] = false; // Initially mark all cells as visible
        }
    }

    // Initialize random number generator
    std::random_device rd; // Obtain a random number from hardware
    std::mt19937 eng(rd()); // Seed the generator

    while (count > 0)
        {
        // Generate random row and column indices
        std::uniform_int_distribution<> distr(0, SIZE - 1); // Define the range for rows and columns
        int row = distr(eng); // Randomly select a row
        int col = distr(eng); // Randomly select a column

        if (!hidden[row][col])
            {
            hidden[row][col] = true; // Mark this cell as hidden
            grid[row][col] = 0; // Remove the number
            --count; // Decrement count of cells to hide
        }
    }
}

void Sudoku::handleClick(int x, int y)
{
    int row = y / CELL_SIZE; // Calculate the row based on the y-coordinate
    int col = x / CELL_SIZE; // Calculate the column based on the x-coordinate
    selectCell(row, col); // Call selectCell to update the selected cell
}

void Sudoku::handleKeyPress(sf::Keyboard::Key key)
{
    if (selectedRow != -1 && selectedCol != -1)
        { // Check if a cell is selected
        int num = 0;

        switch (key)
            {
            case sf::Keyboard::Num1: num = 1; break;
            case sf::Keyboard::Num2: num = 2; break;
            case sf::Keyboard::Num3: num = 3; break;
            case sf::Keyboard::Num4: num = 4; break;
            case sf::Keyboard::Num5: num = 5; break;
            case sf::Keyboard::Num6: num = 6; break;
            case sf::Keyboard::Num7: num = 7; break;
            case sf::Keyboard::Num8: num = 8; break;
            case sf::Keyboard::Num9: num = 9; break;
            case sf::Keyboard::H: provideHint(); return; // Handle hint request
            default: return; // Ignore other keys
        }

        std::cout << "Setting cell: " << num << " at (" << selectedRow << ", " << selectedCol << ")" << std::endl; // Debug output
        setCell(num); // Call setCell to update the grid with the selected number
    }
}

// Provide a hint to the player
void Sudoku::provideHint()
{
    if (hintsUsed >= maxHints)
        {
        std::cout << "No hints left!" << std::endl;
        return;
    }

    for (int row = 0; row < SIZE; ++row)
        {
        for (int col = 0; col < SIZE; ++col)
            {
            if (grid[row][col] == 0)
                {
                // Solve the full board to get the correct number
                int tempGrid[SIZE][SIZE];
                std::copy(&grid[0][0], &grid[0][0] + SIZE * SIZE, &tempGrid[0][0]);
                solveSudoku(tempGrid);

                grid[row][col] = tempGrid[row][col];
                hidden[row][col] = false;
                hintsUsed++;
                std::cout << "Hint given at: (" << row << ", " << col << "). Hints used: " << hintsUsed << std::endl;
                return;
            }
        }
    }

    std::cout << "No empty cells to hint!" << std::endl;
}

// Get remaining hints
int Sudoku::getHintsRemaining() const
{
    return maxHints - hintsUsed;
}

// Method to check if the player has won
bool Sudoku::checkWinCondition() const
{
    for (int row = 0; row < SIZE; ++row)
        {
        for (int col = 0; col < SIZE; ++col)
            {
            if (grid[row][col] == 0) {
                return false; // Incomplete board
            }
        }
    }
    return true; // All cells filled
}

// Method to check if the game is over
bool Sudoku::isGameOver() const
{
    return gameOverState.isGameOver(); // Delegate to the GameOver class
}

// Method to reset the game over state and the incorrect input counter
void Sudoku::resetGameOver()
{
    gameOverState.resetGame(); // Reset the game over state
    incorrectInputCount = 0; // Reset the incorrect input counter
    hintsUsed = 0; // Reset hints used
}

// Update method for flashing effect
void Sudoku::update(float deltaTime)
{
    // Update flash timer
    if (flashRed)
        {
        flashTimer += deltaTime; // Increment timer
        if (flashTimer >= flashDuration)
            {
            flashRed = false; // Stop flashing after the duration
        }
    }
}
// Sudoku.cpp

void Sudoku::setGameWon(bool state)
{
    gameOverState.setGameWon(state); // Delegate to GameOver's setGameWon
}
bool Sudoku::isGameWon() const
{
    return gameOverState.isGameWon(); // Check the game over state for win condition
}
