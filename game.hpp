#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <optional>
#include <SFML/Graphics.hpp>
#include <map>

constexpr int SIZE = 5;
constexpr int CELL_SIZE = 100;

enum Cell {
    CAT,
    BLOCK,
    EMPTY
};

class Position {
public:
    int row;
    int col;

    bool operator<(const Position& rhs) const {
        if (row == rhs.row) return col < rhs.col;
        return row < rhs.row;
    }
};

class Game {
private:
    std::vector<std::vector<Cell>> board;
    Position cat_spot;
    bool game_end = false;

    sf::RenderWindow& window;
    sf::Texture cat_texture;
    std::optional<sf::Sprite> cat_sprite;

    int current_frame = 0;
    float animation_timer = 0.0;
    float animation_speed = 0.1;

    const int frame_width = 32;
    const int frame_height = 32;
    const int frame_count = 10;

    sf::Texture furniture_texture;
    std::vector<sf::IntRect> furnitureRects;

    //fixme?
    std::map<Position, sf::Sprite> furniture_sprites;


public:
    Game(sf::RenderWindow& win);

    void update(float delta_time);
    void player_click(int x, int y);
    void cat_move();

    bool isGameOver() const { return game_end; }
    bool catEscaped() const;

    void draw();
};

#endif