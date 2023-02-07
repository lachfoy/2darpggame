#include "TextureManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

TextureManager::TextureManager()
{
    // generate a default, single pixel, white texture
    unsigned char* data = new unsigned char[3] { 0xff, 0xff, 0xff };
    
    Texture texture;

    texture.w = 1;
    texture.h = 1;

    // create texture
    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    std::cout << "TextureManager:: default texture: " << texture.id << "\n";

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.w, texture.h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    
    // set texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);

    // free data
    delete[] data;

    mTextures.insert({ "default", texture });
}

TextureManager::~TextureManager()
{
    std::cout << "TextureManager:: Destructor called!\n";

    // clear textures
    for (auto& it: mTextures) {
        std::cout << "Deleting texture: " << it.first << std::endl;
        glDeleteTextures(1, &it.second.id);
    }
    
    mTextures.clear();
}

Texture TextureManager::GetTexture(const char* name)
{
    if (!(mTextures.find(name) != mTextures.end())) {
        std::cout << "TextureManager:: loading texture " << name << "\n";
        LoadTexture(name);
    }
    
    return mTextures[name];
}

void TextureManager::LoadTexture(const char* path)
{
    // load data
    int w, h, nChannels;
    unsigned char* data = stbi_load(path, &w, &h, &nChannels, 0);
    
    Texture texture;

    texture.w = w;
    texture.h = h;

    // create texture
    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    // technically every texture should be 32bpp but if not...
    if (nChannels == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.w, texture.h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.w, texture.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); // alpha channel
    }
    
    // set texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);

    // free data
    stbi_image_free(data);

    std::cout << "Created texture from " << path << " with id " << texture.id << std::endl;

    mTextures.insert({ path, texture });
}
