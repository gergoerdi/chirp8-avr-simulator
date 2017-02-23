#pragma once

#include "Board.hh"
#include <SDL.h>

class Keypad
{
private:
    Board &board;
    bool keystate[4][4];

public:
    Keypad(Board& board);
    void keypress(SDL_Scancode sc, bool pressed);
};
