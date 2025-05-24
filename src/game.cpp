#include <vector>
#include <iostream>
#include <random>
#include "game.hpp"

Game::Game(int width, int height, int mines):
    _width(width), _height(height), _mines(mines), _flags(0), _selX(0), _selY(0),
    _board(width * height), _game_state(GameState::Playing) {
    if (mines > width * height) {
        throw std::runtime_error("number of mines must be less than or equal to width * height");
    }
}

void Game::move_selection(int dx, int dy) {
    if (this->_game_state != GameState::Playing) {
        return;
    }

    if (this->_selX + dx < 0)
        this->_selX = this->_width - 1;
    else if (this->_selX + dx >= this->_width)
        this->_selX = 0;
    else
        this->_selX += dx;

    if (this->_selY + dy < 0)
        this->_selY = this->_height - 1;
    else if (this->_selY + dy >= this->_height)
        this->_selY = 0;
    else
        this->_selY += dy;
}

intVec2d Game::get_selection() const {
    return {.x = this->_selX, .y = this->_selY};
}

Tile* Game::get_tile(int x, int y) {
    return &(this->_board[this->_coord_to_pos(x, y)]);
}

Tile* Game::get_tile(intVec2d pos) {
    return &(this->_board[this->_coord_to_pos(pos.x, pos.y)]);
}

void Game::set_tile(Tile tile, int x, int y) {
    this->_board[this->_coord_to_pos(x, y)] = tile;
}

GameState Game::get_gamestate() const {
    return this->_game_state;
}

int Game::_coord_to_pos(int x, int y) const {
    return (y * this->_width) + x;
}

void Game::init_board() {
    this->_place_mines();
    this->_place_numbers();
}

void Game::_place_mines() {
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, (this->_height * this->_width) - 1); // define the range

    for (int i = 0; i < this->_mines; i++) {
        int randPos;

        do {
            randPos = distr(gen);
        } while (this->_board[randPos].type == TileType::Mine);
        
        this->_board[randPos] = {
            .type = TileType::Mine,
            .state = TileState::Hidden,
        };
    }
}

void Game::_place_numbers() {
    for (int x = 0; x < this->_width; x++) {
        for (int y = 0; y < this->_height; y++) {
            Tile *tile = this->get_tile(x, y);

            if (tile->type == TileType::None) {
                int num = this->_count_neighbor(TileType::Mine, x, y);

                if (num == 0) {
                    tile->type = TileType::Empty;
                }
                else {
                    tile->type = TileType::Number;
                    tile->neighbor_mines = num;
                }
            }
        }
    }
}

int Game::_count_neighbor(TileType type, int x, int y) const {
    int count = 0;

    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            int x_ = x + dx;
            int y_ = y + dy;

            if (x_ >= 0 && x_ < this->_width && y_ >= 0 && y_ < this->_height) {
                if (this->_board[this->_coord_to_pos(x_, y_)].type == type) {
                    count++;
                }
            }
        }
    }

    return count;
}

int Game::_count_neighbor(TileState state, int x, int y) const {
    int count = 0;

    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            int x_ = x + dx;
            int y_ = y + dy;

            if (x_ >= 0 && x_ < this->_width && y_ >= 0 && y_ < this->_height) {
                if (this->_board[this->_coord_to_pos(x_, y_)].state == state) {
                    count++;
                }
            }
        }
    }

    return count;
}

void Game::_tile_operation(TileInteraction interaction, int x, int y, bool recursive) {
    Tile *tile = this->get_tile(x, y);

    switch (interaction) {
    case TileInteraction::Reveal: {
        if (tile->type == TileType::Mine && tile->state == TileState::Hidden) {
            this->_game_state = GameState::Lose;
            tile->state = TileState::MineHit;
        }
        else if (tile->type == TileType::Number || tile->type == TileType::Empty) {
            if (tile->state == TileState::Hidden) {
                if (tile->neighbor_mines == 0) {
                    this->_flood_reveal(x, y);
                }

                tile->state = TileState::Revealed;
            }
            else if (tile->state == TileState::Revealed && !recursive && this->_count_neighbor(TileState::Flagged, x, y) == tile->neighbor_mines) {
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        int x_ = x + dx;
                        int y_ = y + dy;
            
                        if (x_ >= 0 && x_ < this->_width && y_ >= 0 && y_ < this->_height) {
                            this->_tile_operation(TileInteraction::Reveal, x_, y_, true);
                        }
                    }
                }
            }
        }

        if (this->_mines == this->_flags && this->_count_hidden_tiles() == 0) {
            this->_game_state = GameState::Win;
        }

        break;
    }
    
    case TileInteraction::Flag: {
        if (tile->state != TileState::Revealed) {
            if (tile->state == TileState::Hidden || tile->state == TileState::Guessed) {
                tile->state = TileState::Flagged;
                this->_flags += 1;
            }
            else if (tile->state == TileState::Flagged) {
                tile->state = TileState::Hidden;
                this->_flags -= 1;
            }
        }

        if (this->_mines == this->_flags && this->_count_hidden_tiles() == 0) {
            this->_game_state = GameState::Win;
        }
        
        break;
    }

    case TileInteraction::Guess: {
        if (tile->state != TileState::Revealed) {
            if (tile->state == TileState::Hidden || tile->state == TileState::Flagged) {
                tile->state = TileState::Guessed;

                if (tile->state == TileState::Flagged) {
                    this->_flags -= 1;
                }
            }
            else if (tile->state == TileState::Guessed) {
                tile->state = TileState::Hidden;
            }
        }

        break;
    }

    default:
        break;
    }
}

void Game::tile_operation(TileInteraction interaction, int x, int y) {
    this->_tile_operation(interaction, x, y);
}

void Game::tile_operation(TileInteraction interaction, intVec2d pos) {
    this->_tile_operation(interaction, pos.x, pos.y);
}

void Game::_flood_reveal(int x, int y) {
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            int x_ = x + dx;
            int y_ = y + dy;

            if (x_ >= 0 && x_ < this->_width && y_ >= 0 && y_ < this->_height) {
                Tile *tile = this->get_tile(x_, y_);
                
                if (tile->state != TileState::Hidden) {
                    continue;
                }

                tile->state = TileState::Revealed;

                if (tile->type == TileType::Empty) {
                    _flood_reveal(x_, y_);
                }
            }
        }
    }
}

int Game::_count_hidden_tiles() const {
    int count = 0;

    for (int y = 0; y < this->_height; y++) {
        for (int x = 0; x < this->_width; x++) {
            Tile tile = this->_board[this->_coord_to_pos(x, y)];
            
            if (tile.state == TileState::Hidden) {
                count++;
            }
        }
    }

    return count;
}