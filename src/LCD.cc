#include "Board.hh"
#include "LCD.hh"
#include "Util.hh"

#include "avr_ioport.h"

namespace {
    namespace Name {
        const char* sce = "LCD.SCE";
        const char* dc = "LCD.DC";
        const char* reset = "LCD.RST";
    }
}

LCD::LCD(Board& board_):
    board(board_),
    nextX(0), nextY(0),
    dirty(true),
    sce(avr_alloc_irq(&(board.avr->irq_pool), 0, 1, &Name::sce)),
    dc(avr_alloc_irq(&(board.avr->irq_pool), 0, 1, &Name::dc)),
    reset(avr_alloc_irq(&(board.avr->irq_pool), 0, 1, &Name::reset))
{
    for (uint8_t x = 0; x < WIDTH; ++x)
        for (uint8_t y = 0; y < HEIGHT; ++y)
            framebuf[x][y] = false;

}

void LCD::message(uint8_t value)
{
    if (sce->value != 0) {
        // printf("LCD not selected\n");
        return;
    }

    if (dc->value == 0)
    {
        // printf("Command to LCD: 0x%02x\n", value);
    } else {
        // printf("LCD %02d %02d\t%02x\n", nextX, nextY, value);

        for (int i = 0; i < 8; ++i, value >>= 1)
        {
            framebuf[nextX][nextY + i] = value & 0x01;
        }
        dirty = true;

        nextY += 8;
        if (nextY >= HEIGHT)
        {
            nextY = 0;
            nextX += 1;
        }
        if (nextX >= WIDTH)
        {
            nextX = 0;
            nextY = 0;
        }
    }
}

void LCD::draw(SDL_PixelFormat* pf, uint32_t pixels[HEIGHT][WIDTH]) const
{
    if (!dirty) return;

    for (int y = 0; y < HEIGHT; ++y)
        for (int x = 0; x < WIDTH; ++x)
        {
            pixels[y][x] = (framebuf[x][y]) ?
                SDL_MapRGB(pf, 0x00, 0x00, 0x00) :
                SDL_MapRGB(pf, 0x73, 0xBD, 0x71);
        }
}
