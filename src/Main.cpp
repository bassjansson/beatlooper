#include <iostream>

#include "Audio.h"

#include <portmidi.h>
#include <sndfile.h>
#include <fftw3.h>

using namespace std;

int main(int argc, char * argv[])
{
    Audio audio;

    if (!audio.open()) return 1;
    if (!audio.start()) return 1;

    while (true);

    if (!audio.stop()) return 1;
    if (!audio.close()) return 1;

    return 0;
}
