#ifndef __EMULATOR_H__
#define __EMULATOR_H__

#pragma once
#include "Chip8.hpp"
#include <SDL2/SDL.h>
#include <chrono>

class Emulator
{
    Chip8 chip8;
    std::string romPath;

    long cpuFrequency; // frequency in Hz
    double cpuDuration;
    double cpuTime;
    double timerDuration;
    double timerTime;
    std::chrono::high_resolution_clock::time_point tpStart, tpEnd;

    SDL_Window *window;
    SDL_Surface *surface;
    SDL_Renderer *renderer;
    float windowScale;
    int windowWidth, windowHeight;

    double GetDeltaTime();
    void InitSDL();
    void DrawFrame();
    bool GetInput();

  public:
    Emulator(long cpuFrequency = 100, float windowScale = 8);
    void Run();
};

#endif // __EMULATOR_H__