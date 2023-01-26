#include "Map.h"

#include <iostream>
#include <fstream> // ifstream, ios::binary, ios::in
#include "Random.h"

Map::Map(int mapWidth, int mapHeight, uint8_t* tileData, bool* solidData, GLuint tilesetTexture, int tileSize, Renderer& renderer)
{
    mMapWidth = mapWidth;
    mMapHeight = mapHeight;
    mTileData = tileData;
    mSolidData = solidData;
    mTilesetTexture = tilesetTexture;
    mTileSize = tileSize;

    renderer.InitMapData(mMapWidth, mMapHeight, mTileData, mTileSize);
}

Map::~Map()
{
    std::cout << "Map:: Destructor called!\n";

    // delete the map data!
    delete[] mSolidData;
    delete[] mTileData;
}

void Map::DrawMap(Renderer& renderer)
{
    renderer.DrawMap(mTilesetTexture);
}

Map* Map::RandomMap(int width, int height, GLuint tilesetTexture, int tileSize, Renderer& renderer)
{
    std::cout << "Map:: Generating random map\n";

    uint8_t* tileData = new uint8_t[width * height];
    bool* solidData = new bool[width * height];

    const int numUniqueTiles = 3;

    for (int i = 0; i < width * height; i++) {
        tileData[i] = gRandom.Range(0, numUniqueTiles - 1);
        solidData[i] = false;
    }

    return new Map(width, height, tileData, solidData, tilesetTexture, tileSize, renderer);
}