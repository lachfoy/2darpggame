#ifndef SHADER_H_
#define SHADER_H_

#include <glad/glad.h>

class Shader
{
public:
    Shader() = default;
    ~Shader();

    void Load(const char* vertexSrc, const char* fragmentSrc);

    GLuint programHandle;

};

#endif