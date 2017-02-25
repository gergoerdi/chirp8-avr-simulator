#include "Board.hh"
#include "LCD.hh"
#include "Util.hh"

#include "avr_ioport.h"

LCD::LCD(Board& board_):
    board(board_)
{
    avr_irq_register_fun(
        avr_io_getirq(board.avr, AVR_IOCTL_IOPORT_GETIRQ('D'), 3),
        [this](avr_irq_t* irq, uint32_t value) {
            this->sce = value;
        });
    avr_irq_register_fun(
        avr_io_getirq(board.avr, AVR_IOCTL_IOPORT_GETIRQ('D'), 4),
        [this](avr_irq_t* irq, uint32_t value) {
        });
    avr_irq_register_fun(
        avr_io_getirq(board.avr, AVR_IOCTL_IOPORT_GETIRQ('D'), 5),
        [this](avr_irq_t* irq, uint32_t value) {
            this->dc = value;
        });

    dirty = true;
}

void LCD::message(uint8_t value)
{
    if (sce) return;

    if (!dc)
    {
        // printf("Command to LCD: 0x%02x\n", value);
    } else {
        printf("LCD %d %d\t%02x\n", nextX, nextY, value);

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
