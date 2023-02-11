#include "Renderer.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "TextureManager.h"

Renderer::~Renderer()
{
    std::cout << "Cleaning up renderer...\n";

    glDeleteVertexArrays(1, &mMapVao);
    glDeleteBuffers(1, &mMapEbo);
    glDeleteBuffers(1, &mMapVbo);

    glDeleteVertexArrays(1, &mQuadVao);
    glDeleteBuffers(1, &mQuadVbo);

    // delete shaders
    glDeleteProgram(mTextShader);
    glDeleteProgram(mShader);
}

void Renderer::InitShaders()
{
    // shader stuff
    // vertex shader
    const char *vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec4 vertex;\n"
        "out vec2 TexCoords;\n"
        "uniform mat4 model;\n"
        "uniform mat4 projection;\n"
        "void main()\n"
        "{\n"
        "   TexCoords = vertex.zw;\n"
        "   gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);\n"
        "}\0";

    // fragment shader
    const char *fragmentShaderSource = "#version 330 core\n"
        "in vec2 TexCoords;\n"
        "out vec4 FragColor;\n"
        "uniform sampler2D image;\n"
        "uniform vec4 color;\n"
        "void main()\n"
        "{\n"
        "   vec4 texColor = color * texture(image, TexCoords);\n"
        "   FragColor = texColor;\n"
        "}\0";
    
    CompileShaderProgram(&mShader, vertexShaderSource, fragmentShaderSource);

    // temp hardcoded -- set projection matrix
    // if the view gets resized this will need to change
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(mRenderWidth), static_cast<float>(mRenderHeight), 0.0f);
    
    glUseProgram(mShader);
    glUniformMatrix4fv(glGetUniformLocation(mShader, "projection"), 1, false, glm::value_ptr(projection));
}

void Renderer::SetCameraPosition(float x, float y)
{
    glm::mat4 projection = glm::ortho(
        x - mRenderWidth / 2.f,
        x + mRenderWidth / 2.f,
        y + mRenderHeight / 2.f,
        y - mRenderHeight / 2.f
    );

    glUseProgram(mShader);
    glUniformMatrix4fv(glGetUniformLocation(mShader, "projection"), 1, false, glm::value_ptr(projection));
}

