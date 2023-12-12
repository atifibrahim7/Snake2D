#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <iostream>
using namespace std;
using namespace sf;

class Menu {
public:

    Sprite menu;
    Texture menu_texture;

    // Add menu attributes here
    Menu()
    {
        menu_texture.loadFromFile("menu.png");
        menu.setTexture(menu_texture);

          // Constructor's body
    }
    void display_menu()
    {
        // Create a window to display the menu
        RenderWindow window(VideoMode(800, 600), " Menu");

        // Create a text object for the overview
        while (window.isOpen())
        {
            // Handle events
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                    window.close();

                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
                {
                    Vector2i mousePos = Mouse::getPosition(window);
                    cout << mousePos.x << " " << mousePos.y << endl;
                    if (mousePos.x >= 257 && mousePos.x <= 520 && mousePos.y >= 237 && mousePos.y <= 315)
                    {
                        cout << "single player";
                        //~Game();
                       // window.close();
                    }
                    if (mousePos.x >= 288 && mousePos.x <= 484 && mousePos.y >= 350 && mousePos.y <= 435)
                    {
                        cout << "Two  player";
                        //  window.close();
                    }
                
                    if (mousePos.x >= 344 && mousePos.x <= 430 && mousePos.y >= 470 && mousePos.y <= 530)
                    {
                        cout << "Exit";
                          window.close();


                    }
                }
            }

            // Draw the menu sprite
            window.clear();
            window.draw(menu);
            window.display();
        }
    }


};
