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
    Track(int trackNumber, int inputChannelLeft, int inputChannelRight) :
        trackNumber(trackNumber),
        trackState(STOPPED),
        recStartFrame(0),
        recLengthFrames(0),
        recLengthFramesMax(TRACK_BUFFER_LENGTH * AUDIO_SAMPLE_RATE),
        inputChannelLeft(inputChannelLeft),
        inputChannelRight(inputChannelRight)
    {
        trackBufferSize = recLengthFramesMax * TRACK_NUM_CHANNELS;
        trackBuffer     = new float[trackBufferSize];
    }

    ~Track()
    {
        delete[] trackBuffer;
    }

    void startRecording()
    {
        recStartFrame   = 0;
        recLengthFrames = 0;

        for (frame_t i = 0; i < trackBufferSize; ++i)
            trackBuffer[i] = 0.0f;

        trackState = RECORDING;
    }

    void stopRecording(TrackState stateAfter)
    {
        trackState = stateAfter;
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
            case STOPPED: trackState = PLAYING;
                break;
            case PLAYING: trackState = STOPPED;
                break;
            case RECORDING: stopRecording(PLAYING);
                break;
        }

        printTrackStatus();
    }

    void printTrackStatus()
    {
        cout << "[Track " << trackNumber << "] ";

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
        frame_t playPositionFrame;

        switch (trackState)
        {
            case STOPPED:
                break;

            case PLAYING:
                if (recLengthFrames > 0)
                {
                    playPositionFrame = (currentFrame - recStartFrame) % recLengthFrames;

                    for (frame_t i = 0; i < framesPerBuffer; ++i)
                    {
                        outputBuffer[i * numOutputChannels + LEFT] +=
                          trackBuffer[(i + playPositionFrame) * TRACK_NUM_CHANNELS + LEFT];
                        outputBuffer[i * numOutputChannels + RIGHT] +=
                          trackBuffer[(i + playPositionFrame) * TRACK_NUM_CHANNELS + RIGHT];
                    }
                }

                break;

            case RECORDING:
                if (recLengthFrames == 0)
                    recStartFrame = currentFrame;

                if (recLengthFrames + framesPerBuffer < recLengthFramesMax)
                {
                    for (frame_t i = 0; i < framesPerBuffer; ++i)
                    {
                        trackBuffer[(i + recLengthFrames) * TRACK_NUM_CHANNELS + LEFT] =
                          inputBuffer[i * numInputChannels + inputChannelLeft];
                        trackBuffer[(i + recLengthFrames) * TRACK_NUM_CHANNELS + RIGHT] =
                          inputBuffer[i * numInputChannels + inputChannelRight];
                    }

                    recLengthFrames += framesPerBuffer;
                }
                else
                {
                    trackState = STOPPED;
                }

                break;
        }
    } // process

private:
    const int trackNumber;
    TrackState trackState;

    frame_t trackBufferSize;
    float * trackBuffer;

    frame_t recStartFrame;
    frame_t recLengthFrames;
    const frame_t recLengthFramesMax;

    int inputChannelLeft;
    int inputChannelRight;
};

#endif // __TRACK_H__
