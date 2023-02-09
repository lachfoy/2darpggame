#include "Game.h"

#include <cstdio>
#include <iostream>
#include <cmath> // floor

#include <AL/al.h>
#include <AL/alc.h>

bool Game::Init()
{
    // initialize SDL video
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL video could not be initialized: %s\n", SDL_GetError());
        return false;
    }

    // create main window
    mWindow = SDL_CreateWindow(
        "test",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );
    if (!mWindow) {
        printf("SDL window could not be created: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    // initialize renderering context
    //Use OpenGL 3.3 core // should be ok for anything about a decade old?
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    mContext = SDL_GL_CreateContext(mWindow);
    if (!mContext) {
        printf("GL Context could not be created: %s\n", SDL_GetError());
        SDL_DestroyWindow(mWindow);
        SDL_Quit();
        return false;
    }

    // initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        printf("GLAD could not be loaded\n");
        SDL_GL_DeleteContext(mContext);
        SDL_DestroyWindow(mWindow);
        SDL_Quit();
        return false;
    }

    // enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // wireframe
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    // Open AL
    const ALCchar *name;
    ALCdevice *device;
    ALCcontext *ctx;

    device = NULL;

    device = alcOpenDevice(NULL);
    if (!device) std::cout << "Could not open AL device\n";

    ctx = alcCreateContext(device, NULL);

    name = alcGetString(device, ALC_DEVICE_SPECIFIER);

    std::cout << "Audio:: Opened " << name << "\n";

    alcMakeContextCurrent(NULL);
    alcDestroyContext(ctx);
    alcCloseDevice(device);

    return true;
}

void Game::Run()
{
    Create();

    Uint32 lastTime = 0;

    SDL_Event event;

    // main loop
    while (mRunning) {
        // handle input events
        while (SDL_PollEvent(&event) != 0) {
            switch(event.type) {
            case SDL_QUIT:
                mRunning = false;
                break;
            case SDL_KEYDOWN:
                mInput.SetKeyDown(event.key.keysym.scancode);
                break;
            case SDL_KEYUP:
                mInput.SetKeyUp(event.key.keysym.scancode);
                break;
            }
        }

        HandleInput();

        // update input
        mInput.UpdateInput();

        // calculate delta time
        Uint32 currentTime = SDL_GetTicks();
		float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        // update
        Update(deltaTime);

        // draw
        Draw();
    }

    Destroy();
}

void Game::Create()
{
    // init render stuff
    mRenderer.InitShaders();
    mRenderer.InitQuadVertexData();
    mRenderer.InitPartialVbo();
    mRenderer.InitTextData();

    // load required fonts
    mRenderer.LoadFont("fonts/arial.ttf", 16);

    // test map generator
    mMap = Map::RandomMap(64, 64, TextureManager::Instance().GetTexture("images/tileset.png"), 16, mRenderer);

    // spawn player
    mPlayer = new Player(Vector2(160.0f, 160.0f), TextureManager::Instance().GetTexture("images/character.png"));

    // spawn enemy
    EnemyManager::Instance().CreateEnemy(Vector2(240.0f, 240.0f), mPlayer, "scripts/enemy.lua");

    mPlayer->AddObserver(&EnemyManager::Instance()); // hmm....

    // open lua
    L = luaL_newstate();
    luaL_openlibs(L); // open the base libraries

    // lua_register(L, "CreateEnemy", wrap_CreateEnemy); // expose api function to lua

    if (luaL_dofile(L, "scripts/main.lua") != 0) {
        // abort, idk. todo: error checking, put the program back into a safe state
        std::cout << lua_tostring(L, -1) << std::endl; // error at top of stack
    }
}

void Game::HandleInput()
{
    if (mInput.IsKeyPressed(SDL_SCANCODE_ESCAPE)) mRunning = false; //esc quit

    mPlayer->HandleInput(mInput);
}

void Game::Update(float deltaTime)
{

    
    mPlayer->Update(deltaTime);

    mRenderer.SetCameraPosition(mPlayer->Position().x, mPlayer->Position().y);

    EnemyManager::Instance().UpdateEnemies(deltaTime);
    
    // temp hack - should be done elsewhere
    for (const auto& enemy : EnemyManager::Instance().Enemies()) {
        enemy->CheckPlayerInRange();
    } 

    // "housekeeping"
    EnemyManager::Instance().HousekeepEnemies();
}

void Game::Draw()
{ 
    glClear(GL_COLOR_BUFFER_BIT);
    mMap->DrawMap(mRenderer);   

    //EnemyManager::Instance().DrawEnemies(mRenderer);
    // EnemyManager::Instance().DrawEnemyHealthbars(mRenderer);

    mPlayer->Draw(mRenderer);

    //mRenderer.DrawText(400, 300, "Hello world");

    SDL_GL_SwapWindow(mWindow);
}

void Game::Destroy()
{
    delete mMap;
    delete mPlayer;
    
    lua_close(L);

    SDL_GL_DeleteContext(mContext);
    SDL_DestroyWindow(mWindow);
    
    SDL_Quit();
}