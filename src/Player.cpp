#include "Player.h"

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

#include "EnemyManager.h"
#include "AnimationKeyFrame.h"

Player::Player(Vector2 position, Texture texture) : Sprite(position, texture)
{
    mSpeed = 1000.0f;
    mMaxHealth = 100;
    mHealth = mMaxHealth;

    // open Lua file
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    
    if (luaL_dofile(L, "scripts/player.lua") != 0) {
        std::cout << lua_tostring(L, -1) << "\n"; // error at top of stack
    }

    // do literally nothing

    // close lua
    lua_close(L);

    // set default state to idle
    mState = PlayerState::STATE_MOVE;

    // pugi::xml_document doc;
    // pugi::xml_parse_result result = doc.load_file("xml/anim.xml");
    // std::cout << "parse result: " << result.description() << "\n";
    
    // for (const auto& animation : doc.child("animations").children()) {
    //     std::cout << animation.attribute("name").value() << "\n";
    //     //Animation anim = {};

    //     for (const auto& frame : animation.children()) {
    //         AnimationKeyFrame animKeyFrame = {};
    //         animKeyFrame.x = std::stoi(frame.attribute("x").value());
    //         animKeyFrame.y = std::stoi(frame.attribute("y").value());
    //         animKeyFrame.w = std::stoi(frame.attribute("w").value());
    //         animKeyFrame.h = std::stoi(frame.attribute("h").value());
    //         animKeyFrame.duration = std::stoi(frame.attribute("duration").value());
    //         std::cout << animKeyFrame.x << ", " << animKeyFrame.y << ", " << animKeyFrame.w << ", " << animKeyFrame.h << ", " << "\n";
    //     }

    //     mAnimations.insert({ animation.attribute("name").value(), anim });
    // }

    // COMPLETELY hardcode animations
    // idle
    Animation* idleSouth = new Animation(1);
    idleSouth->AddKeyFrame({ 0, 0, 16, 32 }, 0);
    mAnimations.insert({ "idle_south", idleSouth });

    Animation* idleEast = new Animation(1);
    idleEast->AddKeyFrame({ 0, 32, 16, 32 }, 0);
    mAnimations.insert({ "idle_east", idleEast });

    Animation* idleNorth = new Animation(1);
    idleNorth->AddKeyFrame({ 0, 64, 16, 32 }, 0);
    mAnimations.insert({ "idle_north", idleNorth });

    Animation* idleWest = new Animation(1);
    idleWest->AddKeyFrame({ 0, 96, 16, 32 }, 0);
    mAnimations.insert({ "idle_west", idleWest });

    // walking
    Animation* walkSouth = new Animation(4);
    walkSouth->AddKeyFrame({ 0,  0, 16, 32 }, 0);
    walkSouth->AddKeyFrame({ 16, 0, 16, 32 }, 1);
    walkSouth->AddKeyFrame({ 32, 0, 16, 32 }, 2);
    walkSouth->AddKeyFrame({ 48, 0, 16, 32 }, 3);
    mAnimations.insert({ "walk_south", walkSouth });

    Animation* walkEast = new Animation(4);
    walkEast->AddKeyFrame({ 0,  32, 16, 32 }, 0);
    walkEast->AddKeyFrame({ 16, 32, 16, 32 }, 1);
    walkEast->AddKeyFrame({ 32, 32, 16, 32 }, 2);
    walkEast->AddKeyFrame({ 48, 32, 16, 32 }, 3);
    mAnimations.insert({ "walk_east", walkEast });

    Animation* walkNorth = new Animation(4);
    walkNorth->AddKeyFrame({ 0,  64, 16, 32 }, 0);
    walkNorth->AddKeyFrame({ 16, 64, 16, 32 }, 1);
    walkNorth->AddKeyFrame({ 32, 64, 16, 32 }, 2);
    walkNorth->AddKeyFrame({ 48, 64, 16, 32 }, 3);
    mAnimations.insert({ "walk_north", walkNorth });

    Animation* walkWest = new Animation(4);
    walkWest->AddKeyFrame({ 0,  96, 16, 32 }, 0);
    walkWest->AddKeyFrame({ 16, 96, 16, 32 }, 1);
    walkWest->AddKeyFrame({ 32, 96, 16, 32 }, 2);
    walkWest->AddKeyFrame({ 48, 96, 16, 32 }, 3);
    mAnimations.insert({ "walk_west", walkWest });

    // attacking
    Animation* attackSouth = new Animation(4);
    attackSouth->AddKeyFrame({ 0,  128, 32, 32 }, 0);
    attackSouth->AddKeyFrame({ 32, 128, 32, 32 }, 1);
    attackSouth->AddKeyFrame({ 64, 128, 32, 32 }, 2);
    attackSouth->AddKeyFrame({ 96, 128, 32, 32 }, 3);
    mAnimations.insert({ "attack_south", attackSouth });

    Animation* attackNorth = new Animation(4);
    attackNorth->AddKeyFrame({ 0,  160, 32, 32 }, 0);
    attackNorth->AddKeyFrame({ 32, 160, 32, 32 }, 1);
    attackNorth->AddKeyFrame({ 64, 160, 32, 32 }, 2);
    attackNorth->AddKeyFrame({ 96, 160, 32, 32 }, 3);
    mAnimations.insert({ "attack_north", attackNorth });

    Animation* attackEast = new Animation(4);
    attackEast->AddKeyFrame({ 0,  192, 32, 32 }, 0);
    attackEast->AddKeyFrame({ 32, 192, 32, 32 }, 1);
    attackEast->AddKeyFrame({ 64, 192, 32, 32 }, 2);
    attackEast->AddKeyFrame({ 96, 192, 32, 32 }, 3);
    mAnimations.insert({ "attack_east", attackEast });

    Animation* attackWest = new Animation(4);
    attackWest->AddKeyFrame({ 0,  224, 32, 32 }, 0);
    attackWest->AddKeyFrame({ 32, 224, 32, 32 }, 1);
    attackWest->AddKeyFrame({ 64, 224, 32, 32 }, 2);
    attackWest->AddKeyFrame({ 96, 224, 32, 32 }, 3);
    mAnimations.insert({ "attack_west", attackWest });
}

