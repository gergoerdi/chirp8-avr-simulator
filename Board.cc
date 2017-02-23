#include "avr_spi.h"

#include "Board.hh"

Board::Board(avr_t *avr_):
    avr(avr_),
    lcd(*this),
    ram(*this)
{
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
