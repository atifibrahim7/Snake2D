# Homework1 README
## Overview
This repository contains the code for Homework 2, which extends the platformer game created in Homework 1. The assignment focuses on implementing a networked multiplayer environment using the ZeroMQ (0MQ) library, alongside enhancing time management and asynchronicity aspects.

## Sections
The homework assignment is divided into several sections, each addressing specific challenges and requirements. 

Section 1: 
In this section, the main game loop is made multithreaded, allowing distinct subsets of the regular update tasks to be handled concurrently.
Managing thread safety.
Implementing distinct update tasks, e.g., character control and moving platforms.

Section 2: 
A Timeline class is introduced to represent time explicitly, supporting various time scales, adjustable tic sizes, and pausing/unpausing.
Object movement based on elapsed time.
Pause and un-pause game time.
Change the game time speed.

Section 3: 
This section delves into basic networking using the ZeroMQ (0MQ) library.
Start a server process to listen for incoming network connections.
Start a client process that connects to the server.
Handle messages sent from the server to clients.
Support multiple clients running in their own separate processes.

Section 4: 
This section integrates the SFML window with ZeroMQ messaging, creating a networked multiplayer environment. 
The server coordinates clients.
Clients have their separate SFML windows.
Server manages essential game elements while clients focus on local character control.
Clients can be started at any time without affecting others.

Section 5: 
The goal is to make the server design fully asynchronous, allowing multiple clients to run at different speeds without impacting each other. Key considerations:

Character anchored to a separate timeline.
Utilizing asynchronous client threads for client updates.
## Implementation Instructions
Building the Code:

Open a terminal on your VM.

Navigate to the directory containing the source code.

Use the g++ compiler to build the code:
g++ -g main.cpp -o main -lsfml-graphics -lsfml-window -lsfml-system -lzmq
The -lzmq flag is essential for linking the ZeroMQ library.

## Running the Game:

After successfully building the code, run the game:

./main

## Starting the Server
Open a terminal or command prompt on your VM.

Navigate to the directory where you built the code (the same directory where the main executable is located).
./main server
This command will start the server, and it will begin listening for incoming client connections. You should see server-related messages in the terminal.

## Starting a Client
Open a new terminal or command prompt on your VM.

Navigate to the same directory where you built the code (the one containing the main executable).
./main client
You can run this command multiple times to start multiple clients, each in its own separate process. Clients will connect to the server, and you should see client-related messages in the terminal.

Once the clients are connected, they will synchronize and display the same game environment with character movement. 

## Game Controls:

Use the arrow keys to control the character's movement (left, right) and jump (spacebar).
Toggle between constant size and proportional scaling modes using the 'S' key.


## Credits
The assignment draws inspiration from various sources and references:
The book "Game Engine Architecture" by Jason Gregory.
Online tutorials and resources, including Suraj Sharma's YouTube playlist.