#include <SFML/Graphics.hpp>
#include <iostream>
#include <locale>
#include <codecvt>
#include "showAlert.h"
extern "C" {
    void openFile(const wchar_t* cstr);
}
using namespace std;
class ClickableElement {

public:
    ClickableElement(const sf::Texture& texture, const sf::Font& font, string comingText, float position, float positionY)
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
    std::cout << "Welcome to Psycho OS";
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
    sf::Texture music;
    music.setSmooth(true);
    if (!music.loadFromFile("D:/music-player.png"))
    {
        return 1;
    }
    sf::Texture git;
    git.setSmooth(true);
    if (!git.loadFromFile("D:/git.png"))
    {
        return 1;
    }

    sf::Texture chrome;
    chrome.setSmooth(true);
    if (!chrome.loadFromFile("D:/chrome.png"))
    {
        return 1;
    }
    
    sf::Texture stopwatch;
    stopwatch.setSmooth(true);
    if (!stopwatch.loadFromFile("D:/stopwatch.png"))
    {
        return 1;
    }





 

    std::vector<ClickableElement> clickableElements;

    ClickableElement mus(music, font, "Music", 50, 100);
    ClickableElement notepad(folder, font, "Notepad", 250, 100);
    ClickableElement gitHub(git, font, "Git", 450, 100);
    ClickableElement Chrome(chrome, font, "Chrome", 50, 300);
    ClickableElement Stopwatch(stopwatch, font, "Stop Watch", 250, 300);
    clickableElements.emplace_back(mus);
    clickableElements.emplace_back(notepad);
    clickableElements.emplace_back(gitHub);
    clickableElements.emplace_back(Chrome);
    clickableElements.emplace_back(Stopwatch);
    sf::RenderWindow window(sf::VideoMode(600, 600), "Psycho OS Simulator");
  /*  sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);*/


    


  

    sf::Text aboutOption("About",font, 15);
    aboutOption.setFillColor(sf::Color::White);
    aboutOption.setPosition(10, 10);

    sf::Texture off;
    off.setSmooth(true);
    if (!off.loadFromFile("D:/off.png"))
    {
        return 1;
    }
    ClickableElement offBtn(off, font, "", 500, 10);



    sf::Clock clickClock;
    sf::Time clickCooldown = sf::milliseconds(900); // Adjust the time threshold for a double click
    sf::Vector2i clickPosition;

    sf::Clock clock;
    bool mouseClicked = false;



    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type) {

            case sf::Event::Closed:
                window.close();
            case sf::Event::MouseButtonPressed:
                sf::Vector2f mousePosition(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                if (aboutOption.getGlobalBounds().contains(mousePosition)) {
                    if (!mouseClicked) {
                        showAlert();
                       
                    }
                }

                if(offBtn.isMouseOver(window)){
					window.close();
                };
                for (auto& clickableElement : clickableElements) {

                    if (clickableElement.isMouseOver(window)) {

                        clickableElement.handleMouseClick(folder, font, clickableElements);

                    }
                }
                 }
        }

        window.clear();
        window.draw(aboutOption);
        offBtn.draw(window);

        for (auto& clickableElement : clickableElements) {

            clickableElement.draw(window);
        }
        window.display();
    }

    return 0;
}