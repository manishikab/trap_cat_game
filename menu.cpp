#include "menu.hpp"
#include <iostream>

Menu::Menu(sf::RenderWindow& windowOpen)
: window(windowOpen), playText(font), aboutText(font), title(font)
{
    if (!font.openFromFile("OpenSans-Bold.ttf")) {
        std::cerr << "Failed to load font.\n";
    }

    playText.setString("Play");
    playText.setCharacterSize(48);
    playText.setFillColor(sf::Color::Black);

    aboutText.setString("About");
    aboutText.setCharacterSize(48);
    aboutText.setFillColor(sf::Color::Black);

    title.setString("Catch the Cat!");
    title.setCharacterSize(64);
    title.setFillColor(sf::Color(139, 69, 90));

    setupText(playText, "Play", 400.f);
    setupText(aboutText, "About", 500.f);
    setupText(title, "Catch the Cat!", 250.f);
}

void Menu::setupText(sf::Text& text, const std::string& str, float y) {
    auto bounds = text.getLocalBounds();
    auto size = bounds.size;
    text.setPosition(sf::Vector2f((window.getSize().x - size.x) / 2.f, y));
}

void Menu::draw() {
    window.draw(title);
    window.draw(playText);
    window.draw(aboutText);
}

bool Menu::handleEvent(const sf::Event& event) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorldPos(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

// change color on hover
    if (playText.getGlobalBounds().contains(mouseWorldPos)) {
        playText.setFillColor(sf::Color(139, 69, 90));
    } 
    else {
        playText.setFillColor(sf::Color::Black);
    }

    if (aboutText.getGlobalBounds().contains(mouseWorldPos)){
        aboutText.setFillColor(sf::Color(139, 69, 90));
    }
    else {
        aboutText.setFillColor(sf::Color::Black);
    }

// click to change screen
    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mouseWorldPos(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

        if (playText.getGlobalBounds().contains(mouseWorldPos)) {
            playSelected = true;
            return true;
        }
        if (aboutText.getGlobalBounds().contains(mouseWorldPos)) {
            aboutSelected = true;
            return true;
        }
    }
    return false;
}

bool Menu::isPlaySelected() {
    bool temp = playSelected;
    playSelected = false;
    return temp;
}

bool Menu::isAboutSelected() {
    bool temp = aboutSelected;
    aboutSelected = false;
    return temp;
}