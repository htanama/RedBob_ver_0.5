#ifndef COLLISIONBOX_H
#define COLLISIONBOX_H

#include <SDL.h>
#include "Player.h"
#include "Level.h"
#include "Object.h"
#include "Enemy.h"

class CollisionBox {
public:

	CollisionBox() : mCollisionBox({ 0, 0, 0, 0 }) {}

	CollisionBox(SDL_Rect &paramDestRect)
	{
		mCollisionBox.x = paramDestRect.x;
		mCollisionBox.y = paramDestRect.y;
		mCollisionBox.w = paramDestRect.w; 
		mCollisionBox.h = paramDestRect.h;
	}
	
	~CollisionBox()
	{
	}
	
	// Copy constructor
	void operator = (const CollisionBox& paramCollisionBox)
	{
		mCollisionBox.x = paramCollisionBox.mCollisionBox.x;
		mCollisionBox.y = paramCollisionBox.mCollisionBox.y;
		mCollisionBox.w = paramCollisionBox.mCollisionBox.w;
		mCollisionBox.h = paramCollisionBox.mCollisionBox.h;
	}
	
	void Render()
	{
		SDL_RenderDrawRect(Game::GameRenderer, &mCollisionBox);
	}

	void Init_CollisionBox_With_LevelMap(Level *LevelMap, int Row, int Col)
	{
		mCollisionBox.x = LevelMap->getTilePositionX(Row, Col);
		mCollisionBox.y = LevelMap->getTilePositionY(Row, Col);
		mCollisionBox.w = LevelMap->getTileWidth(Row, Col);
		mCollisionBox.h = LevelMap->getTileHeight(Row, Col);
	}

	void Init_CollisionBox_With_LevelMap_And_OffsetX(Level *LevelMap, int Row, int Col, int OffsetX)
	{
		mCollisionBox.x = LevelMap->getTilePositionX(Row, Col) + OffsetX;
		mCollisionBox.y = LevelMap->getTilePositionY(Row, Col);
		mCollisionBox.w = LevelMap->getTileWidth(Row, Col);
		mCollisionBox.h = LevelMap->getTileHeight(Row, Col);
	}

	void Init_CollisionBox_With_LevelMap_And_OffsetY(Level *LevelMap, int Row, int Col, int OffsetY)
	{
		mCollisionBox.x = LevelMap->getTilePositionX(Row, Col);
		mCollisionBox.y = LevelMap->getTilePositionY(Row, Col) + OffsetY;
		mCollisionBox.w = LevelMap->getTileWidth(Row, Col);
		mCollisionBox.h = LevelMap->getTileHeight(Row, Col);
	}

	void Init_CollisionBox_With_LevelMap_And_OffsetX_And_OffsetY(Level* LevelMap, int Row, int Col, int OffsetX, int OffsetY)
	{
		mCollisionBox.x = LevelMap->getTilePositionX(Row, Col) + OffsetX;
		mCollisionBox.y = LevelMap->getTilePositionY(Row, Col) + OffsetY;
		mCollisionBox.w = LevelMap->getTileWidth(Row, Col);
		mCollisionBox.h = LevelMap->getTileHeight(Row, Col);
	}

	void Init_CollisionBox_With_Player(Player* player)
	{
		mCollisionBox.x = player->getPlayerPositionX();
		mCollisionBox.y = player->getPlayerPositionY();
		mCollisionBox.w = player->getPlayerWidth();
		mCollisionBox.h = player->getPlayerHeight();
	}

	void Init_CollisionBox_With_Object(Object* object)
	{
		mCollisionBox.x = object->getPosX();
		mCollisionBox.y = object->getPosY();
		mCollisionBox.w = object->getWidth();
		mCollisionBox.h = object->getHeight();
	}

	void Init_CollisionBox_With_Enemy(Enemy* enemy)
	{
		mCollisionBox.x = enemy->getEnemyPositionX();
		mCollisionBox.y = enemy->getEnemyPositionY();
		mCollisionBox.w = enemy->getEnemyWidth();
		mCollisionBox.h = enemy->getEnemyHeight();
	}

