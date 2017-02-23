#include "Board.hh"

#include <stdlib.h>
#include <stdio.h>
/* #include <libgen.h> */
#include <pthread.h>
#include <stdbool.h>

#include "sim_avr.h"
#include "avr_ioport.h"
#include "avr_spi.h"
#include "sim_elf.h"
/* #include "sim_gdb.h" */
/* #include "sim_vcd_file.h" */


static void* avr_run_thread(void* closure)
{
    Board* board = static_cast<Board*>(closure);

    for (;;) {
        board->run();
    }

    return 0;
}

void spi_mosi_cb(struct avr_irq_t* irq, uint32_t value, void* closure)
{
    Board* board = static_cast<Board*>(closure);
    board->mosi(value);
}

void lcd_cb(struct avr_irq_t* irq, uint32_t value, void* closure)
{
    Board* board = static_cast<Board*>(closure);

    printf("LCD pin %d to %d\n", irq->irq, value);
    switch (irq->irq)
    {
    case 3:
        board->lcd.sce = value;
        break;
    case 5:
        board->lcd.dc = value;
    }
}

void sram_cb(struct avr_irq_t* irq, uint32_t value, void* closure)
{
    Board* board = static_cast<Board*>(closure);

    // printf("SerialRAM pin %d to %d\n", irq->irq, value);
    switch (irq->irq)
    {
    case 6:
        board->ram.cs = value;
    }
}

int main(int argc, char *argv[])
{
    elf_firmware_t f;
    elf_read_firmware("image.elf", &f);
    f.frequency = 16e6;

    const char *mmcu = "atmega328p";
    avr_t * avr = avr_make_mcu_by_name(mmcu);
    if (!avr) {
        fprintf(stderr, "%s: AVR '%s' not known\n", argv[0], mmcu);
        exit(1);
    }
    Board board(avr);

    avr_init(avr);
    avr_load_firmware(avr, &f);

    avr_irq_register_notify(
        avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('D'), 3),
        lcd_cb,
        &board);
    avr_irq_register_notify(
        avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('D'), 4),
        lcd_cb,
        &board);
    avr_irq_register_notify(
        avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('D'), 5),
        lcd_cb,
        &board);
    avr_irq_register_notify(
        avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('D'), 6),
        sram_cb,
        &board);

    avr_irq_register_notify(
        avr_io_getirq(avr, AVR_IOCTL_SPI_GETIRQ(0), SPI_IRQ_OUTPUT),
        spi_mosi_cb,
        &board);

    /* // even if not setup at startup, activate gdb if crashing */
    /* avr->gdb_port = 1234; */
    /* if (0) { */
    /*     //avr->state = cpu_Stopped; */
    /*     avr_gdb_init(avr); */
    /* } */

    pthread_t run;
    pthread_create(&run, NULL, avr_run_thread, &board);

    for (;;) {}
}
