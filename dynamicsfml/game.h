//snake
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;
using namespace sf;
struct My_coord
{
    int x;
    int y;
};

class Snake {
private:
    std::vector<sf::RectangleShape> body;
    int length;
    sf::Vector2f direction;
    sf::Color color;
    bool keyPressed;
public:
   
    Snake(sf::Color color, int startX, int startY);
    bool isKeyPressed() const;

    void setKeyPressed(bool pressed);
    bool move(bool = 0);
    void grow();
    void render(sf::RenderWindow& window);
    bool checkCollision(const Snake& other) const;
    bool checkCollisionWithFood(const sf::Vector2f& foodPosition) const;
    void setDirection(int, int);
    const std::vector<sf::RectangleShape>& getBody() const;
    int getLength() const;
    Vector2f getDirection() const;

};



class Food {
private:
    sf::RectangleShape shape;
    sf::Color color;
    sf::Vector2f position;
public:
    Food(sf::Color color);

    void spawn(int maxX, int maxY);
    void render(sf::RenderWindow& window) const;  // Mark render as const

    const sf::Vector2f& getPosition() const;

};

class Game {
private:
    void handleInput();
    int update();
    void render();
    void reset();

    sf::RenderWindow window;
    vector<Snake> players;
    std::vector<Food> foods;
    int turnCounter;
    int respawnCounter;
    Sprite bg; 
    Texture Tex_bg;

public:
    bool cheat1flag, cheat2flag;
    Game();

    int run(int);

};
bool Snake::isKeyPressed() const {
    return keyPressed;
}
Vector2f Snake::getDirection() const
{
    return direction;
}

// Function to set the key pressed state
void Snake::setKeyPressed(bool pressed) {
    keyPressed = pressed;
}

void  Snake::setDirection(int x, int y)
{
    direction.x = x;
    direction.y = y;
}

Snake::Snake(sf::Color color, int startX, int startY)
    : color(color), length(1), direction(0, 1) {
    sf::RectangleShape segment(sf::Vector2f(20, 20));
    segment.setPosition(startX, startY);
    segment.setFillColor(color);
    body.push_back(segment);
}

//void Snake::move() {
  //  sf::Vector2f headPosition = body.front().getPosition();
  //  sf::Vector2f newPosition = headPosition + direction * 20.0f;

  //  if (newPosition.x < 0 || newPosition.x >= 800 || newPosition.y < 0 || newPosition.y >= 600) {
        // Game over, hit the wall
  //      length = 0;
 //       return;
 //   }

 //   for (size_t i = length - 1; i > 0; --i) {
 //       body[i].setPosition(body[i - 1].getPosition());
 //   }

//    body.front().setPosition(newPosition);
//}
bool Snake::move(bool flag )
{
    sf::Vector2f newPosition; 
    sf::Vector2f headPosition = body.front().getPosition();

    if (flag)
    {
             newPosition = headPosition + direction * 15.0f;

    }
    else
    {

         newPosition = headPosition + direction * 5.0f;
    }

    if (newPosition.x < 0 || newPosition.x >= 800 || newPosition.y < 0 || newPosition.y >= 600) {
        // Game over, hit the wall
        length = 0;
        return 1;
    }

    // Check for collision with itself
    
    for (int i = 1; i < length; ++i) {
        if (body[i].getPosition() == newPosition) {
            // Game over, hit itself
            length = 0;
            return 1;
        }
    }
    
    for (int i = length - 1; i > 0; --i) {
        body[i].setPosition(body[i - 1].getPosition());
    }
    body.front().setPosition(newPosition);
    return 0;
}
//correct
void Snake::grow()
{
    sf::RectangleShape segment(sf::Vector2f(20, 20));
    segment.setPosition(-100, -100);
    segment.setFillColor(color);
    body.push_back(segment);
    length++;
}

void Snake::render(sf::RenderWindow& window)
{
    for (const auto& segment : body) {
        window.draw(segment);
    }
}

bool Snake::checkCollision(const Snake& other) const
{
    vector<sf::RectangleShape> otherBody = other.getBody();
    for (int i = 0; i < otherBody.size();i++) {
        if (body.front().getGlobalBounds().intersects(otherBody[i].getGlobalBounds())) {
            return true;
        }
    }
    return false;
}

bool Snake::checkCollisionWithFood(const sf::Vector2f& foodPosition) const
{
    return body.front().getGlobalBounds().contains(foodPosition);
}

const std::vector<sf::RectangleShape>& Snake::getBody() const
{
    return body;
}

int Snake::getLength() const
{
    return length;
}

Food::Food(sf::Color color) : color(color)
{
    shape.setSize(sf::Vector2f(10, 10));
    shape.setFillColor(color);
}

void Food::spawn(int maxX, int maxY)
{
    position.x = rand() % maxX;
    position.y = rand() % maxY;
    shape.setPosition(position);
}

//void Food::render(sf::RenderWindow& window) {
//    window.draw(shape);
//}
void Food::render(sf::RenderWindow& window) const {
    window.draw(shape);
}
const sf::Vector2f& Food::getPosition() const {
    return position;
}

Game::Game() : window(sf::VideoMode(800, 600), "SFML Snake Game"), turnCounter(0), respawnCounter(0), cheat1flag(0), cheat2flag(0) {
    window.setFramerateLimit(60);
   if(!Tex_bg.loadFromFile("bg.png"))cout<<"Bg not loaded;";
    bg.setTexture(Tex_bg);
    
    srand(static_cast<unsigned>(time(nullptr)));

    for (int i = 0; i < 6; ++i)
    {
        foods.emplace_back(sf::Color::Yellow);
        foods[i].spawn(800  + i % 50, 600);
    }
}

