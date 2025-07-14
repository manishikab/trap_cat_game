#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>

using namespace std;

class Menu {
private:
    sf::RenderWindow& window;
    sf::Font font;
    sf::Text playText;
    sf::Text aboutText;
    sf::Text title;

    bool playSelected = false;
    bool aboutSelected = false;

public:
    Menu(sf::RenderWindow& windowOpen);
    void draw();
    void setupText(sf::Text& text, const string& str, float y);
    bool handleEvent(const sf::Event& event);
    
    bool isPlaySelected();
    bool isAboutSelected();

};

#endif