#include "Chip8.hpp"

// Clear Screen
void Chip8::OP_00E0() { memset(video, 0x0, sizeof(video)); }

// Return
void Chip8::OP_00EE() { PC = stack[SP--]; }

// JMP to nnn
void Chip8::OP_1nnn() { PC = opcode & 0x0FFF; }

// CALL nnn
void Chip8::OP_2nnn()
{
    stack[++SP] = PC;
    PC = opcode & 0x0FFF;
}

// SKIP if Vx == kk
void Chip8::OP_3xkk()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t kk = opcode & 0x00FF;
    if (V[x] == kk)
        PC += 2;
}

// SKIP if Vx != kk
void Chip8::OP_4xkk()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t kk = opcode & 0x00FF;
    if (V[x] != kk)
        PC += 2;
}

// SKIP if Vx == Vy
void Chip8::OP_5xy0()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    if (V[x] == V[y])
        PC += 2;
}

// LOAD Vx = kk
void Chip8::OP_6xkk()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t kk = opcode & 0x00FF;
    V[x] = kk;
}

// ADD Vx = Vx + kk
void Chip8::OP_7xkk()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t kk = opcode & 0x00FF;
    V[x] += kk;
}

// LOAD Vx = Vy
void Chip8::OP_8xy0()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    V[x] = V[y];
}

// OR Vx = Vx | Vy
void Chip8::OP_8xy1()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    V[x] |= V[y];
}

// AND Vx = Vx & Vy
void Chip8::OP_8xy2()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    V[x] &= V[y];
}

// XOR Vx = Vx ^ Vy
void Chip8::OP_8xy3()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    V[x] ^= V[y];
}

// ADDC Vx = Vx + Vy, Set VF carry flag
void Chip8::OP_8xy4()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    uint16_t result = V[x] + V[y];
    V[0xF] = result > 255 ? 1 : 0;
    V[x] = result & 0xFF;
}

// SUBC Vx = Vx - Vy, Set VF if NOT borrow
void Chip8::OP_8xy5()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    V[0xF] = V[x] > V[y] ? 1 : 0;
    V[x] -= V[y];
}

// SHR set VF to Vx lsb, Vx >> 1
void Chip8::OP_8xy6()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    V[0xF] = V[x] & 0x1;
    V[x] >>= 1;
}

// SUBN if Vy > Vx set VF, Vx = Vy - Vx
void Chip8::OP_8xy7()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    V[0xF] = V[y] > V[x] ? 1 : 0;
    V[x] = V[y] - V[x];
}

// SHL set VF to Vx msb, Vx << 1
void Chip8::OP_8xyE()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    V[0xF] = (V[x] & 0x80) >> 7;
    V[x] <<= 1;
}

// SKIP if Vx != Vy
void Chip8::OP_9xy0()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    if (V[x] != V[y])
        PC += 2;
}

// LOAD I = nnn
void Chip8::OP_Annn() { I = opcode & 0x0FFF; }

// JMP to nnn + V0
void Chip8::OP_Bnnn() { PC = (opcode & 0x0FFF) + V[0]; }

// Get random byte and AND with kk, Store in Vx
void Chip8::OP_Cxkk()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t kk = opcode & 0x00FF;
    V[x] = Random() & kk;
}

// Draw
void Chip8::OP_Dxyn()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    uint8_t n = opcode & 0x000F;
    uint8_t spriteByte, spritePixel, displayPixel;
    uint16_t displayPos;
    ;

    uint8_t xPos = V[x] % CHIP8_VIDEO_WIDTH;
    uint8_t yPos = V[y] % CHIP8_VIDEO_WIDTH;
    V[0xF] = 0;

    // The sprite dimensions are (8, n); Each line is represented as a byte
    for (uint8_t i = 0; i < n; ++i) {
        spriteByte = memory[I + i];
        for (uint8_t j = 0; j < 8; ++j) {
            spritePixel = (spriteByte & (0x80 >> j)) >> (8 - j - 1);
            displayPos = (yPos + i) * CHIP8_VIDEO_WIDTH + (xPos + j);
            displayPixel = video[displayPos];
            uint8_t result = spritePixel ^ displayPixel;
            if (displayPixel != result)
                V[0xF] = 1;
            // TODO: Check if i should use spritePixel instead of result
            video[displayPos] = result;
        }
    }
}

// SKIP if key with value Vx is pressed
void Chip8::OP_Ex9E()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    if (keypad[V[x]] == 1)
        PC += 2;
}

// SKIP if key with value Vx is not pressed
void Chip8::OP_ExA1()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    if (keypad[V[x]] == 1)
        PC += 2;
}

// LOAD Vx = delayTimer
void Chip8::OP_Fx07()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    V[x] = delayTimer;
}

// Wait for keypress, then store in Vx
void Chip8::OP_Fx0A()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    for (uint8_t i = 0; i < 16; ++i) {
        if (keypad[i] == 1) {
            V[x] = i;
            return;
        }
    }
    PC -= 2;
}

// LOAD delayTimer = Vx
void Chip8::OP_Fx15()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    delayTimer = V[x];
}

// LOAD soundTimer = Vx
void Chip8::OP_Fx18()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    soundTimer = V[x];
}

// ADD I = I + Vx
void Chip8::OP_Fx1E()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    I += V[x];
}

// LOAD Set I to location of sprite digit in Vx
void Chip8::OP_Fx29()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    I = CHIP8_FONTSET_START + V[x] * 5; // Sprites are 5 bytes long
}

// Store BCD of Vx at I, I+1, I+2
void Chip8::OP_Fx33()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t hundred = x / 100;
    uint8_t ten = x % 100 / 10;
    uint8_t one = x % 10;

    memory[I] = hundred;
    memory[I + 1] = ten;
    memory[I + 2] = one;
}

// Store registers V0, V1, ..., Vx at I
void Chip8::OP_Fx55()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    for (uint8_t i = 0; i <= x; ++i)
        memory[I + i] = V[i];
    I += x + 1;
}

// Read registers V0, V1, ..., Vx from I
void Chip8::OP_Fx65()
{
    uint8_t x = (opcode & 0x0F00) >> 8;
    for (uint8_t i = 0; i <= x; ++i)
        V[i] = memory[I + i];
    I += x + 1;
}