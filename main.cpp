#include <SFML/Graphics.hpp>
#include "Sudoku.h"

int main()
{
    constexpr int windowSize = 600; // Increased to fit hint text below the grid
    sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "Sudoku Game");
    window.setFramerateLimit(60);

    Sudoku sudoku; // Create an instance of the Sudoku class
    sudoku.generatePuzzle(30); // Generate a puzzle with 30 cells hidden

    sf::Clock clock; // Clock to track time for updates

    while (window.isOpen())
    {
        sf::Event event; // Declare the event variable

        // Poll events
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close(); // Close the window if the close event is triggered
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                // Handle cell selection on mouse click
                sudoku.handleClick(event.mouseButton.x, event.mouseButton.y);
            }

            if (event.type == sf::Event::KeyPressed)
            {
                // Check if the game is over or won
                if (sudoku.isGameOver() || sudoku.isGameWon())
                    {
                    if (event.key.code == sf::Keyboard::R)
                        {
                        sudoku.resetGameOver(); // Reset the game over state
                        sudoku.generatePuzzle(30); // Generate a new puzzle with 30 cells hidden
                    }
                } else
                    {
                    // Handle key press for number input or hint
                    sudoku.handleKeyPress(event.key.code); // Pass the pressed key to handleKeyPress

                    // Check win condition after a number is set
                    if (sudoku.checkWinCondition())
                        {
                        sudoku.setGameWon(true); // Set game won state
                        std::cout << "Player has won the game!" << std::endl; // Debug output
                    }
                }
            }
        }

        // Calculate delta time
        float deltaTime = clock.restart().asSeconds(); // Get elapsed time since last frame

        // Update flashing effect
        sudoku.update(deltaTime); // Call update method with delta time

        window.clear(sf::Color::Black); // Clear the window before drawing
        sudoku.draw(window); // Draw the Sudoku grid and numbers

        window.display(); // Display the contents of the window
    }

    return 0;
}
