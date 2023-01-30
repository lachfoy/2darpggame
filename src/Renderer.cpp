#include "Renderer.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

Renderer::~Renderer()
{
    std::cout << "Cleaning up renderer...\n";

    mCharacters.clear();

    glDeleteVertexArrays(1, &mTextVao);
    glDeleteBuffers(1, &mTextVbo);

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

    // text shader
    // vertex shader
    const char *vertexTextShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec4 vertex;\n"
        "out vec2 TexCoords;\n"
        "uniform mat4 projection;\n"
        "void main()\n"
        "{\n"
        "   TexCoords = vertex.zw;\n"
        "   gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
        "}\0";

    // fragment shader
    const char *fragmentTextShaderSource = "#version 330 core\n"
        "in vec2 TexCoords;\n"
        "out vec4 FragColor;\n"
        "uniform sampler2D image;\n"
        "uniform vec4 color;\n"
        "void main()\n"
        "{\n"
        "   //vec4 sampled = vec4(1.0, 1.0, 1.0, texture(image, TexCoords).r);\n"
        "   //FragColor = color * sampled;\n"
        "   FragColor = vec4(1.0, 1.0, 1.0, texture(image, TexCoords).r);\n"
        "}\0";
    
    CompileShaderProgram(&mShader, vertexShaderSource, fragmentShaderSource);
    CompileShaderProgram(&mTextShader, vertexTextShaderSource, fragmentTextShaderSource);

    // temp hardcoded -- set projection matrix
    // if the view gets resized this will need to change
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(mRenderWidth), static_cast<float>(mRenderHeight), 0.0f);
    
    glUseProgram(mShader);
    glUniformMatrix4fv(glGetUniformLocation(mShader, "projection"), 1, false, glm::value_ptr(projection));

    glUseProgram(mTextShader);
    glUniformMatrix4fv(glGetUniformLocation(mTextShader, "projection"), 1, false, glm::value_ptr(projection));
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

    glUseProgram(mTextShader);
    glUniformMatrix4fv(glGetUniformLocation(mTextShader, "projection"), 1, false, glm::value_ptr(projection));
}

void Renderer::InitQuadVertexData()
{
    // gen vbo
    glGenBuffers(1, &mQuadVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mQuadVbo);

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
            float tx0 = (tile & 15) * mTileTexSize + mTileTexPadding;
            float ty0 = (tile >> 4) * mTileTexSize + mTileTexPadding;
            float tySize = mTileTexSize - mTileTexPadding * 2;

            // vertex 0 (top left)
            vertexData[i + 0] = x; // position x
            vertexData[i + 1] = y; // position y
            vertexData[i + 2] = tx0; // texcoord x
            vertexData[i + 3] = ty0; // texcoord y
            i += 4;

            // vertex 1 (top right)
            vertexData[i + 0] = x + 1; // position x
            vertexData[i + 1] = y; // position y
            vertexData[i + 2] = tx0 + tySize; // texcoord x
            vertexData[i + 3] = ty0; // texcoord y
            i += 4;

            // vertex 2 (bottom left)
            vertexData[i + 0] = x; // position x
            vertexData[i + 1] = y + 1; // position y
            vertexData[i + 2] = tx0; // texcoord x
            vertexData[i + 3] = ty0 + tySize; // texcoord y
            i += 4;

            // vertex 3 (bottom right)
            vertexData[i + 0] = x + 1; // position x
            vertexData[i + 1] = y + 1; // position y
            vertexData[i + 2] = tx0 + tySize; // texcoord x
            vertexData[i + 3] = ty0 + tySize; // texcoord y
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

void Renderer::InitTextData()
{
    glGenVertexArrays(1, &mTextVao);
    glGenBuffers(1, &mTextVbo);
    glBindVertexArray(mTextVao);
    glBindBuffer(GL_ARRAY_BUFFER, mTextVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::LoadFont(const char* font, unsigned int fontSize)
{
    mCharacters.clear();

    FT_Library library;
    FT_Face face;
    FT_Error error;

    error = FT_Init_FreeType(&library);
    if (error) {
        std::cout << "FT_Error:: could not initialize FT library.\n";
    }

    error = FT_New_Face(library, font, 0, &face);
    if (error) {
        std::cout << "FT_Error:: font file could not open be opened or read.\n";
    }

    // set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, fontSize);
    // disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (GLubyte c = 0; c < 128; c++) // lol see what I did there 
    {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "FT_Error:: Failed to load Glyph\n";
            continue;
        }

        // generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
       
        // now store character for later use
        FTCharacter character = {
            texture,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            face->glyph->bitmap_left,
            face->glyph->bitmap_top,
            face->glyph->advance.x
        };

        mCharacters.insert(std::make_pair(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // clean up FT library
    FT_Done_Face(face);
    FT_Done_FreeType(library);
}

void Renderer::DrawSprite(float x, float y, GLuint texture)
{
    glUseProgram(mShader);

    // bind
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(mQuadVao);

    // set up model matrix
    glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model, glm::vec3(x, y, 0.f));
    model = glm::translate(model, glm::vec3(-0.5f * mTileSize, -0.5f * mTileSize, 0.0f));
    model = glm::scale(model, glm::vec3(mTileSize, mTileSize, 1.f));

    // set uniforms
    glUniformMatrix4fv(glGetUniformLocation(mShader, "model"), 1, false, glm::value_ptr(model));
    glUniform4f(glGetUniformLocation(mShader, "color"), 1.f, 1.f, 1.f, 1.f);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Renderer::DrawMap(GLuint texture)
{
    glUseProgram(mShader);

    //bind
    glBindTexture(GL_TEXTURE_2D, texture);
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

void Renderer::DrawText(float x, float y, std::string text)
{
    glUseProgram(mTextShader);

    // bind
    glBindVertexArray(mTextVao);

    // iterate over character
    for (auto& c : text) {
        FTCharacter ch = mCharacters[c];

        float xPos = x + ch.bearingX;
        float yPos = y + (mCharacters['H'].bearingY - ch.bearingY);

        float vertices[6][4] = {
            { xPos,        yPos + ch.h, 0.0f, 1.0f },            
            { xPos + ch.w, yPos,        1.0f, 0.0f },
            { xPos, yPos,               0.0f, 0.0f },

            { xPos,        yPos + ch.h, 0.0f, 1.0f },
            { xPos + ch.w, yPos + ch.h,        1.0f, 1.0f },
            { xPos + ch.w, yPos, 1.0f, 0.0f }  
        };

        glBindTexture(GL_TEXTURE_2D, ch.id);

        glBindBuffer(GL_ARRAY_BUFFER, mTextVbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.advance >> 6); // bitshift by 6 to get value in pixels (2^6 = 64)
    }
}

void Renderer::DrawRectangle(float x, float y, int w, int h, float r, float g, float b, float a)
{
    glUseProgram(mShader);

    // bind
    glBindTexture(GL_TEXTURE_2D, 1);
    glBindVertexArray(mQuadVao);

    // set up model matrix
    glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model, glm::vec3(x * mTileSize, y * mTileSize, 0.f));
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
