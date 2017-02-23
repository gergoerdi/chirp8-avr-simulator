#include "Board.hh"

#include "avr_spi.h"
#include "avr_ioport.h"

#include <iostream>;

namespace {
    void spi_mosi_cb(struct avr_irq_t* irq, uint32_t value, void* closure)
    {
        Board* board = static_cast<Board*>(closure);
        board->mosi(value);
    }
}

Board::Board(avr_t *avr_):
    avr(avr_),
    lcd(*this),
    ram(*this),
    keypad(*this)
{
    avr_irq_register_notify(
        avr_io_getirq(avr, AVR_IOCTL_SPI_GETIRQ(0), SPI_IRQ_OUTPUT),
        spi_mosi_cb,
        this);

    avr_irq_t* const * keypadCols = keypad.getScanCols();

    // avr_connect_irq(keypadCols[0], avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('C'), 5));
    // avr_connect_irq(keypadCols[1], avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('C'), 4));
    // avr_connect_irq(keypadCols[2], avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('C'), 3));
    // avr_connect_irq(keypadCols[3], avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('C'), 2));
}

void Board::run()
{
    avr_run(avr);
}

void Board::miso(uint8_t value)
{
    avr_raise_irq(
        avr_io_getirq(avr, AVR_IOCTL_SPI_GETIRQ(0), SPI_IRQ_INPUT),
        value);
}

void Board::mosi(uint8_t value)
{
    ram.message(value);
    lcd.message(value);
}
