#ifndef MAP_H_
#define MAP_H_

#include "Renderer.h"
#include "TextureManager.h"
#include <glad/glad.h>

class Map
{
public:
    Map(int mapWidth, int mapHeight, uint8_t* tileData, bool* solidData, GLuint tilesetTexture, int tileSize, Renderer& renderer);
    ~Map();

    // map generation
    static Map* RandomMap(int width, int height, GLuint tilesetTexture, int tileSize, Renderer& renderer);
    
    void DrawMap(Renderer& renderer);

private:
    int mMapWidth;
    int mMapHeight;
    uint8_t* mTileData;
    bool* mSolidData;
    GLuint mTilesetTexture;
    int mTileSize;

};

#endif