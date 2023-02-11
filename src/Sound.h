#ifndef SOUND_H_
#define SOUND_H_

#include <AL/al.h>
#include <AL/alc.h>

struct Sound
{
    ALuint buffer;
    ALuint source;
};

#endif