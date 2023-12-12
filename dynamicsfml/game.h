//snake
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

struct My_coord
{
    int x;
    int y;
};

class Snake {
public:
    Snake(sf::Color color, int startX, int startY);

    void move();
    void grow();
    void render(sf::RenderWindow& window);
    bool checkCollision(const Snake& other) const;
    bool checkCollisionWithFood(const sf::Vector2f& foodPosition) const;

    const std::vector<sf::RectangleShape>& getBody() const;
    int getLength() const;

private:
    std::vector<sf::RectangleShape> body;
    int length;
    sf::Vector2f direction;
    sf::Color color;
};

//class Food {
//public:
  //  Food(sf::Color color);

   // void spawn(int maxX, int maxY);
   // void render(sf::RenderWindow& window);

   // const sf::Vector2f& getPosition() const;

//private:
  //  sf::RectangleShape shape;
  //  sf::Color color;
  //  sf::Vector2f position;
//};

class Food {
public:
    Food(sf::Color color);

    void spawn(int maxX, int maxY);
    void render(sf::RenderWindow& window) const;  // Mark render as const

    const sf::Vector2f& getPosition() const;

private:
    sf::RectangleShape shape;
    sf::Color color;
    sf::Vector2f position;
};

class Game {
public:
    Game();

    void run();

private:
    void handleInput();
    void update();
    void render();
    void reset();

    sf::RenderWindow window;
    Snake player1;
    Snake player2;
    std::vector<Food> foods;
    int turnCounter;
    int respawnCounter;
};

Snake::Snake(sf::Color color, int startX, int startY)
    : color(color), length(1), direction(1, 0) {
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
void Snake::move()
{
    sf::Vector2f headPosition = body.front().getPosition();
    sf::Vector2f newPosition = headPosition + direction * 20.0f;

    if (newPosition.x < 0 || newPosition.x >= 800 || newPosition.y < 0 || newPosition.y >= 600) {
        // Game over, hit the wall
        length = 0;
        return;
    }

    // Check for collision with itself
    for (size_t i = 1; i < length; ++i) {
        if (body[i].getGlobalBounds().contains(newPosition)) {
            // Game over, hit itself
            length = 0;
            return;
        }
    }

    for (size_t i = length - 1; i > 0; --i) {
        body[i].setPosition(body[i - 1].getPosition());
    }

    body.front().setPosition(newPosition);
}
void Snake::grow()
{
    sf::RectangleShape segment(sf::Vector2f(20, 20));
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
    for (const auto& segment : other.body) {
        if (body.front().getGlobalBounds().intersects(segment.getGlobalBounds())) {
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
    shape.setSize(sf::Vector2f(20, 20));
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

Game::Game() : window(sf::VideoMode(800, 600), "SFML Snake Game"), player1(sf::Color::Green, 100, 100),
player2(sf::Color::Red, 700, 500), turnCounter(0), respawnCounter(0) {
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

    // Player 1 controls
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        player1.move();


    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        player1.move();

    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        player1.move();


    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        player1.move();

    }

    // Player 2 controls
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        player2.move();

    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        player2.move();

    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        player2.move();

    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        player2.move();

    }
}

void Game::update() {
    player1.move();
    player2.move();

    // Check for collisions
    if (player1.checkCollision(player2) || player1.getLength() == 0 || player2.getLength() == 0) {
        //  std::cout << "Game Over! Player 1 score: " << player1.getLength() - 1 << ", Player 2 score: "
          //          << player2.getLength() - 1 << std::endl;
        reset();
    }

    // Check for collision with food and handle respawn
    for (auto& food : foods) {
        if (player1.checkCollisionWithFood(food.getPosition())) {
            player1.grow();
            food.spawn(800, 600);
        }

        if (player2.checkCollisionWithFood(food.getPosition())) {
            player2.grow();
            food.spawn(800, 600);
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
}

void Game::render() {
    window.clear();

    // Render snakes
    player1.render(window);
    player2.render(window);

    // Render food
    for (const auto& food : foods) {
        food.render(window);
    }

    window.display();
}
void Game::reset() {
    player1 = Snake(sf::Color::Green, 100, 100);
    player2 = Snake(sf::Color::Red, 700, 500);

    for (auto& food : foods) {
        food.spawn(800, 600);
    }

    turnCounter = 0;
    respawnCounter = 0;
}

void Game::run() {
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Time elapsed = clock.restart();
        float dt = elapsed.asSeconds();

        handleInput();
        update();
        render();

        sf::sleep(sf::seconds(0.5));  // Adjust the sleep time for controlling snake speed
    }
}


