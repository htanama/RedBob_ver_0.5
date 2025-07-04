#ifndef OBJECT_H
#define OBJECT_H

#include <SDL.h>
#include "glm/glm.hpp"
#include "TextureManager.h"

class Object
{
public:
	Object(glm::vec2 Position, glm::vec2 Scale, int pTileWidth, int pTileHeight)
	{
		this->mPosition = Position;
		this->mScale = Scale;
		this->tileWidth = pTileWidth;
		this->tileHeight = pTileHeight;

		destRect = {
			static_cast<int>(mPosition.x),
			static_cast<int>(mPosition.y),
			static_cast<int>(tileWidth * mScale.x),
			static_cast<int>(tileHeight * mScale.y)
		};

		/* so after we updated the width and heigh of destRect.w and destRecth, we need to scale the tileWidth and tileHeight 
		because if we scale the tileWidth in the begining, destRect.w and destRect.h will be scale twice. */
		tileWidth = tileWidth * mScale.x;
		tileHeight = tileHeight * mScale.y;

		isDrawTexture = false;
	}

	Object(glm::vec2 Position, glm::vec2 Scale, const char* filename, int pTileWidth, int pTileHeight, int scrRectX, int srcRectY)
	{
		this->mPosition = Position;
		this->mScale = Scale;
		this->tileWidth = pTileWidth;
		this->tileHeight = pTileHeight;

		mTexture = TextureManager::LoadTexture(filename);
		SDL_QueryTexture(mTexture, NULL, NULL, &SpriteSheetWidth, &SpriteSheetHeight);

		srcRect.x = scrRectX * pTileWidth;   // or tileFrame * tileWidth
		srcRect.y = srcRectY * pTileHeight;   // pixel heigth 
		srcRect.w = pTileWidth;
		srcRect.h = pTileHeight;

		destRect.x = mPosition.x;
		destRect.y = mPosition.y;
		destRect.w = tileWidth * mScale.x;
		destRect.h = tileHeight * mScale.y;

		tileWidth *= mScale.x;
		tileHeight *= mScale.y;

		isDrawTexture = true;
	}

	Object(glm::vec2 Position, glm::vec2 Scale, const char* filename, int pTileWidth, int pTileHeight, 
		int scrRectX, // scrRectX = tileWidth * frameNumberX or frame_index_X
		int srcRectY, // scrRectY = tileHeight * frameNumberY or frame_index_Y
		int number_of_frames, 
		bool IsAnimated)
	{
		this->mPosition = Position;
		this->mScale = Scale;
		this->tileWidth = pTileWidth;
		this->tileHeight = pTileHeight;

		mTexture = TextureManager::LoadTexture(filename);
		SDL_QueryTexture(mTexture, NULL, NULL, &SpriteSheetWidth, &SpriteSheetHeight);

		srcRect.x = scrRectX * pTileWidth;	// srcRect.x = tileWidth * frameNumberX or frame_index_X
		srcRect.y = srcRectY * pTileHeight;   // srcRect.y = tileHeight * frameNumberY or frame_index_Y
		srcRect.w = pTileWidth;
		srcRect.h = pTileHeight;

		destRect.x = mPosition.x;
		destRect.y = mPosition.y;
		destRect.w = tileWidth * mScale.x;
		destRect.h = tileHeight * mScale.y;

		tileWidth *= mScale.x;
		tileHeight *= mScale.y;

		isDrawTexture = true;

		this->mIsAnimated = IsAnimated;
		this->startTime = SDL_GetTicks();
		this->numFrames = number_of_frames;
	}

	~Object()
	{
		if (mTexture != NULL)
		{
			SDL_DestroyTexture(mTexture);
		}
	}

	void Update()
	{
		destRect.x = static_cast<int>(mPosition.x);
		destRect.y = static_cast<int>(mPosition.y);

		if(mIsAnimated)
			Animation();

		if(mIsGravityApplied)
			Gravity();

		if(mPosition.y >= Game::GROUND_POSITION && mIsGravityApplied)
		{
			mPosition.y = Game::GROUND_POSITION;
			mIsOnPlatformOrGround = true;
		}
		else
		{
			mIsOnPlatformOrGround = false;
		}
	}

