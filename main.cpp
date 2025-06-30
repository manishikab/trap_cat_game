#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include "game.hpp"

enum class GameState { MENU, PLAYING, GAME_OVER };

int main() {
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(SIZE * CELL_SIZE, SIZE * CELL_SIZE)), "Catch the Cat");

    sf::Font font;
    if (!font.openFromFile("arial.ttf")) {
        std::cerr << "Failed to load font\n";
        return 1;
    }

    std::unique_ptr<Game> game;
    GameState state = GameState::MENU;

    sf::Text menuText(font);
    menuText.setString("Click to Start");
    menuText.setCharacterSize(40);    
    menuText.setFillColor(sf::Color::Blue);

    sf::Text endText(font);
    endText.setString("");
    endText.setCharacterSize(40);
    endText.setFillColor(sf::Color::Red);

    sf::Clock endClock;
    bool endClockStarted = false;

    while (window.isOpen()) {
        while (auto eventOpt = window.pollEvent()) {
            if (!eventOpt.has_value()) break;
            const sf::Event& event = *eventOpt;

            if (event.is<sf::Event::Closed>()) {
                window.close();
            }

            if (state == GameState::MENU) {
    if (event.is<sf::Event::MouseButtonPressed>()) {
        std::cout << "Starting game, switching to PLAYING state\n";
        game = std::make_unique<Game>(window);
        state = GameState::PLAYING;
        endClockStarted = false;
        endText.setString("");
    }


            } else if (state == GameState::PLAYING && game) {
                if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
                    if (mousePressed->button == sf::Mouse::Button::Left &&
                        !game->isGameOver() && !game->catEscaped()) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        game->player_click(mousePos.x, mousePos.y);
                        game->cat_move();
                    }
                }
            }
        }

       window.clear(sf::Color::White);

if (state == GameState::MENU) {
    window.draw(menuText);
} else if (state == GameState::PLAYING && game) {
    game->draw();

    sf::Text playingText(font);
    playingText.setString("Game Running...");
    playingText.setCharacterSize(30);
    playingText.setFillColor(sf::Color::Green);
    playingText.setPosition(sf::Vector2f(10, 10));
    window.draw(playingText);

    if (game->isGameOver()) {
        endText.setString("You Win!");
        state = GameState::GAME_OVER;
        endClock.restart();
        endClockStarted = true;
    } else if (game->catEscaped()) {
        endText.setString("You Lose.");
        state = GameState::GAME_OVER;
        endClock.restart();
        endClockStarted = true;
    }
} else if (state == GameState::GAME_OVER) {
    window.draw(endText);

    if (!endClockStarted) {
        endClock.restart();
        endClockStarted = true;
    }

    if (endClock.getElapsedTime().asSeconds() > 5.0) {
        game.reset();
        state = GameState::MENU;
        endClockStarted = false;
        endText.setString("");
    }
}

window.display();
    }

    return 0;
}