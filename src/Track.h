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
    Track(int inputChannelLeft, int inputChannelRight) :
        MAX_TRACK_TICKS(TRACK_BUFFER_LENGTH * AUDIO_SAMPLE_RATE / AUDIO_BUFFER_SIZE),
        inputChannelLeft(inputChannelLeft),
        inputChannelRight(inputChannelRight)
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

        recStartTicks  = 0;
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
        unsigned long offset;

        switch (trackState)
        {
            case STOPPED:
                break;

            case PLAYING:
                if (recLengthTicks > 0)
                {
                    offset = ((currentTicks - recStartTicks) % recLengthTicks) * AUDIO_BUFFER_SIZE;

                    for (int i = 0; i < framesPerBuffer; ++i)
                    {
                        outputBuffer[i * numOutputChannels + LEFT] +=
                          trackBuffer[(i + offset) * TRACK_NUM_CHANNELS + LEFT];
                        outputBuffer[i * numOutputChannels + RIGHT] +=
                          trackBuffer[(i + offset) * TRACK_NUM_CHANNELS + RIGHT];
                    }
                }

                break;

            case RECORDING:
                if (recStartTicks == 0 && recLengthTicks == 0)
                    recStartTicks = currentTicks;

                if (recLengthTicks < MAX_TRACK_TICKS)
                {
                    offset = recLengthTicks * AUDIO_BUFFER_SIZE;

                    for (int i = 0; i < framesPerBuffer; ++i)
                    {
                        trackBuffer[(i + offset) * TRACK_NUM_CHANNELS + LEFT] =
                          inputBuffer[i * numInputChannels + inputChannelLeft];
                        trackBuffer[(i + offset) * TRACK_NUM_CHANNELS + RIGHT] =
                          inputBuffer[i * numInputChannels + inputChannelRight];
                    }

                    recLengthTicks++;

                    if (recLengthTicks >= MAX_TRACK_TICKS)
                        trackState = PLAYING;
                }

                break;
        }
    } // process

private:
    TrackState trackState;
    unsigned long trackBufferSize;
    float * trackBuffer;

    const tick_t MAX_TRACK_TICKS;
    tick_t recStartTicks;
    tick_t recLengthTicks;

    int inputChannelLeft;
    int inputChannelRight;
};

#endif // __TRACK_H__