Player::~Player()
{
    // clear animations
    for (auto& it: mAnimations) {
        std::cout << "Deleting animation: " << it.first << std::endl;
        delete it.second;
    }
    
    mAnimations.clear();
}

void Player::TakeDamage(int damage)
{
    if (mHealth - damage > 0) {
        mHealth -= damage;
    } else {
        mHealth = 0;
        // dead
    }
}

void Player::Heal(int amount)
{
    if (mHealth + amount > mMaxHealth) {
        mHealth = mMaxHealth;
    } else {
        mHealth += amount;
    }
}

void Player::Attack()
{
    // option 1 - handle through a decoupled interface
    for (int i = 0; i < mNumObservers; i++) {
        mObservers[i]->OnNotify(*this, 1);
    }

    // option 2 - just leave it coupled 
    EnemyManager::Instance().HandlePlayerAttack(*this);
}

void Player::Update(float deltaTime)
{
    switch(mState) {
    case PlayerState::STATE_MOVE:
        // select animation based on direction - has the player pressed a movement key?
        if (mDirection == Vector2::Zero()) {
            switch(mFacingDirection) {
            case FacingDirection::FACING_SOUTH: mCurrentAnimation = "idle_south"; break;
            case FacingDirection::FACING_EAST:  mCurrentAnimation = "idle_east"; break;
            case FacingDirection::FACING_NORTH: mCurrentAnimation = "idle_north"; break;
            case FacingDirection::FACING_WEST:  mCurrentAnimation = "idle_west"; break;
            }
        } else {
            mDirection.Normalize();
            switch(mFacingDirection) {
            case FacingDirection::FACING_SOUTH: mCurrentAnimation = "walk_south"; break;
            case FacingDirection::FACING_EAST:  mCurrentAnimation = "walk_east"; break;
            case FacingDirection::FACING_NORTH: mCurrentAnimation = "walk_north"; break;
            case FacingDirection::FACING_WEST:  mCurrentAnimation = "walk_west"; break;
            }
        }

        // apply speed and friction
        mAcceleration = mDirection * mSpeed;
        mAcceleration -= mVelocity * mFriction;

        // calculate new position and velocity from acceleration
        mPosition += mVelocity * deltaTime + mAcceleration * 0.5f * deltaTime * deltaTime;
        mVelocity += mAcceleration * deltaTime;

        // reset direction
        mDirection = Vector2::Zero();

        break;
    case PlayerState::STATE_ATTACK:
        switch(mFacingDirection) {
            case FacingDirection::FACING_SOUTH: mCurrentAnimation = "attack_south"; break;
            case FacingDirection::FACING_EAST:  mCurrentAnimation = "attack_east"; break;
            case FacingDirection::FACING_NORTH: mCurrentAnimation = "attack_north"; break;
            case FacingDirection::FACING_WEST:  mCurrentAnimation = "attack_west"; break;
        }

        EnemyManager::Instance().HandlePlayerAttack(*this);

        break;
    }

    //std::cout << "current anim: " << mCurrentAnimation << "\n";
    mAnimations[mCurrentAnimation]->Update(deltaTime);    
}

void Player::Draw(Renderer& renderer)
{
    mAnimations[mCurrentAnimation]->DrawCurrentFrame(renderer, mPosition, mTexture);
    
    // draw hitbox for debug
    HitBox().Draw(renderer);
}

void Player::DrawHealthbar(Renderer& renderer)
{ 

}