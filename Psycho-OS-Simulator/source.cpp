#include <SFML/Graphics.hpp>
#include <iostream>
#include <locale>
#include <codecvt>
extern "C" {
    void openFile(const wchar_t* cstr);
}
using namespace std;
class ClickableElement {

public:
    ClickableElement(const sf::Texture& texture, const sf::Font& font, string comingText, float position, float positionY = 300)
        : sprite(texture), isClicked(false) {
        // Set up sprite and text
        sprite.setPosition(sf::Vector2f(position, positionY));
        sprite.setScale(sf::Vector2f(0.4, 0.4));
        // Set up bounding box for click detection
        boundingBox = sprite.getGlobalBounds();

        // Set up text
        text.setFont(font);
        text.setCharacterSize(20); // Adjust the size as needed

      
        text.setString(comingText);
        

        text.setPosition(sf::Vector2f(position+20, positionY + 120)); // Adjust the position as needed
        text.setFillColor(sf::Color::White);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(sprite);
        window.draw(text);
    }

    bool isMouseOver(const sf::RenderWindow& window) const {
        sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        return boundingBox.contains(mousePosition);
    }
    void singleClick() {
        cout << "Click";
    }
    void handleMouseClick(sf::Texture& folder, sf::Font& font, std::vector<ClickableElement>& clickableElements, int maxFoldersToShow = 6) {
        cout << text.getString().toAnsiString();
        // convert to wstring
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::wstring wide = converter.from_bytes(text.getString().toAnsiString());
        // convert to c string
        const wchar_t* cstr = wide.c_str();
        // call the C function
        openFile(cstr);
        isClicked = true;
       



    }

    void handleMouseRelease() {
        isClicked = false;
    }

    bool wasClicked() const {
        return isClicked;
    }

    sf::Text getText() {
		return text;
	}

private:
    sf::Sprite sprite;
    sf::Text text; // Make sure you have included #include <SFML/Graphics/Text.hpp>
    sf::FloatRect boundingBox;
    bool isClicked;
    std::vector<ClickableElement> clickableElements; // Member vector to store elements

};
int main()
{
    std::cout << "Hello";
    sf::Font font;
    if (!font.loadFromFile("D:/Roboto-Bold.ttf")) {
        // Handle error
        return EXIT_FAILURE;
    }
    sf::Texture folder;
    folder.setSmooth(true);
    if (!folder.loadFromFile("D:/notepad.png"))
    {
        return 1;
    }
   
    folder.setSmooth(true);
    folder.setRepeated(false);

    // Create sprite and set its texture
    sf::Sprite sprite;
    sprite.setTexture(folder);

    // Set the size you want the sprite to appear
    float scaleFactor = 0.5f; // Example: scale the sprite to half of its original size
    sprite.setScale(scaleFactor, scaleFactor);


    std::vector<ClickableElement> clickableElements;

    ClickableElement music(folder, font, "Music", 100);
    ClickableElement notepad(folder, font, "Notepad", 300);
    ClickableElement git(folder, font, "Git", 500);
    clickableElements.emplace_back(music);
    clickableElements.emplace_back(notepad);
    clickableElements.emplace_back(git);
    sf::RenderWindow window(sf::VideoMode(600, 600), "Psycho OS Simulator");
  /*  sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);*/


    


  

    sf::Text aboutOption("About",font, 15);
    aboutOption.setFillColor(sf::Color::White);
    aboutOption.setPosition(10, 10);

    sf::Clock clickClock;
    sf::Time clickCooldown = sf::milliseconds(900); // Adjust the time threshold for a double click
    sf::Vector2i clickPosition;

    sf::Clock clock;


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type) {

            case sf::Event::Closed:
                window.close();
            case sf::Event::MouseButtonPressed:
                for (auto& clickableElement : clickableElements) {

                    if (clickableElement.isMouseOver(window)) {

                        clickableElement.handleMouseClick(folder, font, clickableElements);

                    }
                }
                 }
        }

        window.clear();
        window.draw(aboutOption);

        for (auto& clickableElement : clickableElements) {

            clickableElement.draw(window);
        }
        window.display();
    }

    return 0;
}