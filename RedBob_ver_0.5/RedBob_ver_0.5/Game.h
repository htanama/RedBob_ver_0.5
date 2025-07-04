#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <iostream>
#include "glm/glm.hpp"

const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game
{
public: 
    Game();
    ~Game();
    void init();
    void SetupLevel1();
    void SetupLevel2();
    void SetupLevel3();
    void SetupLevel4();
    void Run();
    void ProcessInput();
    void Update();
    void Render();
    void CleanUP();    
     
    bool CheckAABB_Collision(int posAX, int posAY, int AWidth, int AHeight, int posBX, int posBY, int BWidth, int BHeight); 
    bool CheckTop_Collision(int posAY, int posBY, int BHeight);
    bool topCollision;
    bool CheckBottom_Collision(int posAY, int AHeight, int posBY);
    bool bottomCollision;
    bool CheckRight_Collision(int posAX, int AWidth, int posBX);
    bool rightCollision;
    bool CheckLeft_Collision(int posAX, int posBX, int BWidth);
    bool leftCollision;

    void Setup_LevelOneCollision();
    void LevelOneCollision();

    void Setup_LevelTwoCollision();
    void LevelTwoCollision();

    void Setup_LevelThreeCollision();
    void LevelThreeCollision();

    void Setup_LevelFourCollision();
    void LevelFourCollision();
    void EnemyMovementAI();

    static SDL_Renderer *GameRenderer;    
    static double deltaTime;
    static float fGravityDeathFalling; // originlaly 10.0f
    static int windowWidth;
    static int windowHeight;
    static int mapWidth;
    static int mapHeight;
    static int player_Old_PositionX;
    static int tmpTotalScore;
    static const int GROUND_POSITION;
    
    static bool playerFinishedMap1;
    static bool playerFinishedMap2;
    static bool playerFinishedMap3;
    static bool playerFinishedMap4;
    static bool IsPlayerOnAir;
    static bool isFallFromSky;
    static bool isJumpKeyPressed;
    static bool isSprintPressed;
    static bool is_Collided_With_Object;
    static bool isLeftKeyPressed;
    static bool isRightKeyPressed;
    static bool isPlayerIdle;
    static bool isPlayerOnTopOfBlock;
    static bool is_movingFloorMoveToLeft;    

private:
    int millisecsPreviousFrame = 0;
    
    SDL_Window  *mGameWindow = nullptr;
    SDL_Texture *mBackground = nullptr;
    SDL_Rect backgroundRect; 
    SDL_Event sdlEvent;
    

    const Uint16 SCREEN_WIDTH = 1200;
    const Uint16 SCREEN_HEIGHT = 700;
    bool isDebug = false;
    bool mGameIsRunning = false;
    bool isSpiderChasePlayer = true;
    bool enemyDeathFall = false;
    bool collisionOccured = false;
    bool isPlayerOutofBounds = false;
    bool isSpiderWalkToRight = false;
    bool isTextureChanged = false;
    int player_Old_PositionY;   

    
    Mix_Music* Game_Music = nullptr;
    Mix_Chunk* JumpSound = nullptr;  
    Mix_Chunk* ObjectAppearSound = nullptr;
    Mix_Chunk* coinCollectSound = nullptr;
    Mix_Chunk* BumpSound = nullptr;
    Mix_Chunk* objectCollectedSound = nullptr;

};
#endif