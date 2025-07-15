#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <optional>
#include <SFML/Graphics.hpp>
#include <map>

const int SIZE = 10;
const int CELL_SIZE = 64;

enum Cell {
    CAT,
    BLOCK,
    EMPTY
};

class Position {
public:
    int row;
    int col;

    bool operator==(const Position& rhs) const {
        return row == rhs.row && col == rhs.col;
    }

    bool operator!=(const Position& rhs) const {
        return !(*this == rhs);
    }

    bool operator<(const Position& rhs) const {
        if (row == rhs.row){
            return col < rhs.col;
        }
        return row < rhs.row;
    }
};

class Game {
private:
    std::vector<std::vector<Cell>> board;
    Position cat_spot;

   

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

    std::map<Position, sf::Sprite> furniture_sprites;

    float x_offset = 0.0f;
    float y_offset = 0.0f;
    
public:
    Game(sf::RenderWindow& win);

    void update(float delta_time);
    bool player_click(int x, int y);
    void cat_move();

    bool won(){return game_won;}
    bool lost() {return game_lost;}

    bool catEscapedFrom(const Position& pos) const;
    bool catEscaped() const;

    void draw();

    bool game_lost = false;
    bool game_won = false;
    bool loss_pending = false;
    float loss_delay_timer = 0.0f;
};

#endif