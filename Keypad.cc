#include "Board.hh"
#include "Keypad.hh"

#include "avr_ioport.h"

#include <string>
#include <iostream>

namespace {
    void selector_cb(struct avr_irq_t* irq, uint32_t value, void* closure)
    {
        std::pair<Keypad*, int>* data = reinterpret_cast<std::pair<Keypad*, int>*>(closure);

        Keypad* keypad = data->first;
        int row = data->second;
        keypad->selectRow(row, value);
    }
}

Keypad::Keypad(Board& board_):
    board(board_)
{
    for (int i = 0; i < 4; ++i)
    {
        // std::string rowName = "ROW" + i;
        // std::string colName = "COL" + i;
        scanCols[i] = avr_alloc_irq(&(board.avr->irq_pool), 0, 1, 0); // rowName.c_str());
    }

    std::pair<Keypad*, int>* closure;

    // closure = new std::pair<Keypad*, int>(this, 0);
    // avr_irq_register_notify(
    //     avr_io_getirq(board.avr, AVR_IOCTL_IOPORT_GETIRQ('C'), 1),
    //     selector_cb, closure);
    // closure = new std::pair<Keypad*, int>(this, 1);
    // avr_irq_register_notify(
    //     avr_io_getirq(board.avr, AVR_IOCTL_IOPORT_GETIRQ('C'), 0),
    //     selector_cb, closure);
    // closure = new std::pair<Keypad*, int>(this, 2);
    // avr_irq_register_notify(
    //     avr_io_getirq(board.avr, AVR_IOCTL_IOPORT_GETIRQ('B'), 2),
    //     selector_cb, closure);
    // closure = new std::pair<Keypad*, int>(this, 3);
    // avr_irq_register_notify(
    //     avr_io_getirq(board.avr, AVR_IOCTL_IOPORT_GETIRQ('B'), 1),
    //     selector_cb, closure);
}

#include <iostream>

void Keypad::setState(int x, int y, bool pressed)
{
    keystate[x][y] = pressed;
}

void Keypad::keypress(SDL_Scancode sc, bool pressed)
{
    switch (sc)
    {
    case SDL_SCANCODE_1:
        setState(0, 0, pressed);
        break;
    case SDL_SCANCODE_2:
        setState(0, 1, pressed);
        break;
    case SDL_SCANCODE_3:
        setState(0, 2, pressed);
        break;
    case SDL_SCANCODE_4:
        setState(0, 3, pressed);
        break;
    case SDL_SCANCODE_Q:
        setState(1, 0, pressed);
        break;
    case SDL_SCANCODE_W:
        setState(1, 1, pressed);
        break;
    case SDL_SCANCODE_E:
        setState(1, 2, pressed);
        break;
    case SDL_SCANCODE_R:
        setState(1, 3, pressed);
        break;
    case SDL_SCANCODE_A:
        setState(2, 0, pressed);
        break;
    case SDL_SCANCODE_S:
        setState(2, 1, pressed);
        break;
    case SDL_SCANCODE_D:
        setState(2, 2, pressed);
        break;
    case SDL_SCANCODE_F:
        setState(2, 3, pressed);
        break;
    case SDL_SCANCODE_Z:
        setState(3, 0, pressed);
        break;
    case SDL_SCANCODE_X:
        setState(3, 1, pressed);
        break;
    case SDL_SCANCODE_C:
        setState(3, 2, pressed);
        break;
    case SDL_SCANCODE_V:
        setState(3, 3, pressed);
        break;
    default:
        break;
    }
}

void Keypad::selectRow(int row, bool state)
{
    std::cerr << "selectRow " << row << " " << state << std::endl;
    selectRows[row] = state;

    for (int x = 0; x < 4; ++x)
    {
        bool found = false;
        for (int y = 0; y < 4; ++y)
        {
            if (!selectRows[y])
                found = found || keystate[x][y];
        }

        avr_raise_irq(scanCols[x], !found);
    }
}

avr_irq_t* const * Keypad::getScanCols() const
{
    return scanCols;
}
