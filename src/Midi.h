#ifndef __MIDI_H__
#define __MIDI_H__

#include <iostream>
#include <cmath>
#include <portmidi.h>
#include <porttime.h>

#define MIDI_TIMER_INTERVAL    1   // ms
#define MIDI_INPUT_BUFFER_SIZE 100 // messages

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

        cout << "[Midi] Available midi devices:" << endl;
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
        while (Pm_Poll(inputStream))
        {
            if (Pm_Read(inputStream, &midiEvent, 1) > 0)
            {
                // TODO: Process midi events here
                cout << Pm_MessageStatus(midiEvent.message) << ", ";
                cout << Pm_MessageData1(midiEvent.message) << ", ";
                cout << Pm_MessageData2(midiEvent.message) << endl;
            }
        }
    }

    static void ptCallback(PtTimestamp timeStamp, void * userData)
    {
        ((Midi *) userData)->ptCallbackMethod(timeStamp);
    }

    PmError pmInitError;
    PmEvent midiEvent;
    PmStream * inputStream;
};

#endif // __MIDI_H__
