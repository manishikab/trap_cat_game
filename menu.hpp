#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>

using namespace std;

class Menu {
private:
    sf::RenderWindow& window;
    sf::Font font;
    std::optional<sf::Text> playText;
    std::optional<sf::Text> aboutText;


    bool playSelected = false;
    bool aboutSelected = false;

public:
    Menu(sf::RenderWindow& windowOpen);
    void draw();
    //fix me
    void setupText(sf::Text& text, const string& str, float y);
    bool handleEvent(const sf::Event& event);
    
    bool isPlaySelected() const;
    bool isAboutSelected() const;
};

#endif