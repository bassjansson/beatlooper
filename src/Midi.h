#ifndef __MIDI_H__
#define __MIDI_H__

#include <iostream>
#include <cmath>
#include <portmidi.h>
#include <porttime.h>

#define MIDI_TIMER_INTERVAL    1 // ms
#define MIDI_INPUT_BUFFER_SIZE 100

using namespace std;

class Midi
{
public:
    Midi()
    {
        pmInitError = Pm_Initialize();
        inputStream = NULL;

        if (pmInitError != pmNoError)
            cout << "[Midi] Failed to initialize PortMidi." << endl;
    }

    ~Midi()
    {
        if (pmInitError == pmNoError)
            Pm_Terminate();
    }

    bool open()
    {
        if (pmInitError != pmNoError)
        {
            cout << "[Midi] Cannot open midi inputStream if PortMidi failed to initialize." << endl;
            return false;
        }

        PmDeviceID numDevices = Pm_CountDevices();
        PmDeviceID deviceIndex;
        const PmDeviceInfo * deviceInfo;

        cout << endl << "[Midi] Available midi devices:" << endl;
        for (deviceIndex = 0; deviceIndex < numDevices; ++deviceIndex)
        {
            deviceInfo = Pm_GetDeviceInfo(deviceIndex);
            cout << deviceIndex << ": " << deviceInfo->name;
            cout << (deviceInfo->input ? " (input)" : " (output)") << endl;
        }

        cout << "[Midi] Select midi input device by index: ";
        cin >> deviceIndex;

        deviceInfo = Pm_GetDeviceInfo(deviceIndex);
        if (deviceIndex == pmNoDevice || !deviceInfo)
        {
            cout << "[Midi] Failed to get selected midi device info." << endl;
            return false;
        }

        cout << "[Midi] Selected midi input device: " << deviceInfo->name << endl << endl;

        PtError ptError = Pt_Start(
            MIDI_TIMER_INTERVAL,
            &Midi::ptCallback,
            this);

        if (ptError != ptNoError)
        {
            cout << "[Midi] Failed to start midi timer callback." << endl;
            return false;
        }

        PmError pmError = Pm_OpenInput(
            &inputStream,
            deviceIndex,
            NULL,
            MIDI_INPUT_BUFFER_SIZE,
            NULL,
            NULL);

        if (pmError != pmNoError)
        {
            cout << "[Midi] Failed to open midi input stream." << endl;
            Pt_Stop();
            return false;
        }

        return true;
    } // open

    void close()
    {
        Pm_Close(inputStream);
        Pt_Stop();
    }

private:
    void ptCallbackMethod(PtTimestamp timeStamp)
    {
        // TODO
    }

    static void ptCallback(PtTimestamp timeStamp, void * userData)
    {
        ((Midi *) userData)->ptCallbackMethod(timeStamp);
    }

    PmError pmInitError;
    PortMidiStream * inputStream;
};

#endif // __MIDI_H__
