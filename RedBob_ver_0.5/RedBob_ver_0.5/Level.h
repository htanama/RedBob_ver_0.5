#ifndef LEVEL_H
#define LEVEL_H

#include "Game.h"    
#include "glm/glm.hpp"
#include "TextureManager.h"
#include <fstream>
#include <string>

extern bool playerFinishedMap1;
extern bool playerFinishedMap2;
extern bool playerFinishedMap3;
extern bool playerFinishedMap4;

class Level
{
public:

	Level(int tileSize, const char* filename, int &mapWidth, int &mapHeight)
	{
		mTexture = TextureManager::LoadTexture(filename);
		LoadFromFile(mapWidth, mapHeight);

		/*
		for (int row = 0; row < mapNumRow; row++) {
			for (int col = 0; col < mapNumCol; col++) {

				destRect[row][col].x = col * (tileScale * tileSize);
				destRect[row][col].y = row * (tileScale * tileSize);
				destRect[row][col].w = tileSize * tileScale;
				destRect[row][col].h = tileSize * tileScale;			
			}
		}
		
		for (int frame = 0; frame < 7; frame++)
		{
			srcRect[frame].x = tileSize * frame;
			srcRect[frame].y = 0 * tileSize;
			srcRect[frame].w = tileSize;
			srcRect[frame].h = tileSize;
		}

		for (int frame = 8; frame < 15; frame++)
		{
			srcRect[frame].x = tileSize * frame;
			srcRect[frame].y = 1 * tileSize;
			srcRect[frame].w = tileSize;
			srcRect[frame].h = tileSize;
		}

		for (int frame = 16; frame < 23; frame++)
		{
			srcRect[frame].x = tileSize * frame;
			srcRect[frame].y = 2 * tileSize;
			srcRect[frame].w = tileSize;
			srcRect[frame].h = tileSize;
		}

		for (int frame = 23; frame < 30; frame++)
		{
			srcRect[frame].x = tileSize * frame;
			srcRect[frame].y = 3 * tileSize;
			srcRect[frame].w = tileSize;
			srcRect[frame].h = tileSize;
		}
		*/

		this->mTileSize = tileSize;
	}

	~Level()
	{
		if (mTexture != NULL)
		{
			SDL_DestroyTexture(mTexture);
		}
	}
	
	void LoadFromFile(int &mapWidth, int &mapHeight)
	{
		std::fstream fileMap;
		fileMap.open("./assets/level1.map");

		if (!fileMap)
		{
			printf("Failed to open map \n");
		}

		for (int row = 0; row < mapNumRow; row++)
		{
			for (int col = 0; col < mapNumCol; col++)
			{
				char ch;
				fileMap.get(ch);
				int srcRectY = std::atoi(&ch) * mTileSize;
				fileMap.get(ch);
				int srcRectX = std::atoi(&ch) * mTileSize;
				fileMap.ignore();

				this->mPosition.x = srcRectX;
				this->mPosition.y = srcRectY;

				// We set the location of the destRect. 
				destRect[row][col].x = static_cast<int>(col * (tileScale * mTileSize));
				destRect[row][col].y = static_cast<int>(row * (tileScale * mTileSize));
				destRect[row][col].w = static_cast<int>(mTileSize * tileScale);
				destRect[row][col].h = static_cast<int>(mTileSize * tileScale);

				srcRect[row][col].x = srcRectX;
				srcRect[row][col].y = srcRectY;
				srcRect[row][col].w = mTileSize;
				srcRect[row][col].h = mTileSize;

			}
		}
		fileMap.close();

		mapWidth = mapNumCol * mTileSize * tileScale;
		mapHeight = mapNumRow * mTileSize * tileScale;
	}

