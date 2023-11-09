#include <iostream>
#include "geesespotter_lib.h"

char *create_board(std::size_t x_dim, std::size_t y_dim){
    char *board{ new char[(x_dim*y_dim)]};
    for (int i{}; i < (x_dim*y_dim); ++i){
        board[i] = 0;
    }
    return board;
}

void clean_board(char *board){
    delete[] board; 
    board = nullptr;
}


void print_board(char *board, std::size_t x_dim, std::size_t y_dim){
    for (int j{}; j < (x_dim*y_dim); ++j){
        if ((board[j] & marked_mask()) == marked_mask()){
            std::cout << 'M';
        }
        else if((board[j] & hidden_mask()) == hidden_mask()){
            std::cout << "*";
        }
        else {
            std::cout << (board[j] & value_mask());
        }
        
    
        
        if (((j+1)%x_dim) == 0){
            std::cout<<std::endl;
        }
    }

}

void hide_board(char *board, std::size_t x_dim, std::size_t y_dim){
    for (int i{}; i<(x_dim*y_dim); ++i){
        board[i] = (board[i] | hidden_mask());
    }
}

int mark(char *board, std::size_t x_dim, std::size_t y_dim, std::size_t x_loc, std::size_t y_loc){
    if((board[(((y_loc)*x_dim) + x_loc)] & hidden_mask()) == hidden_mask()){
        board[(((y_loc)*x_dim) + x_loc)] = (board[(((y_loc)*x_dim) + x_loc)] ^ marked_mask());
        return 0;
    }
    else if((board[(((y_loc)*x_dim) + x_loc)] & hidden_mask()) != hidden_mask()){
        return 2;
    }
}

void compute_neighbours(char * board, std::size_t x_dim, std::size_t y_dim){
    int row_movement[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int col_movement[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int i = 0; i < x_dim * y_dim; ++i){
        int counter{};
        int x_pos = i;
        int y_pos{};
        if (board[i] != 9){
            if (i > x_dim-1){
                while (x_pos > x_dim-1){
                    x_pos -= x_dim;
                    ++y_pos; 
                }
            }

            for (int s = 0; s < 8; ++s){
                int new_x = x_pos + col_movement[s];
                int new_y = y_pos + row_movement[s];
                if (new_x >= 0 && new_x < x_dim && new_y >= 0 && new_y < y_dim){
                    int new_index = new_y * x_dim + new_x;
                    if (board[new_index] == 9){
                        ++counter;
                    }
                }
            }
        board[i] ^= counter;
        }
        
    }
        
}

bool is_game_won(char * board, std::size_t x_dim, std::size_t y_dim){
    int counter{};
    for (int i{}; i < x_dim*y_dim; ++i){
        if (((board[i] & value_mask()) != 9)){
            ++counter; 
        }
    }
    if ((x_dim*y_dim) == counter){
        return true;
    }

    for (int i{}; i < x_dim*y_dim; ++i){
        if (((board[i] & value_mask()) != 9) && ((board[i] & hidden_mask()) == hidden_mask())){
            return false;
        }
    }
    return true;
}

int reveal(char * board, std::size_t x_dim, std::size_t y_dim, std::size_t x_loc, std::size_t y_loc){
   
    if ((board[(((y_loc)*x_dim) + x_loc)] & marked_mask()) == marked_mask()){
        return 1;
    }
    if ((board[(((y_loc)*x_dim) + x_loc)] & hidden_mask()) != hidden_mask()){
        return 2;
    }
    if((board[(((y_loc)*x_dim) + x_loc)] & hidden_mask()) == hidden_mask()){
        if ((board[(((y_loc)*x_dim) + x_loc)] & value_mask()) == 9){
            board[(((y_loc)*x_dim) + x_loc)] = (board[(((y_loc)*x_dim) + x_loc)] ^ hidden_mask()); 
            return 9;
        }
        if ((board[(((y_loc)*x_dim) + x_loc)] & value_mask()) == 0){
            int row_movement[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
            int col_movement[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
            board[(((y_loc)*x_dim) + x_loc)] = (board[(((y_loc)*x_dim) + x_loc)] ^ hidden_mask());
                for (int s = 0; s < 8; ++s){
                    int new_x = x_loc + col_movement[s];
                    int new_y = y_loc + row_movement[s];
                    if (new_x >= 0 && new_x < x_dim && new_y >= 0 && new_y < y_dim && ((board[(((new_y)*x_dim) + new_x)] & marked_mask()) != marked_mask())){
                        board[(((new_y)*x_dim) + new_x)] &= ~hidden_mask();
                    }
                }
        }
        else {
            board[(((y_loc)*x_dim) + x_loc)] = (board[(((y_loc)*x_dim) + x_loc)] ^ hidden_mask());
            return 0;
        }  
    }
    return 0;
    
}

