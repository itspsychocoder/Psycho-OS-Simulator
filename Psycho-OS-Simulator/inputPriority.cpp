#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include "inputPriority.h"
extern "C" int inputPriorityFunc() {
    sf::RenderWindow window(sf::VideoMode(500, 200), "Input Thread Priority (1-7)");

    std::string userInput;  // To store the inputted text
    bool isCreate = false;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Handle key press events for text input
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    char typedChar = static_cast<char>(event.text.unicode);

                    // Check for Enter key to end input
                    if (typedChar == '\r' || typedChar == '\n') {
                        std::cout << "User Input: " << userInput << std::endl;

                        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                        std::wstring inputed = converter.from_bytes(userInput);
                        
                        //std::cout << "Folder Create? " << userInput;
                        //userInput.clear();  // Clear the input for the next entry
                        // convert string to int
                        

                        //window.close();
                        int num = std::stoi(userInput);

                        std::cout << "User Input: " << num << std::endl;


                        return num;
                    }
                    else {
                        userInput += typedChar;
                    }
                }
            }
        }

        window.clear(sf::Color::White);
        // Draw your content here

        // Optionally, you can display the user input on the window
        sf::Font font;
        font.loadFromFile("D:/Roboto-Bold.ttf");

        sf::Text inputText(userInput, font, 20);
        inputText.setPosition(10, 10);
        inputText.setFillColor(sf::Color::Black);
        window.draw(inputText);

        window.display();
    }

    
}