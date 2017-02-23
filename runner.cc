#include "Board.hh"

#include <cstdlib>
#include <stdexcept>
#include <pthread.h>
#include <stdbool.h>

#include "sim_avr.h"
#include "avr_ioport.h"
#include "avr_spi.h"
#include "sim_elf.h"
/* #include "sim_gdb.h" */
/* #include "sim_vcd_file.h" */

#include <SDL.h>

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
        break;
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

Board setupBoard()
{
    elf_firmware_t f;
    elf_read_firmware("image.elf", &f);
    f.frequency = 16e6;

    const char *mmcu = "atmega328p";
    avr_t * avr = avr_make_mcu_by_name(mmcu);
    if (!avr)
        throw std::runtime_error("Initializing SimAVR failed");
    Board board(avr);

    avr_init(avr);
    avr_load_firmware(avr, &f);

    avr_irq_register_notify(
        avr_io_getirq(avr, AVR_IOCTL_SPI_GETIRQ(0), SPI_IRQ_OUTPUT),
        spi_mosi_cb,
        &board);

    avr_irq_register_notify(
        avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('D'), 3),
        lcd_cb, &board);
    avr_irq_register_notify(
        avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('D'), 4),
        lcd_cb, &board);
    avr_irq_register_notify(
        avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('D'), 5),
        lcd_cb, &board);
    avr_irq_register_notify(
        avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('D'), 6),
        sram_cb, &board);

    return board;
}

int main(int argc, char *argv[])
{
    Board board = setupBoard();

    if (SDL_Init( SDL_INIT_VIDEO ) < 0)
        throw std::runtime_error(SDL_GetError());

    SDL_Window* window = SDL_CreateWindow ("CHIP-8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                           board.lcd.WIDTH * 8, board.lcd.HEIGHT * 8,
                                           SDL_WINDOW_SHOWN);

    if (!window)
        throw std::runtime_error(SDL_GetError());

    uint32_t pixels[board.lcd.HEIGHT][board.lcd.WIDTH];
    SDL_Surface* screenSurface = SDL_GetWindowSurface(window);
    SDL_PixelFormat* pf = screenSurface->format;
    SDL_Surface* drawSurface = SDL_CreateRGBSurfaceFrom(pixels, board.lcd.WIDTH, board.lcd.HEIGHT, 32,
                                                        board.lcd.WIDTH * 4, 0, 0, 0, 0);

    // for (int row = 0; row < 4; ++row)
    //     for (int col = 0; col < 4; ++col)
    //         keystate[row][col] = false;


    /* // even if not setup at startup, activate gdb if crashing */
    /* avr->gdb_port = 1234; */
    /* if (0) { */
    /*     //avr->state = cpu_Stopped; */
    /*     avr_gdb_init(avr); */
    /* } */

    pthread_t run;
    pthread_create(&run, NULL, avr_run_thread, &board);

    for (;;) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0)
        {
            switch (e.type)
            {
            case SDL_QUIT:
                goto quit;
                break;
            }
        }

        board.lcd.draw(pf, pixels);
        SDL_BlitScaled (drawSurface, 0, screenSurface, 0);
        SDL_UpdateWindowSurface(window);
        SDL_Delay(17);
    }

quit:
    SDL_Quit();
}
