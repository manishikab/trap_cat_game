#include <iostream>
#include <vector>

using namespace std;

const int SIZE = 5;

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
    public:
        Game(){ 
            board = vector<vector<Cell>>(SIZE, std::vector<Cell>(SIZE, EMPTY));
            cat_spot = {SIZE/2, SIZE/2};
            board[cat_spot.row][cat_spot.col] = CAT;
            // random gen
            srand(time(nullptr));
        }

        void print_board(){
            cout << "\n=== BOARD ===\n";
            for (int i = 0; i < SIZE; ++i) {
                for (int j = 0; j < SIZE; ++j) {
                    if (i == cat_spot.row && j == cat_spot.col){
                        cout << "c";
                    } 
                    else if (board[i][j] == BLOCK) {
                        cout << "#";
                    }
                    else {
                        cout << ".";
                    }
                }
                cout << endl;
            }
        }

        void player_move(){
            int row;
            int col;

            cout << "Place a block-- enter row and column number separated by a space:";
            cin >> row >> col;

            if (row < 0 || col < 0 || row >= SIZE || col >= SIZE){
                cout << "Invalid move.";
                player_move();
            }
            else {
                board[row][col] = BLOCK;
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
                    && board[new_row][new_col] == EMPTY){
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

        void run(){
            while (!game_end){
                print_board();
                if (has_escaped()) {
                    std::cout << "The cat has escaped! You lose.\n";
                return;
                }

                player_move();
                cat_move();
            }
            if (game_end){
                cout << "You caught the cat!";
            }
        }
};
