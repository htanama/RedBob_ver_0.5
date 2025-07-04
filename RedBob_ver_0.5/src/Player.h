#ifndef PLAYER_H
#define PLAYER_H

#include "Game.h"    
#include "Level.h"
#include "TextureManager.h"
#include "glm/glm.hpp"

#define ORIGINAL_VELOCITY 300.0f
#define MAX_JUMP (-14000.0f)

class Player
{
    private:
        glm::vec2 mPosition;
        glm::vec2 mScale;

        SDL_Texture *mTexture;
        SDL_Rect srcRect, destRect;
        SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;        
        int originalTileWidth;
        int tileWidth;
        int tileHeight;
        bool m_isTextureChanged = false;
        bool m_isFallingAnimationOn = false;

        // for movement
        const float mAccelerationX = 90.0f;        
        float friction = 0.02f;
        float mVelocityX = ORIGINAL_VELOCITY;
        float mGravity = 350.0; // Original value 250.0    
        float jumpVelocity = MAX_JUMP;

        // for animation
        int numFrames; // number of frames
        const int frameSpeed = 10; 
        int currentFrame;
        int startTime;
    
        const Uint8* keyStateArray;

    public:
                
        Player(glm::vec2 Position, glm::vec2 Scale, const char* filename, int pTileWidth, int pTileHeight, int scrRectX, int srcRectY, int number_of_frames)
        {
            this->mPosition = Position;
            this->mScale = Scale;
            this->tileWidth = pTileWidth;
            this->tileHeight = pTileHeight;
            this->originalTileWidth = pTileWidth;

            mTexture = TextureManager::LoadTexture(filename);

            srcRect.x = scrRectX;   // or tileFrame * tileWidth
            srcRect.y = srcRectY;   // pixel heigh 
            srcRect.w = pTileWidth;
            srcRect.h = pTileHeight;
                        
            destRect.x = static_cast<int>(mPosition.x);
            destRect.y = static_cast<int>(mPosition.y);
            destRect.w = static_cast<int>(tileWidth * mScale.x);
            destRect.h = static_cast<int>(tileHeight * mScale.y);

            tileWidth *= mScale.x;
            tileHeight *= mScale.y;
              
            this->startTime = SDL_GetTicks();
            this->numFrames = number_of_frames;
                        
        }

        ~Player()
        {
            SDL_DestroyTexture(mTexture);        
        }
        void Animation()
        {            
            currentFrame = ( (SDL_GetTicks() - startTime) * frameSpeed / 1000) % numFrames;
            srcRect.x = currentFrame * srcRect.w;
        }

        void Update(SDL_Event &inputEvent)
        {
            Animation();
            keyStateArray = SDL_GetKeyboardState(NULL);
            
            if (Game::isRightKeyPressed == true && Game::isFallFromSky == false && mVelocityX != 0)
            {
                moveRight();
                mPosition.x += mVelocityX * Game::deltaTime;   
               
            }
            if (Game::isLeftKeyPressed == true && Game::isFallFromSky == false && mVelocityX != 0)
            {
                moveLeft();
                mPosition.x -= mVelocityX * Game::deltaTime;              
            }
            
            destRect.x = static_cast<int>(mPosition.x);
            destRect.y = static_cast<int>(mPosition.y);

            // player boundary
            if (mPosition.x < 0)
            {
                mPosition.x = 3;
            }
            if (mPosition.y < 0)
            {
                mPosition.y = 10;
            }
            if(mPosition.y > Game::windowHeight)
            {
                Game::IsGameOver = true;
                //mPosition.y = Game::windowHeight - 10;
            }

            // Gravity condition to pull the player down. 
            Gravity();

            // Player is on the ground
            if (Game::isFallFromSky == false && mPosition.y >= Game::GROUND_POSITION)
            {                
                mPosition.y = Game::GROUND_POSITION;
                Game::IsPlayerOnAir = false;
            }                  
        }
        
        void Render()
        {
            //SDL_RenderFillRect(Game::GameRenderer, &destRect);
            TextureManager::Draw(mTexture, srcRect, destRect, spriteFlip);
        }
        
        void setPlayerTexture(const char* filename, int pTileWidth, int pTileHeight, int scrRectX, int srcRectY, int number_of_frames,
        SDL_RendererFlip pSpriteFlip)
        {   
            mTexture = TextureManager::LoadTexture(filename);

            srcRect.x = scrRectX;   // or tileFrame * tileWidth
            srcRect.y = srcRectY;   // pixel heigh 
            srcRect.w = pTileWidth; 
            srcRect.h = pTileHeight;

            spriteFlip = pSpriteFlip;
        }

        void Gravity()
        {            
            if (Game::isFallFromSky == false && mPosition.y < Game::GROUND_POSITION)
            {                                
                mPosition.y += mGravity * Game::deltaTime;
            }            
        }

        inline void setVelocityX_ToZero()
		{
			mVelocityX = 0;
		}

        int getPlayerPositionX() const
        {
            return static_cast<int>(mPosition.x);
        }
        void setPlayerPositionX(double newXPos)
        {
            this->mPosition.x = newXPos;
        }

        void setPlayerPositionY(double newYPos)
        {
            this->mPosition.y = newYPos;
        }

        int getPlayerPositionY() const
        {
            return static_cast<int>(mPosition.y);
        }

        int getPlayerWidth() const
        {
            return tileWidth;
        }

        int getPlayerHeight() const
        {
            return tileHeight;
        }
        
        void moveRight()
        {             
            mVelocityX += (mAccelerationX * Game::deltaTime) - (friction * mVelocityX); 
        }

        void moveLeft()
        {          
            mVelocityX -= (mAccelerationX * Game::deltaTime) + (friction * mVelocityX);                    
        }

        inline void setJumpVelocity(float VelocityY)
        {
            jumpVelocity = VelocityY;
        }

        inline void Sprint()
        {            
            mVelocityX = 800; // Original 500.0                  
        }
        
        inline void setOriginalVelocityX()
        {
            if (Game::isSprintPressed == false)
                mVelocityX = ORIGINAL_VELOCITY;
        }

        void Jump()
        {
           if (Game::is_Collided_With_Object == true) {
               jumpVelocity = -4800.0;
               mPosition.y += jumpVelocity * Game::deltaTime;
           }
           else 
           {
               jumpVelocity = MAX_JUMP;
               mPosition.y += jumpVelocity * Game::deltaTime;
           }           
        }

        void Falling(float velocityY)
        {
            mPosition.y += (velocityY * Game::deltaTime) * 4;
        }

        // TESTING ONLY
       // To avoid collition with bottom tile
        inline void moveUp()
        {
            mPosition.y -= (300 * Game::deltaTime);
        }
        // To avoid collition with top tile or ceiling
        inline void moveDown()
        {
            //mPosition.y += (300 * Game::deltaTime);
            mVelocityX = 0;
        }          

        inline void setFallingAnimationToTrue()
        {
            m_isFallingAnimationOn = true;
        }

        inline bool getFallingAnimationStatus()
        {
            return m_isFallingAnimationOn;
        }
};
#endif