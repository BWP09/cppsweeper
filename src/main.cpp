#include <SFML/Graphics.hpp>
#include <iostream>
#include "game.hpp"
#include "fs_helper.hpp"
#include "config.hpp"

int main() {
    bool debug = false;
    
    sf::RenderWindow window(sf::VideoMode(config::WIDTH * config::TILE_SIZE, config::HEIGHT * config::TILE_SIZE), "Minesweeper", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    sf::Font font;
    
    if (!font.loadFromFile(get_executable_dir() / "../../assets" / "HussarBoldWebEdition-xq5O.otf")) {
        throw std::runtime_error("error loading font");
    }

    bool close = false;

    while (window.isOpen() && !close) {
        Game game(config::WIDTH, config::HEIGHT, config::MINES);
        game.init_board();

        bool restart = false;

        while (window.isOpen() && !restart && !close) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
                 
                if (event.type == sf::Event::KeyPressed) {
                    switch (event.key.code) {
                    case sf::Keyboard::Escape:
                        close = true;
                        break;

                    case sf::Keyboard::Enter:
                        debug = !debug;
                        break;

                    case sf::Keyboard::Left:
                        game.move_selection(-1, 0);
                        break;
                    
                    case sf::Keyboard::Right:
                        game.move_selection(1, 0);
                        break;
                    
                    case sf::Keyboard::Up:
                        game.move_selection(0, -1);
                        break;
                
                    case sf::Keyboard::Down:
                        game.move_selection(0, 1);
                        break;

                    case sf::Keyboard::Z:
                        game.tile_operation(TileInteraction::Flag, game.get_selection());
                        break;

                    case sf::Keyboard::X:
                        game.tile_operation(TileInteraction::Reveal, game.get_selection());
                        break;

                    case sf::Keyboard::C:
                        game.tile_operation(TileInteraction::Guess, game.get_selection());
                        break;

                    case sf::Keyboard::Space:
                        if (game.get_gamestate() == GameState::Win || game.get_gamestate() == GameState::Lose)
                            restart = true;
                        break;

                    default:
                        break;
                    }
                }
            }

            // RENDER START
            
            GameState gamestate = game.get_gamestate();

            window.clear(config::COLOR_OUTLINE);

            for (int y = 0; y < config::HEIGHT; y++) {
                for (int x = 0; x < config::WIDTH; x++) {
                    sf::RectangleShape d_tile(sf::Vector2f(config::TILE_SIZE - 2, config::TILE_SIZE - 2));
                    d_tile.setPosition(x * config::TILE_SIZE + 1, y * config::TILE_SIZE + 1);
    
                    intVec2d sel = game.get_selection();
    
                    if (sel.x == x && sel.y == y) {
                        d_tile.setOutlineColor(config::COLOR_SELECTION);
                        d_tile.setOutlineThickness(2);
                    }
    
                    Tile tile = *game.get_tile(x, y);
    
                    switch (tile.state) {
                    case TileState::Flagged:
                    case TileState::Guessed:
                    case TileState::Hidden:
                        d_tile.setFillColor(config::COLOR_HIDDEN);
                        break;
                    
                    case TileState::Revealed:
                        d_tile.setFillColor(config::COLOR_REVEALED);
                        break;
                    
                    default:
                        break;
                    }
    
                    window.draw(d_tile);
    
                    if (tile.type == TileType::Number && tile.state == TileState::Revealed) {
                        int number_mines = tile.neighbor_mines;
                        
                        sf::Text text((char)(number_mines + 48), font, config::TILE_SIZE - 6);
    
                        switch (number_mines) {
                        case 1:
                            text.setFillColor(config::COLOR_1);
                            text.setPosition(x * config::TILE_SIZE + 6, y * config::TILE_SIZE + 3);
                            break;
    
                        case 2:
                            text.setFillColor(config::COLOR_2);
                            text.setPosition(x * config::TILE_SIZE + 4, y * config::TILE_SIZE + 3);
                            break;
    
                        case 3:
                            text.setFillColor(config::COLOR_3);
                            text.setPosition(x * config::TILE_SIZE + 5, y * config::TILE_SIZE + 3);
                            break;
                        
                        case 4:
                            text.setFillColor(config::COLOR_4);
                            text.setPosition(x * config::TILE_SIZE + 4, y * config::TILE_SIZE + 3);
                            break;
    
                        case 5:
                            text.setFillColor(config::COLOR_5);
                            text.setPosition(x * config::TILE_SIZE + 6, y * config::TILE_SIZE + 3);
                            break;
    
                        case 6:
                            text.setFillColor(config::COLOR_6);
                            text.setPosition(x * config::TILE_SIZE + 4, y * config::TILE_SIZE + 3);
                            break;
    
                        case 7:
                            text.setFillColor(config::COLOR_7);
                            text.setPosition(x * config::TILE_SIZE + 5, y * config::TILE_SIZE + 3);
                            break;
                        
                        case 8:
                            text.setFillColor(config::COLOR_8);
                            text.setPosition(x * config::TILE_SIZE + 5, y * config::TILE_SIZE + 3);
                            break;
                        
                        default:
                            break;
                        }
    
                        window.draw(text);
                    }
                    else if (tile.state == TileState::Flagged) {
                        sf::Text text(config::TEXT_FLAG, font, config::TILE_SIZE - 6);

                        text.setFillColor(config::COLOR_FLAG);
                        text.setPosition(x * config::TILE_SIZE + 5, y * config::TILE_SIZE + 3);

                        window.draw(text);
                    }
                    else if (tile.state == TileState::Guessed) {
                        sf::Text text(config::TEXT_GUESS, font, config::TILE_SIZE - 6);

                        text.setFillColor(config::COLOR_GUESS);
                        text.setPosition(x * config::TILE_SIZE + 6, y * config::TILE_SIZE + 2);

                        window.draw(text);
                    }
                    else if (debug && tile.type == TileType::Mine) {
                        sf::Text text(config::TEXT_MINE, font, config::TILE_SIZE - 6);
                        
                        text.setFillColor(config::COLOR_MINE);
                        text.setPosition(x * config::TILE_SIZE + 5, y * config::TILE_SIZE + 3);
    
                        window.draw(text);
                    }
                }
            }

            if (gamestate == GameState::Win) {
                for (int y = 0; y < config::HEIGHT; y++) {
                    for (int x = 0; x < config::WIDTH; x++) {
                        Tile *tile = game.get_tile(x, y);

                        if (tile->state == TileState::Flagged && tile->type == TileType::Mine) {
                            sf::Text flag(config::TEXT_FLAG, font, config::TILE_SIZE - 6);
                                        
                            flag.setFillColor(config::COLOR_FLAG_WIN);
                            flag.setPosition(x * config::TILE_SIZE + 5, y * config::TILE_SIZE + 3);
                            flag.setOutlineColor(sf::Color::Black);
                            flag.setOutlineThickness(1);
            
                            window.draw(flag);
                        }
                    }
                }

                sf::Text text(config::TEXT_WIN, font, config::TILE_SIZE * 2);
                text.setFillColor(config::COLOR_TEXT_WIN);

                sf::FloatRect t_bb = text.getGlobalBounds();

                text.setPosition(((config::WIDTH * config::TILE_SIZE) - t_bb.width) / 2, (((config::HEIGHT * config::TILE_SIZE) - t_bb.height) / 2) - 10);

                sf::RectangleShape rect(sf::Vector2f(t_bb.width + 2 * config::PADDING_RECT, t_bb.height + 2 * config::PADDING_RECT));
                rect.setFillColor(config::COLOR_RECT);
                rect.setPosition((((config::WIDTH * config::TILE_SIZE) - t_bb.width) / 2) - config::PADDING_RECT, (((config::HEIGHT * config::TILE_SIZE) - t_bb.height) / 2) - config::PADDING_RECT);

                window.draw(rect);
                window.draw(text);
            }
            else if (gamestate == GameState::Lose) {
                for (int y = 0; y < config::HEIGHT; y++) {
                    for (int x = 0; x < config::WIDTH; x++) {
                        Tile *tile = game.get_tile(x, y);

                        if (tile->state == TileState::Flagged && tile->type != TileType::Mine) {
                            sf::Text mine(config::TEXT_MINE, font, config::TILE_SIZE - 6);
                                        
                            mine.setFillColor(config::COLOR_MINE_LOSE);
                            mine.setPosition(x * config::TILE_SIZE + 5, y * config::TILE_SIZE + 3);
                            mine.setOutlineColor(sf::Color::Black);
                            mine.setOutlineThickness(1);
            
                            window.draw(mine);
                        }
                        else if (tile->state != TileState::Flagged && tile->type == TileType::Mine) {
                            sf::Text mine(config::TEXT_MINE, font, config::TILE_SIZE - 6);
                                        
                            mine.setFillColor(config::COLOR_MINE);
                            mine.setPosition(x * config::TILE_SIZE + 5, y * config::TILE_SIZE + 3);
            
                            window.draw(mine);
                        }
                    }
                }

                intVec2d sel = game.get_selection();
                Tile *tile = game.get_tile(sel);

                if (tile->state == TileState::MineHit && tile->type == TileType::Mine) {
                    sf::Text mine(config::TEXT_MINE, font, config::TILE_SIZE - 6);
                                        
                    mine.setFillColor(config::COLOR_MINE_LOSE);
                    mine.setPosition(sel.x * config::TILE_SIZE + 5, sel.y * config::TILE_SIZE + 3);
    
                    window.draw(mine);
                }

                sf::Text text(config::TEXT_LOSE, font, config::TILE_SIZE * 2);
                text.setFillColor(config::COLOR_TEXT_LOSE);

                sf::FloatRect t_bb = text.getGlobalBounds();

                text.setPosition(((config::WIDTH * config::TILE_SIZE) - t_bb.width) / 2, (((config::HEIGHT * config::TILE_SIZE) - t_bb.height) / 2) - 10);

                sf::RectangleShape rect(sf::Vector2f(t_bb.width + 2 * config::PADDING_RECT, t_bb.height + 2 * config::PADDING_RECT));
                rect.setFillColor(config::COLOR_RECT);
                rect.setPosition((((config::WIDTH * config::TILE_SIZE) - t_bb.width) / 2) - config::PADDING_RECT, (((config::HEIGHT * config::TILE_SIZE) - t_bb.height) / 2) - config::PADDING_RECT);

                window.draw(rect);
                window.draw(text);
            }

            window.display();
        }
    }

    return 0;
}
