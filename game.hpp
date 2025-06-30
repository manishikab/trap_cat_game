#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <optional>
#include <SFML/Graphics.hpp>

constexpr int SIZE = 5;
constexpr int CELL_SIZE = 100;

enum Cell {
    CAT,
    BLOCK,
    EMPTY
};

struct Position {
    int row;
    int col;
};

class Game {
private:
    std::vector<std::vector<Cell>> board;
    Position cat_spot;
    bool game_end = false;

    sf::RenderWindow& window;
    sf::Texture cat_texture;
    std::optional<sf::Sprite> cat_sprite;

public:
    explicit Game(sf::RenderWindow& win);

    void player_click(int x, int y);
    void cat_move();

    bool isGameOver() const { return game_end; }
    bool catEscaped() const;

    void draw();
};

#endif