void Renderer::InitQuadVertexData()
{
    // gen vbo
    glGenBuffers(1, &mQuadVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mQuadVbo);

    // ccw winding order
    float vertices[] = { 
        // xy       // uv
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // gen vao
    glGenVertexArrays(1, &mQuadVao);
    glBindVertexArray(mQuadVao);
    glBindBuffer(GL_ARRAY_BUFFER, mQuadVbo);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
}

void Renderer::InitPartialVbo()
{
    glGenVertexArrays(1, &mPartialVao);
    glGenBuffers(1, &mPartialVbo);
    glBindVertexArray(mPartialVao);
    glBindBuffer(GL_ARRAY_BUFFER, mPartialVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::InitMapData(int mapWidth, int mapHeight, uint8_t* tileData, int tileSize)
{    
    // https://github.com/davudk/OpenGL-TileMap-Demos
    mTileMapLength = mapWidth * mapHeight;
    mTileSize = tileSize;

    // generate vbo
    glGenBuffers(1, &mMapVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mMapVbo);

    int vertexDataLength = mapWidth * mapHeight * 4 * 4; // for each tile, 4 vertices, each vertex has 4 attributes
    float* vertexData = new float[vertexDataLength];
    int i = 0;
    for (int x = 0; x < mapWidth; x++) {
        for (int y = 0; y < mapHeight; y++) {
            uint8_t tile = tileData[x + mapWidth * y];
            float tx0 = (tile & 15) * mTileTexSize;
            float ty0 = (tile >> 4) * mTileTexSize;

            // vertex 0 (top left)
            vertexData[i + 0] = x; // position x
            vertexData[i + 1] = y; // position y
            vertexData[i + 2] = tx0; // texcoord x
            vertexData[i + 3] = ty0; // texcoord y
            i += 4;

            // vertex 1 (top right)
            vertexData[i + 0] = x + 1; // position x
            vertexData[i + 1] = y; // position y
            vertexData[i + 2] = tx0 + mTileTexSize; // texcoord x
            vertexData[i + 3] = ty0; // texcoord y
            i += 4;

            // vertex 2 (bottom left)
            vertexData[i + 0] = x; // position x
            vertexData[i + 1] = y + 1; // position y
            vertexData[i + 2] = tx0; // texcoord x
            vertexData[i + 3] = ty0 + mTileTexSize; // texcoord y
            i += 4;

            // vertex 3 (bottom right)
            vertexData[i + 0] = x + 1; // position x
            vertexData[i + 1] = y + 1; // position y
            vertexData[i + 2] = tx0 + mTileTexSize; // texcoord x
            vertexData[i + 3] = ty0 + mTileTexSize; // texcoord y
            i += 4;
        }
    }

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexDataLength, vertexData, GL_STATIC_DRAW);
    delete[] vertexData; // am i allowed to do this?

    // generate ebo
    glGenBuffers(1, &mMapEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mMapEbo);

    const int indicesLength = mapWidth * mapHeight * 6; // for each tile there are 6 vertices
    uint32_t* indices = new uint32_t[indicesLength];
    uint32_t index = 0, j = 0;
    for (int x = 0; x < mapWidth; x++) {
        for (int y = 0; y < mapHeight; y++) {
            indices[index + 0] = j;
            indices[index + 1] = j + 1;
            indices[index + 2] = j + 2;
            indices[index + 3] = j + 1;
            indices[index + 4] = j + 2;
            indices[index + 5] = j + 3;
            index += 6;
            j += 4;   
        }
    }

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indicesLength, indices, GL_STATIC_DRAW);
    delete[] indices; // am i allowed to do this?

    // generate vao
    glGenVertexArrays(1, &mMapVao);
    glBindVertexArray(mMapVao);

    // x, y, u, v
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
}

void Renderer::DrawSprite(float x, float y, Texture& texture)
{
    glUseProgram(mShader);

    // bind
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glBindVertexArray(mQuadVao);

    // set up model matrix
    glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model, glm::vec3(x, y, 0.f));
    model = glm::translate(model, glm::vec3(-0.5f * texture.w, -0.5f * texture.h, 0.0f));
    model = glm::scale(model, glm::vec3(texture.w, texture.h, 1.f));

    // set uniforms
    glUniformMatrix4fv(glGetUniformLocation(mShader, "model"), 1, false, glm::value_ptr(model));
    glUniform4f(glGetUniformLocation(mShader, "color"), 1.f, 1.f, 1.f, 1.f);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

// should be in pixels actually
void Renderer::DrawPartialSprite(float x, float y, int offsetX, int offsetY, int w, int h, Texture& texture)
{
    glUseProgram(mShader);

    // bind
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glBindVertexArray(mPartialVao);

    // calculate texture coordinates
    float tx0 = offsetX * (1.0f / texture.w);
    float ty0 = offsetY * (1.0f / texture.h);
    float tx1 = (offsetX + w) * (1.0f / texture.w);
    float ty1 = (offsetY + h) * (1.0f / texture.h);
    
    // ccw winding order
    float vertices[] = { 
        // xy       // uv
        0.0f, 1.0f, tx0, ty1,
        1.0f, 0.0f, tx1, ty0,
        0.0f, 0.0f, tx0, ty0,

        0.0f, 1.0f, tx0, ty1,
        1.0f, 1.0f, tx1, ty1,
        1.0f, 0.0f, tx1, ty0
    };

    glBindBuffer(GL_ARRAY_BUFFER, mPartialVbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // set up model matrix
    glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model, glm::vec3(x, y, 0.f));
    model = glm::translate(model, glm::vec3(-0.5f * w, -0.5f * h, 0.0f));
    model = glm::scale(model, glm::vec3(w, h, 1.f));

    // set uniforms
    glUniformMatrix4fv(glGetUniformLocation(mShader, "model"), 1, false, glm::value_ptr(model));
    glUniform4f(glGetUniformLocation(mShader, "color"), 1.f, 1.f, 1.f, 1.f);
    
    // draw
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Renderer::DrawString(std::string text, float x, float y, Font& font)
{
    glUseProgram(mShader);

    // bind
    glBindTexture(GL_TEXTURE_2D, font.textureHandle);
    glBindVertexArray(mPartialVao);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        CharInfo charInfo = font.chars[*c];

        // ccw winding order
        float vertices[24] = { 
            // // xy       // uv
            // 0.0f, 1.0f, tx0, ty1,
            // 1.0f, 0.0f, tx1, ty0,
            // 0.0f, 0.0f, tx0, ty0,

            // 0.0f, 1.0f, tx0, ty1,
            // 1.0f, 1.0f, tx1, ty1,
            // 1.0f, 0.0f, tx1, ty0
        };

        glBindBuffer(GL_ARRAY_BUFFER, mPartialVbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // draw
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    
    // draw
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Renderer::DrawMap(Texture& texture)
{
    glUseProgram(mShader);

    //bind
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glBindVertexArray(mMapVao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mMapEbo);

    // set up model matrix
    glm::mat4 model = glm::mat4(1.f);
    model = glm::scale(model, glm::vec3(mTileSize, mTileSize, 1.f));

    // set uniforms
    glUniformMatrix4fv(glGetUniformLocation(mShader, "model"), 1, false, glm::value_ptr(model));
    glUniform4f(glGetUniformLocation(mShader, "color"), 1.f, 1.f, 1.f, 1.f);

    // draw
    glDrawElements(GL_TRIANGLES, mTileMapLength * 6, GL_UNSIGNED_INT, 0);
}

void Renderer::DrawRectangle(float x, float y, int w, int h, float r, float g, float b, float a)
{
    glUseProgram(mShader);

    // bind
    glBindTexture(GL_TEXTURE_2D, TextureManager::Instance().GetTexture("default").id);
    glBindVertexArray(mQuadVao);

    // set up model matrix
    glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model, glm::vec3(x, y, 0.f));
    model = glm::scale(model, glm::vec3(w, h, 1.f)); 

    // set uniforms
    glUniformMatrix4fv(glGetUniformLocation(mShader, "model"), 1, false, glm::value_ptr(model));
    glUniform4f(glGetUniformLocation(mShader, "color"), r, g, b, a);

    // draw
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Renderer::ResizeRenderer(int newWidth, int newHeight)
{
    // weh
    glViewport(0, 0, newWidth, 600);

    // temp hardcoded
    glUseProgram(mShader);
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(newWidth), static_cast<float>(newHeight), 0.0f);
    glUniformMatrix4fv(glGetUniformLocation(mShader, "projection"), 1, false, glm::value_ptr(projection));
}

void Renderer::CompileShaderProgram(GLuint* shader, const char* vertexSrc, const char* fragmentSrc)
{
    GLuint sVertex, sFragment;
    
    // vertex shader
    sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &vertexSrc, 0);
    glCompileShader(sVertex);
    CheckShaderCompilationError(sVertex);

    // fragment shader
    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &fragmentSrc, 0);
    glCompileShader(sFragment);
    CheckShaderCompilationError(sFragment);

    // link to shader program
    *shader = glCreateProgram();
    glAttachShader(*shader, sVertex);
    glAttachShader(*shader, sFragment);
    glLinkProgram(*shader);
    
    // error check linking
    int success;
    char infoLog[512];
    glGetProgramiv(*shader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(*shader, 512, NULL, infoLog);
        printf("Failed to link shader:\n%s\n", infoLog);
    }

    // the vertex and fragment shader are no longer used after they have been linked into the shader program
    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
}

void Renderer::CheckShaderCompilationError(GLuint id)
{
    int success;
    char infoLog[512];
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(id, 512, NULL, infoLog);
        printf("Failed to compile shader:\n%s\n", infoLog);
    }
}
