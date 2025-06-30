#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    // VideoMode now takes a Vector2u (width, height)
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 600)), "Image Test");

    sf::Texture texture;
    
    if (!texture.loadFromFile("/Users/manishika/desktop/game_test/resources/happy_cat.png")) {
        std::cerr << "Failed to load image\n";
        return 1;
    }

    sf::Sprite sprite(texture);

    while (window.isOpen()) {
        // pollEvent now returns std::optional<Event>
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear(sf::Color::White);
        window.draw(sprite);
        window.display();
    }

    return 0;
}