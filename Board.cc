#include "Board.hh"
#include "Util.hh"

#include "avr_spi.h"
#include "avr_ioport.h"

#include <iostream>

Board::Board(avr_t *avr_):
    avr(avr_),
    lcd(*this),
    ram(*this),
    keypad(*this)
{
    avr_irq_register_fun(
        avr_io_getirq(avr, AVR_IOCTL_SPI_GETIRQ(0), SPI_IRQ_OUTPUT),
        [this](avr_irq_t* irq, uint32_t value) {
            this->mosi(value);
        });

    std::vector<avr_irq_t*> keypadCols = keypad.getCols();
    avr_connect_irq(keypadCols[0], avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('C'), 5));
    avr_connect_irq(keypadCols[1], avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('C'), 4));
    avr_connect_irq(keypadCols[2], avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('C'), 3));
    avr_connect_irq(keypadCols[3], avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('C'), 2));

    std::vector<avr_irq_t*> keypadRows = keypad.getRows();
    avr_connect_irq(avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('C'), 1), keypadRows[0]);
    avr_connect_irq(avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('C'), 0), keypadRows[1]);
    avr_connect_irq(avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('B'), 2), keypadRows[2]);
    avr_connect_irq(avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('B'), 1), keypadRows[3]);

    for (int i = 0; i < 4; ++i)
        avr_irq_register_fun(
            keypadCols[i],
            [i](avr_irq_t *irq, uint32_t value){
                if (value == 0)
                    std::cerr << "keypadCols " << i << std::endl;
            });


    avr_connect_irq(avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('D'), 3), lcd.getSCE());
    avr_connect_irq(avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('D'), 4), lcd.getReset());
    avr_connect_irq(avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('D'), 5), lcd.getDC());
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
