#include "game.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>

#include <set>
#include <map>


Game::Game(sf::RenderWindow& w) : window(w), game_won(false), game_lost(false), loss_pending(false) {
    board = std::vector<std::vector<Cell>>(SIZE, std::vector<Cell>(SIZE, EMPTY));
    cat_spot = { SIZE/2, SIZE/2 };
    board[cat_spot.row][cat_spot.col] = CAT;

    float grid_width = SIZE * CELL_SIZE;
    float grid_height = SIZE * CELL_SIZE;

    // center on screen
    float x_offset = (window.getSize().x - grid_width) / 2.0f;
    float y_offset = (window.getSize().y - grid_height) / 2.0f;

    // random gen
    srand(static_cast<unsigned>(time(nullptr)));

    if (!cat_texture.loadFromFile("resources/idle.png")) {
        std::cerr << "Failed to load cat image\n";
    } else {
        cat_sprite.emplace(cat_texture);
        cat_sprite->setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(frame_width, frame_height)));
        cat_sprite->setScale(sf::Vector2f(static_cast<float>(CELL_SIZE) / frame_width, static_cast<float>(CELL_SIZE) / frame_height));
    }

    if (!furniture_texture.loadFromFile("resources/furniture_edited.png")) {
        std::cerr << "Failed to load furniture image\n";
    }

    for (int i = 0; i < 12; ++i) {
        furnitureRects.push_back(sf::IntRect(sf::Vector2i(i * 64, 0), sf::Vector2i(64, 64)));
    }

    // place random blocks
    for (int i = 0; i < 10; ++i) {
    int r = rand() % SIZE;
    int c = rand() % SIZE;

    // Avoid placing on the cat or on other blocks
    if ((r == cat_spot.row && c == cat_spot.col) || board[r][c] != EMPTY) {
        --i;
        continue;
    }

    board[r][c] = BLOCK;

    sf::IntRect rect = furnitureRects[rand() % furnitureRects.size()];
    sf::Vector2i size = rect.size;

    sf::Sprite furniture(furniture_texture);
    furniture.setTextureRect(rect);
    furniture.setScale(sf::Vector2f(static_cast<float>(CELL_SIZE) / size.x, static_cast<float>(CELL_SIZE) / size.y));
    furniture.setPosition(sf::Vector2f(c * CELL_SIZE + x_offset, r * CELL_SIZE + y_offset));

    furniture_sprites.insert({ {r, c}, furniture });
    }
}

//cat tail animation
void Game::update(float delta_time) {
    animation_timer += delta_time;
    if (animation_timer >= animation_speed) {
        animation_timer -= animation_speed;
        current_frame = (current_frame + 1) % frame_count;

        if (cat_sprite) {
            cat_sprite->setTextureRect(sf::IntRect(sf::Vector2i(current_frame * frame_width, 0), sf::Vector2i(frame_width, frame_height)));
        }
    }

// fixed bug: you lose/win screen comes up too fast
    if (loss_pending) {
        loss_delay_timer += delta_time;
        if (loss_delay_timer > 0.3f) {  
            game_lost = true;
            loss_pending = false;
        }
    }
}


bool Game::player_click(int x, int y) {
    int col = (x - static_cast<int>(x_offset)) / CELL_SIZE;
    int row = (y - static_cast<int>(y_offset)) / CELL_SIZE;

    if (row < 0 || col < 0 || row >= SIZE || col >= SIZE)
        return false;

    if (board[row][col] == EMPTY) {
        board[row][col] = BLOCK;

        sf::IntRect rect = furnitureRects[rand() % furnitureRects.size()];
        sf::Vector2i size = rect.size;

        sf::Sprite furniture(furniture_texture);
        furniture.setTextureRect(rect);
        furniture.setScale(sf::Vector2f(static_cast<float>(CELL_SIZE) / size.x, static_cast<float>(CELL_SIZE) / size.y));
        furniture.setPosition(sf::Vector2f(col * CELL_SIZE + x_offset, row * CELL_SIZE + y_offset)); // Offset needed here too

        furniture_sprites.insert({ {row, col}, furniture });
        return true;
    } 
    else {
        std::cout << "That cell is already occupied. Try again\n";
        return false;
    }
}


// new: pathfinding, BFS
void Game::cat_move() {
    std::queue<Position> q;
    std::map<Position, Position> came_from;
    std::set<Position> visited;

    q.push(cat_spot);
    visited.insert(cat_spot);

    bool found_exit = false;
    Position exit_spot;

    while (!q.empty()) {
        Position current = q.front();
        q.pop();

        if (catEscapedFrom(current)) {
            found_exit = true;
            exit_spot = current;
            break;
        }

        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                // no diagonal moves
                if (std::abs(dr) + std::abs(dc) != 1){
                    continue;
                }

                Position neighbor = { current.row + dr, current.col + dc };

                if (neighbor.row >= 0 && neighbor.col >= 0 &&
                    neighbor.row < SIZE && neighbor.col < SIZE &&
                    board[neighbor.row][neighbor.col] == EMPTY &&
                    visited.find(neighbor) == visited.end()) {

                    visited.insert(neighbor);
                    came_from[neighbor] = current;
                    q.push(neighbor);
                }
            }
        }
    }

    if (!found_exit) {
        game_won = true;
        return;
    }

    // Reconstruct path
    Position step = exit_spot;
    while (came_from[step] != cat_spot) {
        step = came_from[step];
    }

    board[cat_spot.row][cat_spot.col] = EMPTY;
    cat_spot = step;
    board[cat_spot.row][cat_spot.col] = CAT;

    if (catEscaped()) {
        loss_pending = true;
    }
}

bool Game::catEscapedFrom(const Position& pos) const {
    return pos.row == 0 || pos.row == SIZE - 1 ||
           pos.col == 0 || pos.col == SIZE - 1;
}

bool Game::catEscaped() const {
    return cat_spot.row == 0 || cat_spot.row == SIZE - 1 ||
           cat_spot.col == 0 || cat_spot.col == SIZE - 1;
}

void Game::draw() {
    float grid_width = SIZE * CELL_SIZE;
    float grid_height = SIZE * CELL_SIZE;
    x_offset = (window.getSize().x - grid_width) / 2.0f;
    y_offset = (window.getSize().y - grid_height) / 2.0f;


    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
            cell.setPosition(sf::Vector2f(j * CELL_SIZE + x_offset, i * CELL_SIZE + y_offset));

            
            cell.setOutlineThickness(2);
            cell.setOutlineColor(sf::Color(139, 69, 90));  
            window.draw(cell);  
            if (board[i][j] == BLOCK) {
                auto it = furniture_sprites.find({i, j});
                if (it != furniture_sprites.end()) {
                    window.draw(it->second);  
                }
            } else if (board[i][j] == CAT) {
                if (cat_sprite.has_value()) {
                    cat_sprite->setPosition(sf::Vector2f(j * CELL_SIZE + x_offset, i * CELL_SIZE + y_offset));
                    window.draw(cat_sprite.value());
                }
            } else {
                cell.setFillColor(sf::Color::White);
                window.draw(cell);
            }


        }
    }
}