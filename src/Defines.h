#ifndef __DEFINES_H__
#define __DEFINES_H__

#define AUDIO_SAMPLE_RATE   48000 // Hz
#define AUDIO_BUFFER_SIZE   64    // samples

#define MIDI_TIMER_INTERVAL 1   // ms
#define MIDI_IN_BUFFER_SIZE 100 // messages

#define TRACK_BUFFER_LENGTH 60 // seconds
#define TRACK_NUM_CHANNELS  2

typedef tick_t unsigned long;

#endif // __DEFINES_H__
