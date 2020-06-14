#include "catch.hpp"

#include "Chip8.hpp"
#include <iostream>

TEST_CASE("OPCODE 00E0", "[opcode]")
{
    Chip8 chip8;
    chip8.opcode = 0x00E0;
    memset(chip8.video, 0xFF, sizeof(chip8.video));

    chip8.ExecuteOpcode();

    bool allZero = true;
    for (int i = 0; i < CHIP8_VIDEO_WIDTH * CHIP8_VIDEO_HEIGHT; ++i)
        if (chip8.video[i] != 0) {
            allZero = false;
            break;
        }

    REQUIRE(allZero == true);
}

TEST_CASE("OPCODE 00EE", "[opcode]")
{
    Chip8 chip8;
    chip8.opcode = 0x00EE;
    chip8.PC = 0x240;
    chip8.SP = 3;
    chip8.stack[3] = 0x220;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.PC == 0x220);
    REQUIRE(chip8.SP == 2);
}

TEST_CASE("OPCODE 1NNN", "[opcode]")
{
    Chip8 chip8;
    chip8.opcode = 0x1234;
    chip8.PC = 0x0300;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.PC == 0x0234);
}

TEST_CASE("OPCODE 2NNN", "[opcode]")
{
    Chip8 chip8;
    chip8.opcode = 0x2345;
    chip8.PC = 0x0200;
    chip8.SP = 2;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.PC == 0x0345);
    REQUIRE(chip8.SP == 3);
    REQUIRE(chip8.stack[chip8.SP] == 0x0200);
}

TEST_CASE("OPCODE 3XKK", "[opcode]")
{
    Chip8 chip8;
    chip8.V[2] = 0x3;
    chip8.opcode = 0x3201;
    chip8.PC = 0x0300;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.PC == 0x0300);

    chip8.V[2] = 0x1;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.PC == 0x0302);
}

TEST_CASE("OPCODE 4XKK", "[opcode]")
{
    Chip8 chip8;
    chip8.V[2] = 0x3;
    chip8.opcode = 0x4201;
    chip8.PC = 0x0300;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.PC == 0x0302);

    chip8.V[2] = 0x1;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.PC == 0x0302);
}

TEST_CASE("OPCODE 5XY0", "[opcode]")
{
    Chip8 chip8;
    chip8.V[3] = 3;
    chip8.V[4] = 4;
    chip8.opcode = 0x5340;
    chip8.PC = 0x0300;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.PC == 0x0300);

    chip8.V[3] = 4;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.PC == 0x0302);
}

TEST_CASE("OPCODE 6XKK", "[opcode]")
{
    Chip8 chip8;
    chip8.V[2] = 2;
    chip8.opcode = 0x62AA;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.V[2] == 0xAA);
}

TEST_CASE("OPCODE 7XKK", "[opcode]")
{
    Chip8 chip8;
    chip8.V[2] = 2;
    chip8.opcode = 0x72A0;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.V[2] == 0xA0 + 2);
}

TEST_CASE("OPCODE 8XY0", "[opcode]")
{
    Chip8 chip8;
    chip8.V[2] = 2;
    chip8.V[3] = 3;
    chip8.opcode = 0x8230;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.V[2] == chip8.V[3]);
}

TEST_CASE("OPCODE 8XY1", "[opcode]")
{
    Chip8 chip8;
    chip8.V[2] = 0xAA;
    chip8.V[3] = 0x55;
    chip8.opcode = 0x8231;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.V[2] == 0xFF);
}

TEST_CASE("OPCODE 8XY2", "[opcode]")
{
    Chip8 chip8;
    chip8.V[2] = 0xAA;
    chip8.V[3] = 0x55;
    chip8.opcode = 0x8232;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.V[2] == 0x00);
}

TEST_CASE("OPCODE 8XY3", "[opcode]")
{
    Chip8 chip8;
    chip8.V[2] = 0x94;
    chip8.V[3] = 0xD6;
    chip8.opcode = 0x8233;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.V[2] == 0x42);
}

