# Tetris-SFML

A classic Tetris implementation built using SFML (Simple and Fast Multimedia Library). This project brings the timeless puzzle game to life with modern graphics and smooth gameplay.

## Description

This is a feature-complete Tetris game implemented using SFML. The game follows the standard Tetris mechanics and rules as described on [Wikipedia](https://en.wikipedia.org/wiki/Tetris). 

## Features

- Classic Tetris gameplay mechanics
- Smooth piece movement and rotation
- Score tracking system
- Increasing difficulty levels
- Modern graphics using SFML

## Screenshots

![Tetris Game Screenshot 1](Screenshot%202022-09-02%20at%2010.22.08%20AM.png)
![Tetris Game Screenshot 2](Screenshot%202022-09-02%20at%2010.22.25%20AM.png)

## Prerequisites

Before you begin, ensure you have the following installed:

- [SFML](http://www.sfml-dev.org/) - Simple and Fast Multimedia Library
- [Git](http://git-scm.com/) - Version Control System
- [CMake](http://cmake.org/) - Cross-platform build system

## Build and Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/Tetris-SFML.git
   cd Tetris-SFML
   ```

2. Create a build directory:
   ```bash
   mkdir build
   cd build
   ```

3. Generate build files with CMake:
   ```bash
   cmake ..
   ```

4. Build the project:
   ```bash
   cmake --build .
   ```

## Controls

- Left Arrow: Move piece left
- Right Arrow: Move piece right
- Down Arrow: Soft drop
- Space: Hard drop
- Up Arrow: Rotate piece
- P: Pause game
- ESC: Exit game

## Game Rules

- Pieces fall from the top of the screen
- Complete lines are cleared and award points
- Game ends when pieces stack up to the top
- Score increases with each line cleared
- Speed increases as you progress

## Contributing

Contributions are welcome! Feel free to submit issues and pull requests. For major changes, please open an issue first to discuss what you would like to change.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/YourFeature`)
3. Commit your changes (`git commit -m 'Add some feature'`)
4. Push to the branch (`git push origin feature/YourFeature`)
5. Open a Pull Request


## Acknowledgments

- Original Tetris game design by Alexey Pajitnov
- SFML development team for their excellent multimedia library
- The open-source community for various resources and inspirations