	void setNextLevel1()
	{
		std::fstream fileMap;
		fileMap.open("./assets/level1.map");

		if (!fileMap)
		{
			printf("Failed to open map \n");
		}

		for (int row = 0; row < mapNumRow; row++)
		{
			for (int col = 0; col < mapNumCol; col++)
			{
				char ch;
				fileMap.get(ch);
				int srcRectY = std::atoi(&ch) * mTileSize;
				fileMap.get(ch);
				int srcRectX = std::atoi(&ch) * mTileSize;
				fileMap.ignore();

				this->mPosition.x = srcRectX;
				this->mPosition.y = srcRectY;

				// We set the location of the destRect. 
				destRect[row][col].x = static_cast<int>(col * (tileScale * mTileSize));
				destRect[row][col].y = static_cast<int>(row * (tileScale * mTileSize));
				destRect[row][col].w = static_cast<int>(mTileSize * tileScale);
				destRect[row][col].h = static_cast<int>(mTileSize * tileScale);

				srcRect[row][col].x = srcRectX;
				srcRect[row][col].y = srcRectY;
				srcRect[row][col].w = mTileSize;
				srcRect[row][col].h = mTileSize;

			}
		}
		fileMap.close();
	}

	void setNextLevel2()
	{
		std::fstream fileMap;
		fileMap.open("./assets/level2.map");

		if (!fileMap)
		{
			printf("Failed to open map \n");
		}

		for (int row = 0; row < mapNumRow; row++)
		{
			for (int col = 0; col < mapNumCol; col++)
			{
				char ch;
				fileMap.get(ch);
				int srcRectY = std::atoi(&ch) * mTileSize;
				fileMap.get(ch);
				int srcRectX = std::atoi(&ch) * mTileSize;
				fileMap.ignore();

				this->mPosition.x = srcRectX;
				this->mPosition.y = srcRectY;

				// We set the location of the destRect. 
				destRect[row][col].x = static_cast<int>(col * (tileScale * mTileSize));
				destRect[row][col].y = static_cast<int>(row * (tileScale * mTileSize));
				destRect[row][col].w = static_cast<int>(mTileSize * tileScale);
				destRect[row][col].h = static_cast<int>(mTileSize * tileScale);

				srcRect[row][col].x = srcRectX;
				srcRect[row][col].y = srcRectY;
				srcRect[row][col].w = mTileSize;
				srcRect[row][col].h = mTileSize;

			}
		}
		fileMap.close();
	}

	void setNextLevel3()
	{
		std::fstream fileMap;
		fileMap.open("./assets/level3.map");

		if (!fileMap)
		{
			printf("Failed to open map \n");
		}

		for (int row = 0; row < mapNumRow; row++)
		{
			for (int col = 0; col < mapNumCol; col++)
			{
				char ch;
				fileMap.get(ch);
				int srcRectY = std::atoi(&ch) * mTileSize;
				fileMap.get(ch);
				int srcRectX = std::atoi(&ch) * mTileSize;
				fileMap.ignore();

				this->mPosition.x = srcRectX;
				this->mPosition.y = srcRectY;

				// We set the location of the destRect. 
				destRect[row][col].x = static_cast<int>(col * (tileScale * mTileSize));
				destRect[row][col].y = static_cast<int>(row * (tileScale * mTileSize));
				destRect[row][col].w = static_cast<int>(mTileSize * tileScale);
				destRect[row][col].h = static_cast<int>(mTileSize * tileScale);

				srcRect[row][col].x = srcRectX;
				srcRect[row][col].y = srcRectY;
				srcRect[row][col].w = mTileSize;
				srcRect[row][col].h = mTileSize;

			}
		}
		fileMap.close();
	}

