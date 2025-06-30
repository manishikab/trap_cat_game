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
    std::map<Position, sf::Sprite> furniture_sprites;

    const int furniture_frame_width = 32;
    const int furniture_frame_height = 32;

std::vector<sf::IntRect> furnitureRects = {
   sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(64, 64)),        // white frame
sf::IntRect(sf::Vector2i(64, 0), sf::Vector2i(64, 64)),       // brown frame
sf::IntRect(sf::Vector2i(128, 0), sf::Vector2i(64, 64)),      // pink scratching post
sf::IntRect(sf::Vector2i(192, 0), sf::Vector2i(64, 64)),      // green scratching post
sf::IntRect(sf::Vector2i(256, 0), sf::Vector2i(64, 64)),      // blue scratching post
sf::IntRect(sf::Vector2i(320, 0), sf::Vector2i(64, 64)),      // blue scratching post base
sf::IntRect(sf::Vector2i(384, 0), sf::Vector2i(64, 64)),      // green scratching post base
sf::IntRect(sf::Vector2i(448, 0), sf::Vector2i(64, 64)),      // pink scratching post base
sf::IntRect(sf::Vector2i(512, 0), sf::Vector2i(64, 64)),      // brown scratching post base
sf::IntRect(sf::Vector2i(576, 0), sf::Vector2i(64, 64)),      // white scratching post base
sf::IntRect(sf::Vector2i(640, 0), sf::Vector2i(64, 64)),      // yellow scratching post base
sf::IntRect(sf::Vector2i(704, 0), sf::Vector2i(64, 64)),      // yellow scratching post
sf::IntRect(sf::Vector2i(768, 0), sf::Vector2i(64, 64)),      // purple scratching post
sf::IntRect(sf::Vector2i(832, 0), sf::Vector2i(64, 64)),      // red scratching post
sf::IntRect(sf::Vector2i(896, 0), sf::Vector2i(64, 64)) 
};


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