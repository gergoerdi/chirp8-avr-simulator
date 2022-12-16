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
    std::vector<avr_irq_t*> rows;
    std::vector<avr_irq_t*> cols;

    void setState(int x, int y, bool pressed);

public:
    Keypad(Board& board);
    void processKeys(const uint8_t* keys);

    void selectRow(int row, bool state);
    std::vector<avr_irq_t*> getCols() const { return cols; };
    std::vector<avr_irq_t*> getRows() const { return rows; };
};
