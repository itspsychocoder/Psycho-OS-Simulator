#include <SFML/Graphics.hpp>
#include "showAlert.h"
void alert(sf::RenderWindow& window, const std::string& message) {
    // Create a font
    sf::Font font;
    if (!font.loadFromFile("D:/Roboto-Bold.ttf")) {
        // Error handling: Font file not found
        return;
    }

    // Create a text object
    sf::Text text(message, font, 24);
    text.setFillColor(sf::Color::White);
    text.setPosition(100, 100); // Adjust position as needed


    // Main loop to display the alert
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        window.draw(text);
        window.display();
    }
}

int showAlert() {
    sf::RenderWindow window(sf::VideoMode(600, 200), "About");
    window.setFramerateLimit(60);

    alert(window, "Psycho OS Made by Hussnain & Majo!");

    return 0;
}