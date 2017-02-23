#pragma once

#include "Board.hh"

class LCD
{
    Board &board;

public:
    bool sce;
    bool dc;

public:
    LCD(Board &board_);

    void message(uint8_t value);
};
