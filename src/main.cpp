#include "Emulator.hpp"

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cerr << "Invalid number of args\nUsage: chip8 rom_file\nExiting\n";
        return 1;
    }

    Emulator emu = Emulator();
    emu.SetRomPath(argv[1]);
    emu.Run();
}