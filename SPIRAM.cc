#include "Board.hh"
#include "SPIRAM.hh"

#include "avr_ioport.h"

#include <iostream>

namespace {
    void sram_cb(struct avr_irq_t* irq, uint32_t value, void* closure)
    {
        SPIRAM* ram = static_cast<SPIRAM*>(closure);

        // printf("SerialRAM pin %d to %d\n", irq->irq, value);
        switch (irq->irq)
        {
        case 6:
            ram->cs = value;
        }
    }
}

SPIRAM::SPIRAM(Board &board_):
    board(board_),
    state(COMMAND),
    addr(0)
{
    avr_irq_register_notify(
        avr_io_getirq(board.avr, AVR_IOCTL_IOPORT_GETIRQ('D'), 6),
        sram_cb, this);
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
                // printf("ADDR = 0x%04x, VALUE = 0x%02x\n", addr, value);
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
