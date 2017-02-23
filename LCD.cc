#include "Board.hh"
#include "LCD.hh"

#include <stdlib.h>
#include <stdio.h>

LCD::LCD(Board &board_):
    board(board_)
{
}

void LCD::message(uint8_t value)
{
    if (sce) return;

    printf("Message to LCD: 0x%02x\n", value);
}
