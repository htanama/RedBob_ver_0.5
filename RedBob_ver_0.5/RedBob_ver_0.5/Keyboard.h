#ifndef KEYBOARD_H  
#define KEYBOARD_H

#include "Game.h"
#include <SDL.h>

class Keyboard
{
private: 
    const Uint8* keyStateArray;

public:
    Keyboard() = default;
    
    void KeyboardEvent(SDL_Event& e, Mix_Chunk *JumpSound, Player *player)
    {
        keyStateArray = SDL_GetKeyboardState(NULL);

            if (e.type == SDL_KEYDOWN)
            {
                if (keyStateArray[SDL_SCANCODE_SPACE] && keyStateArray[SDL_SCANCODE_RIGHT] &&
                    Game::isFallFromSky == false && Game::IsPlayerOnAir == false && Game::isJumpKeyPressed == false)
                {
                    player->Jump();
                    Game::isJumpKeyPressed = true;
                    player->moveRight();
                    Mix_PlayChannel(-1, JumpSound, 0);

                    if (Game::isRightKeyPressed == true)
                    {
                        player->setPlayerTexture("assets/player-right.png", 16, 16, 32, 0, 5, SDL_FLIP_NONE);
                    }
                    Game::IsPlayerOnAir = true; // no double jump			                    
                }

                if (keyStateArray[SDL_SCANCODE_SPACE] && keyStateArray[SDL_SCANCODE_LEFT] &&
                    Game::isFallFromSky == false && Game::IsPlayerOnAir == false && Game::isJumpKeyPressed == false)
                {
                    player->Jump();
                    Game::isJumpKeyPressed = true;
                    player->moveLeft();                                     
                    Mix_PlayChannel(-1, JumpSound, 0);
                    if (Game::isLeftKeyPressed == true)
                    {
                        player->setPlayerTexture("assets/player-left.png", 16, 16, 1, 0, 5, SDL_FLIP_NONE);

                    }
                    Game::IsPlayerOnAir = true;
                }

                if (keyStateArray[SDL_SCANCODE_SPACE] && Game::isFallFromSky == false && 
                    Game::IsPlayerOnAir == false && Game::isJumpKeyPressed == false)
                {                    
                    player->Jump();
                    Game::isJumpKeyPressed = true;
                    Mix_PlayChannel(-1, JumpSound, 0);

                    if (Game::isRightKeyPressed == true)
                    {
                        player->setPlayerTexture("assets/player-right.png", 16, 16, 32, 0, 5, SDL_FLIP_NONE);
                    }

                    if (Game::isLeftKeyPressed == true)
                    {
                        player->setPlayerTexture("assets/player-left.png", 16, 16, 32, 0, 5, SDL_FLIP_NONE);
                    }

                    Game::IsPlayerOnAir = true; // no double jump

                }

                if (keyStateArray[SDL_SCANCODE_RIGHT] && Game::isFallFromSky == false)
                {   
                    player->setOriginalVelocityX();
                    player->moveRight();
                    player->setPlayerTexture("assets/player-right.png", 16, 16, 0, 0, 5, SDL_FLIP_NONE);
                    Game::isRightKeyPressed = true;                     
                }

                if (keyStateArray[SDL_SCANCODE_LEFT] && Game::isFallFromSky == false)
                {
                    player->setOriginalVelocityX();
                    player->moveLeft();
                    player->setPlayerTexture("assets/player-left.png", 16, 16, 0, 0, 5, SDL_FLIP_NONE);
                    Game::isLeftKeyPressed = true;                        
                }
            }                                    
            
            if (e.type == SDL_KEYUP)
            {
                switch (e.key.keysym.sym)
                {
                case SDLK_LEFT:
                    Game::isLeftKeyPressed = false;
                    break;
                case SDLK_RIGHT:
                    Game::isRightKeyPressed = false;
                case SDLK_SPACE:
                    Game::isJumpKeyPressed = false;
                    break;
                case SDLK_z:
                    Game::isSprintPressed = false;
                    player->setOriginalVelocityX(); // set velocity to original velocity, after sprinting
                    break;

                }
            }

            if (e.type == SDL_KEYDOWN) 
            {
                switch (e.key.keysym.sym)
                {                
                case SDLK_z:
                    if (Game::IsPlayerOnAir == false && Game::isFallFromSky == false &&
                     Game::isSprintPressed == false)
                    {
                        Game::isSprintPressed = true;				
                        player->Sprint();
                    }break;

                /*/for testing only
                case SDLK_UP:
                    player->moveUp();
                    break;
                */
                case SDLK_DOWN:
                    if(Game::isFallFromSky == false)
                    {   
                        player->moveDown();
                    }
                    break;
                }
            }



    }
    ~Keyboard() {
        if (keyStateArray != nullptr)  { keyStateArray = nullptr; }
    }    
};

#endif