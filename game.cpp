#include "game.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>

Game::Game(sf::RenderWindow& win) : window(win) {
    board = std::vector<std::vector<Cell>>(SIZE, std::vector<Cell>(SIZE, EMPTY));
    cat_spot = { SIZE / 2, SIZE / 2 };
    board[cat_spot.row][cat_spot.col] = CAT;

    srand(static_cast<unsigned>(time(nullptr)));

    if (!cat_texture.loadFromFile("/Users/manishika/desktop/game_test/resources/idle.png")) {
        std::cerr << "Failed to load cat image\n";
    } else {
        cat_sprite.emplace(cat_texture);
        cat_sprite->setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(frame_width, frame_height)));
        cat_sprite->setScale(sf::Vector2f(static_cast<float>(CELL_SIZE) / frame_width, static_cast<float>(CELL_SIZE) / frame_height));
    }

    if (!furniture_texture.loadFromFile("/Users/manishika/desktop/game_test/resources/furniture_edited.png")) {
        std::cerr << "Failed to load furniture image\n";
    }

    for (int i = 0; i < 12; ++i) {
        furnitureRects.push_back(sf::IntRect(sf::Vector2i(i * 64, 0), sf::Vector2i(64, 64)));
    }
}

void Game::update(float delta_time) {
    animation_timer += delta_time;
    if (animation_timer >= animation_speed) {
        animation_timer -= animation_speed;
        current_frame = (current_frame + 1) % frame_count;

        if (cat_sprite) {
            cat_sprite->setTextureRect(sf::IntRect(sf::Vector2i(current_frame * frame_width, 0), sf::Vector2i(frame_width, frame_height)));
        }
    }
}

void Game::player_click(int x, int y) {
    int col = x / CELL_SIZE;
    int row = y / CELL_SIZE;

    if (row < 0 || col < 0 || row >= SIZE || col >= SIZE)
        return;

    if (board[row][col] == EMPTY) {
        board[row][col] = BLOCK;

        sf::IntRect rect = furnitureRects[rand() % furnitureRects.size()];

        sf::Vector2i size = rect.size;

        sf::Sprite furniture(furniture_texture);
        furniture.setTextureRect(rect);
        furniture.setScale(sf::Vector2f(static_cast<float>(CELL_SIZE) / size.x, static_cast<float>(CELL_SIZE) / size.y));
        furniture.setPosition(sf::Vector2f(col * CELL_SIZE, row * CELL_SIZE));

        // Store the sprite in the map to be drawn later
        furniture_sprites.insert({ {row, col}, furniture });
    } else {
        std::cout << "That cell is already occupied. Try again\n";
    }
}

void Game::cat_move() {
    std::vector<Position> possible_moves;

    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            int new_row = cat_spot.row + dr;
            int new_col = cat_spot.col + dc;

            if (new_row >= 0 && new_col >= 0 && new_row < SIZE && new_col < SIZE &&
                board[new_row][new_col] == EMPTY &&
                std::abs(dr) + std::abs(dc) == 1) {
                possible_moves.push_back({new_row, new_col});
            }
        }
    }

    if (possible_moves.empty()) {
        game_end = true; 
        return;
    }

    Position move = possible_moves[rand() % possible_moves.size()];
    board[cat_spot.row][cat_spot.col] = EMPTY;
    cat_spot = move;
    board[cat_spot.row][cat_spot.col] = CAT;
}

bool Game::catEscaped() const {
    return cat_spot.row == 0 || cat_spot.row == SIZE - 1 ||
           cat_spot.col == 0 || cat_spot.col == SIZE - 1;
}

void Game::draw() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
            cell.setPosition(sf::Vector2f(j * CELL_SIZE, i * CELL_SIZE));

            if (board[i][j] == BLOCK) {
                // Draw the furniture only if it's in the map
                auto it = furniture_sprites.find({i, j});
                if (it != furniture_sprites.end()) {
                    window.draw(it->second);  
                }
            } else if (board[i][j] == CAT) {
                if (cat_sprite.has_value()) {
                    cat_sprite->setPosition(sf::Vector2f(j * CELL_SIZE, i * CELL_SIZE));
                    window.draw(cat_sprite.value());
                }
            } else {
                cell.setFillColor(sf::Color::White);
                window.draw(cell);
            }
        }
    }
}