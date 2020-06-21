#ifndef __EMULATOR_H__
#define __EMULATOR_H__

#pragma once
#include "Chip8.hpp"
#include "Audio.hpp"
#include <SDL2/SDL.h>
#include <chrono>

class Emulator
{
    Chip8 chip8;
    std::string romPath;
    bool debug; // Flag to print debug info in terminal

    long cpuFrequency;  // frequency in Hz
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

    Audio audio;

    double GetDeltaTime();
    void InitSDL();
    void DrawFrame();
    bool GetInput();

  public:
    Emulator(long cpuFrequency = 1000, float windowScale = 16);
    void Run();
    void SetRomPath(std::string romPath);
    std::string GetRomPath();
    void SetCpuFrequency(long clockFrequency);
    void SetWindowScale(float windowScale);
    void SetDebug(bool debug);
};

#endif // __EMULATOR_H__