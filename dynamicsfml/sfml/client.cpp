#include <zmq.hpp>  // Include the ZeroMQ library for messaging
#include "../shared.hpp"  // Include a custom header file for shared data structures
#include <iostream>  // Include the standard input/output library
#include <SFML/Graphics.hpp>  // Include the Simple and Fast Multimedia Library for graphics

// Defining a function to serialize the platform state
zmq::message_t SerializePlatformState(const PlatformState &platformState) {
    // Creating a message of the same size as the PlatformState struct
    zmq::message_t message(sizeof(PlatformState));
    
    // Copying the platformState data into the message
    memcpy(message.data(), &platformState, sizeof(PlatformState));
    
    // Return the serialized message
    return message;
}

// Defined a function to deserialize the platform state
PlatformState DeserializePlatformState(const zmq::message_t &message) {
    // Create an empty PlatformState object
    PlatformState platformState;
    
    // Copied the data from the received message into the platformState object
    memcpy(&platformState, message.data(), sizeof(PlatformState));
    
    // Returned the deserialized platformState
    return platformState;
}

// Define a function to serialize the character state
zmq::message_t SerializeCharacterState(const CharacterState &characterState) {
    // Create a message of the same size as the CharacterState struct
    zmq::message_t message(sizeof(CharacterState));
    
    // Copy the characterState data into the message
    memcpy(message.data(), &characterState, sizeof(CharacterState));
    
    // Return the serialized message
    return message;
}

// Define a function to deserialize the character state
CharacterState DeserializeCharacterState(const zmq::message_t &message) {
    // Create an empty CharacterState object
    CharacterState characterState;
    
    // Copy the data from the received message into the characterState object
    memcpy(&characterState, message.data(), sizeof(CharacterState));
    
    // Return the deserialized characterState
    return characterState;
}

// Define a function to handle the client-side logic
void HandleClient(int clientId, zmq::socket_t &characterSubscriber, zmq::socket_t &platformSubscriber) {
    // Initialize the Simple and Fast Multimedia Library (SFML) and create a window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Client Window");

    // Create a CharacterState object to represent the client's character
    CharacterState characterState;
    characterState.clientId = clientId;  // Assign a unique client ID
    characterState.x = 100.0f;  // Initial X-coordinate for the character
    characterState.y = 100.0f;  // Initial Y-coordinate for the character

    float moveSpeed = 2.0f;  // Speed at which the character moves
    float jumpStrength = -5.0f;  // Strength of the character's jump
    bool isJumping = false;  // Flag to track if the character is jumping

    // Game loop - the main loop for the client
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Check for window closure events
            if (event.type == sf::Event::Closed)
                window.close();

            // Check for keyboard input events
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Left) {
                    characterState.x -= moveSpeed;  // Move the character to the left
                }
                if (event.key.code == sf::Keyboard::Right) {
                    characterState.x += moveSpeed;  // Move the character to the right
                }
                if (event.key.code == sf::Keyboard::Space && !isJumping) {
                    characterState.y += jumpStrength;  // Make the character jump
                    isJumping = true;  // Set the jump flag
                }
            }
        }

        // Apply gravity when the character is in the air
        if (characterState.y < 100.0f) {
            characterState.y += 1.0f;
        } else {
            isJumping = false;  // Reset the jump flag when the character lands
        }

        // Receive character state updates from the server
        zmq::message_t characterMessage;
        characterSubscriber.recv(characterMessage);

        // Deserialize the received character state
        characterState = DeserializeCharacterState(characterMessage);

        // Receive platform state updates from the server
        zmq::message_t platformMessage;
        platformSubscriber.recv(platformMessage);

        // Deserialize the received platform state
        PlatformState platformState = DeserializePlatformState(platformMessage);

        // Clear the window for rendering
        window.clear();

        // Create a platform rectangle and set its position
        sf::RectangleShape platform(sf::Vector2f(100.0f, 10.0f));
        platform.setPosition(platformState.x, platformState.y);

        // Render the platform
        window.draw(platform);

        // Create a character circle, set its color and position
        sf::CircleShape character(20.0f);
        character.setFillColor(sf::Color::Green);
        character.setPosition(characterState.x, characterState.y);

        // Render the character
        window.draw(character);

        // Display the rendered scene
        window.display();
    }
}

// Main function to start the client
int main() {
    zmq::context_t context(1);  // Create a ZeroMQ context

    // Create a subscriber socket for character updates and connect it to the server
    zmq::socket_t characterSubscriber(context, ZMQ_SUB);
    characterSubscriber.connect("tcp://localhost:5555");

    // Subscribe to all messages
    characterSubscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);

    // Create a subscriber socket for platform updates and connect it to the server
    zmq::socket_t platformSubscriber(context, ZMQ_SUB);
    platformSubscriber.connect("tcp://localhost:5556");

    // Subscribe to all messages
    platformSubscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);

    int clientId;
    // ...

    // Start the client's main loop
    HandleClient(clientId, characterSubscriber, platformSubscriber);

    return 0;
}