	void setNextLevel4()
	{
		std::fstream fileMap;
		fileMap.open("./assets/level4.map");

		if (!fileMap)
		{
			printf("Failed to open map \n");
		}

		for (int row = 0; row < mapNumRow; row++)
		{
			for (int col = 0; col < mapNumCol; col++)
			{
				char ch;
				fileMap.get(ch);
				int srcRectY = std::atoi(&ch) * mTileSize;
				fileMap.get(ch);
				int srcRectX = std::atoi(&ch) * mTileSize;
				fileMap.ignore();

				this->mPosition.x = srcRectX;
				this->mPosition.y = srcRectY;

				// We set the location of the destRect. 
				destRect[row][col].x = static_cast<int>(col * (tileScale * mTileSize));
				destRect[row][col].y = static_cast<int>(row * (tileScale * mTileSize));
				destRect[row][col].w = static_cast<int>(mTileSize * tileScale);
				destRect[row][col].h = static_cast<int>(mTileSize * tileScale);

				srcRect[row][col].x = srcRectX;
				srcRect[row][col].y = srcRectY;
				srcRect[row][col].w = mTileSize;
				srcRect[row][col].h = mTileSize;
			}
		}
		
		XOffset = 10;
		destRect[6][16].x = destRect[6][16].x + XOffset;

		fileMap.close();
	}

	void Update()
	{		

		if (Game::playerFinishedMap1 == true && Game::playerFinishedMap2 == false)		
		{
			setNextLevel2();
		}

		if (Game::playerFinishedMap1 == true && Game::playerFinishedMap2 == true && Game::playerFinishedMap3 == false)
		{
			setNextLevel3();
		}

		if (Game::playerFinishedMap1 == true && Game::playerFinishedMap2 == true && Game::playerFinishedMap3 == true && Game::playerFinishedMap4 == false)
		{
			setNextLevel4();
		}

		if (Game::playerFinishedMap1 == false && Game::playerFinishedMap4 == true)
		{
			setNextLevel1();			
		}
	}

	void Render()
	{
		for (int row = 0; row < mapNumRow; row++)
		{
			for (int col = 0; col < mapNumCol; col++)
			{
				TextureManager::Draw(mTexture, srcRect[row][col], destRect[row][col], SDL_FLIP_NONE);

				//SDL_RenderFillRect(Game::GameRenderer, &destRect[5][5]);

			}
		}
	}

	inline void setTileTextureToBlockHit(int row, int col)
	{
		destRect[row][col];
		srcRect[row][col].x = 2 * mTileSize;
		srcRect[row][col].y = 0 * mTileSize;
	}

	int getTilePositionX(int row, int col)
	{
		return destRect[row][col].x;
	}

	int getTilePositionY(int row, int col)
	{
		return destRect[row][col].y;
	}

	int getTileWidth(int row, int col)
	{
		return destRect[row][col].w;
	}

	int getTileHeight(int row, int col)
	{
		return destRect[row][col].h;
	}

	inline void Is_Collided(int row, int col, bool IsTileCollided)
	{
		destRect[row][col];
		mIsTileCollided = IsTileCollided;
	}

	inline bool IsCollided(int row, int col)
	{
		destRect[row][col];
		return mIsTileCollided;
	}

	void setTileTextureToBlock(int row, int col)	{		

		destRect[row][col];
		srcRect[row][col].x = 2 * mTileSize;
		srcRect[row][col].y = 0 * mTileSize;
	}
		
private:

	SDL_Texture* mTexture;
	SDL_Rect srcRect[10][30];
	SDL_Rect destRect[10][30];
	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;
	
	int mTileSize = 16;
	float tileScale = 3.0;
	int mapNumCol = 30;
	int mapNumRow = 10;
	int XOffset;
	int YOffset;
	glm::vec2 mPosition;
	bool mIsTileCollided = false;

	/*
	int tileMap[10][30] = {
		{05,05,20,21,05,05,10,05,05,05,20,21,05,05,05,05,05,10,05,05,05,05,05,05,05,05,05,05,05,05},
		{05,05,30,31,05,05,05,05,05,05,30,31,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05},
		{05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05},
		{05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05},
		{05,05,05,05,05,05,05,00,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05},
		{05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,15,05,05,05,05,05,05,05,05},
		{05,05,05,05,05,05,05,05,05,05,05,05,05,05,05,06,07,05,05,05,24,25,26,05,05,05,05,05,05,05},
		{05,05,12,13,05,05,05,05,05,05,05,05,05,05,05,16,17,05,05,33,34,35,36,37,05,05,05,05,05,05},
		{00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00},
		{00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00}
	};
	*/

};

#endif
