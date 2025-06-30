#include "menu.hpp"
#include <iostream>

Menu::Menu(sf::RenderWindow& windowOpen) 
: window(windowOpen) {
    if (!font.openFromFile("arial.ttf")) {
        cerr << "Failed to load font.\n";
    }
    setupText(*playText, "Play", 200.f);
    setupText(*aboutText, "About", 300.f);
}

void Menu::setupText(sf::Text& text, const std::string& str, float y) {
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(48);
    text.setFillColor(sf::Color::Black);

    auto size = text.getLocalBounds().size;
    text.setPosition(sf::Vector2f((window.getSize().x - size.x) / 2, y)); 
}

void Menu::draw() {
    window.clear(sf::Color::White);
    window.draw(*playText);
    window.draw(*aboutText);
    window.display();
}

bool Menu::handleEvent(const sf::Event& event) {
    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePressed->button == sf::Mouse::Button::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f mouseWorldPos(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

            if (playText && playText->getGlobalBounds().contains(mouseWorldPos)) {
                playSelected = true;
                return true;
            }
            if (aboutText && aboutText->getGlobalBounds().contains(mouseWorldPos)) {
                aboutSelected = true;
                return true;
            }
        }
    }
    return false;
}

bool Menu::isPlaySelected() const {
    return playSelected;
}

bool Menu::isAboutSelected() const {
    return aboutSelected;
}