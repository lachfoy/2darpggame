#ifndef TEXTURE_MANAGER_H_
#define TEXTURE_MANAGER_H_

#include <unordered_map>
#include <string>
#include <glad/glad.h>
#include "Texture.h"

class TextureManager
{
public:
    static TextureManager& Instance()
    {
        static TextureManager instance;
        return instance;
    }

    ~TextureManager();

    Texture GetTexture(const char* name);
    //void LoadTexture(const char* path);
    
private:
    TextureManager();

    void LoadTexture(const char* path);

    std::unordered_map<std::string, Texture> mTextures;

};

#endif