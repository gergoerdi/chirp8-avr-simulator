#include "Board.hh"
#include "SPIRAM.hh"

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

// #include "sim_avr.h"
// #include "avr_ioport.h"
// #include "avr_spi.h"
// #include "sim_elf.h"
// /* #include "sim_gdb.h" */
// /* #include "sim_vcd_file.h" */

SPIRAM::SPIRAM(Board &board_):
    board(board_),
    state(COMMAND),
    addr(0)
{
}

void SPIRAM::message(uint8_t value)
{
    if (cs) return;

    switch (state)
    {
    case COMMAND:
        command = (SPICommand)value;
        switch (command) {
        case INIT:
            state = PARAM;
            break;
        default:
            state = ADDR1;
            break;
        }
        break;
    case PARAM:
        switch (command) {
        case WRITE:
                printf("ADDR = 0x%04x, VALUE = 0x%02x\n", addr, value);
                ram[addr] = value;
                break;
            case READ:
                printf("ADDR = 0x%04x\n", addr);
                board.miso(ram[addr]);
                break;
        };
        state = COMMAND;
        break;
    case ADDR1:
        addr = (addr << 8) + value;
        state = ADDR2;
        break;
    case ADDR2:
        addr = (addr << 8) + value;
        state = PARAM;
        break;
    }
}
