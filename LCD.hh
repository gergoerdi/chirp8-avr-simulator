#pragma once

#include "Board.hh"
#include <SDL.h>

class LCD
{
public:
    static const int WIDTH = 84, HEIGHT = 48;

private:
    Board &board;
    bool framebuf[WIDTH][HEIGHT];
    int nextX, nextY;
    bool dirty;

public:
    bool sce;
    bool dc;

public:
    LCD(Board& board);

    void message(uint8_t value);
    void draw(SDL_PixelFormat* pf, uint32_t pixels[HEIGHT][WIDTH]) const;
};