TEST_CASE("OPCODE 8XY4", "[opcode]")
{
    Chip8 chip8;
    chip8.V[2] = 2;
    chip8.V[3] = 3;
    chip8.opcode = 0x8234;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.V[2] == 5);
    REQUIRE(chip8.V[0xF] == 0);

    chip8.V[2] = 0xDE;
    chip8.V[3] = 0xAD;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.V[2] == 0x8B);
    REQUIRE(chip8.V[0xF] == 1);
}

TEST_CASE("OPCODE 8XY5", "[opcode]")
{
    Chip8 chip8;
    chip8.V[2] = 2;
    chip8.V[3] = 3;
    chip8.opcode = 0x8235;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.V[2] == 0xFF);
    REQUIRE(chip8.V[0xF] == 0);

    chip8.V[2] = 3;
    chip8.V[3] = 2;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.V[2] == 1);
    REQUIRE(chip8.V[0xF] == 1);
}

TEST_CASE("OPCODE 8XY6", "[opcode]")
{
    Chip8 chip8;
    chip8.V[2] = 5;
    chip8.opcode = 0x8206;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.V[2] == 2);
    REQUIRE(chip8.V[0xF] == 1);

    chip8.V[2] = 6;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.V[2] == 3);
    REQUIRE(chip8.V[0xF] == 0);
}

TEST_CASE("OPCODE 8XY7", "[opcode]")
{
    Chip8 chip8;
    chip8.V[2] = 2;
    chip8.V[3] = 3;
    chip8.opcode = 0x8237;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.V[2] == 1);
    REQUIRE(chip8.V[0xF] == 1);

    chip8.V[2] = 3;
    chip8.V[3] = 2;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.V[2] == 0xFF);
    REQUIRE(chip8.V[0xF] == 0);
}

TEST_CASE("OPCODE 8XYE", "[opcode]")
{
    Chip8 chip8;
    chip8.V[2] = 2;
    chip8.opcode = 0x820E;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.V[2] == 4);
    REQUIRE(chip8.V[0xF] == 0);

    chip8.V[2] = 0xA0;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.V[2] == 0x40);
    REQUIRE(chip8.V[0xF] == 1);
}

TEST_CASE("OPCODE 9XY0", "[opcode]")
{
    Chip8 chip8;
    chip8.V[2] = 2;
    chip8.V[3] = 2;
    chip8.PC = 0x0300;
    chip8.opcode = 0x9230;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.PC == 0x0300);

    chip8.V[3] = 3;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.PC == 0x0302);
}

TEST_CASE("OPCODE ANNN", "[opcode]")
{
    Chip8 chip8;
    chip8.I = 0x0100;
    chip8.opcode = 0xA123;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.I == 0x0123);
}

TEST_CASE("OPCODE BNNN", "[opcode]")
{
    Chip8 chip8;
    chip8.PC = 0x0300;
    chip8.V[0] = 0x20;
    chip8.opcode = 0xB400;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.PC == 0x0420);
}

TEST_CASE("OPCODE DXYN", "[opcode]")
{
    // Draw a "1" sprite and check the video memory
    uint8_t spriteAddress = CHIP8_FONTSET_START + 5;
    uint8_t posX = 0x2;
    uint8_t posY = 0x3;
    Chip8 chip8;
    chip8.V[2] = posX;
    chip8.V[3] = posY;
    chip8.I = spriteAddress;
    chip8.opcode = 0xD235;

    chip8.ExecuteOpcode();

    for (uint8_t i = posY; i < posY + 5; ++i) {
        uint8_t videoByte = 0;
        for (uint8_t j = posX; j < posX + 8; ++j) {
            videoByte =
                (videoByte << 1) | chip8.video[i * CHIP8_VIDEO_WIDTH + j];
        }
        REQUIRE(videoByte == chip8.memory[spriteAddress + (i - posY)]);
    }
    REQUIRE(chip8.V[0xF] == 0);

    chip8.ExecuteOpcode();

    REQUIRE(chip8.V[0xF] == 1);
}

