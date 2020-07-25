#pragma once

#include "Board.hh"
#include <SDL.h>

#include <sim_avr.h>

class LCD
{
public:
    static const int WIDTH = 84, HEIGHT = 48;

private:
    Board &board;
    bool framebuf[WIDTH][HEIGHT];
    int nextX, nextY;
    bool dirty;

    avr_irq_t* sce;
    avr_irq_t* dc;
    avr_irq_t* reset;
public:
    LCD(Board& board);

    void message(uint8_t value);
    void draw(SDL_PixelFormat* pf, uint32_t pixels[HEIGHT][WIDTH]) const;

    avr_irq_t* const getSCE() const { return sce; }
    avr_irq_t* const getReset() const{ return reset; }
    avr_irq_t* const getDC() const { return dc; }
};
