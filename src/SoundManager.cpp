#include "SoundManager.h"

#include "sndfile.h"

#include <iostream>

SoundManager::SoundManager()
{
}

SoundManager::~SoundManager()
{
    std::cout << "SoundManager:: Destructor called!\n";

    // clear Sounds
    for (auto& it: mSounds) {
        std::cout << "Deleting Sound: " << it.first << std::endl;
        
        // delete sound
        alDeleteSources(1, &it.second.source);
        alDeleteBuffers(1, &it.second.buffer);
    }
    
    mSounds.clear();
}

Sound SoundManager::GetSound(const char* name)
{
    if (!(mSounds.find(name) != mSounds.end())) {
        std::cout << "SoundManager:: loading sound " << name << "\n";
        LoadSound(name);
    }
    
    return mSounds[name];
}

void SoundManager::LoadSound(const char* path)
{
    // load sound
    SNDFILE *sndfile;
    SF_INFO sfinfo;
    sndfile = sf_open("sounds/swing.wav", SFM_READ, &sfinfo);

    if (!sndfile) {
        std::cout << "Could not load sound: " << sf_strerror(sndfile) << "\n";
    }

    std::cout << "channels: " << sfinfo.channels << "\n"; // assume 2 , i.e. AL_FORMAT_STEREO16
    
    short *membuf = new short[sfinfo.frames * sfinfo.channels];
    sf_count_t num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
    if (num_frames < 1) {
        delete[] membuf;
        sf_close(sndfile);
    }

    Sound sound;
    alGenBuffers(1, &sound.buffer);
    alBufferData(sound.buffer, AL_FORMAT_STEREO16, membuf, (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short), sfinfo.samplerate);

    delete[] membuf;
    sf_close(sndfile);

    // create a source to play the sound with
    alGenSources(1, &sound.source);
    alSourcei(sound.source, AL_BUFFER, sound.buffer);

    std::cout << "Created Sound from " << path << " with id " << sound.source << std::endl;

    mSounds.insert({ path, sound });
}