	inline void setCollisionBox(int XPosition, int YPoisition, int Width, int Height)
	{
		mCollisionBox.x = XPosition;
		mCollisionBox.y = YPoisition;
		mCollisionBox.w = Width;
		mCollisionBox.h = Height;
	}

	inline bool Checking_AABB_Collision(CollisionBox& HitBox)
	{
		if (mCollisionBox.x < HitBox.getPosX() + HitBox.getWidth() &&
			mCollisionBox.x + mCollisionBox.w > HitBox.getPosX() &&
			mCollisionBox.y < HitBox.getPosY() + HitBox.getHeight() &&
			mCollisionBox.y + mCollisionBox.h > HitBox.getPosY())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	inline bool TopCollision(CollisionBox& HitBox)
	{
		return ( mCollisionBox.y < HitBox.getPosY() + HitBox.getHeight() );
	}

	inline bool BottomCollision(CollisionBox& HitBox)
	{
		return ( mCollisionBox.y + mCollisionBox.h > HitBox.getPosY() );
	}

	inline bool LeftCollision(CollisionBox& HitBox)
	{
		return ( mCollisionBox.x < HitBox.getPosX() + HitBox.getWidth() );
	}

	inline bool RightCollision(CollisionBox& HitBox)
	{
		return ( mCollisionBox.x + mCollisionBox.w > HitBox.getPosX() );
	}

	void CollisionCheck_PlayerAndTile_PlayerOnTop(CollisionBox& HitBox, Player *player, Level *LevelMap, int Row, int Col)
	{		
		if (mCollisionBox.x < HitBox.getPosX() + HitBox.getWidth() &&
			mCollisionBox.x + mCollisionBox.w > HitBox.getPosX() &&
			mCollisionBox.y < HitBox.getPosY() + HitBox.getHeight() &&
			mCollisionBox.y + mCollisionBox.h > HitBox.getPosY())
		{
			// TOP: posAY < posBY + BHeight
			// Player Respond to Tile, put player on top of block
			if (TopCollision(HitBox))
			{
				player->setPlayerPositionY(LevelMap->getTilePositionY(Row, Col) - player->getPlayerHeight());
				Game::IsPlayerOnAir = false;	
				Game::isFallFromSky = false;
				Game::isPlayerOnTopOfBlock = true;
			}
						
		}
		Game::isPlayerOnTopOfBlock = false;
	}

	void CollisionCheck_PlayerAndTile_PlayerToLeft(CollisionBox& HitBox, Player* player, Level* LevelMap, int Row, int Col)
	{		
		if (mCollisionBox.x < HitBox.getPosX() + HitBox.getWidth() &&
			mCollisionBox.x + mCollisionBox.w > HitBox.getPosX() &&
			mCollisionBox.y < HitBox.getPosY() + HitBox.getHeight() &&
			mCollisionBox.y + mCollisionBox.h > HitBox.getPosY())
		{
			// Collided to the left of the tile
			// LEFT: posAX < posBX + BWidth
			// Player collided with TileLeft, put player on left of Tile
			if(Game::isPlayerOnTopOfBlock == false && LeftCollision(HitBox))
			{
				player->setPlayerPositionX(LevelMap->getTilePositionX(Row, Col) - LevelMap->getTileWidth(Row,Col));
			}
		}

	}

	void CollisionCheck_PlayerAndTile_PlayerToRight(CollisionBox& HitBox, Player* player, Level* LevelMap, int Row, int Col)
	{
		if (mCollisionBox.x < HitBox.getPosX() + HitBox.getWidth() &&
			mCollisionBox.x + mCollisionBox.w > HitBox.getPosX() &&
			mCollisionBox.y < HitBox.getPosY() + HitBox.getHeight() &&
			mCollisionBox.y + mCollisionBox.h > HitBox.getPosY())
		{
			// Collided to the right of the tile
			// RIGHT: posAX + AWidth > posBX
			// Player collided with TileRight, put player on right of Tile
			if (RightCollision(HitBox))
			{
				player->setPlayerPositionX(LevelMap->getTilePositionX(Row, Col) + HitBox.getWidth());
			}
		}
	}
	// Player collision with the tile in the front, player cannot move to the new posiiton. 
	void CollisionCheck_PlayerAndTile_PlayerToLeftStop(CollisionBox& HitBox, Player* player, Level* LevelMap, int Row, int Col)
	{	
		Game::player_Old_PositionX = player->getPlayerPositionX();
		// Collided to the left of the tile
		// LEFT: posAX < posBX + BWidth
		// player cannot move to the new posiiton. 
		if (mCollisionBox.x < HitBox.getPosX() + HitBox.getWidth() &&
			mCollisionBox.x + mCollisionBox.w > HitBox.getPosX() &&
			mCollisionBox.y < HitBox.getPosY() + HitBox.getHeight() &&
			mCollisionBox.y + mCollisionBox.h > HitBox.getPosY())
		{
			if (LeftCollision(HitBox))
			{
				player->setPlayerPositionX(Game::player_Old_PositionX);								
			}			
		}			
	}
	
	void CollisionCheck_PlayerAndTile_HitBlockForItem(CollisionBox& HitBox, Player* player, Object* object, Level* LevelMap, Mix_Chunk *BumpSound, int Row, int Col)
	{
		if (mCollisionBox.x < HitBox.getPosX() + HitBox.getWidth() &&
			mCollisionBox.x + mCollisionBox.w > HitBox.getPosX() &&
			mCollisionBox.y < HitBox.getPosY() + HitBox.getHeight() &&
			mCollisionBox.y + mCollisionBox.h > HitBox.getPosY())
		{		
			if (BottomCollision(HitBox))
			{				
				LevelMap->setTileTextureToBlockHit(Row, Col);
				Mix_PlayChannel(-1, BumpSound, 0);

				if(object->getIsObjectAppears() == false){
					object->setPositionX(LevelMap->getTilePositionX(Row-1, Col));
					object->setPositionY(LevelMap->getTilePositionY(Row-1, Col));
					object->setIsObjectAppears(true);
				}	
				HitBox.setIsCollisionBoxHit(true);
			}
		}
	}

	void CollisionCheck_PlayerAndObject_PlayerToTop(CollisionBox& HitBox, Player* player, Object* object)
	{
		// Player collided with ObjectTop, put player on top of Object
		if (mCollisionBox.x < HitBox.getPosX() + HitBox.getWidth() &&
			mCollisionBox.x + mCollisionBox.w > HitBox.getPosX() &&
			mCollisionBox.y < HitBox.getPosY() + HitBox.getHeight() &&
			mCollisionBox.y + mCollisionBox.h > HitBox.getPosY())
		{
			if (TopCollision(HitBox))
			{
				player->setPlayerPositionY(object->getPosY() - mCollisionBox.h);
				Game::IsPlayerOnAir = false;
				Game::isFallFromSky = false;	
			}
		}		
	}

	void CollisionCheck_PlayerAndObject_CollectItem(CollisionBox& HitBox, Player* player, Object* object, Mix_Chunk *CollectSound)
	{		
		if (mCollisionBox.x < HitBox.getPosX() + HitBox.getWidth() &&
			mCollisionBox.x + mCollisionBox.w > HitBox.getPosX() &&
			mCollisionBox.y < HitBox.getPosY() + HitBox.getHeight() &&
			mCollisionBox.y + mCollisionBox.h > HitBox.getPosY())
		{
			if (BottomCollision(HitBox))
			{
				if (!object->getIsObjectCollected()) {
					Mix_PlayChannel(-1, CollectSound, 0);
					object->setIsObjectCollected(true);
					Game::tmpTotalScore++;					
				}
			}
		}
	}

	void CollisionCheck_PlayerAndTile_NoJump(CollisionBox& HitBox, Level* LevelMap, int Row, int Col)
	{
		// Player has no ability to jump
		if (mCollisionBox.x < HitBox.getPosX() + HitBox.getWidth() &&
			mCollisionBox.x + mCollisionBox.w > HitBox.getPosX() &&
			(mCollisionBox.y + mCollisionBox.h) < HitBox.getPosY() + HitBox.getHeight() &&
			(mCollisionBox.y + mCollisionBox.h) + mCollisionBox.h > HitBox.getPosY())
		{
			Game::isJumpKeyPressed = true;
			printf("No Jump\n");
		}		

	}

	// player collision with the above tile, block jump because of obsticle above.
	void CollisionCheck_PlayerAndTile_JumpRestriction(CollisionBox &HitBox, Level* LevelMap, int Row, int Col)
	{
		if (mCollisionBox.x < HitBox.getPosX() + HitBox.getWidth() &&
			mCollisionBox.x + mCollisionBox.w > HitBox.getPosX() &&
			mCollisionBox.y < HitBox.getPosY() + HitBox.getHeight() &&
			mCollisionBox.y + mCollisionBox.h > HitBox.getPosY())
		{
			Game::is_Collided_With_Object = true;			
		}
	}

	// Release jumping restriction, allow player to perform normal jump.
	void CollisionCheck_PlayerAndTile_ReleaseJumpRestriction(CollisionBox &HitBox, Level* LevelMap, int Row, int Col)
	{
		if (mCollisionBox.x < HitBox.getPosX() + HitBox.getWidth() &&
			mCollisionBox.x + mCollisionBox.w > HitBox.getPosX() &&
			mCollisionBox.y < HitBox.getPosY() + HitBox.getHeight() &&
			mCollisionBox.y + mCollisionBox.h > HitBox.getPosY())
		{
			Game::is_Collided_With_Object = false;
		}
	}

	void CollisionCheck_PlayerAndTile_StopFreeFall(CollisionBox& HitBox, Level* LevelMap, int Row, int Col)
	{
		if (mCollisionBox.x < HitBox.getPosX() + HitBox.getWidth() &&
			mCollisionBox.x + mCollisionBox.w > HitBox.getPosX() &&
			mCollisionBox.y < HitBox.getPosY() + HitBox.getHeight() &&
			mCollisionBox.y + mCollisionBox.h > HitBox.getPosY())
		{
			// making sure player did not free fall from the next tile.
			Game::isFallFromSky = false;				
		}
	}

	void CollisionCheck_PlayerAndTile_FallingDeath(CollisionBox& HitBox, Player *player, Level* LevelMap, int Row, int Col)
	{
		if (mCollisionBox.x < HitBox.getPosX() + HitBox.getWidth() &&
			mCollisionBox.x + mCollisionBox.w > HitBox.getPosX() &&
			mCollisionBox.y < HitBox.getPosY() + HitBox.getHeight() &&
			mCollisionBox.y + mCollisionBox.h > HitBox.getPosY())
		{
			// Player fall down from the sky
			Game::isFallFromSky = true;		
			player->Falling(Game::fGravityDeathFalling);			
		}
	}

	void CollisionCheck_ObjectAndTile_ObjectToRight(CollisionBox& HitBox, Object* object, Level* LevelMap, int Row, int Col)
	{
		if (mCollisionBox.x < HitBox.getPosX() + HitBox.getWidth() &&
			mCollisionBox.x + mCollisionBox.w > HitBox.getPosX() &&
			mCollisionBox.y < HitBox.getPosY() + HitBox.getHeight() &&
			mCollisionBox.y + mCollisionBox.h > HitBox.getPosY())
		{
			// Collided to the right of the tile
			// RIGHT: posAX + AWidth > posBX
			// Player collided with TileRight, put player on right of Tile
			if (RightCollision(HitBox))
			{
				object->setPositionX(LevelMap->getTilePositionX(Row, Col) + LevelMap->getTileWidth(Row, Col));
				Game::is_movingFloorMoveToLeft = false; // stop moving floor from moving to the left
			}
		}
	}

	void CollisionCheck_ObjectAndTile_ObjectToLeft(CollisionBox& HitBox, Object* object, Level* LevelMap, int Row, int Col)
	{
		// Collided to the left of the tile
		// LEFT: posAX < posBX + BWidth
		// Player collided with TileLeft, put player on left of Tile
		if (mCollisionBox.x < HitBox.getPosX() + HitBox.getWidth() &&
			mCollisionBox.x + mCollisionBox.w > HitBox.getPosX() &&
			mCollisionBox.y < HitBox.getPosY() + HitBox.getHeight() &&
			mCollisionBox.y + mCollisionBox.h > HitBox.getPosY())
		{
			if (LeftCollision(HitBox))
			{
				object->setPositionX(LevelMap->getTilePositionX(Row, Col) - object->getWidth());
				Game::is_movingFloorMoveToLeft = true; // start moving floor to the left
			}
		}
	}

	void CollisionCheck_EnemyAndTile_EnemyToLeft(CollisionBox& HitBox, Enemy* enemy, Level* LevelMap, int Row, int Col)
	{
		// Collided to the left of the tile
		// LEFT: posAX < posBX + BWidth
		// Enemy collided with TileLeft, put enemy on left of Tile
		if (mCollisionBox.x < HitBox.getPosX() + HitBox.getWidth() &&
			mCollisionBox.x + mCollisionBox.w > HitBox.getPosX() &&
			mCollisionBox.y < HitBox.getPosY() + HitBox.getHeight() &&
			mCollisionBox.y + mCollisionBox.h > HitBox.getPosY())
		{
			if (LeftCollision(HitBox))
			{
				enemy->setEnemyPositionX(LevelMap->getTilePositionX(Row, Col) - enemy->getEnemyWidth());
			}
		}
	}

	void CollisionCheck_EnemyAndTile_EnemyToRight(CollisionBox& HitBox, Enemy* enemy, Level* LevelMap, int Row, int Col)
	{
		// Collided to the right of the tile
		// RIGHT: posAX + AWidth > posBX
		// Enemy collided with TileRight, put enemy on right of Tile
		if (mCollisionBox.x < HitBox.getPosX() + HitBox.getWidth() &&
			mCollisionBox.x + mCollisionBox.w > HitBox.getPosX() &&
			mCollisionBox.y < HitBox.getPosY() + HitBox.getHeight() &&
			mCollisionBox.y + mCollisionBox.h > HitBox.getPosY())
		{
			if (RightCollision(HitBox))
			{
				enemy->setEnemyPositionX(LevelMap->getTilePositionX(Row, Col) + LevelMap->getTileWidth(Row, Col));
			}
		}
	}
	inline void setPosX(int XPosition)
	{
		mCollisionBox.x = XPosition;
	}	
		
	inline void setPosY(int YPosition)
	{
		mCollisionBox.y = YPosition;
	}

	inline void setWidth(int Width)
	{
		mCollisionBox.w = Width;
	}
	
	inline void setHeight(int Height)
	{
		mCollisionBox.h = Height;
	}
	
	inline int getPosX() const
	{
		return mCollisionBox.x;
	}

	inline int getPosY() const
	{
		return mCollisionBox.y;
	}

	inline int getWidth() const
	{
		return mCollisionBox.w;
	}

	inline int getHeight() const
	{
		return mCollisionBox.h;
	}

	inline bool getIsCollisionBoxHit() const
	{
		return mIsCollisionBoxHit;
	}

	inline void setIsCollisionBoxHit(bool isHit)
	{
		mIsCollisionBoxHit = isHit;
	}

private:
	SDL_Rect mCollisionBox;
	bool mIsCollisionBoxHit = false;
	
};
#endif 