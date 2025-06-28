#include <iostream>
#include <vector>

#include <cstdlib>
#include <ctime>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


using namespace std;

const int SIZE = 5;
const int CELL_SIZE = 100;

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
        vector<vector<Cell>> board;
        Position cat_spot;
        bool game_end = false;
        sf::RenderWindow window;

    public:
    Game() 
    : window(sf::VideoMode(sf::Vector2u(SIZE * CELL_SIZE, SIZE * CELL_SIZE)), "Catch the Cat") {
    board = vector<vector<Cell>>(SIZE, vector<Cell>(SIZE, EMPTY));
    cat_spot.row = SIZE / 2;
    cat_spot.col = SIZE / 2;
    board[cat_spot.row][cat_spot.col] = CAT;
    srand(time(nullptr));
}


        void print_board(){
            window.clear(sf::Color::White);

            for (int i = 0; i < SIZE; ++i) {
                for (int j = 0; j < SIZE; ++j) {
                    sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                    cell.setPosition(sf::Vector2f(j * CELL_SIZE, i * CELL_SIZE));      

                    if (board[i][j] == BLOCK){
                        cell.setFillColor(sf::Color::Black);
                    }
                    else if (board[i][j] == CAT) {
                        cell.setFillColor(sf::Color::Yellow);
                    }
                    else {
                        cell.setFillColor(sf::Color::White);
                    }

                    window.draw(cell);
            }
        }
        window.display();
    }

/**
        void player_move(){
            int row;
            int col;

            while (true) {
                cout << "Place a block! Enter row and column number separated by a space: ";
                cin >> row >> col;
        
                if (row < 0 || col < 0 || row >= SIZE || col >= SIZE) {
                    cout << "Invalid move. Try again." << endl;
                } else if (board[row][col] != EMPTY) {
                    cout << "That cell is already occupied. Try again" << endl;
                } else {
                    board[row][col] = BLOCK;
                    break;
                }
        }
    }
*/

    void player_click(int x, int y){
        int col = x / CELL_SIZE;
        int row = y / CELL_SIZE;

        if (col < 0 || row >= SIZE || col < 0 || col >= SIZE){
            return;
        }

        if (board[row][col] == EMPTY){
            board[row][col] = BLOCK;
        }
        else {
            cout << "That cell is already occupied. Try again" << endl;
        }
    }

        void cat_move(){
            vector<Position>possible_moves;
            
            // current spot of cat is (0,0)
            for (int row = -1; row <= 1; row++){
                for (int col = -1; col <= 1; col++){

                    int new_row = cat_spot.row + row;
                    int new_col = cat_spot.col + col;

                    if (new_row >= 0 && new_col >= 0 
                    && new_row < SIZE && new_col < SIZE
                    && board[new_row][new_col] == EMPTY
                    && abs(row) + abs(col) == 1)
                {
                        possible_moves.push_back({new_row, new_col});
                    }
                }
            }
            
            if (possible_moves.empty()){
                game_end = true;
                return;
            }
            
            Position move = possible_moves[rand() % possible_moves.size()];
            board[cat_spot.row][cat_spot.col] = EMPTY;
            cat_spot = move;
            board[cat_spot.row][cat_spot.col] = CAT;
        }

        bool has_escaped() {
            return (cat_spot.row == 0 || cat_spot.row == SIZE - 1 || cat_spot.col == 0 || cat_spot.col == SIZE - 1);
        }

        void run() {

        // from sfml website (updated event)
            while (window.isOpen()){
                while (const std::optional event = window.pollEvent()){
                    if (event->is<sf::Event::Closed>()){
                        window.close();
                    }
                    else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape){
                            window.close();
                    }
                }
                else if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()){
                    if (mousePressed->button == sf::Mouse::Button::Left){
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        player_click(mousePos.x, mousePos.y);

                        cat_move();

                        if (has_escaped()){
                            window.clear();
                
                            sf::Font font;
                
                            if (!font.openFromFile("arial.ttf")) {
                                cerr << "Failed to load font.\n";
                                return;
                            }
                
                            sf::Text text(font, "You Lose.", 24);
                            window.draw(text);
                            window.display();
                            sf::sleep(sf::seconds(5));
                            return;
                        }

                    }
                }

            }



            print_board();

        
        }

        

    }


};
