#ifndef GAME_H_
#define GAME_H_

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <SDL2/SDL_opengl.h>
#include <AL/al.h>
#include <AL/alc.h>

#ifdef __APPLE__
extern "C"
{
    #include <lua/lua.h>
    #include <lua/lauxlib.h>
    #include <lua/lualib.h>
}
#else
extern "C"
{
    #include <lua.h>
    #include <lauxlib.h>
    #include <lualib.h>
}
#endif

#include "Input.h"
#include "Renderer.h"
#include "EnemyManager.h"
#include "Map.h"
#include "Player.h"
#include "TextureManager.h"
#include "Vector2.h"

#include <random>

class Game
{
public:
    Game() {};

    bool Init();
    void Run();

private:
    void Create();
    void HandleInput();
    void Update(float deltaTime);
    void Draw();
    void Destroy();
    
    SDL_Window* mWindow;
    SDL_GLContext mContext;
    
    ALCdevice *mAlDevice;
    ALCcontext *mAlContext;

    bool mRunning = true;

    lua_State *L;

    Map* mMap;
    Player* mPlayer;

    Renderer mRenderer;
    Input mInput;

    Font mFont;

};

#endif