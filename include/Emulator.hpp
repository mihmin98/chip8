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
    double cpuDuration; // all times are in ms
    double cpuTime;
    double timerDuration;
    double timerTime;
    std::chrono::high_resolution_clock::time_point tp1, tp2;

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
    Emulator(long cpuFrequency = 1000, float windowScale = 16);
    void Run();
    void SetRomPath(std::string romPath);
    void SetCpuFrequency(long clockFrequency);
    void SetWindowScale(float windowScale);
};

#endif // __EMULATOR_H__