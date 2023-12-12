#include <zmq.hpp>  // Include the ZeroMQ C++ bindings
#include <thread>   // Include the C++ standard thread library
#include <vector>   // Include the C++ standard vector container
#include <iostream> // Include the C++ standard input and output library
#include <mutex>    // Include the C++ standard mutex for thread synchronization
#include <chrono>   // Include the C++ standard chrono library for time handling
#include <random>   // Include the C++ random library for generating random numbers

//I defined a data structure to store character states
struct Character {
    int clientId;  // I initialized an identifier for each client
    float x;       // X-coordinate of the character
    float y;       // Y-coordinate of the character
};

// Defined a data structure to store platform state
struct Platform {
    float x; // X-coordinate of the platform
    float y; // Y-coordinate of the platform
};

// Here is a Vector to store character states
std::vector<Character> characters;

// Platform state
Platform platform;

// Mutex to protect the characters vector and platform
std::mutex charactersMutex;
std::mutex platformMutex;

// Function to handle each client in a separate thread
void HandleClient(zmq::socket_t& pullSocket, zmq::socket_t& publisher, int clientId) {
    // Initialize the character state for this client
    Character character;
    character.clientId = clientId;
    character.x = 100.0f;
    character.y = 100.0f;

    // Initialize the platform state
    Platform localPlatform;
    localPlatform.x = 250.0f;
    localPlatform.y = 400.0f;

    while (true) {
        // Update character state based on game logic
        character.x += 2.0f;
        if (character.x >= 400.0f) character.x = 100.0f;
        character.y += 1.0f;
        if (character.y >= 200.0f) character.y = 100.0f;

        // Update the platform state (simulating moving platforms)
        {
            std::lock_guard<std::mutex> lock(platformMutex);
            platform = localPlatform;
        }

        // Send the updated character state and platform state to the clients
        zmq::message_t characterMessage(sizeof(Character));
        zmq::message_t platformMessage(sizeof(Platform));

        {
            std::lock_guard<std::mutex> lock(charactersMutex);
            memcpy(characterMessage.data(), &character, sizeof(Character));
        }

        {
            std::lock_guard<std::mutex> lock(platformMutex);
            memcpy(platformMessage.data(), &platform, sizeof(Platform));
        }

        publisher.send(characterMessage, zmq::send_flags::none);
        publisher.send(platformMessage, zmq::send_flags::none);

        // Simulate game tick rate
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

int main() {
    zmq::context_t context(1);  // Create a ZeroMQ context with a single I/O thread
    zmq::socket_t publisher(context, ZMQ_PUB);  // Create a publisher socket
    publisher.bind("tcp://*:5555");  // Bind the publisher to an address and port

    zmq::socket_t pull(context, ZMQ_PULL);  // Create a pull socket for receiving client updates
    pull.bind("tcp://*:5556");  // Bind the pull socket to an address and port

    std::vector<std::thread> clientThreads;  // Vector to store client threads

    // Create threads for three clients (you can change the number of clients)
    for (int i = 1; i <= 3; ++i) {
        // Create a thread for each client, passing the pull socket, publisher socket, and a client ID
        clientThreads.emplace_back(HandleClient, std::ref(pull), std::ref(publisher), i);
    }

    // Game loop
    while (true) {
        // Simulate platform movement
        {
            std::lock_guard<std::mutex> lock(platformMutex);
            platform.x += 1.0f;
            if (platform.x >= 400.0f) platform.x = 100.0f;
        }

        // Broadcast platform state to clients
        zmq::message_t platformMessage(sizeof(Platform));
        {
            std::lock_guard<std::mutex> lock(platformMutex);
            memcpy(platformMessage.data(), &platform, sizeof(Platform));
        }
        publisher.send(platformMessage, zmq::send_flags::none);

        // Simulate game tick rate
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    // Wait for client threads to finish
    for (std::thread& thread : clientThreads) {
        thread.join();
    }

    return 0;
}
