#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <condition_variable>
class Timeline {
public:
    // Constructor to initialize timeline variables
    Timeline() : paused(false), timeScale(1.0f), elapsedSeconds(0.0f), ticCount(0), ticWhilePaused(0) {}

    // Update the timeline based on elapsed time
    void update(float deltaTime) {
        if (!paused) {
            elapsedSeconds += deltaTime * timeScale;
            ticCount++;
        } else {
            ticWhilePaused++;
        }
    }

    // Toggle the pause state of the timeline
    void togglePause() {
        paused = !paused;
    }

    // Set the time scale for timeline (speed up or slow down)
    void setTimeScale(float scale) {
        timeScale = scale;
    }

    // Get the elapsed time on the timeline
    float getElapsedTime() const {
        return elapsedSeconds;
    }

    // Get the number of time ticks
    int getTicCount() const {
        return ticCount;
    }

    // Get the number of time ticks during paused state
    int getTicWhilePaused() const {
        return ticWhilePaused;
    }

private:
    float elapsedSeconds;
    float timeScale;
    bool paused;
    int ticCount;
    int ticWhilePaused;
};
class Platform : public sf::ConvexShape {
public:
    Platform(float width, float height, float x, float y, float cornerRadius) {
        setPointCount(8);
        setPoint(0, sf::Vector2f(0, 0));
        setPoint(1, sf::Vector2f(cornerRadius, 0));
        setPoint(2, sf::Vector2f(width - cornerRadius, 0));
        setPoint(3, sf::Vector2f(width, cornerRadius));
        setPoint(4, sf::Vector2f(width, height - cornerRadius));
        setPoint(5, sf::Vector2f(width - cornerRadius, height));
        setPoint(6, sf::Vector2f(cornerRadius, height));
        setPoint(7, sf::Vector2f(0, height - cornerRadius));
        setPosition(x, y);
        setFillColor(sf::Color::Blue);
    }
};

class MovingPlatform : public sf::RectangleShape {
public:
    MovingPlatform(float width, float height, float x, float y, float distance, sf::Texture* texture) {
        setSize(sf::Vector2f(width, height));
        setTexture(texture);
        setPosition(x, y);
        start_x = x;
        end_x = x + distance;
        direction = 1;
    }

    void updatePosition(float deltaTime) {
        float step = moveSpeed * direction * deltaTime;
        float new_x = getPosition().x + step;

        if (new_x < start_x) {
            new_x = start_x;
            direction = 1;
        } else if (new_x > end_x) {
            new_x = end_x;
            direction = -1;
        }

        setPosition(new_x, getPosition().y);
    }

private:
    const float moveSpeed = 200.0f;
    float start_x, end_x;
    int direction;
};

class Character : public sf::RectangleShape {
public:
    Character(float width, float height, float x, float y, sf::Texture* texture) {
        setSize(sf::Vector2f(width, height));
        setTexture(texture);
        setPosition(x, y);
    }

    void moveLeft() {
        move(-moveSpeed, 0);
    }

    void moveRight() {
        move(moveSpeed, 0);
    }

    void jump() {
        if (canJump) {
            velocity.y = -jumpSpeed;
            canJump = false;
        }
    }

    void update(float deltaTime, const std::vector<Platform>& platforms, const MovingPlatform& movingPlatform) {
        velocity.y += gravity * deltaTime;
        move(velocity * deltaTime);

        if (getPosition().y >= groundY) {
            setPosition(getPosition().x, groundY);
            velocity.y = 0;
            canJump = true;
        }

        for (const Platform& platform : platforms) {
            sf::FloatRect characterBounds = getGlobalBounds();
            sf::FloatRect platformBounds = platform.getGlobalBounds();
            sf::FloatRect movingPlatformBounds = movingPlatform.getGlobalBounds();

            if (characterBounds.intersects(platformBounds) && velocity.y >= 0 &&
                characterBounds.top + characterBounds.height - 5 < platformBounds.top) {
                setPosition(getPosition().x, platformBounds.top - characterBounds.height);
                velocity.y = 0;
                canJump = true;
            }
            else if(characterBounds.intersects(movingPlatformBounds) && velocity.y >= 0 &&
                characterBounds.top + characterBounds.height - 5 < movingPlatformBounds.top) {
                setPosition(getPosition().x, movingPlatformBounds.top - characterBounds.height);
                velocity.y = 0;
                canJump = true;
            }
        }
    }

private:
    const float moveSpeed = 2.0f;
    const float jumpSpeed = 400.0f;
    const float gravity = 800.0f;
    const float groundY = 480.0f;
    sf::Vector2f velocity;
    bool canJump = true;
};

class ThreadExample {
    bool busy;
    int i;
    ThreadExample* other;
    std::mutex* _mutex;
    std::condition_variable* _condition_variable;

public:
    ThreadExample(int i, ThreadExample* other, std::mutex* _mutex, std::condition_variable* _condition_variable) {
        this->i = i;
        if (i == 0) { busy = true; }
        else { this->other = other; }
        this->_mutex = _mutex;
        this->_condition_variable = _condition_variable;
    }

