#include "Board.hh"
#include "LCD.hh"

#include <stdlib.h>
#include <stdio.h>

LCD::LCD(Board& board_):
    board(board_)
{
}

void LCD::message(uint8_t value)
{
    if (sce) return;

    if (!dc)
    {
        printf("Command to LCD: 0x%02x\n", value);
    } else {
        bool *block = framebuf[nextX];
        for (int i = 0; i < 8; ++i, value >>= 1)
        {
            block[nextY + i] = value & 0x01;
        }

        nextY += 8;
        if (nextY >= HEIGHT)
        {
            nextY = 0;
            nextX += 1;
        }
        if (nextX >= WIDTH)
        {
            nextY = 0;
            nextX = 0;
        }
    }
}

void LCD::draw(SDL_PixelFormat* pf, uint32_t pixels[HEIGHT][WIDTH]) const
{
    for (int y = 0; y < HEIGHT; ++y)
        for (int x = 0; x < WIDTH; ++x)
        {
            pixels[y][x] = framebuf[y][x] ?
                SDL_MapRGB(pf, 0x00, 0x00, 0x00) :
                SDL_MapRGB(pf, 0x73, 0xBD, 0x71);
        }
}
