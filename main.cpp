#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <iostream>
#include "game.hpp"
#include "menu.hpp"

enum class GameState { MENU, PLAYING, GAME_OVER, ABOUT };

int main() {
    // fixed screen size
    sf::VideoMode videoMode(sf::Vector2u(800, 800));
    sf::RenderWindow window(videoMode, "Catch the Cat", sf::Style::Titlebar | sf::Style::Close);

    sf::Font font;
    if (!font.openFromFile("OpenSans-Bold.ttf")) {
        std::cerr << "Failed to load font\n";
        return 1;
    }

    Game* game = nullptr;
    GameState state = GameState::MENU;

    sf::Text endText(font);
    endText.setString("");
    endText.setCharacterSize(40);
    endText.setFillColor(sf::Color(139, 69, 90));

    sf::Clock frameClock;
    Menu menu(window);

    while (window.isOpen()) {
        float delta_time = frameClock.restart().asSeconds();

        // if user clicks
        while (auto eventOpt = window.pollEvent()) {
            
            //no click
            if (!eventOpt.has_value()) break;
            const sf::Event& event = *eventOpt;

            if (event.is<sf::Event::Closed>()) {
                window.close();
            }

            if (state == GameState::MENU) {
                if (menu.handleEvent(event)) {
                    if (menu.isPlaySelected()) {
                        // new game
                        game = new Game(window); 
                        state = GameState::PLAYING;
                    } else if (menu.isAboutSelected()) {
                        state = GameState::ABOUT;
                    }
                }
            } else if (state == GameState::ABOUT) {
                // Click anywhere to return to menu
                if (event.is<sf::Event::MouseButtonPressed>()) {
                    state = GameState::MENU;
                }

            } else if (state == GameState::GAME_OVER) {
                // Click anywhere to return to menu
                if (event.is<sf::Event::MouseButtonPressed>()) {
                    delete game;  // free memory
                    game = nullptr; 
                    state = GameState::MENU;
                }

            } else if (state == GameState::PLAYING && game) {
                // move
                if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
                    if (mousePressed->button == sf::Mouse::Button::Left &&
                        !game->won() && !game->lost()) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        if (game->player_click(mousePos.x, mousePos.y)) {
                            game->cat_move(); 
                        }
                    }
                }
            }
        }

        if (state == GameState::PLAYING) {
            window.clear(sf::Color::White);  
        } else {
            window.clear(sf::Color(255, 182, 193));   
        }

        if (state == GameState::MENU) {
            menu.draw();  
        } else if (state == GameState::PLAYING && game) {
            game->update(delta_time);
            game->draw();

            if (game->won()) {
                endText.setString("You Win!\nClick anywhere to return to menu.");
                state = GameState::GAME_OVER;
            } else if (game->lost()) {
                endText.setString("You Lose.\nClick anywhere to return to menu.");
                state = GameState::GAME_OVER;
            }
        } 
        
        else if (state == GameState::GAME_OVER) {
            auto bounds = endText.getLocalBounds();
            auto size = bounds.size;
            endText.setPosition(sf::Vector2f(
                (window.getSize().x - size.x) / 2.f,
                (window.getSize().y - size.y) / 2.f
            ));
            window.draw(endText);
        } else if (state == GameState::ABOUT) {
            sf::Text aboutText(font);
            aboutText.setString("Catch the Cat!\n\nA puzzle game where you try to trap the cat.\nDon't let him get to the edge! \n\nClick anywhere to return.");
            aboutText.setCharacterSize(24);
            aboutText.setFillColor(sf::Color::Black);

            auto bounds = aboutText.getLocalBounds();
            auto size = bounds.size;
            aboutText.setPosition(sf::Vector2f(
                (window.getSize().x - size.x) / 2.f,
                (window.getSize().y - size.y) / 2.f)
            );

            window.draw(aboutText);
        }

        window.display();
    }
    delete game;

    return 0;
}