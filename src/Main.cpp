#include <iostream>

#include "Audio.h"
#include "Midi.h"

#include <sndfile.h>
#include <fftw3.h>

using namespace std;

int main(int argc, const char * argv[])
{
    Audio audio;
    Midi midi;

    if (!audio.open())
        return 1;

    if (!midi.open())
    {
        audio.close();
        return 1;
    }

    while (true);

    midi.close();
    audio.close();

    return 0;
}
