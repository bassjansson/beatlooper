#ifndef __TRACK_H__
#define __TRACK_H__

#include <iostream>
#include <cmath>

#include "Defines.h"
#include "Fractions.h"

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
    Track(int trackIndex, int inputChannelLeft, int inputChannelRight) :
        trackIndex(trackIndex),
        trackState(STOPPED),
        recStartFrame(0),
        recLengthFrames(0),
        recLengthFramesMax(TRACK_BUFFER_LENGTH * AUDIO_SAMPLE_RATE),
        preRecPointerFrame(0),
        preRecLengthFrames(TRACK_PREREC_LENGTH * AUDIO_SAMPLE_RATE / 1000),
        inputChannelLeft(inputChannelLeft),
        inputChannelRight(inputChannelRight),
        shouldFadeIn(false)
    {
        trackBufferSize  = recLengthFramesMax * TRACK_NUM_CHANNELS;
        preRecBufferSize = preRecLengthFrames * TRACK_NUM_CHANNELS;

        trackBuffer  = new float[trackBufferSize];
        preRecBuffer = new float[preRecBufferSize];

        for (frame_t i = 0; i < trackBufferSize; ++i)
            trackBuffer[i] = 0.0f;

        for (frame_t i = 0; i < preRecBufferSize; ++i)
            preRecBuffer[i] = 0.0f;
    }

    ~Track()
    {
        delete[] trackBuffer;
        delete[] preRecBuffer;
    }

    void startRecording()
    {
        recStartFrame   = 0;
        recLengthFrames = 0;

        for (frame_t i = 0; i < trackBufferSize; ++i)
            trackBuffer[i] = 0.0f;

        trackState = RECORDING;
    }

    void stopRecording(bool shouldStartPlayback)
    {
        stopPlayback();

        frame_t minRecLengthFrames = recLengthFrames;

        if (recLengthDenominator == 0)
        {
            recLengthDenominator = recLengthFrames;

            cout << "[Track " << trackIndex + 1 << "] ";
            cout << "Setting track and denominator recording length to ";
            cout << (float) recLengthDenominator / AUDIO_SAMPLE_RATE << " seconds." << endl;
        }
        else
        {
            double recLengthFraction = (double) recLengthFrames / recLengthDenominator;
            double idealFraction;

            double previousDiff = 1000.0;
            double currentDiff;

            double chosenFraction   = 1.0;
            int chosenFractionIndex = FRACTIONS_ARRAY_LENGTH / 2;

            for (int i = 0; i < FRACTIONS_ARRAY_LENGTH; ++i)
            {
                idealFraction = (double) FRACTIONS_ARRAY[i].x / FRACTIONS_ARRAY[i].y;
                currentDiff   = abs(recLengthFraction - idealFraction);

                if (currentDiff < previousDiff)
                {
                    previousDiff        = currentDiff;
                    chosenFraction      = idealFraction;
                    chosenFractionIndex = i;
                }
            }

            recLengthFrames = (frame_t) (recLengthDenominator * chosenFraction + 0.5);

            cout << "[Track " << trackIndex + 1 << "] ";
            cout << "Setting track recording length to ";
            cout << FRACTIONS_ARRAY[chosenFractionIndex].x << "/";
            cout << FRACTIONS_ARRAY[chosenFractionIndex].y << " of denominator." << endl;
        }

        if (recLengthFrames < minRecLengthFrames)
            minRecLengthFrames = recLengthFrames;

        frame_t fadeLengthFrames = TRACK_FADE_LENGTH * AUDIO_SAMPLE_RATE / 1000;

        if (fadeLengthFrames * 2 > minRecLengthFrames)
            fadeLengthFrames = minRecLengthFrames / 2;

        for (frame_t i = 0; i < fadeLengthFrames; ++i)
        {
            float f = (float) i / (fadeLengthFrames - 1);

            // Fade in
            trackBuffer[i * TRACK_NUM_CHANNELS + LEFT]  *= f;
            trackBuffer[i * TRACK_NUM_CHANNELS + RIGHT] *= f;

            // Fade out
            trackBuffer[(minRecLengthFrames - 1 - i) * TRACK_NUM_CHANNELS + LEFT]  *= f;
            trackBuffer[(minRecLengthFrames - 1 - i) * TRACK_NUM_CHANNELS + RIGHT] *= f;
        }

        if (shouldStartPlayback)
            startPlayback();
    } // stopRecording

    void startPlayback()
    {
        shouldFadeIn = true;
        trackState   = PLAYING;
    }

    void stopPlayback()
    {
        trackState = STOPPED;
    }

    void toggleRecording()
    {
        switch (trackState)
        {
            case STOPPED: startRecording();
                break;
            case PLAYING: startRecording();
                break;
            case RECORDING: stopRecording(STOPPED);
                break;
        }

        printTrackStatus();
    }

    void togglePlayback()
    {
        switch (trackState)
        {
            case STOPPED: startPlayback();
                break;
            case PLAYING: stopPlayback();
                break;
            case RECORDING: stopRecording(PLAYING);
                break;
        }

        printTrackStatus();
    }

    void printTrackStatus()
    {
        cout << "[Track " << trackIndex + 1 << "] ";

        switch (trackState)
        {
            case STOPPED: cout << "STOPPED";
                break;
            case PLAYING: cout << "PLAYING";
                break;
            case RECORDING: cout << "RECORDING";
                break;
        }

        cout << endl;
    }

    void process(
        const float * inputBuffer,
        float *       outputBuffer,
        frame_t       framesPerBuffer,
        int           numInputChannels,
        int           numOutputChannels,
        frame_t       currentFrame)
    {
        if (trackState == RECORDING)
        {
            if (recLengthFrames == 0)
                recStartFrame = currentFrame - (TRACK_PREREC_LENGTH * AUDIO_SAMPLE_RATE / 1000);

            if (recLengthFrames + framesPerBuffer < recLengthFramesMax)
            {
                for (frame_t i = 0; i < framesPerBuffer; ++i)
                {
                    trackBuffer[recLengthFrames * TRACK_NUM_CHANNELS + LEFT] =
                      inputBuffer[i * numInputChannels + inputChannelLeft];
                    trackBuffer[recLengthFrames * TRACK_NUM_CHANNELS + RIGHT] =
                      inputBuffer[i * numInputChannels + inputChannelRight];

                    recLengthFrames++;
                }
            }
            else
            {
                trackState = STOPPED;
            }
        }
        else // trackState != RECORDING
        {
            // Write input to pre recording buffer
            for (frame_t i = 0; i < framesPerBuffer; ++i)
            {
                preRecBuffer[preRecPointerFrame * TRACK_NUM_CHANNELS + LEFT] =
                  inputBuffer[i * numInputChannels + inputChannelLeft];
                preRecBuffer[preRecPointerFrame * TRACK_NUM_CHANNELS + RIGHT] =
                  inputBuffer[i * numInputChannels + inputChannelRight];

                preRecPointerFrame = (preRecPointerFrame + 1) % preRecLengthFrames;
            }

            if (trackState == PLAYING)
            {
                if (recLengthFrames > 0)
                {
                    frame_t p;

                    if (shouldFadeIn)
                    {
                        shouldFadeIn = false;
                        float f;

                        for (frame_t i = 0; i < framesPerBuffer; ++i)
                        {
                            p = (currentFrame + i - recStartFrame) % recLengthFrames;
                            f = (float) i / (framesPerBuffer - 1);

                            outputBuffer[i * numOutputChannels + LEFT] +=
                              trackBuffer[p * TRACK_NUM_CHANNELS + LEFT] * f;
                            outputBuffer[i * numOutputChannels + RIGHT] +=
                              trackBuffer[p * TRACK_NUM_CHANNELS + RIGHT] * f;
                        }
                    }
                    else
                    {
                        for (frame_t i = 0; i < framesPerBuffer; ++i)
                        {
                            p = (currentFrame + i - recStartFrame) % recLengthFrames;

                            outputBuffer[i * numOutputChannels + LEFT] +=
                              trackBuffer[p * TRACK_NUM_CHANNELS + LEFT];
                            outputBuffer[i * numOutputChannels + RIGHT] +=
                              trackBuffer[p * TRACK_NUM_CHANNELS + RIGHT];
                        }
                    }
                }
            }
        }
    } // process

private:
    const int trackIndex;
    TrackState trackState;

    frame_t trackBufferSize;
    float * trackBuffer;

    frame_t recStartFrame;
    frame_t recLengthFrames;
    const frame_t recLengthFramesMax;
    static frame_t recLengthDenominator;

    frame_t preRecBufferSize;
    float * preRecBuffer;

    frame_t preRecPointerFrame;
    const frame_t preRecLengthFrames;

    int inputChannelLeft;
    int inputChannelRight;

    bool shouldFadeIn;
};

frame_t Track::recLengthDenominator = 0;

#endif // __TRACK_H__