	void Render()
	{
		if (isDrawTexture == true) {
			TextureManager::Draw(mTexture, srcRect, destRect, spriteFlip);
		}

		if (isDrawTexture == false)
		{
			SDL_SetRenderDrawColor(Game::GameRenderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
			SDL_RenderFillRect(Game::GameRenderer, &destRect);
		}
	}

	void setTexture(const char* filename, int pTileWidth, int pTileHeight, int scrRectX, int srcRectY, SDL_RendererFlip pSpriteFlip)
	{
		mTexture = TextureManager::LoadTexture(filename);

		srcRect.x = scrRectX;   // or tileFrame * tileWidth
		srcRect.y = srcRectY;   // pixel heigh 
		srcRect.w = pTileWidth;
		srcRect.h = pTileHeight;

		spriteFlip = pSpriteFlip;
	}

	void Animation()
	{
		// There are 2 ways to do Frame Animation
		// First Way of Animation
		// currentFrame = ( (SDL_GetTicks() - startTime) * frameSpeed / 1000) % numFrame;
		// srcRect.x = currentFrame * srcRect.w;

		// Second Way of Animation
		frameTime++;
		if ((FPS / frameTime) == numFrames)
		{
			frameTime = 0;
			srcRect.x += srcRect.w; // we use srcRect.w because tileWidth was Scale 3x
			if (srcRect.x >= SpriteSheetWidth)
			{
				srcRect.x = 0;
			}
		}
	}
	inline void setIsObjectOnTheAir(bool isObjectOnTheAir)
	{
		this->mIsObjectOnTheAir = isObjectOnTheAir;
	}

	inline bool getIsObjectOnTheAir()
	{
		return mIsObjectOnTheAir;
	}

	inline void setIsObjectAppears(bool isObjectAppears){
		this->mIsObjectAppears = isObjectAppears;
	}
	inline bool getIsObjectAppears() const {
		return mIsObjectAppears;
	}

	inline void setPositionX(int posX)
	{
		mPosition.x = posX;
	}

	inline void setPositionY(int posY)
	{
		mPosition.y = posY;
	}

	inline void setVelocityX(float VelocityX)
	{
		this->mVelocity = VelocityX;
	}

	inline void moveToLeft()
	{
		mPosition.x -= mVelocity * Game::deltaTime;
	}

	inline void moveToRight()
	{
		mPosition.x += mVelocity * Game::deltaTime;
	}

	inline void setApplyGravity(bool isGravityApplied)
	{
		this->mIsGravityApplied = isGravityApplied;
	}

	inline void Gravity()
	{	
		if (mIsOnPlatformOrGround == false)
			mPosition.y += mGravity * Game::deltaTime;
	}

	inline int getPosX() const
	{
		return mPosition.x;
	}

	inline int getPosY() const
	{
		return mPosition.y;
	}

	inline int getWidth() const
	{
		return tileWidth;
	}

	inline int getHeight() const
	{
		return tileHeight;
	}

	inline void Falling(float velocity)
	{
		mPosition.y += velocity * Game::deltaTime;
	}

	inline void setIsObjectCollected(bool isObjectCollected)
	{
		this->mIsObjectCollected = isObjectCollected;
	}

	inline bool getIsObjectCollected() const
	{
		return mIsObjectCollected;
	}


private:
	SDL_Rect srcRect, destRect;
	SDL_Texture* mTexture;
	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	glm::vec2 mPosition;
	glm::vec2 mScale;

	int tileWidth;
	int tileHeight;
	float mVelocity = 10.0;
	bool isDrawTexture;

	// for animation
	int numFrames;
	int frameTime = 0;
	// FPS or Frame Rate Speed or FrameSpeed
	const int frameSpeed = 60;
	const int FPS = 60; // Frame rate is the measurement of how quickly a number of frames appears within a second, which is why it's also called FPS (frames per second).
	int startTime;
	int currentFrame;
	int SpriteSheetWidth;
	int SpriteSheetHeight;
	bool mIsAnimated = false;
	bool mIsObjectCollected = false;
	bool mIsObjectOnTheAir = false; 

	bool mIsGravityApplied = false;
	bool mIsOnPlatformOrGround = false;
	bool mIsObjectAppears = false;
	float mGravity = 20.0f;
};
#endif 