#pragma once
#include <SFML/Graphics.hpp>

// #define LIGHT_MODE
#define DARK_MODE

#if defined(LIGHT_MODE)
namespace config {
    const bool DEBUG = false;

    const int TILE_SIZE = 20;
    
    const sf::Color COLOR_HIDDEN = sf::Color(225, 225, 225);
    const sf::Color COLOR_REVEALED = sf::Color(155, 155, 155);
    const sf::Color COLOR_OUTLINE = sf::Color(0, 0, 0);
    const sf::Color COLOR_SELECTION = sf::Color(255, 0, 0);

    const sf::Color COLOR_1 = sf::Color(25, 25, 255);
    const sf::Color COLOR_2 = sf::Color(25, 225, 255);
    const sf::Color COLOR_3 = sf::Color(50, 255, 50);
    const sf::Color COLOR_4 = sf::Color(225, 225, 50);
    const sf::Color COLOR_5 = sf::Color(255, 100, 0);
    const sf::Color COLOR_6 = sf::Color(255, 150, 0);
    const sf::Color COLOR_7 = sf::Color(255, 0, 255);
    const sf::Color COLOR_8 = sf::Color(255, 0, 100);

    const char TEXT_FLAG = 'P';
    const sf::Color COLOR_FLAG = sf::Color(225, 10, 10);
    const sf::Color COLOR_FLAG_WIN = sf::Color(0, 225, 0);
    
    const char TEXT_MINE = 'X';
    const sf::Color COLOR_MINE = sf::Color(50, 50, 50);
    const sf::Color COLOR_MINE_LOSE = sf::Color(255, 75, 75);

    const char TEXT_GUESS = '?';
    const sf::Color COLOR_GUESS = sf::Color(50, 50, 50);

    const char *TEXT_WIN = "You win!";
    const sf::Color COLOR_TEXT_WIN = sf::Color(0, 255, 0);

    const char *TEXT_LOSE = "You lose";
    const sf::Color COLOR_TEXT_LOSE = sf::Color(255, 0, 0);

    const int PADDING_RECT = 20;
    const sf::Color COLOR_RECT = sf::Color(0, 0, 0, 150);
}

#elif defined(DARK_MODE)
namespace config {
    const bool DEBUG = false;

    const int TILE_SIZE = 20;
    
    const sf::Color COLOR_HIDDEN = sf::Color(76, 84, 92);
    const sf::Color COLOR_REVEALED = sf::Color(46, 54, 62);
    const sf::Color COLOR_OUTLINE = sf::Color(40, 48, 56);
    const sf::Color COLOR_SELECTION = sf::Color(255, 50, 50);

    const sf::Color COLOR_1 = sf::Color(100, 100, 255);
    const sf::Color COLOR_2 = sf::Color(50, 225, 255);
    const sf::Color COLOR_3 = sf::Color(75, 255, 75);
    const sf::Color COLOR_4 = sf::Color(225, 225, 50);
    const sf::Color COLOR_5 = sf::Color(255, 100, 0);
    const sf::Color COLOR_6 = sf::Color(225, 100, 150);
    const sf::Color COLOR_7 = sf::Color(255, 0, 255);
    const sf::Color COLOR_8 = sf::Color(255, 0, 100);

    const char TEXT_FLAG = 'P';
    const sf::Color COLOR_FLAG = sf::Color(225, 50, 50);
    const sf::Color COLOR_FLAG_WIN = sf::Color(0, 225, 0);
    
    const char TEXT_MINE = 'X';
    const sf::Color COLOR_MINE = sf::Color(0, 0, 0);
    const sf::Color COLOR_MINE_LOSE = sf::Color(255, 75, 75);

    const char TEXT_GUESS = '?';
    const sf::Color COLOR_GUESS = sf::Color(200, 200, 200);

    const char *TEXT_WIN = "You win!";
    const sf::Color COLOR_TEXT_WIN = sf::Color(0, 255, 0);

    const char *TEXT_LOSE = "You lose";
    const sf::Color COLOR_TEXT_LOSE = sf::Color(255, 0, 0);

    const int PADDING_RECT = 20;
    const sf::Color COLOR_RECT = sf::Color(0, 0, 0, 150);
}

#endif