TEST_CASE("OPCODE EX9E", "[opcode]")
{
    Chip8 chip8;
    chip8.V[2] = 1;
    chip8.PC = 0x0300;
    chip8.opcode = 0xE29E;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.PC == 0x0300);

    chip8.keypad[1] = 1;
    chip8.ExecuteOpcode();

    REQUIRE(chip8.PC == 0x0302);
}

TEST_CASE("OPCODE EXA1", "[opcode]")
{
    Chip8 chip8;
    chip8.V[2] = 1;
    chip8.keypad[1] = 1;
    chip8.PC = 0x0300;
    chip8.opcode = 0xE2A1;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.PC == 0x0300);

    chip8.keypad[1] = 0;
    chip8.ExecuteOpcode();

    REQUIRE(chip8.PC == 0x0302);
}

TEST_CASE("OPCODE FX07", "[opcode]")
{
    Chip8 chip8;
    chip8.delayTimer = 0x66;
    chip8.opcode = 0xF207;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.V[2] == 0x66);
}

TEST_CASE("OPCODE FX0A", "[opcode]")
{
    Chip8 chip8;
    chip8.PC = 0x0302;
    chip8.opcode = 0xF20A;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.PC == 0x0300);

    chip8.keypad[0xA] = 1;
    chip8.keypad[0xF] = 1;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.PC == 0x0300);
    REQUIRE(chip8.V[2] == 0x0A);
}

TEST_CASE("OPCODE FX15", "[opcode]")
{
    Chip8 chip8;
    chip8.V[2] = 0x66;
    chip8.opcode = 0xF215;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.delayTimer == 0x66);
}

TEST_CASE("OPCODE FX18", "[opcode]")
{
    Chip8 chip8;
    chip8.V[2] = 0x66;
    chip8.opcode = 0xF218;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.soundTimer == 0x66);
}

TEST_CASE("OPCODE FX1E", "[opcode]")
{
    Chip8 chip8;
    chip8.V[2] = 0x02;
    chip8.I = 0x0100;
    chip8.opcode = 0xF21E;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.I == 0x0102);
}

TEST_CASE("OPCODE FX29", "[opcode]")
{
    Chip8 chip8;
    chip8.V[2] = 0x02;
    chip8.opcode = 0xF229;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.I == CHIP8_FONTSET_START + 10);
}

TEST_CASE("OPCODE FX33", "[opcode]")
{
    Chip8 chip8;
    chip8.V[2] = 123;
    chip8.I = 0x0400;
    chip8.opcode = 0xF233;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.memory[chip8.I] == 1);
    REQUIRE(chip8.memory[chip8.I + 1] == 2);
    REQUIRE(chip8.memory[chip8.I + 2] == 3);
}

TEST_CASE("OPCODE FX55", "[opcode]")
{
    Chip8 chip8;
    chip8.V[0] = 0xDE;
    chip8.V[1] = 0xAD;
    chip8.V[2] = 0xBE;
    chip8.V[3] = 0xEF;
    chip8.I = 0x0300;
    chip8.opcode = 0xF355;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.I == 0x0304);
    REQUIRE(chip8.memory[0x0300] == 0xDE);
    REQUIRE(chip8.memory[0x0301] == 0xAD);
    REQUIRE(chip8.memory[0x0302] == 0xBE);
    REQUIRE(chip8.memory[0x0303] == 0xEF);
}

TEST_CASE("OPCODE FX65", "[opcode]")
{
    Chip8 chip8;
    chip8.memory[0x0300] = 0xDE;
    chip8.memory[0x0301] = 0xAD;
    chip8.memory[0x0302] = 0xBE;
    chip8.memory[0x0303] = 0xEF;
    chip8.I = 0x0300;
    chip8.opcode = 0xF365;

    chip8.ExecuteOpcode();

    REQUIRE(chip8.I == 0x0304);
    REQUIRE(chip8.V[0] == 0xDE);
    REQUIRE(chip8.V[1] == 0xAD);
    REQUIRE(chip8.V[2] == 0xBE);
    REQUIRE(chip8.V[3] == 0xEF);
}