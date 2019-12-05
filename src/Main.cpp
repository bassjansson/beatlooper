#include <iostream>

#include "Audio.h"
#include "Midi.h"

#include <sndfile.h>
#include <fftw3.h>

using namespace std;

int main(int argc, const char * argv[])
{
    int audioDeviceIndex = -1;
    int midiDeviceIndex  = -1;

    if (argc > 1) audioDeviceIndex = atoi(argv[1]);
    if (argc > 2) midiDeviceIndex = atoi(argv[2]);

    Audio audio;
    Midi midi;

    if (!audio.open(audioDeviceIndex))
        return 1;

    if (!midi.open(midiDeviceIndex))
    {
        audio.close();
        return 1;
    }

    while (true)
    { }

    midi.close();
    audio.close();

    return 0;
}
