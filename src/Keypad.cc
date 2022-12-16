#include "Board.hh"
#include "Keypad.hh"
#include "Util.hh"

#include "avr_ioport.h"

#include <functional>
#include <iostream>

Keypad::Keypad(Board& board_):
    board(board_)
{
    for (int i = 0; i < 4; ++i)
    {
        // TODO: pin names
        cols.push_back(avr_alloc_irq(&(board.avr->irq_pool), 0, 1, 0));
        rows.push_back(avr_alloc_irq(&(board.avr->irq_pool), 0, 1, 0));

        avr_irq_register_fun(
            rows[i],
            [this, i](avr_irq_t *irq, uint32_t value){
                selectRow(i, value);
            });
    }
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            keystate[i][j] = false;

    avr_irq_register_fun(
        avr_io_getirq(board.avr, AVR_IOCTL_IOPORT_GETIRQ('C'), 1),
        [this](avr_irq_t *irq, uint32_t value){
            selectRow(0, value);
        });
    avr_irq_register_fun(
        avr_io_getirq(board.avr, AVR_IOCTL_IOPORT_GETIRQ('C'), 0),
        [this](avr_irq_t *irq, uint32_t value){
            selectRow(1, value);
        });
    avr_irq_register_fun(
        avr_io_getirq(board.avr, AVR_IOCTL_IOPORT_GETIRQ('B'), 2),
        [this](avr_irq_t *irq, uint32_t value){
            selectRow(2, value);
        });
    avr_irq_register_fun(
        avr_io_getirq(board.avr, AVR_IOCTL_IOPORT_GETIRQ('B'), 1),
        [this](avr_irq_t *irq, uint32_t value){
            selectRow(3, value);
        });
}

void Keypad::setState(int row, int col, bool pressed)
{
    keystate[row][col] = pressed;
}

void Keypad::processKeys(const uint8_t* keys)
{
    setState(0, 0, keys[SDL_SCANCODE_1]);
    setState(0, 1, keys[SDL_SCANCODE_2]);
    setState(0, 2, keys[SDL_SCANCODE_3]);
    setState(0, 3, keys[SDL_SCANCODE_4]);

    setState(1, 0, keys[SDL_SCANCODE_Q]);
    setState(1, 1, keys[SDL_SCANCODE_W]);
    setState(1, 2, keys[SDL_SCANCODE_E]);
    setState(1, 3, keys[SDL_SCANCODE_R]);

    setState(2, 0, keys[SDL_SCANCODE_A]);
    setState(2, 1, keys[SDL_SCANCODE_S]);
    setState(2, 2, keys[SDL_SCANCODE_D]);
    setState(2, 3, keys[SDL_SCANCODE_F]);

    setState(3, 0, keys[SDL_SCANCODE_Z]);
    setState(3, 1, keys[SDL_SCANCODE_X]);
    setState(3, 2, keys[SDL_SCANCODE_C]);
    setState(3, 3, keys[SDL_SCANCODE_V]);
}

void Keypad::selectRow(int row, bool state)
{
    bool verbose = false;

    // std::cerr << "selectRow " << row << " " << state;
    // selectRows[row] = state;

    if (verbose)
    {
        for (int y = 0; y < 4; ++y)
        {
            std::cerr << ((rows[y]->value == 0) ? '>' : ' ') << ' ';
            for (int x = 0; x < 4; ++x)
            {
                std::cerr << (keystate[y][x] ? '#' : ' ');
            }
            std::cerr << std::endl;
        }

        std::cerr << "  ";
    }

    for (int col = 0; col < 4; ++col)
    {
        bool found = false;
        for (int row = 0; row < 4; ++row)
        {
            if (rows[row]->value == 0)
                found = found || (keystate[row][col]);
        }

        if (verbose)
            std::cerr << (found ? '^' : ' ');

        avr_raise_irq(cols[col], !found);
    }
    if (verbose)
    {
        std::cerr << std::endl;

        std::cerr << std::endl;
    }
}
