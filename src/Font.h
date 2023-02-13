#ifndef FONT_H_
#define FONT_H_

#include <unordered_map>
#include <glad/glad.h>

struct CharInfo
{
    unsigned short x;
    unsigned short y;
    unsigned short width;
    unsigned short height;
    short xoffset;
    short yoffset;
    short advance;
};

class Font
{
public:
    Font() = default;
    ~Font();

    void Load(const char* path);

    GLuint textureHandle;
    int w;
    int h;

    unsigned short lineHeight;
    unsigned short base;
    std::unordered_map<unsigned int, CharInfo> chars;

};

#endif