# Homework4 README
You need to have these libraries installed: 
Run this command on your VM:
>>g++ -g server.cpp -o server -lsfml-graphics -lsfml-window -lsfml-system -lpthread -lX11 -lzmq
>>./server
>>g++ -g main.cpp -o main -lsfml-graphics -lsfml-window -lsfml-system -lpthread -lX11 -lzmq
>>/.main
## Overview
This repository contains the code and assets for Homework 4. The assignment is focused on building a platformer game using the SFML library.
- **objects:** This game uses royalty free character texture, background and platform textures downloaded from the internet.

- **Platformer Elements:** The game includes platforms, a moving platform,a vertical moving platform, a death zone, a spawn location , and a character with basic movement controls (left, right, and jump), gravity, and collision detection.

## How to Play
- Use the arrow keys to move the character left or right.

- Press the spacebar to make the character jump.

- P to Pause

## Credits
- The assignment draws inspiration from various sources, including the book "Game Engine Architecture" by Jason Gregory and online tutorial- [Suraj Sharma](https://www.youtube.com/playlist?list=PL6xSOsbVA1eb_QqMTTcql_3PdOiE928up).

