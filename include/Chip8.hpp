#ifndef __CHIP8_H__
#define __CHIP8_H__

#pragma once
#include <cstdint>
#include <string>

#define CHIP8_MEMORY_SIZE 4096
#define CHIP8_PROGRAM_START 0x200
#define CHIP8_TIMER_FREQUENCY 60
#define CHIP8_VIDEO_WIDTH 64
#define CHIP8_VIDEO_HEIGHT 32

class Chip8
{
  public:
    uint8_t memory[CHIP8_MEMORY_SIZE];
    uint8_t V[16]; // Data registers
    uint16_t I;    // Address register
    uint8_t delayTimer;
    uint8_t soundTimer;
    uint16_t PC; // Program counter
    uint8_t SP;  // Stack pointer
    uint16_t stack[16];
    uint8_t keypad[16];
    uint8_t video[CHIP8_VIDEO_WIDTH * CHIP8_VIDEO_HEIGHT];

    uint16_t opcode; // current opcode

    Chip8();
    void LoadROM(std::string romPath);
    void Cycle();
    void Execute();
    uint8_t Random();

    // Opcodes
    void OP_00E0(); // Clear Screen
    void OP_00EE(); // Return
    void OP_1nnn(); // JMP to nnn
    void OP_2nnn(); // CALL nnn
    void OP_3xkk(); // SKIP if Vx == kk
    void OP_4xkk(); // SKIP if Vx != kk
    void OP_5xy0(); // SKIP if Vx == Vy
    void OP_6xkk(); // LOAD Vx = kk
    void OP_7xkk(); // ADD Vx = Vx + kk
    void OP_8xy0(); // LOAD Vx = Vy
    void OP_8xy1(); // OR Vx = Vx | Vy
    void OP_8xy2(); // AND Vx = Vx & Vy
    void OP_8xy3(); // XOR Vx = Vx ^ Vy
    void OP_8xy4(); // ADDC Vx = Vx + Vy, Set VF carry flag
    void OP_8xy5(); // SUBC Vx = Vx - Vy, Set VF if NOT borrow
    void OP_8xy6(); // SHR set VF to Vx lsb, Vx >> 1
    void OP_8xy7(); // SUBN if Vy > Vx set VF, Vx = Vy - Vx
    void OP_8xyE(); // SHL set VF to Vx msb, Vx << 1
    void OP_9xy0(); // SKIP if Vx != Vy
    void OP_Annn(); // LOAD I = nnn
    void OP_Bnnn(); // JMP to nnn + V0
    void OP_Cxkk(); // Get random byte and AND with kk, Store in Vx
    void OP_Dxyn(); // Draw
    void OP_Ex9E(); // SKIP if key with value Vx is pressed
    void OP_ExA1(); // SKIP if key with value Vx is not pressed
    void OP_Fx07(); // LOAD Vx = delayTimer
    void OP_Fx0A(); // Wait for keypress, then store in Vx
    void OP_Fx15(); // LOAD delayTimer = Vx
    void OP_Fx18(); // LOAD soundTimer = Vx
    void OP_Fx1E(); // ADD I = I + Vx
    void OP_Fx29(); // LOAD Set I to location of sprite digit in Vx
    void OP_Fx33(); // Store BCD of Vx at I, I+1, I+2
    void OP_Fx55(); // Store registers V0, V1, ..., Vx at I
    void OP_Fx65(); // Read registers V0, V1, ..., Vx from I
};

#endif // __CHIP8_H__