#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <iostream>
#include "game.h"
using namespace std;
using namespace sf;

class Menu {
public:

    Sprite menu,end;
    Texture menu_texture,end_tex;
    

    // Add menu attributes here
    Menu()
    {
        menu_texture.loadFromFile("menu.png");
        menu.setTexture(menu_texture);
        end_tex.loadFromFile("end.png");
        end.setTexture(end_tex);

          // Constructor's body
    }
    void display_menu(int& choice)
    {
        // Create a window to display the menu
        RenderWindow window(VideoMode(800, 600), " Menu");

        // Create a snake object for aethetics
        Snake snake(sf::Color::Green, 10,10);
        for(int i=0;i<20;i++)
            snake.grow();

        // Create a rectangle object for the overview
        Vector2i posFloat;
        RectangleShape rectangle(Vector2f(-100, -100));
        Color green(0, 255, 0, 128);
        rectangle.setFillColor(green);

        // Create a text object for the overview
        while (window.isOpen())
        {
            // Handle events
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                    window.close();

                
                posFloat = Mouse::getPosition(window);
                if (posFloat.x >= 257 && posFloat.x <= 520 && posFloat.y >= 237 && posFloat.y <= 315)
                {
					rectangle.setPosition(257, 237);
					rectangle.setSize(Vector2f(263, 78));
				}
				else if (posFloat.x >= 288 && posFloat.x <= 484 && posFloat.y >= 350 && posFloat.y <= 435)
				{
					rectangle.setPosition(288, 350);
					rectangle.setSize(Vector2f(196, 85));
				}
				else if (posFloat.x >= 344 && posFloat.x <= 430 && posFloat.y >= 470 && posFloat.y <= 530)
				{
					rectangle.setPosition(344, 470);
					rectangle.setSize(Vector2f(86, 60));
				}
                else
                    rectangle.setPosition(-100, -100);
                
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
                {
                    Vector2i mousePos = Mouse::getPosition(window);
                    cout << mousePos.x << " " << mousePos.y << endl;
                    if (mousePos.x >= 257 && mousePos.x <= 520 && mousePos.y >= 237 && mousePos.y <= 315)
                    {
                        cout << "single player";
                        choice = 1;
                        //~Game()
                        window.close();
                    }
                    if (mousePos.x >= 288 && mousePos.x <= 484 && mousePos.y >= 350 && mousePos.y <= 435)
                    {
                        cout << "Two  player";
                        choice = 2;
                          window.close();
                    }
                
                    if (mousePos.x >= 344 && mousePos.x <= 430 && mousePos.y >= 470 && mousePos.y <= 530)
                    {
                        cout << "Exit";
                        choice = 0;
                          window.close();


                    }
                }
            }
            //SNAKE ANIMATION
            vector<RectangleShape> body = snake.getBody();
            if (body[0].getPosition() == Vector2f(10,500))
            {
                if (snake.getDirection() != Vector2f(0, 1))
                {
                    snake.setDirection(0, -1);
                    snake.move();
                }


            }
            else if (body[0].getPosition() == Vector2f(750, 10))
            {
                if (snake.getDirection() != Vector2f(0, -1))
                {
                    snake.setDirection(0, 1);
                    snake.move();
                }

            }
            else if (body[0].getPosition() == Vector2f(750, 500))
            {
                if (snake.getDirection() != Vector2f(1, 0))
                {

                    snake.setDirection(-1, 0);
                    snake.move();

                }

            }
            else if (body[0].getPosition() == Vector2f(10, 10))
            {
                if (snake.getDirection() != Vector2f(-1, 0))
                {

                    snake.setDirection(1, 0);
                    snake.move();

                }
            }
            snake.move();
            // Draw the menu sprite
            window.clear();
            window.draw(menu);
            window.draw(rectangle);
            snake.render(window);
            window.display();

            sf::sleep(sf::seconds(0.1));
        }
    }

    int display_end()
    {

        RenderWindow window(VideoMode(800, 600), " Menu");


        // Create a rectangle object for the overview
        Vector2i posFloat;
        RectangleShape rectangle(Vector2f(-100, -100));
        Color green(0, 255, 0, 128);
        rectangle.setFillColor(green);


        while (window.isOpen())
        {
            // Handle events
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                    window.close();

                posFloat = Mouse::getPosition(window);
                if (posFloat.x >= 288 && posFloat.x <= 484 && posFloat.y >= 350 && posFloat.y <= 435)
                {
					rectangle.setPosition(258, 350);
					rectangle.setSize(Vector2f(290, 85));
				}
				else if (posFloat.x >= 344 && posFloat.x <= 430 && posFloat.y >= 470 && posFloat.y <= 530)
				{
					rectangle.setPosition(344, 470);
					rectangle.setSize(Vector2f(86, 60));
				}
				else
					rectangle.setPosition(-100, -100);

                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
                {
                    Vector2i mousePos = Mouse::getPosition(window);
                    cout << mousePos.x << " " << mousePos.y << endl;
                  
                    if (mousePos.x >= 288 && mousePos.x <= 484 && mousePos.y >= 350 && mousePos.y <= 435)
                    {
                        cout << "Two  player";
                        return 1;       // play again
                    }

                    if (mousePos.x >= 344 && mousePos.x <= 430 && mousePos.y >= 470 && mousePos.y <= 530)
                    {   
                        return 0;       //end
                        cout << "Exit";
                        window.close();


                    }
                }
            }
          
            window.clear();
            window.draw(end);
            window.draw(rectangle);
            window.display();

            sf::sleep(sf::seconds(0.1));
        }

    }
};
