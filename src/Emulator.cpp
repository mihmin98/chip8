#include "Emulator.hpp"

Emulator::Emulator(long cpuFrequency, float windowScale)
{
    this->cpuFrequency = cpuFrequency;
    this->windowScale = windowScale;

    window = NULL;

    cpuTime = 0;
    timerTime = 0;

    cpuDuration = 1.0 / cpuFrequency * 1000;
    timerDuration = 1.0 / cpuFrequency * 1000;

    romPath = "";
}

double Emulator::GetDeltaTime()
{
    return std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(
               tp2 - tp1)
        .count();
}

void Emulator::Run()
{
    if (!chip8.LoadROM(romPath))
        return;

    InitSDL();

    bool quit = false;
    tp1 = tp2 = std::chrono::high_resolution_clock::now();
    while (!quit) {
        tp1 = tp2;
        tp2 = std::chrono::high_resolution_clock::now();

        cpuTime += GetDeltaTime();
        timerTime += GetDeltaTime();

        if (cpuTime >= cpuDuration) {
            quit = GetInput();
            chip8.Cycle();
            DrawFrame();

            cpuTime -= cpuDuration;
        }

        if (timerTime >= timerDuration) {
            if (chip8.delayTimer > 0)
                --chip8.delayTimer;
            if (chip8.soundTimer > 0)
                --chip8.soundTimer;
            timerTime -= timerDuration;
        }
    }
}

void Emulator::InitSDL()
{
    windowWidth = CHIP8_VIDEO_WIDTH * windowScale;
    windowHeight = CHIP8_VIDEO_HEIGHT * windowScale;

    window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, windowWidth,
                              windowHeight, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        std::cerr << "InitSDL() error: " << SDL_GetError() << "\n";
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_RenderClear(renderer);
    SDL_RenderSetScale(renderer, windowScale, windowScale);
    SDL_RenderPresent(renderer);
}

void Emulator::DrawFrame()
{
    SDL_Point whitePoints[CHIP8_VIDEO_HEIGHT * CHIP8_VIDEO_WIDTH],
        blackPoints[CHIP8_VIDEO_HEIGHT * CHIP8_VIDEO_WIDTH];

    int whiteCount = 0, blackCount = 0;

    // Iterate over the video memory and see which pixels are white or black
    for (int i = 0; i < CHIP8_VIDEO_HEIGHT; ++i) {
        for (int j = 0; j < CHIP8_VIDEO_WIDTH; ++j) {
            if (chip8.video[i * CHIP8_VIDEO_WIDTH + j] == 1) {
                whitePoints[whiteCount].x = j;
                whitePoints[whiteCount].y = i;
                ++whiteCount;
            } else {
                blackPoints[blackCount].x = j;
                blackPoints[blackCount].y = i;
                ++blackCount;
            }
        }
    }

    SDL_RenderClear(renderer);

    // Draw black pixels
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawPoints(renderer, blackPoints, blackCount);

    // Draw white pixels
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawPoints(renderer, whitePoints, whiteCount);

    SDL_RenderPresent(renderer);
}

bool Emulator::GetInput()
{
    bool quit = false;
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            quit = true;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                quit = true;
                break;
            case SDLK_x:
                chip8.keypad[0] = 1;
                break;
            case SDLK_1:
                chip8.keypad[1] = 1;
                break;
            case SDLK_2:
                chip8.keypad[2] = 1;
                break;
            case SDLK_3:
                chip8.keypad[3] = 1;
                break;
            case SDLK_q:
                chip8.keypad[4] = 1;
                break;
            case SDLK_w:
                chip8.keypad[5] = 1;
                break;
            case SDLK_e:
                chip8.keypad[6] = 1;
                break;
            case SDLK_a:
                chip8.keypad[7] = 1;
                break;
            case SDLK_s:
                chip8.keypad[8] = 1;
                break;
            case SDLK_d:
                chip8.keypad[9] = 1;
                break;
            case SDLK_z:
                chip8.keypad[0xA] = 1;
                break;
            case SDLK_c:
                chip8.keypad[0xB] = 1;
                break;
            case SDLK_4:
                chip8.keypad[0xC] = 1;
                break;
            case SDLK_r:
                chip8.keypad[0xD] = 1;
                break;
            case SDLK_f:
                chip8.keypad[0xE] = 1;
                break;
            case SDLK_v:
                chip8.keypad[0xF] = 1;
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.sym) {
            case SDLK_x:
                chip8.keypad[0] = 0;
                break;
            case SDLK_1:
                chip8.keypad[1] = 0;
                break;
            case SDLK_2:
                chip8.keypad[2] = 0;
                break;
            case SDLK_3:
                chip8.keypad[3] = 0;
                break;
            case SDLK_q:
                chip8.keypad[4] = 0;
                break;
            case SDLK_w:
                chip8.keypad[5] = 0;
                break;
            case SDLK_e:
                chip8.keypad[6] = 0;
                break;
            case SDLK_a:
                chip8.keypad[7] = 0;
                break;
            case SDLK_s:
                chip8.keypad[8] = 0;
                break;
            case SDLK_d:
                chip8.keypad[9] = 0;
                break;
            case SDLK_z:
                chip8.keypad[0xA] = 0;
                break;
            case SDLK_c:
                chip8.keypad[0xB] = 0;
                break;
            case SDLK_4:
                chip8.keypad[0xC] = 0;
                break;
            case SDLK_r:
                chip8.keypad[0xD] = 0;
                break;
            case SDLK_f:
                chip8.keypad[0xE] = 0;
                break;
            case SDLK_v:
                chip8.keypad[0xF] = 0;
                break;
            }
            break;
        }
    }
    return quit;
}

void Emulator::SetRomPath(std::string romPath) { this->romPath = romPath; }

std::string Emulator::GetRomPath() { return romPath; }

void Emulator::SetCpuFrequency(long cpuFrequency)
{
    this->cpuFrequency = cpuFrequency;
}

void Emulator::SetWindowScale(float windowScale)
{
    this->windowScale = windowScale;
}