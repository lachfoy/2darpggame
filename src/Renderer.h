#ifndef RENDERER_H_
#define RENDERER_H_

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <map>
#include <string>
#include "Texture.h"
#include "Font.h"

// singleton?
class Renderer
{
public:
    Renderer() = default;
    ~Renderer();

    void InitShaders();
    void InitQuadVertexData();
    void InitPartialVbo();
    void InitTextVbo();
    void InitMapData(int mapWidth, int mapHeight, uint8_t* tileData, int tileSize);

    void DrawSprite(float x, float y, Texture& texture);
    void DrawPartialSprite(float x, float y, int offsetX, int offsetY, int w, int h, Texture& texture);
    void DrawString(std::string string, float x, float y, Font& font);
    void DrawMap(Texture& texture);

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

    GLuint mPartialVbo;
    GLuint mPartialVao;

    GLuint mTextVbo;
    GLuint mTextVao;

    // map data
    GLuint mMapVbo;
    GLuint mMapEbo;
    GLuint mMapVao;

    int mTileMapLength;
    int mTileSize;
    const float mTileTexSize = 1.0f / 16.0f; // always 16 tiles per row in tileset

    int mScreenWidth = 800;
    int mScreenHeight = 600;
    float mCameraScale = 0.25f;

    void CompileShaderProgram(GLuint* shader, const char* vertexSrc, const char* fragmentSrc);
    void CheckShaderCompilationError(GLuint id);
    
};

#endif