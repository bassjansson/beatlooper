#ifndef __AUDIO_H__
#define __AUDIO_H__

#include <iostream>
#include <cmath>
#include <portaudio.h>

#define SAMPLE_RATE       48000
#define FRAMES_PER_BUFFER 64

using namespace std;

class Audio
{
public:
    Audio()
    {
        initError = Pa_Initialize();
        stream    = NULL;

        if (initError != paNoError)
            cout << "[Audio] Failed to initialize PortAudio." << endl;
    }

    ~Audio()
    {
        if (initError == paNoError)
            Pa_Terminate();
    }

    bool open()
    {
        if (initError != paNoError)
        {
            cout << "[Audio] Cannot open audio stream if PortAudio failed to initialize." << endl;
            return false;
        }

        int numDevices = Pa_GetDeviceCount();
        PaDeviceIndex defaultDeviceIndex = Pa_GetDefaultOutputDevice();

        cout << endl << "[Audio] Available audio devices:" << endl;
        for (int i = 0; i < numDevices; ++i)
        {
            cout << i << ": " << Pa_GetDeviceInfo(i)->name;
            cout << (defaultDeviceIndex == i ? " (default)" : "") << endl;
        }

        const PaDeviceInfo * deviceInfo = Pa_GetDeviceInfo(defaultDeviceIndex);
        if (defaultDeviceIndex == paNoDevice || !deviceInfo)
        {
            cout << "[Audio] Failed to get default audio device info." << endl;
            return false;
        }

        cout << endl << "[Audio] Selected default audio device: " << deviceInfo->name << endl;

        numInputChannels  = deviceInfo->maxInputChannels;
        numOutputChannels = deviceInfo->maxOutputChannels;

        PaSampleFormat sampleFormat = paFloat32;
        PaTime lowLatency = deviceInfo->defaultLowInputLatency;

        PaStreamParameters inputParameters;

        inputParameters.device           = defaultDeviceIndex;
        inputParameters.channelCount     = numInputChannels;
        inputParameters.sampleFormat     = sampleFormat;
        inputParameters.suggestedLatency = lowLatency;
        inputParameters.hostApiSpecificStreamInfo = NULL;

        PaStreamParameters outputParameters;

        outputParameters.device           = defaultDeviceIndex;
        outputParameters.channelCount     = numOutputChannels;
        outputParameters.sampleFormat     = sampleFormat;
        outputParameters.suggestedLatency = lowLatency;
        outputParameters.hostApiSpecificStreamInfo = NULL;

        PaError paError = Pa_OpenStream(
            &stream,
            &inputParameters,
            &outputParameters,
            SAMPLE_RATE,
            FRAMES_PER_BUFFER,
            paNoFlag,
            &Audio::paCallback,
            this);

        if (paError != paNoError)
        {
            cout << "[Audio] Failed to open audio stream." << endl;
            return false;
        }

        paError = Pa_SetStreamFinishedCallback(stream, &Audio::paStreamFinished);

        if (paError != paNoError)
        {
            cout << "[Audio] Failed to assign stream finished callback." << endl;
            Pa_CloseStream(stream);
            return false;
        }

        return true;
    } // open

    bool close()
    {
        if (!stream) return false;

        return Pa_CloseStream(stream) == paNoError;
    }

    bool start()
    {
        if (!stream) return false;

        return Pa_StartStream(stream) == paNoError;
    }

    bool stop()
    {
        if (!stream) return false;

        return Pa_StopStream(stream) == paNoError;
    }

private:
    int paCallbackMethod(
        const float *                    inputBuffer,
        float *                          outputBuffer,
        unsigned long                    framesPerBuffer,
        const PaStreamCallbackTimeInfo * timeInfo,
        PaStreamCallbackFlags            statusFlags)
    {
        for (unsigned long i = 0; i < framesPerBuffer; ++i)
        {
            float input = inputBuffer[i * numInputChannels]; // channel 0

            outputBuffer[i * numOutputChannels + 0] = input; // left
            outputBuffer[i * numOutputChannels + 1] = input; // right
        }

        return paContinue;
    }

    void paStreamFinishedMethod()
    {
        cout << "[Audio] Stream Finished." << endl;
    }

    static int paCallback(
        const void *                     inputBuffer,
        void *                           outputBuffer,
        unsigned long                    framesPerBuffer,
        const PaStreamCallbackTimeInfo * timeInfo,
        PaStreamCallbackFlags            statusFlags,
        void *                           userData)
    {
        return ((Audio *) userData)->paCallbackMethod(
            (float *) inputBuffer,
            (float *) outputBuffer,
            framesPerBuffer,
            timeInfo,
            statusFlags);
    }

    static void paStreamFinished(void * userData)
    {
        return ((Audio *) userData)->paStreamFinishedMethod();
    }

    PaError initError;
    PaStream * stream;

    int numInputChannels;
    int numOutputChannels;
};

#endif // __AUDIO_H__
