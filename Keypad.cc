#include "Board.hh"
#include "Keypad.hh"

Keypad::Keypad(Board& board_):
    board(board_)
{
}

void Keypad::keypress(SDL_Scancode sc, bool pressed)
{
    switch (sc)
    {
    case SDL_SCANCODE_1:
        keystate[0][0] = pressed;
        break;
    case SDL_SCANCODE_2:
        keystate[0][1] = pressed;
        break;
    case SDL_SCANCODE_3:
        keystate[0][2] = pressed;
        break;
    case SDL_SCANCODE_4:
        keystate[0][3] = pressed;
        break;
    case SDL_SCANCODE_Q:
        keystate[1][0] = pressed;
        break;
    case SDL_SCANCODE_W:
        keystate[1][1] = pressed;
        break;
    case SDL_SCANCODE_E:
        keystate[1][2] = pressed;
        break;
    case SDL_SCANCODE_R:
        keystate[1][3] = pressed;
        break;
    case SDL_SCANCODE_A:
        keystate[2][0] = pressed;
        break;
    case SDL_SCANCODE_S:
        keystate[2][1] = pressed;
        break;
    case SDL_SCANCODE_D:
        keystate[2][2] = pressed;
        break;
    case SDL_SCANCODE_F:
        keystate[2][3] = pressed;
        break;
    case SDL_SCANCODE_Z:
        keystate[3][0] = pressed;
        break;
    case SDL_SCANCODE_X:
        keystate[3][1] = pressed;
        break;
    case SDL_SCANCODE_C:
        keystate[3][2] = pressed;
        break;
    case SDL_SCANCODE_V:
        keystate[3][3] = pressed;
        break;
    default:
        break;
    }
}
