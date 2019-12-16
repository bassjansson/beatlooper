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
        // Return if we are already recording
        if (trackState == RECORDING)
            return;

        // Reset start frame and recording length
        recStartFrame   = 0;
        recLengthFrames = preRecLengthFrames;

        // Clear track buffer
        for (frame_t i = 0; i < trackBufferSize; ++i)
            trackBuffer[i] = 0.0f;

        // Start recording
        trackState = RECORDING;

        // Write pre recording buffer to track buffer
        for (frame_t i = 0; i < preRecBufferSize; ++i)
            trackBuffer[i] = preRecBuffer[(preRecPointerFrame * TRACK_NUM_CHANNELS + i) % preRecBufferSize];
    }

    void stopRecording(bool shouldStartPlayback)
    {
        // Return if we are not recording
        if (trackState != RECORDING)
            return;

        // Stop recording
        trackState = STOPPED;

        // Save initial minimum recording length
        frame_t minRecLengthFrames = recLengthFrames;

        // Remove pre recording length at end of recording length
        recLengthFrames -= preRecLengthFrames;

        // Check if this is the very first recording which will set the fraction denominator
        if (recLengthDenominator == 0)
        {
            // Set the fraction denominator to the current recording length
            recLengthDenominator = recLengthFrames;

            // Log it
            cout << "[Track " << trackIndex + 1 << "] ";
            cout << "Setting track and denominator recording length to ";
            cout << (float) recLengthDenominator / AUDIO_SAMPLE_RATE << " seconds." << endl;
        }
        // Else, round the recording length to the nearest fraction
        else
        {
            double recLengthFraction = (double) recLengthFrames / recLengthDenominator;
            double idealFraction;

            double previousDiff = 1000.0;
            double currentDiff;

            double chosenFraction   = 1.0;
            int chosenFractionIndex = FRACTIONS_ARRAY_LENGTH / 2;

            // Find the nearest fraction
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

            // Set recording length to the chosen fraction and clip it by the maximum
            recLengthFrames = (frame_t) (recLengthDenominator * chosenFraction + 0.5);

            if (recLengthFrames > recLengthFramesMax)
                recLengthFrames = recLengthFramesMax;

            // Log it
            cout << "[Track " << trackIndex + 1 << "] ";
            cout << "Setting track recording length to ";
            cout << FRACTIONS_ARRAY[chosenFractionIndex].x << "/";
            cout << FRACTIONS_ARRAY[chosenFractionIndex].y << " of denominator." << endl;
        }

        // In case recording length got smaller than the minimum, set the minimum to it
        if (recLengthFrames < minRecLengthFrames)
            minRecLengthFrames = recLengthFrames;

        // Get fade length within the bounderies of the minimum recording length
        frame_t fadeLengthFrames = TRACK_FADE_LENGTH * AUDIO_SAMPLE_RATE / 1000;

        if (fadeLengthFrames * 2 > minRecLengthFrames)
            fadeLengthFrames = minRecLengthFrames / 2;

        // Fade in and fade out the track buffer
        float f;
        for (frame_t i = 0; i < fadeLengthFrames; ++i)
        {
            f = (float) i / (fadeLengthFrames - 1);

            // Fade in
            trackBuffer[i * TRACK_NUM_CHANNELS + LEFT]  *= f;
            trackBuffer[i * TRACK_NUM_CHANNELS + RIGHT] *= f;

            // Fade out
            trackBuffer[(minRecLengthFrames - 1 - i) * TRACK_NUM_CHANNELS + LEFT]  *= f;
            trackBuffer[(minRecLengthFrames - 1 - i) * TRACK_NUM_CHANNELS + RIGHT] *= f;
        }

        // Start playback if requested
        if (shouldStartPlayback)
            startPlayback();
    } // stopRecording

    void startPlayback()
    {
        // Return if we are already playing
        if (trackState == PLAYING)
            return;

        // Start playback
        shouldFadeIn = true;
        trackState   = PLAYING;
    }

    void stopPlayback()
    {
        // Return if we are not playing
        if (trackState != PLAYING)
            return;

        // Stop playback
        // TODO: add fade out
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
        // Check if we are recording
        if (trackState == RECORDING)
        {
            // In case the start frame is not set yet, set it to the current frame
            if (recStartFrame == 0)
                recStartFrame = currentFrame - preRecLengthFrames;

            // Check if we have space left in the track buffer for recording
            if (recLengthFrames + framesPerBuffer <= recLengthFramesMax)
            {
                // Write input to track buffer
                for (frame_t i = 0; i < framesPerBuffer; ++i)
                {
                    trackBuffer[recLengthFrames * TRACK_NUM_CHANNELS + LEFT] =
                      inputBuffer[i * numInputChannels + inputChannelLeft];
                    trackBuffer[recLengthFrames * TRACK_NUM_CHANNELS + RIGHT] =
                      inputBuffer[i * numInputChannels + inputChannelRight];

                    recLengthFrames++;
                }
            }
            // Else, clear and stop the recording
            else
            {
                // Reset start frame and recording length
                recStartFrame   = 0;
                recLengthFrames = 0;

                // Clear track buffer
                for (frame_t i = 0; i < trackBufferSize; ++i)
                    trackBuffer[i] = 0.0f;

                // Stop recording
                trackState = STOPPED;
            }
        }
        // Else, we are in playback
        else
        {
            // TODO: Should we do this also during record?
            // Write input to pre recording buffer
            for (frame_t i = 0; i < framesPerBuffer; ++i)
            {
                preRecBuffer[preRecPointerFrame * TRACK_NUM_CHANNELS + LEFT] =
                  inputBuffer[i * numInputChannels + inputChannelLeft];
                preRecBuffer[preRecPointerFrame * TRACK_NUM_CHANNELS + RIGHT] =
                  inputBuffer[i * numInputChannels + inputChannelRight];

                preRecPointerFrame = (preRecPointerFrame + 1) % preRecLengthFrames;
            }

            // Check if we are playing and have any frames to play
            if (trackState == PLAYING && recLengthFrames > 0)
            {
                // Check if we should fade in
                if (shouldFadeIn)
                {
                    // We shouldn't fade in a second time
                    shouldFadeIn = false;

                    // Write track buffer to output with fade in
                    frame_t p;
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
                // Else, just play
                else
                {
                    // Write track buffer to output
                    frame_t p;
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
