# chip8
A CHIP-8 emulator written in C++ that uses SDL2

## Getting Started
### Prerequisites
[SDL2](https://www.libsdl.org/index.php) must be installed.
To install SDL2, visit [this](https://wiki.libsdl.org/Installation) page.


### Installing
Just clone the repository and use the makefile
```
git clone https://github.com/mihmin98/chip8.git
cd chip8
make
```
The emulator will be located in ```chip8/build/app```

### Running
```
chip8 [-cpu frequency_in_MHz] [-scale windowscale] [d|--debug] rom_file
  -cpu: Specify the cpu frequency in MHz
  -scale: Specify the window scale
  -d|--debug: Print debug info in terminal
```

### Running Tests
This program uses the [Catch2](https://github.com/catchorg/Catch2) test suite.
To run the tests just do ```make test```

## Built with
* [SDL2](https://www.libsdl.org/index.php) - Graphics Libraru
* [Catch2](https://github.com/catchorg/Catch2) - Test Suite

## License
This project is licensed under the MIT License - See [LICENSE.md](LICENSE.md) for more info

## Acknowledgments
Cowgod for providing the [CHIP-8 technical documentation](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
