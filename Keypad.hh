#pragma once

#include "Board.hh"
#include <SDL.h>
#include <vector>

#include "sim_avr.h"

class Keypad
{
private:
    Board &board;
    bool keystate[4][4];
    bool selectRows[4];
    std::vector<avr_irq_t*> cols;

    void setState(int x, int y, bool pressed);

public:
    Keypad(Board& board);
    void keypress(SDL_Scancode sc, bool pressed);

    void selectRow(int row, bool state);
    std::vector<avr_irq_t*> getCols() const { return cols; };
};
