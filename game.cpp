#include "game.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

Game::Game(sf::RenderWindow& win) : window(win) {
    board = std::vector<std::vector<Cell>>(SIZE, std::vector<Cell>(SIZE, EMPTY));
    cat_spot = { SIZE / 2, SIZE / 2 };
    board[cat_spot.row][cat_spot.col] = CAT;

    srand(static_cast<unsigned>(time(nullptr)));

    if (!cat_texture.loadFromFile("/Users/manishika/desktop/game_test/resources/idle.png")) {
        std::cerr << "Failed to load cat image\n";
    } else {
        cat_sprite = sf::Sprite(cat_texture);
        cat_sprite->setScale(sf::Vector2f(
            static_cast<float>(CELL_SIZE) / cat_texture.getSize().x,
            static_cast<float>(CELL_SIZE) / cat_texture.getSize().y
        ));
    }
}

void Game::player_click(int x, int y) {
    int col = x / CELL_SIZE;
    int row = y / CELL_SIZE;

    if (row < 0 || col < 0 || row >= SIZE || col >= SIZE)
        return;

    if (board[row][col] == EMPTY) {
        board[row][col] = BLOCK;
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
                cell.setFillColor(sf::Color::Black);
                window.draw(cell);
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