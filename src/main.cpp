#include "Emulator.hpp"

void PrintHelp();

int main(int argc, char **argv)
{
    if (argc < 2) {
        std::cerr << "Invalid number of args\nUsage: chip8 [parameters] "
                     "rom_file\nUse chip8 -h for help\nExiting\n";
        return 1;
    }

    if (std::string(argv[1]) == "-h") {
        PrintHelp();
        return 0;
    }

    Emulator emu = Emulator();

    // Parse args
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-cpu") {
            if (i + 1 < argc)
                try {
                    emu.SetCpuFrequency(std::stol(argv[++i]));
                } catch (const std::invalid_argument &ia) {
                    std::cerr << "Invalid cpu argument; must be a number;"
                              << ia.what() << "\n";
                }
        }

        else if (std::string(argv[i]) == "-scale") {
            if (i + 1 < argc)
                try {
                    emu.SetWindowScale(std::stof(argv[++i]));
                } catch (const std::invalid_argument &ia) {
                    std::cerr
                        << "Invalid window scale argument; must be a float;"
                        << ia.what() << "\n";
                }
        }

        else if (std::string(argv[i]) == "-d" ||
                 std::string(argv[i]) == "--debug") {
            emu.SetDebug(true);
        }

        else {
            emu.SetRomPath(argv[i]);
        }
    }

    if (emu.GetRomPath() == "") {
        std::cerr << "No rom file was given\nUsage: chip8 [parameters] "
                     "rom_file\nExiting\n";
        return 1;
    }

    emu.Run();
}

void PrintHelp()
{
    std::cout << "USAGE: chip8 [-cpu frequency_in_MHz] [-scale windowscale] "
                 "[-d|--debug] rom_file\n\n"
              << "-cpu:\tSpecify the cpu frequency in "
                 "MHz\n"
              << "-scale:\tSpecify the window scale\n"
              << "-d|--debug:\tPrint "
                 "debug info in terminal\n";
}