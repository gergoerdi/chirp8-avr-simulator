#pragma once

#include "Board.hh"
#include <SDL.h>

#include "sim_avr.h"

class Keypad
{
private:
    Board &board;
    bool keystate[4][4];
    bool selectRows[4];
    avr_irq_t* scanCols[4];

    void setState(int x, int y, bool pressed);

public:
    Keypad(Board& board);
    void keypress(SDL_Scancode sc, bool pressed);

    void selectRow(int row, bool state);
    avr_irq_t* const * getScanCols() const;
};