    bool isBusy() {
        std::lock_guard<std::mutex> lock(*_mutex);
        return busy;
    }

    void run() {
        if (i == 0) {
            try {
                std::this_thread::sleep_for(std::chrono::milliseconds(4000));
                {
                    std::unique_lock<std::mutex> cv_lock(*this->_mutex);
                    std::cout << "Thread " << i << " is done sleeping" << std::endl;
                    _condition_variable->notify_all();
                    std::cout << "Thread " << i << " is notifying with busy=" << busy << std::endl;
                }
                std::cout << "Thread " << i << " is sleeping" << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(4000));
                {
                    std::unique_lock<std::mutex> cv_lock(*this->_mutex);
                    _condition_variable->notify_all();
                    std::cout << "Thread " << i << " is notifying with busy=" << busy << std::endl;
                }
                std::cout << "Thread " << i << " is sleeping" << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(4000));
                {
                    std::unique_lock<std::mutex> cv_lock(*this->_mutex);
                    busy = !busy;
                    _condition_variable->notify_all();
                    std::cout << "Thread " << i << " is notifying with busy=" << busy << std::endl;
                    std::cout << "Thread " << i << " is FINISHED!" << std::endl;
                }
            }
            catch (...) {
                std::cerr << "Thread " << i << " caught an exception." << std::endl;
            }
        }
        else {
            while (other->isBusy()) {
                std::cout << "Thread " << i << " is waiting!" << std::endl;
                try {
                    std::unique_lock<std::mutex> lock(*_mutex);
                    _condition_variable->wait(lock);
                    std::cout << "Thread " << i << " inside the lock scope." << std::endl;
                }
                catch (...) {
                    std::cerr << "Thread " << i << " caught an exception." << std::endl;
                }
            }
            std::cout << "Thread " << i << " is FINISHED!" << std::endl;
        }
    }
};

void run_wrapper(ThreadExample* fe) {
    fe->run();
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Ascend", sf::Style::Resize);
// Mutex to handle locking, condition variable
    std::mutex m;
    std::condition_variable cv;

    // Create thread objects
    ThreadExample t1(0, nullptr, &m, &cv);
    ThreadExample t2(1, &t1, &m, &cv);

    // Start threads
    std::thread first(run_wrapper, &t1);
    std::thread second(run_wrapper, &t2);

    // These are textures for the character, platform, and moving platform
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Texture characterTexture;
    sf::Texture movingPlatformTexture;

    if (!backgroundTexture.loadFromFile("background.jpg") ||
        !characterTexture.loadFromFile("character.png") ||
        !movingPlatformTexture.loadFromFile("platform.jpg")) {
        std::cerr << "Failed to load image 'background.jpg'" << std::endl;
        // Handle texture loading error
        return 1;
    }

    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setPosition(0, 0);
    float zoomFactor = 0.19f;
    backgroundSprite.setScale(zoomFactor, zoomFactor);

    Platform platform(200.f, 40.f, 300.f, 500.f, 7);
    Platform platform1(200.f, 40.f, 200.f, 250.f, 7);
    Platform platform2(200.f, 40.f, 600.f, 400.f, 7);
    Character character(100.f, 100.f, 20.f, 450.f, &characterTexture);
    MovingPlatform movingPlatform(100.f, 40.f, 400.f, 300.f, 200.f, &movingPlatformTexture);

    std::vector<Platform> platforms = {platform, platform1, platform2};

    sf::Clock clock;
    bool proportionalScaling = false;
    Timeline gameTimeline;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::S) {
                    proportionalScaling = !proportionalScaling;
                    if (proportionalScaling) {
                        window.setView(sf::View(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y)));
                    } else {
                        window.setView(sf::View(sf::FloatRect(0, 0, 800, 600)));
                    }
                } else if (event.key.code == sf::Keyboard::P) {
                    gameTimeline.togglePause();
                } else if (event.key.code == sf::Keyboard::Num1) {
                    gameTimeline.setTimeScale(0.5f);
                } else if (event.key.code == sf::Keyboard::Num2) {
                    gameTimeline.setTimeScale(1.0f);
                } else if (event.key.code == sf::Keyboard::Num3) {
                    gameTimeline.setTimeScale(2.0f);
                }
            }
        }


        // Updating the position of the moving platform

        float deltaTime = clock.restart().asSeconds();
        gameTimeline.update(deltaTime);

        // Character movement based on elapsed time
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            character.moveLeft();
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            character.moveRight();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            character.jump();
        }
        character.update(deltaTime, platforms, movingPlatform);

        // Update the position of the moving platform based on elapsed time
        movingPlatform.updatePosition(deltaTime);

        window.clear();
        window.draw(backgroundSprite);
        window.draw(platform);
        window.draw(platform1);
        window.draw(platform2);
        window.draw(character);
        window.draw(movingPlatform);

        window.display();
    }

    // Wait for threads to finish
    first.join();
    second.join();

    return 0;
}
