#ifndef ENEMY_H
#define ENEMY_H

#include "Game.h"    
#include "glm/glm.hpp"
#include "TextureManager.h"
#include "Player.h"

class Enemy
{
public:
    Enemy(glm::vec2 Position, glm::vec2 Scale, const char* filename, int pTileWidth, int pTileHeight, int scrRectX, int srcRectY, int number_of_frames)
    {
        this->mPosition = Position;
        this->mScale = Scale;
        this->tileWidth = pTileWidth;
        this->tileHeight = pTileHeight;

        mTexture = TextureManager::LoadTexture(filename);
        SDL_QueryTexture(mTexture, NULL, NULL, &SpriteSheetWidth, &SpriteSheetHeight);

        srcRect.x = scrRectX * pTileWidth;   // or tileFrame * tileWidth
        srcRect.y = srcRectY;   // pixel heigh 
        srcRect.w = pTileWidth; // Original Tile Width
        srcRect.h = pTileHeight;

        destRect.x = mPosition.x;
        destRect.y = mPosition.y;
        destRect.w = tileWidth * mScale.x;
        destRect.h = tileHeight * mScale.y;

        tileWidth *= mScale.x;
        tileHeight *= mScale.y;

        this->startTime = SDL_GetTicks();
        this-> numFrames = number_of_frames;
    }

    ~Enemy() 
    {      
        SDL_DestroyTexture(mTexture);
	}

    void Update()
    {
        destRect.x = static_cast<int>(mPosition.x);
        destRect.y = static_cast<int>(mPosition.y);      

        Animation();
    }

    void Render()
    {
        if (isWalkRightTextureOn) {
            TextureManager::Draw(mTextureWalkRight, srcRect, destRect, spriteFlip);
        }
        else
        {
            TextureManager::Draw(mTexture, srcRect, destRect, spriteFlip);
        }
    }


    void Animation()
    {   
        // There are 2 ways to do Frame Animation
        /*/ First Way of Animation
        currentFrame = ( (SDL_GetTicks() - startTime) * frameSpeed / 1000) % numFrames;
        srcRect.x = currentFrame * srcRect.w;
        */
        
        // Second Way of Animation
        frameTime++;
        if ((FPS / frameTime)  == numFrames)
        {            
            frameTime = 0;
            srcRect.x += srcRect.w; // we use srcRect.w because tileWidth was Scale 3x
            if (srcRect.x >= SpriteSheetWidth)
            {
                srcRect.x = 0;
            }
        }
                
    }

    void setTextureWalkRight(const char* filename, int pTileWidth, int pTileHeight, int scrRectX, int srcRectY)
    {
        mTextureWalkRight = TextureManager::LoadTexture(filename);

        srcRect.x = scrRectX * pTileWidth;   // or tileFrame * tileWidth
        srcRect.y = srcRectY;   // pixel heigh 
        srcRect.w = pTileWidth;
        srcRect.h = pTileHeight;
    }

    inline void setEnemyPositionX(int posX) 
    {
        mPosition.x = posX;
    }

    inline void setEnemyPositionY(int posY)
    {
        mPosition.y = posY;
    }

    inline SDL_Rect getEnemyDestRect() const
    {
        return destRect;
    }

    inline int getEnemyPositionX() const
    {
        return static_cast<int>(mPosition.x);
    }

    inline int getEnemyPositionY() const
    {
        return static_cast<int>(mPosition.y);
    }

    int getEnemyWidth() const
    {
        return tileWidth;
    }

    int getEnemyHeight() const
    {
        return tileHeight;
    }

    void moveRight()
    {
        isWalkRightTextureOn = true;
        mPosition.x += velocityX * Game::deltaTime;
    }

    void moveLeft()
    {
        isWalkRightTextureOn = false;
        mPosition.x -= velocityX * Game::deltaTime;
    }

    void ChasingPlayer(int PlayerPositionX)
    {
        if (PlayerPositionX < mPosition.x)
        {
            moveLeft();
            Update();
        }

        if (PlayerPositionX > mPosition.x)
        {
            moveRight();
            Update();
        }
    }

    inline void Falling(float velocity)
    {
        mPosition.y += velocity * Game::deltaTime;
    }

private:
    glm::vec2 mPosition;
    glm::vec2 mScale;

    SDL_Texture* mTexture;
    SDL_Texture* mTextureWalkRight;
    SDL_Rect srcRect, destRect;
    SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

    float velocityX = 10.0f;
    int tileWidth;
    int tileHeight;
    
    // for animation
    int numFrames;
    int frameTime = 0;
    // FPS or Frame Rate Speed or FrameSpeed
    const int frameSpeed = 10;
    const int FPS = 60; // Frame rate is the measurement of how quickly a number of frames appears within a second, which is why it's also called FPS (frames per second).
    int startTime;
    int currentFrame;
    int SpriteSheetWidth;
    int SpriteSheetHeight;
    bool isWalkRightTextureOn = false;

};

#endif 