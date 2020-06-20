#include "Chip8.hpp"

Chip8::Chip8()
{
    // Clear all
    memset(memory, 0x0, sizeof(memory));
    memset(V, 0x0, sizeof(V));
    memset(stack, 0x0, sizeof(stack));
    memset(keypad, 0x0, sizeof(keypad));
    memset(video, 0x0, sizeof(video));
    drawFrame = false;

    // Init registers
    PC = CHIP8_PROGRAM_START;
    SP = 0;
    I = 0;
    delayTimer = 0;
    soundTimer = 0;

    // Add fontset
    uint8_t fontset[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    for (int i = 0; i < 80; ++i) {
        memory[CHIP8_FONTSET_START + i] = fontset[i];
    }
}

bool Chip8::LoadROM(std::string romPath)
{
    std::ifstream rom(romPath, std::ios::binary | std::ios::ate);
    if (rom.is_open()) {
        std::streamsize romSize = rom.tellg();
        if (romSize > 4096 - CHIP8_PROGRAM_START) {
            std::cerr << "ROM is too large\n";
            return false;
        }
        rom.seekg(0, std::ios::beg);
        rom.read((char *)(memory + CHIP8_PROGRAM_START), romSize);
        return true;
    } else {
        std::cerr << "ROM " << romPath << " could not be opened\n";
        return false;
    }
}

uint8_t Chip8::Random()
{
    srand(time(NULL));
    return rand() % 0xFF;
}

void Chip8::Cycle()
{
    opcode = (memory[PC] << 8) | memory[PC + 1];
    PC += 2;
    ExecuteOpcode();
}

void Chip8::ExecuteOpcode()
{
    switch ((opcode & 0xF000) >> 12) {
    case 0x0:
        switch (opcode & 0x00FF) {
        case 0xE0:
            OP_00E0();
            break;
        case 0xEE:
            OP_00EE();
            break;
        }
        break;
    case 0x1:
        OP_1nnn();
        break;
    case 0x2:
        OP_2nnn();
        break;
    case 0x3:
        OP_3xkk();
        break;
    case 0x4:
        OP_4xkk();
        break;
    case 0x5:
        OP_5xy0();
        break;
    case 0x6:
        OP_6xkk();
        break;
    case 0x7:
        OP_7xkk();
        break;
    case 0x8:
        switch (opcode & 0x000F) {
        case 0x0:
            OP_8xy0();
            break;
        case 0x1:
            OP_8xy1();
            break;
        case 0x2:
            OP_8xy2();
            break;
        case 0x3:
            OP_8xy3();
            break;
        case 0x4:
            OP_8xy4();
            break;
        case 0x5:
            OP_8xy5();
            break;
        case 0x6:
            OP_8xy6();
            break;
        case 0x7:
            OP_8xy7();
            break;
        case 0xE:
            OP_8xyE();
            break;
        }
        break;
    case 0x9:
        OP_9xy0();
        break;
    case 0xA:
        OP_Annn();
        break;
    case 0xB:
        OP_Bnnn();
        break;
    case 0xC:
        OP_Cxkk();
        break;
    case 0xD:
        OP_Dxyn();
        break;
    case 0xE:
        switch (opcode & 0x00FF) {
        case 0x9E:
            OP_Ex9E();
            break;
        case 0xA1:
            OP_ExA1();
            break;
        }
        break;
    case 0xF:
        switch (opcode & 0x00FF) {
        case 0x07:
            OP_Fx07();
            break;
        case 0x0A:
            OP_Fx0A();
            break;
        case 0x15:
            OP_Fx15();
            break;
        case 0x18:
            OP_Fx18();
            break;
        case 0x1E:
            OP_Fx1E();
            break;
        case 0x29:
            OP_Fx29();
            break;
        case 0x33:
            OP_Fx33();
            break;
        case 0x55:
            OP_Fx55();
            break;
        case 0x65:
            OP_Fx65();
            break;
        }
        break;
    }
}

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
    drawFrame = true;
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    uint8_t n = opcode & 0x000F;
    uint8_t spriteByte, spritePixel, displayPixel;
    uint16_t displayPos;

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
            // If white pixel is turned to black, then a collision occurs
            if (displayPixel == 1 && result == 0)
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
    if (keypad[V[x]] != 1)
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
    uint8_t hundred = V[x] / 100;
    uint8_t ten = V[x] % 100 / 10;
    uint8_t one = V[x] % 10;

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

void Chip8::PrintRegisters()
{
    std::cout << "\nRegisters\n";
    printf("PC: 0x%03X  opcode: 0x%04X  I: 0x%03X\n", PC, opcode, I);
    printf("SP: 0x%02X\n", SP);
    for (uint8_t i = 0; i < 16; ++i) {
        printf("%X: 0x%02x  ", i, stack[i]);
        if ((i + 1) % 8 == 0)
            printf("\n");
    }
    printf("\n");
    for (uint8_t i = 0; i < 16; ++i) {
        printf("V%X: 0x%02x  ", i, V[i]);
        if ((i + 1) % 8 == 0)
            printf("\n");
    }
    printf("\ndelay: %#02X  sound: %#02X\n", delayTimer, soundTimer);
    printf("keypad\n");
    for (uint8_t i = 0; i < 16; ++i) {
        printf("%X: %d  ", i, keypad[i]);
        if ((i + 1) % 8 == 0)
            printf("\n");
    }
}

// TODO: Add these debug functions
void Chip8::PrintMemory() {}

void Chip8::PrintVideoMemory() {}