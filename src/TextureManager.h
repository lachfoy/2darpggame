#ifndef TEXTURE_MANAGER_H_
#define TEXTURE_MANAGER_H_

#include <unordered_map>
#include <string>
#include <glad/glad.h>

class TextureManager
{
public:
    static TextureManager& Instance()
    {
        static TextureManager instance;
        return instance;
    }

    ~TextureManager();

    GLuint GetTexture(const char* name);
    void LoadTexture(const char* path);


private:
    TextureManager();

    std::unordered_map<std::string, GLuint> mTextures;

};

#endif