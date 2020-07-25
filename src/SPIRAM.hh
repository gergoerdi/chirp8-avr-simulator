#pragma once

#include "Board.hh"
#include <stdint.h>

class SPIRAM {
    Board &board;
    uint8_t ram[2 << 16];

    enum SPIState {
        COMMAND,
        PARAM,
        ADDR1,
        ADDR2
    };

    enum SPICommand {
        INIT = 0x01,
        WRITE = 0x02,
        READ = 0x03
    };

    SPIState state;
    SPICommand command;
    uint16_t addr;

public:
    bool cs;

public:
    SPIRAM(Board &board_);

    void message(uint8_t value);
};