void Game::handleInput() {
    sf::Event event;

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }

    int size = players.size();

    // Player 1 controls
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {   
        if (players[0].getDirection() != Vector2f(0, 1))
        {
            players[0].setDirection(0, -1);
            if (cheat1flag)
            {
                players[0].move(1);

            }
            else
            {
                players[0].move(0);

            }
        }


    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        if (players[0].getDirection() != Vector2f(0, -1))
        {
            players[0].setDirection(0, 1);
            if (cheat1flag)
            {
                players[0].move(1);

            }
            else
            {
                players[0].move(0);

            }
        }

    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        if (players[0].getDirection() != Vector2f(1, 0))
        {

            players[0].setDirection(-1, 0);
            if (cheat1flag)
            {
                players[0].move(1);

            }
            else
            {
                players[0].move(0);

            }

        }

    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        if (players[0].getDirection() != Vector2f(-1, 0))
        {

            players[0].setDirection(1, 0);
            if (cheat1flag)
            {
                players[0].move(1);

            }
            else
            {
                players[0].move(0);

            }

        }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
    {
        cheat1flag = !cheat1flag;
        if (cheat1flag)cout << "Cheats up ";
        else cout << "normal speed";
    }

    // Player 2 controls
    if (size == 2)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            if (players[1].getDirection() != Vector2f(0, 1))
            {
                players[1].setDirection(0, -1);
                if (cheat1flag)
                {
                    players[1].move(1);

                }
                else
                {
                    players[1].move(0);

                }
            }

        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            if (players[1].getDirection() != Vector2f(0, -1))
            {
                players[1].setDirection(0, 1);
                if (cheat1flag)
                {
                    players[1].move(1);
                }
                else
                {
                    players[1].move(0);
                }
            }

        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            if (players[1].getDirection() != Vector2f(1, 0))
            {

                players[1].setDirection(-1, 0);
                if (cheat1flag)
                {
                    players[1].move(1);
                }
                else
                {
                    players[1].move(0);
                }

            }

        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            if (players[1].getDirection() != Vector2f(-1, 0))
            {

                players[1].setDirection(1, 0);
                if (cheat2flag)
                {
                    players[1].move(1);

                }
                else
                {
                    players[1].move(0);
                }

            }

        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::V))
        {
            cheat2flag = !cheat2flag;
        }
    }
}

int Game::update() {
    
    for (int i = 0; i < players.size(); i++)
    {
        if (players[i].move(0))
        {
            //cout << "TESTING GAME OVER" << endl;
            int i = 1;
            for (const auto& element : players) {
                cout << "Player " << i << " Score : ";
                std::cout << element.getLength()  << " ";
                i++;
            }
            return 0;       // 0 for end screen
            //Game OVER
			//reset()
		}

    }
    //PLAYER COLLISION CHECK
    if (players.size() == 2)
    {
        if (players[0].checkCollision(players[1]) || players[1].checkCollision(players[0]))
        {
            //cout << "TESTING GAME OVER" << endl;
            int i = 1;
            for (const auto& element : players) {
                cout << "Player " << i << " Score : ";
                std::cout << element.getLength() - 1 << " ";
                i++;
            }
            return 0;       // 0 for end screen
        }
    }
    // Check for collision with food and handle respawn
    for (auto& food : foods) {
        for (int i = 0; i < players.size(); i++)
        {
            if (players[i].checkCollisionWithFood(food.getPosition()))
            {
                players[i].grow();
                food.spawn(800, 600);
            }
        }
    }

    // Respawn food if needed
    respawnCounter++;
    if (respawnCounter <= 6) {
        for (auto& food : foods) {
            if (!food.getPosition().x && !food.getPosition().y) {
                food.spawn(800, 600);
            }
        }
        respawnCounter = 0;
    }

    turnCounter++;
    return 1; // 1 for normal working
}

void Game::render() {
    window.clear();
    window.draw(bg);
    // Render snakes
    for(int i=0;i<players.size();i++)
        players[i].render(window);

    // Render food
    for (const auto& food : foods) {
        food.render(window);
    }

    window.display();
}
void Game::reset() {
    int size = players.size();
    players.clear();
    Snake player1(sf::Color::Green, 100, 100);
    players.push_back(player1);
    if (size == 2)
    {
        Snake player2(sf::Color::Green, 700, 500);
        players.push_back(player2);
    }

    for (auto& food : foods) {
        food.spawn(800, 600);
    }

    turnCounter = 0;
    respawnCounter = 0;
}

int Game::run(int play) {
    sf::Clock clock;

    Snake player1(sf::Color::Green, 100, 100);
    players.push_back(player1);
    if (play == 2)
    {
        Snake player2(sf::Color::Red, 700, 500);
        players.push_back(player2);
    }


    while (window.isOpen()) {
        sf::Time elapsed = clock.restart();
        float dt = elapsed.asSeconds();

        handleInput();
        if (update());
        else {
            cout << "call for end";
            return 0;   // zero for end screen ;
        }
       
        render();
        

        sf::sleep(sf::seconds(0.1));  // Adjust the sleep time for controlling snake speed
    }
}


