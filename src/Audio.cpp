#include "Audio.hpp"

Audio::Audio()
{
    SDL_AudioSpec desiredSpec;

    desiredSpec.freq = AUDIO_FREQUENCY;
    desiredSpec.format = AUDIO_S16SYS;
    desiredSpec.channels = 1;
    desiredSpec.samples = 2048;
    desiredSpec.callback = audio_callback;
    desiredSpec.userdata = this;

    SDL_AudioSpec obtainedSpec;

    SDL_OpenAudio(&desiredSpec, &obtainedSpec);

    SDL_PauseAudio(0);
}

Audio::~Audio() { SDL_CloseAudio(); }

void Audio::beep(double frequency, int duration)
{
    Beep b;
    b.frequency = frequency;
    b.samplesLeft = duration * AUDIO_FREQUENCY / 1000;

    SDL_LockAudio();
    currentBeep = b;
    SDL_UnlockAudio();
}

void Audio::generateSamples(int16_t *stream, int length)
{
    int i = 0;
    while (i < length) {
        // If there are no more samples to play, fill the stream with 0
        if (currentBeep.samplesLeft == 0) {
            memset(stream + i, 0, sizeof(int16_t) * (length - i));
            return;
        }

        // Calculate the number of samples to do
        // minimum between stream pos + samples left and the length of the stream
        int samplesToDo = std::min(i + currentBeep.samplesLeft, length);
        currentBeep.samplesLeft -= samplesToDo - i;

        while (i < samplesToDo) {
            stream[i++] = AUDIO_AMPLITUDE * std::sin(x * 2 * M_PI / AUDIO_FREQUENCY);
            x += currentBeep.frequency;
        }
    }
}

void Audio::audio_callback(void *_audio, uint8_t *_stream, int _length)
{
    Audio *audio = (Audio *)_audio;
    int16_t *stream = (int16_t *)_stream;
    int length = _length / 2;

    audio->generateSamples(stream, length);
}