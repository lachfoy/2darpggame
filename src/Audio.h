#ifndef AUDIO_H_
#define AUDIO_H_

#include "sndfile.h"

#include <AL/al.h>
#include <AL/alc.h>

// singleton?
class Audio
{
public:
    Audio() = default;
    ~Audio();

    void Init();

    void PlaySoundEffect(const char* sound);

private:
    ALCdevice *device;
    ALCcontext *ctx;
    
};

#endif