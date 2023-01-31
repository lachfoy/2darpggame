#ifndef RENDERER_H_
#define RENDERER_H_

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <iostream>
#include <map>
#include <string>
#include "Texture.h"

struct FTCharacter
{
    GLuint id;
    unsigned int w;
    unsigned int h;
    int bearingX;
    int bearingY;
    long int advance;
};

// singleton?
class Renderer
{
public:
    Renderer() = default;
    ~Renderer();

    void InitShaders();
    void InitQuadVertexData();
    void InitMapData(int mapWidth, int mapHeight, uint8_t* tileData, int tileSize);

    void InitTextData();
    void LoadFont(const char* font, unsigned int fontSize);

    void DrawSprite(float x, float y, Texture& texture);
    void DrawMap(Texture& texture);
    void DrawText(float x, float y, std::string text);

    void DrawRectangle(float x, float y, int w, int h, float r, float g, float b, float a);

    void SetCameraPosition(float x, float y);
    void ResizeRenderer(int newWidth, int newHeight);

private:
    GLuint mShader;
    GLuint mTextShader;
    
    // quad data
    GLuint mQuadVbo;
    GLuint mQuadVao;
    int mSpriteSize;

    // map data
    GLuint mMapVbo;
    GLuint mMapEbo;
    GLuint mMapVao;

    // text data
    GLuint mTextVbo;
    GLuint mTextVao;
    std::map<char, FTCharacter> mCharacters; // precalculated character table

    int mTileMapLength;
    int mTileSize;
    const float mTileTexSize = 1.0f / 16.0f; // always 16 tiles per row in tileset

    int mRenderWidth = 320 / 2;
    int mRenderHeight = 240 / 2;

    void CompileShaderProgram(GLuint* shader, const char* vertexSrc, const char* fragmentSrc);
    void CheckShaderCompilationError(GLuint id);
    
};

#endif