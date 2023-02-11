#include "Audio.h"

#include <iostream>

Audio::Audio()
{
    
}

Audio::~Audio()
{
    // close openAL
    alcMakeContextCurrent(NULL);
    alcDestroyContext(ctx);
    alcCloseDevice(device);
}

void Audio::Init()
{
    // init Open AL
    const ALCchar *name;

    device = NULL;

    device = alcOpenDevice(NULL); 
    if (!device) std::cout << "Could not open AL device\n";

    ctx = alcCreateContext(device, NULL);

    name = alcGetString(device, ALC_DEVICE_SPECIFIER);

    std::cout << "Audio:: Opened " << name << "\n";
}