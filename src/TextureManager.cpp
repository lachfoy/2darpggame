#include "TextureManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

TextureManager::TextureManager()
{
    // generate a default, single pixel, white texture
    GLuint defaultTexture;
    unsigned char* data = new unsigned char[3] { 0xff, 0xff, 0xff };
    
    // create texture
    glGenTextures(1, &defaultTexture);
    glBindTexture(GL_TEXTURE_2D, defaultTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    
    // set texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);

    // free data
    delete[] data;
}

TextureManager::~TextureManager()
{
    std::cout << "TextureManager:: Destructor called!\n";

    glDeleteTextures(1, 0); // delete default texture

    // clear textures
    for (auto& it: mTextures) {
        std::cout << "Deleting texture: " << it.first << std::endl;
        glDeleteTextures(1, &it.second);
    }
    
    mTextures.clear();
}

GLuint TextureManager::GetTexture(const char* name)
{
    if (!mTextures[name]) {
        std::cout << "TextureManager:: Warning: accessing texture that has not been loaded!\n";
        return 0;
    }
    
    return mTextures[name];
}

void TextureManager::LoadTexture(const char* path)
{
    // load data
    int w, h, nChannels;
    unsigned char* data = stbi_load(path, &w, &h, &nChannels, 0);
    
    GLuint texture;

    // create texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // technically every texture should be 32bpp but if not...
    if (nChannels == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); // alpha channel
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

    std::cout << "Created texture from " << path << " with id " << texture << std::endl;

    mTextures.insert({ path, texture });
}
