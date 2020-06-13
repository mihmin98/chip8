#include "catch.hpp"

#include "Chip8.hpp"

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