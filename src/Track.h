#ifndef __TRACK_H__
#define __TRACK_H__

#include <iostream>
#include <cmath>

#include "Defines.h"

using namespace std;

enum TrackState
{
    STOPPED = 0,
    PLAYING,
    RECORDING
};

class Track
{
public:
    Track() : MAX_TRACK_TICKS(TRACK_BUFFER_LENGTH * AUDIO_SAMPLE_RATE / AUDIO_BUFFER_SIZE)
    {
        trackState      = STOPPED;
        trackBufferSize = MAX_TRACK_TICKS * AUDIO_BUFFER_SIZE * TRACK_NUM_CHANNELS;
        trackBuffer     = new float[trackBufferSize];

        clearRecording();
    }

    ~Track()
    {
        delete[] trackBuffer;
    }

    void clearRecording()
    {
        for (unsigned long i = 0; i < trackBufferSize; ++i)
            trackBuffer[i] = 0.0f;

        recStartTicks = 0;
        recLengthTicks = 0;
    }

    void toggleRecord()
    {
        switch (trackState)
        {
            case STOPPED:
                clearRecording();
                trackState = RECORDING;
                break;

            case PLAYING:
                clearRecording();
                trackState = RECORDING;
                break;

            case RECORDING:
                trackState = STOPPED;
                break;
        }
    }

    void togglePlayStop()
    {
        switch (trackState)
        {
            case STOPPED:
                trackState = PLAYING;
                break;

            case PLAYING:
                trackState = STOPPED;
                break;

            case RECORDING:
                trackState = PLAYING;
                break;
        }
    }

    void process(
        const float * inputBuffer,
        float *       outputBuffer,
        unsigned long framesPerBuffer,
        int           numInputChannels,
        int           numOutputChannels,
        tick_t        currentTicks)
    {
        switch (trackState)
        {
            case STOPPED:
                break;

            case PLAYING:
                unsigned long offset = ((currentTicks - recStartTicks) % recLengthTicks) * AUDIO_BUFFER_SIZE;

                for (int i = 0; i < framesPerBuffer; ++i)
                {
                    outputBuffer[i * numOutputChannels + 0] += trackBuffer[(i + offset) * TRACK_NUM_CHANNELS + 0];
                    outputBuffer[i * numOutputChannels + 1] += trackBuffer[(i + offset) * TRACK_NUM_CHANNELS + 1];
                }

                break;

            case RECORDING:
                if (recStartTicks == 0 && recLengthTicks == 0)
                    recStartTicks = currentTicks;

                if (recLengthTicks < MAX_TRACK_TICKS)
                {
                    unsigned long offset = recLengthTicks * AUDIO_BUFFER_SIZE;

                    for (int i = 0; i < framesPerBuffer; ++i)
                    {
                        trackBuffer[(i + offset) * TRACK_NUM_CHANNELS + 0] = inputBuffer[i * numInputChannels + 0];
                        trackBuffer[(i + offset) * TRACK_NUM_CHANNELS + 1] = inputBuffer[i * numInputChannels + 1];
                    }

                    recLengthTicks++;

                    if (recLengthTicks >= MAX_TRACK_TICKS)
                        trackState = PLAYING;
                }

                break;
        }
    }

private:
    TrackState trackState;
    unsigned long trackBufferSize;
    float * trackBuffer;

    const tick_t MAX_TRACK_TICKS;
    tick_t recStartTicks;
    tick_t recLengthTicks;
};

#endif // __TRACK_H__
