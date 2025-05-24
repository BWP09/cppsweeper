#pragma once
#include <vector>

struct intVec2d {
    int x;
    int y;
};

enum class TileType {
    None,
    Empty,
    Number,
    Mine,
};

enum class TileState {
    Hidden,
    Revealed,
    Flagged,
    Guessed,
    MineHit,
};

enum class TileInteraction {
    Reveal,
    Flag,
    Guess,
};

struct Tile {
    TileType type = TileType::None;
    TileState state = TileState::Hidden;
    int neighbor_mines = 0;
};

enum class GameState {
    Playing,
    Lose,
    Win,
};

class Game {
public:
    Game(int width, int height, int mines);
    void move_selection(int x, int y);
    intVec2d get_selection() const;
    void init_board();
    Tile *get_tile(int x, int y);
    Tile *get_tile(intVec2d pos);
    void set_tile(Tile tile, int x, int y);
    void tile_operation(TileInteraction interaction, int x, int y);
    void tile_operation(TileInteraction interaction, intVec2d pos);
    GameState get_gamestate() const;

private:
    int _width, _height, _mines, _flags;
    int _selX, _selY;
    std::vector<Tile> _board;
    GameState _game_state;

    void _place_mines();
    void _place_numbers();
    int _coord_to_pos(int x, int y) const;
    int _count_neighbor(TileType type, int x, int y) const;
    int _count_neighbor(TileState state, int x, int y) const;
    void _tile_operation(TileInteraction interaction, int x, int y, bool recursive = false);
    void _flood_reveal(int x, int y);
    int _count_hidden_tiles() const;
};
