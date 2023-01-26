#include "Game.h"

#include <cstdio>
#include <iostream>
#include <cmath> // floor

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

    return true;
}

void Game::Run()
{
    Create();

    Uint32 lastTime = 0;

    SDL_Event event;
    bool running = true;

    // main loop
    while (running) {
        // handle input events
        while (SDL_PollEvent(&event) != 0) {
            switch(event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) running = false; //esc quit
                mInput.SetKeyDown(event.key.keysym.scancode);
                break;
            case SDL_KEYUP:
                mInput.SetKeyUp(event.key.keysym.scancode);
                break;
            }
        }

        // calculate delta time
        Uint32 currentTime = SDL_GetTicks();
		float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        // update
        Update(deltaTime);

        // update input
        mInput.UpdateInput();

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
    mRenderer.InitTextData();

    // load required textures
    TextureManager::Instance().LoadTexture("images/test.png");
    TextureManager::Instance().LoadTexture("images/tileset.png");

    // load required fonts
    mRenderer.LoadFont("fonts/arial.ttf", 16);

    // test map generator
    mMap = Map::RandomMap(64, 64, TextureManager::Instance().GetTexture("images/tileset.png"), 16, mRenderer);

    mPlayer = new Player(Vector2(3.0f, 3.0f), TextureManager::Instance().GetTexture("images/test.png"));

    // open lua
    L = luaL_newstate();
    luaL_openlibs(L); // open the base libraries

    // lua_register(L, "CreateEnemy", wrap_CreateEnemy); // expose api function to lua

    if (luaL_dofile(L, "scripts/test.lua") != 0) {
        // abort, idk. todo: error checking, put the program back into a safe state
        std::cout << lua_tostring(L, -1) << std::endl; // error at top of stack
    }

    lua_getglobal(L, "init"); // get function from lua script, i.e. push it onto lua stack
    if (lua_isfunction(L, -1)) {
        lua_pushlightuserdata(L, this); // push "game" object pointer to lua

        // call the function
        if (lua_pcall(L, 1, 0, 0) != 0) {
            std::cout << lua_tostring(L, -1) << std::endl; // error
        }

        std::cout << "C++: called lua function init\n";
    }
}

void Game::Update(float deltaTime)
{
    // player movement
    mPlayer->SetDirectionX(0.0f);
    mPlayer->SetDirectionY(0.0f);
    if (mInput.IsKeyHeld(SDL_SCANCODE_W) || mInput.IsKeyHeld(SDL_SCANCODE_UP)) {
        mPlayer->SetDirectionY(-1.0f);
    }
    if (mInput.IsKeyHeld(SDL_SCANCODE_A) || mInput.IsKeyHeld(SDL_SCANCODE_LEFT)) {
        mPlayer->SetDirectionX(-1.0f);
    }
    if (mInput.IsKeyHeld(SDL_SCANCODE_S) || mInput.IsKeyHeld(SDL_SCANCODE_DOWN)) {
        mPlayer->SetDirectionY(1.0f);
    }
    if (mInput.IsKeyHeld(SDL_SCANCODE_D) || mInput.IsKeyHeld(SDL_SCANCODE_RIGHT)) {
        mPlayer->SetDirectionX(1.0f);
    }
    if (mInput.IsKeyPressed(SDL_SCANCODE_SPACE)) {
        mPlayer->TakeDamage(10);
        std::cout << "position: " << mPlayer->Position().x << ", " << mPlayer->Position().y << "\n";
    }

    mPlayer->Update(deltaTime);

    mRenderer.SetCameraPosition(mPlayer->Position().x, mPlayer->Position().y);

    //EnemyManager::Instance().UpdateEnemies(*mMap, *mPlayer);

    // "housekeeping"
    //EnemyManager::Instance().HousekeepEnemies();

}

void Game::Draw()
{ 
    glClear(GL_COLOR_BUFFER_BIT);
    mMap->DrawMap(mRenderer);   

    // EnemyManager::Instance().DrawEnemies(mRenderer);
    // EnemyManager::Instance().DrawEnemyHealthbars(mRenderer);

    mPlayer->Draw(mRenderer);

    mRenderer.DrawText(400, 300, "Hello world");

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