#ifndef __AUDIO_H__
#define __AUDIO_H__

#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <cmath>
#include <queue>

#define AUDIO_AMPLITUDE 14000
#define AUDIO_FREQUENCY 44100

struct Beep {
    double frequency;
    int samplesLeft;
};

class Audio
{
    double x;
    Beep currentBeep;

  public:
    Audio();
    ~Audio();
    void beep(double frequency, int duration); // duration in ms
    void generateSamples(int16_t *stream, int length);

    static void audio_callback(void *audio, uint8_t *stream, int length);
};

#endif // __AUDIO_H__