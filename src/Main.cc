#include "Board.hh"

#include <cstdlib>
#include <stdexcept>

#include "sim_avr.h"
#include "avr_ioport.h"
#include "avr_spi.h"
#include "sim_elf.h"
#include "sim_gdb.h"
/* #include "sim_vcd_file.h" */

#include <SDL.h>

#include <iostream>

Board setupBoard(const char* fileName)
{
    elf_firmware_t f;
    elf_read_firmware(fileName, &f);
    f.frequency = 16e6;

    const char *mmcu = "atmega328p";
    avr_t * avr = avr_make_mcu_by_name(mmcu);
    if (!avr)
        throw std::runtime_error("Initializing SimAVR failed");
    avr_init(avr);
    avr_load_firmware(avr, &f);

    avr->gdb_port = 1234;
    avr_gdb_init(avr);

    std::cout << "Press ENTER when ready to start" << std::endl;
    std::string s;
    std::getline(std::cin, s);

    return Board(avr);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Need image.elf filename argument" << std::endl;
        return 1;
    }

    Board board = setupBoard(argv[1]);

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

    for (;;) {
        uint32_t targetTime = SDL_GetTicks() + 17;

        SDL_Event e;
        while (SDL_PollEvent(&e) != 0)
        {
            switch (e.type)
            {
            case SDL_QUIT:
                goto quit;
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                board.keypad.keypress(e.key.keysym.scancode, e.key.state == SDL_PRESSED);
                break;
            }
        }

        board.lcd.draw(pf, pixels);
        SDL_BlitScaled (drawSurface, 0, screenSurface, 0);
        SDL_UpdateWindowSurface(window);

        while (SDL_GetTicks() < targetTime)
            board.run();
    }

quit:
    SDL_Quit();
}
