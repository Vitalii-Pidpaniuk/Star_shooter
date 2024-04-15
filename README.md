# Star_shooter

This project is a C++ game developed using the SFML library. The game involves controlling a spaceship that shoots at figures to accumulate score points.

## Game Description

The player controls a spaceship in outer space. The spaceship can move up, down, left, and right. The goal of the player is to destroy as many figures appearing on the screen using the ship's firepower. Each destroyed figure earns the player a certain amount of points.

The game features:
- Smooth control of the spaceship using keyboard keys.
- Health points mechanics, to make game more difficult.
- Score calculation and display on the screen.

## Dependencies

To compile and run the project, you need to have the SFML library installed, newest version.

## Installation and Usage

1. Clone the repository to your computer.
2. Install SFML following the instructions.
3. Open the project in your favorite code editor.
4. Compile and run the project by executing your compiler's commands.

```bash
g++ -o main main.cpp -lsfml-graphics -lsfml-window -lsfml-system
./main
