#ifndef SOUND_MANAGER_H_
#define SOUND_MANAGER_H_

#include <unordered_map>
#include <string>
#include <glad/glad.h>
#include "Sound.h"

class SoundManager
{
public:
    static SoundManager& Instance()
    {
        static SoundManager instance;
        return instance;
    }

    ~SoundManager();

    Sound GetSound(const char* name);
    //void LoadSound(const char* path);
    
private:
    SoundManager();

    void LoadSound(const char* path);

    std::unordered_map<std::string, Sound> mSounds;

};

#endif