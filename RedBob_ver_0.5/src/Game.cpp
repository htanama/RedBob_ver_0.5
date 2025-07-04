#include "Game.h"
#include "Player.h"
#include "Enemy.h"
#include "Level.h"
#include "Object.h"
//#include "DebugGUI.h"
#include "Keyboard.h"
#include <SDL_ttf.h>
#include "CollisionBox.h"
#include "ScoreSystem.h"

// static SDL_Renderer *GameRenderer; 
SDL_Renderer* Game::GameRenderer = NULL; 
double Game::deltaTime;
int Game::windowWidth;
int Game::windowHeight;
int Game::mapWidth;
int Game::mapHeight;
int Game::player_Old_PositionX;
int Game::tmpTotalScore = 0;
float Game::fGravityDeathFalling = 50.0f; 
bool Game::playerFinishedMap1 = false;
bool Game::playerFinishedMap2 = false;
bool Game::playerFinishedMap3 = false;
bool Game::playerFinishedMap4 = false;
bool Game::isJumpKeyPressed = false;
bool Game::IsPlayerOnAir = false;
bool Game::isFallFromSky = false;
bool Game::is_Collided_With_Object = false;
bool Game::isSprintPressed = false;
bool Game::isLeftKeyPressed = false;
bool Game::isRightKeyPressed = false;
bool Game::isPlayerIdle = false;
bool Game::is_movingFloorMoveToLeft = true;
bool Game::isPlayerOnTopOfBlock = false;
bool Game::IsGameOver = false;

// Tile triple scale (48pixel) and Payer original scale (16pixel)
//const int  Game::GROUND_POSITION = 340; // 340

// Full Screen Scacle 5X 
const int  Game::GROUND_POSITION = 565; // 560

Keyboard KeyboardController;
Player* player = nullptr;
Level* LevelMap = nullptr;
Enemy* Spider1 = nullptr;
Object* movingFloor = nullptr, *coin17 = nullptr, * coin44 = nullptr, * coin47 = nullptr, * coin42 = nullptr, * coin410 = nullptr, * coin515 = nullptr;

Object *mushroom = nullptr, *greenMushroom = nullptr, *redMushroom = nullptr, *mushroomGreen = nullptr;
Object* star = nullptr;
TTF_Font* font = NULL;
SDL_Surface* text = NULL;
ScoreSystem* playerScore = nullptr;
//DebugGUI myDebugGUI; // ImGUI Debug

CollisionBox PlayerCollisionBox, MovingFloorCollisionBox, SpiderCollisionBox, Coin17_CollisionBox, Coin44_CollisionBox, Coin47_CollisionBox, Coin42_CollisionBox, Coin410_CollisionBox, Coin515_CollisionBox, MushroomCollisionBox, GreenMushroomCollisionBox, RedMushroomCollisionBox, GreenMushroomCollisionBox2;
CollisionBox TileCollisionBox27, TileCollisionBox27Bottom, TileCollisionBox39, TileCollisionBox39Left, TileCollisionBox39Bottom, TileCollisionBox46, TileCollisionBox47, TileCollisionBox48, TileCollisionBox48Left, TileCollisionBox49, TileCollisionBox54, TileCollisionBox54Bottom, TileCollisionBox55, TileCollisionBox55Bottom, TileCollisionBox57, TileCollisionBox57Bottom, TileCollisionBox57Left, TileCollisionBox59, TileCollisionBox64, TileCollisionBox65, TileCollisionBox66, TileCollisionBox66Left, TileCollisionBox68,TileCollisionBox69, TileCollisionBox70, TileCollisionBox73, TileCollisionBox74, TileCollisionBox75, TileCollisionBox76, TileCollisionBox77, TileCollisionBox78, TileCollisionBox79, TileCollisionBox84,
TileCollisionBox86, TileCollisionBox87, TileCollisionBox88,TileCollisionBox89, TileCollisionBox210, TileCollisionBox210Left, TileCollisionBox211, TileCollisionBox216, TileCollisionBox216Bottom, TileCollisionBox311, TileCollisionBox411, TileCollisionBox414, TileCollisionBox416, TileCollisionBox417, TileCollisionBox418, TileCollisionBox511, TileCollisionBox510, TileCollisionBox510Bottom, TileCollisionBox515, TileCollisionBox516, TileCollisionBox516Bottom, TileCollisionBox517, TileCollisionBox611, TileCollisionBox615, TileCollisionBox616,TileCollisionBox617, TileCollisionBox710, TileCollisionBox711, TileCollisionBox714, TileCollisionBox714_B, TileCollisionBox715, TileCollisionBox716, TileCollisionBox717, TileCollisionBox717Left, TileCollisionBox717Right, TileCollisionBox718, TileCollisionBox724, TileCollisionBox810,TileCollisionBox811, TileCollisionBox812, TileCollisionBox813, TileCollisionBox814;

Game::Game() {mGameIsRunning = true;}
Game::~Game() {
	if (player != nullptr) { delete player; }
	SDL_DestroyRenderer(GameRenderer);
	SDL_DestroyWindow(mGameWindow);
}

void Game::init()
{
	//Game::windowWidth = SCREEN_WIDTH;
	//Game::windowHeight = SCREEN_HEIGHT;

    if( SDL_Init (SDL_INIT_EVERYTHING) != 0){
	    printf( "SDL cannot initialize: %s\n", IMG_GetError() );
		return;
	}
	
	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);
	windowWidth = display_mode.w;
	windowHeight = display_mode.h;
	
   	mGameWindow = SDL_CreateWindow("Game Menu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_BORDERLESS);
	//SDL_SetWindowResizable(mGameWindow,SDL_FALSE);
    if (!mGameWindow)
	{
		printf( "SDL_Window could not initialize! mGameWindow Error: %s\n", SDL_GetError() );
		return;
	}

    GameRenderer = SDL_CreateRenderer(mGameWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!GameRenderer)
	{
		printf( "SDL_Renderer could not initialize! GameRenderer Error: %s\n", SDL_GetError() );
		return;
	}
	SDL_SetWindowFullscreen(mGameWindow, SDL_WINDOW_FULLSCREEN);

	Mix_OpenAudio(48000, AUDIO_S16SYS, 2, 2048);
	Game_Music = Mix_LoadMUS("./assets/GroundTheme.mp3");	
	mGameIsRunning = true;
	Mix_PlayMusic(Game_Music, 0);
	
	JumpSound = Mix_LoadWAV("./assets/smb_jump-small.wav");
	if (JumpSound == NULL) {
		printf("\033[1;31m Unable to load ogg file: %s\n", Mix_GetError());
		printf("\x1B[0m"); // reset back to normal - all atributes off
	}
	ObjectAppearSound = Mix_LoadWAV("./assets/smb_powerup_appears.wav");
	if (ObjectAppearSound == NULL) {
		printf("\033[1;31m Unable to load wav file: %s\n", Mix_GetError());
		printf("\x1B[0m"); // reset back to normal - all atributes off
	}
	coinCollectSound = Mix_LoadWAV("./assets/smb_coin.wav");
	if (coinCollectSound == NULL) {
		printf("\033[1;31m Unable to load wav file: %s\n", Mix_GetError());
		printf("\x1B[0m"); // reset back to normal - all atributes off
	}
	BumpSound = Mix_LoadWAV("./assets/smb_bump.wav");
	if (BumpSound == NULL) {
		printf("\033[1;31m Unable to load wav file: %s\n\x1B[0m", Mix_GetError());
	}
	objectCollectedSound = Mix_LoadWAV("./assets/smb_powerup.wav");
	if (objectCollectedSound == NULL) {
		printf("\033[1;31m Unable to load wav file: %s\n\x1B[0m", Mix_GetError());
	}
	playerDiesSound = Mix_LoadWAV("./assets/smb_mariodie.wav");
	if (playerDiesSound == NULL) {
		printf("\033[1;31m Unable to load wav file: %s\n\x1B[0m", Mix_GetError());
	}
	if (TTF_Init() < 0)
	{
		printf("FAILED to initialized TTF_Init: %s\n", TTF_GetError());
		return;
	}	
	/*/ ImGUI
	myDebugGUI.Init();
	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForSDLRenderer(mGameWindow, Game::GameRenderer);
	ImGui_ImplSDLRenderer2_Init(Game::GameRenderer);
	*/
	mBackground = TextureManager::LoadTexture("./assets/background.png");    
}

void Game::ProcessInput() {	

    while (SDL_PollEvent(&sdlEvent)) {
        switch (sdlEvent.type) {
            case SDL_QUIT:
                mGameIsRunning = false;
                break;
            case SDL_KEYDOWN:
                if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
                    mGameIsRunning = false;
                }
				if (sdlEvent.key.keysym.sym == SDLK_d) {
				//	isDebug = !isDebug; // Toggle Debug on and off. (T/F, T/F,...)
				}
				break;			
		}

		KeyboardController.KeyboardEvent(sdlEvent, JumpSound, player);
		
		/*/ MouseEvent for Debugging Only
		SDL_Point mousePosition;
		SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

		// ImGUI
		ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
		io.MousePos = ImVec2(mousePosition.x, mousePosition.y);
		
		switch (sdlEvent.type)
		{
		case SDL_MOUSEBUTTONDOWN:
			if (sdlEvent.button.button == SDL_BUTTON_RIGHT)
			{
				io.AddMouseButtonEvent(sdlEvent.button.button, true);
				printf("\x1b[1;36mmouse PosX %i, PosY %i \n", mousePosition.x, mousePosition.y);
				printf("\033[0m");
			}
		}break;
		*/
    }
}

void Game::SetupLevel1(){
	LevelMap = new Level(16, "./assets/OverWorld.png", mapWidth, mapHeight);

	player = new Player(glm::vec2(
		LevelMap->getTilePositionX(7,0),
		LevelMap->getTilePositionY(7,0)),
		glm::vec2(5.0, 5.0), 
		"./assets/player-idle.png", 16, 16, 0, 0, 4);

	Spider1 = new Enemy(glm::vec2(
		LevelMap->getTilePositionX(7,14), 
		LevelMap->getTilePositionY(7, 14)), 
		glm::vec2(5.0, 5.0), 
		"./assets/spider-sprites-left.png", 32, 16, 0, 0, 2);
	
	Spider1->setTextureWalkRight("./assets/spider-sprites-right.png", 32, 16, 0, 0);
	star = new Object(glm::vec2(60, 60), glm::vec2(4.0, 4.0), "./assets/Object-Items.png", 16, 16, 3, 0);

	coin17 = new Object(glm::vec2(
		LevelMap->getTilePositionX(1,7), 
		LevelMap->getTilePositionY(1,7)),
		glm::vec2(4.0, 4.0),				// Image Scaling
		"./assets/coins.png", 16, 16, 0, 0, 7, true);

	coin44 = new Object(glm::vec2(
		LevelMap->getTilePositionX(4,4), 
		LevelMap->getTilePositionY(4,4)),
		glm::vec2(4.0, 4.0), 
		"./assets/coins.png", 16, 16, 0, 0, 7, true);

	coin47 = new Object(glm::vec2(
		LevelMap->getTilePositionX(4,7), 
		LevelMap->getTilePositionY(4,7)),
		glm::vec2(4.0, 4.0), 
		"./assets/coins.png", 16, 16, 0, 0, 7, true);

	coin42 = new Object(glm::vec2(
		LevelMap->getTilePositionX(4,2), 
		LevelMap->getTilePositionY(4,2)),
		glm::vec2(4.0, 4.0), 
		"./assets/coins2.png", 16, 16, 0, 0, 4, true);

	coin410 = new Object(glm::vec2(
		LevelMap->getTilePositionX(4,10), 
		LevelMap->getTilePositionY(4,10)),
		glm::vec2(4.0, 4.0), 
		"./assets/coins.png", 16, 16, 0, 0, 7, true);

	coin515 = new Object(glm::vec2(
		LevelMap->getTilePositionX(5,15),
		LevelMap->getTilePositionY(5,15)), 
		glm::vec2(4.0, 4.0), 
		"./assets/coins2.png", 16, 16, 0, 0, 4, true);

	mushroom = new Object(glm::vec2(0, 0), glm::vec2(5.0, 5.0), "./assets/Object-Items.png", 16, 16, 0, 0);
	greenMushroom = new Object(glm::vec2(0, 0), glm::vec2(5.0, 5.0), "./assets/Object-Items.png", 16, 16, 1, 0);
	redMushroom = new Object(glm::vec2(0, 0), glm::vec2(5.0, 5.0), "./assets/Object-Items.png", 16, 16, 0, 0);
	mushroomGreen = new Object(glm::vec2(0, 0), glm::vec2(5.0, 5.0), "./assets/Object-Items.png", 16, 16, 1, 0);
	backgroundRect = { 0,0, Game::windowWidth, Game::windowHeight };

	font = TTF_OpenFont("./assets/ariblk.ttf", 50);
	if (!font) {
		printf("\033[1;31mFAILED to initialized TTF_OpenFont: %s\n", TTF_GetError());
		printf("\033[0m"); 
	}

	playerScore = new ScoreSystem();
}

void Game::SetupLevel2()
{	
	LevelMap = new Level(16, "./assets/OverWorld.png", mapWidth, mapHeight);
	
	player = new Player(glm::vec2(
		LevelMap->getTilePositionX(7, 0),
		LevelMap->getTilePositionY(7, 0)),
		glm::vec2(5.0, 5.0),
		"./assets/player-idle.png", 16, 16, 0, 0, 4);

	Spider1 = new Enemy(glm::vec2(
		LevelMap->getTilePositionX(7,7),
		LevelMap->getTilePositionY(7,7)), 
		glm::vec2(5.0, 5.0), 
		"./assets/spider-sprites-left.png", 32, 16, 0, 0, 3);	
	Spider1->setTextureWalkRight("./assets/spider-sprites-right.png", 32, 16, 0, 0);

	coin17 = new Object(glm::vec2(
		LevelMap->getTilePositionX(1, 7),
		LevelMap->getTilePositionY(1, 7)),
		glm::vec2(4.0, 4.0),				// Image Scaling
		"./assets/coins.png", 16, 16, 0, 0, 7, true);

	coin44 = new Object(glm::vec2(
		LevelMap->getTilePositionX(4, 4),
		LevelMap->getTilePositionY(4, 4)),
		glm::vec2(4.0, 4.0),
		"./assets/coins.png", 16, 16, 0, 0, 7, true);

	coin47 = new Object(glm::vec2(
		LevelMap->getTilePositionX(4, 7),
		LevelMap->getTilePositionY(4, 7)),
		glm::vec2(4.0, 4.0),
		"./assets/coins.png", 16, 16, 0, 0, 7, true);

	coin42 = new Object(glm::vec2(
		LevelMap->getTilePositionX(4, 2),
		LevelMap->getTilePositionY(4, 2)),
		glm::vec2(4.0, 4.0),
		"./assets/coins2.png", 16, 16, 0, 0, 4, true);

	coin410 = new Object(glm::vec2(
		LevelMap->getTilePositionX(4, 10),
		LevelMap->getTilePositionY(4, 10)),
		glm::vec2(4.0, 4.0),
		"./assets/coins.png", 16, 16, 0, 0, 7, true);

	coin515 = new Object(glm::vec2(
		LevelMap->getTilePositionX(4, 15),
		LevelMap->getTilePositionY(4, 15)),
		glm::vec2(4.0, 4.0),
		"./assets/coins2.png", 16, 16, 0, 0, 4, true);

	mushroom = new Object(glm::vec2(0, 0), glm::vec2(5.0, 5.0), "./assets/Object-Items.png", 16, 16, 0, 0);
	greenMushroom = new Object(glm::vec2(0, 0), glm::vec2(5.0, 5.0), "./assets/Object-Items.png", 16, 16, 1, 0);
	redMushroom = new Object(glm::vec2(0, 0), glm::vec2(5.0, 5.0), "./assets/Object-Items.png", 16, 16, 0, 0);

	//movingFloor at tile dstRect[][9]
	movingFloor = new Object(glm::vec2(
		LevelMap->getTilePositionX(7,9),
		LevelMap->getTilePositionY(7,9)), 
		glm::vec2(4.0, 4.0), 
		"./assets/Platform.png", 48, 16, 0, 0);

	movingFloor->setVelocityX(50.0);

	star = new Object(glm::vec2(60, 60), glm::vec2(4.0, 4.0), "./assets/items.png", 16, 16, 48, 0);
	backgroundRect = { 0,0, Game::windowWidth, Game::windowHeight };

	font = TTF_OpenFont("./assets/ariblk.ttf", 50);
	if (!font) {
		printf("\033[1;31mFAILED to initialized TTF_OpenFont: %s\n", TTF_GetError());
		printf("\033[0m");
	}
}

void Game::SetupLevel3()
{
	LevelMap = new Level(16, "./assets/OverWorld.png", mapWidth, mapHeight);

	player = new Player(glm::vec2(
		LevelMap->getTilePositionX(7, 0),
		LevelMap->getTilePositionY(7, 0)),
		glm::vec2(5.0, 5.0),
		"./assets/player-idle.png", 16, 16, 0, 0, 4);

	Spider1 = new Enemy(glm::vec2(
		LevelMap->getTilePositionX(7, 19),
		LevelMap->getTilePositionY(7, 19)),
		glm::vec2(5.0, 5.0),
		"./assets/spider-sprites-left.png", 32, 16, 0, 0, 3);
	Spider1->setTextureWalkRight("./assets/spider-sprites-right.png", 32, 16, 0, 0);

	coin17 = new Object(glm::vec2(
		LevelMap->getTilePositionX(1, 7),
		LevelMap->getTilePositionY(1, 7)),
		glm::vec2(4.0, 4.0),				// Image Scaling
		"./assets/coins.png", 16, 16, 0, 0, 7, true);

	coin44 = new Object(glm::vec2(
		LevelMap->getTilePositionX(4, 4),
		LevelMap->getTilePositionY(4, 4)),
		glm::vec2(4.0, 4.0),
		"./assets/coins.png", 16, 16, 0, 0, 7, true);

	coin47 = new Object(glm::vec2(
		LevelMap->getTilePositionX(4, 7),
		LevelMap->getTilePositionY(4, 7)),
		glm::vec2(4.0, 4.0),
		"./assets/coins.png", 16, 16, 0, 0, 7, true);

	coin42 = new Object(glm::vec2(
		LevelMap->getTilePositionX(4, 2),
		LevelMap->getTilePositionY(4, 2)),
		glm::vec2(4.0, 4.0),
		"./assets/coins2.png", 16, 16, 0, 0, 4, true);

	coin410 = new Object(glm::vec2(
		LevelMap->getTilePositionX(4, 10),
		LevelMap->getTilePositionY(4, 10)),
		glm::vec2(4.0, 4.0),
		"./assets/coins.png", 16, 16, 0, 0, 7, true);

	coin515 = new Object(glm::vec2(
		LevelMap->getTilePositionX(4, 15),
		LevelMap->getTilePositionY(4, 15)),
		glm::vec2(4.0, 4.0),
		"./assets/coins2.png", 16, 16, 0, 0, 4, true);

	redMushroom = new Object(glm::vec2(0, 0), glm::vec2(5.0, 5.0), "./assets/Object-Items.png", 16, 16, 0, 0);
	mushroomGreen = new Object(glm::vec2(0, 0), glm::vec2(5.0, 5.0), "./assets/Object-Items.png", 16, 16, 1, 0);
	
	//movingFloor at tile dstRect[4][12]
	movingFloor = new Object(glm::vec2(
		LevelMap->getTilePositionX(4, 12),
		LevelMap->getTilePositionY(4, 12)),
		glm::vec2(4.0, 4.0),
		"./assets/Platform.png", 48, 16, 0, 0);
	movingFloor->setVelocityX(50.0);

	star = new Object(glm::vec2(200.0, 10.0), glm::vec2(4.0, 4.0), "./assets/items.png", 16, 16, 48, 0);

	backgroundRect = { 0,0, Game::windowWidth, Game::windowHeight };	

	font = TTF_OpenFont("./assets/ariblk.ttf", 50);
	if (!font) {
		printf("\033[1;31mFAILED to initialized TTF_OpenFont: %s\n\033[0m", TTF_GetError());
	}
}

void Game::SetupLevel4()
{
	LevelMap = new Level(16, "./assets/OverWorld.png", mapWidth, mapHeight);

	player = new Player(glm::vec2(
		LevelMap->getTilePositionX(7, 0),
		LevelMap->getTilePositionY(7, 0)),
		glm::vec2(5.0, 5.0),
		"./assets/player-idle.png", 16, 16, 0, 0, 4);

	Spider1 = new Enemy(glm::vec2(
		LevelMap->getTilePositionX(7, 18),
		LevelMap->getTilePositionY(7, 18)),
		glm::vec2(5.0, 5.0),
		"./assets/spider-sprites-left.png", 32, 16, 0, 0, 3);
	Spider1->setTextureWalkRight("./assets/spider-sprites-right.png", 32, 16, 0, 0);

	star = new Object(glm::vec2(200.0, 10.0), glm::vec2(4.0, 4.0), "./assets/items.png", 16, 16, 48, 0);

	backgroundRect = { 0,0, Game::windowWidth, Game::windowHeight };
}

void Game::Run()
{
	SetupLevel1();

	while(mGameIsRunning)
	{
		ProcessInput();	
		Update();
		Render();
	}
}

bool Game::CheckAABB_Collision(int posAX, int posAY, int AWidth, int AHeight, int posBX, int posBY, int BWidth, int BHeight)
{
	return (
		posAX < posBX + BWidth &&
		posAX + AWidth > posBX &&
		posAY < posBY + BHeight &&
		posAY + AHeight > posBY
		);
}

bool Game::CheckTop_Collision(int posAY, int posBY, int BHeight)
{
	return (posAY < posBY + BHeight);
}

bool Game::CheckBottom_Collision(int posAY, int AHeight, int posBY)
{
	return (posAY + AHeight > posBY);		
}

bool Game::CheckRight_Collision(int posAX, int AWidth, int posBX)
{
	return (posAX + AWidth > posBX);
}

bool Game::CheckLeft_Collision(int posAX, int posBX, int BWidth)
{
	return (posAX < posBX + BWidth);
}

void Game::Setup_LevelOneCollision()
{
	PlayerCollisionBox.Init_CollisionBox_With_Player(player);

	SpiderCollisionBox.Init_CollisionBox_With_Enemy(Spider1);
	SpiderCollisionBox.setWidth(SpiderCollisionBox.getWidth() / 1.4);
	SpiderCollisionBox.setHeight(SpiderCollisionBox.getHeight() / 1.4);
	SpiderCollisionBox.setPosX(SpiderCollisionBox.getPosX() + 17);
	SpiderCollisionBox.setPosY(SpiderCollisionBox.getPosY() + 25);

	MushroomCollisionBox.Init_CollisionBox_With_Object(mushroom);
	GreenMushroomCollisionBox.Init_CollisionBox_With_Object(greenMushroom);
	RedMushroomCollisionBox.Init_CollisionBox_With_Object(redMushroom);
	GreenMushroomCollisionBox2.Init_CollisionBox_With_Object(mushroomGreen);
	
	TileCollisionBox27.Init_CollisionBox_With_LevelMap(LevelMap, 2, 7); // block location

	TileCollisionBox27Bottom.Init_CollisionBox_With_LevelMap_And_OffsetX_And_OffsetY(LevelMap, 2, 7, 12, 40);
	TileCollisionBox27Bottom.setHeight(TileCollisionBox27Bottom.getHeight() / 1.5);
	TileCollisionBox27Bottom.setWidth(TileCollisionBox27Bottom.getWidth() / 1.5);

	TileCollisionBox46.Init_CollisionBox_With_LevelMap_And_OffsetX(LevelMap, 4, 6, -45); // reset jump	
	TileCollisionBox47.Init_CollisionBox_With_LevelMap(LevelMap, 4, 7); // restrict jump
	TileCollisionBox48.Init_CollisionBox_With_LevelMap_And_OffsetX(LevelMap, 4, 8, 45); // reset jump
	TileCollisionBox54.Init_CollisionBox_With_LevelMap(LevelMap, 5, 4); // block location. 	

	TileCollisionBox54Bottom.Init_CollisionBox_With_LevelMap_And_OffsetX_And_OffsetY(LevelMap, 5, 4, 12, 40);
	TileCollisionBox54Bottom.setHeight(TileCollisionBox54Bottom.getHeight() / 1.5);
	TileCollisionBox54Bottom.setWidth(TileCollisionBox54Bottom.getWidth() / 1.5);

	TileCollisionBox57.Init_CollisionBox_With_LevelMap(LevelMap, 5, 7);
	TileCollisionBox57Bottom.Init_CollisionBox_With_LevelMap_And_OffsetX_And_OffsetY(LevelMap, 5, 7, 12, 40);
	TileCollisionBox57Bottom.setHeight(TileCollisionBox57Bottom.getHeight() / 1.5);
	TileCollisionBox57Bottom.setWidth(TileCollisionBox57Bottom.getWidth() / 1.5);

	TileCollisionBox73.Init_CollisionBox_With_LevelMap_And_OffsetX(LevelMap, 7, 3, -10); // reset jump	
	TileCollisionBox74.Init_CollisionBox_With_LevelMap(LevelMap, 7, 4); // restrict jump		
	TileCollisionBox75.Init_CollisionBox_With_LevelMap_And_OffsetX(LevelMap, 7, 5, 40); // reset jump	
	TileCollisionBox77.Init_CollisionBox_With_LevelMap(LevelMap, 7, 7); // restrict jump
	TileCollisionBox78.Init_CollisionBox_With_LevelMap_And_OffsetX(LevelMap, 7, 8, 40); // reset jump	
	
	TileCollisionBox510.Init_CollisionBox_With_LevelMap(LevelMap, 5, 10); // block location.
	TileCollisionBox510Bottom.Init_CollisionBox_With_LevelMap_And_OffsetX_And_OffsetY(LevelMap, 5, 10, 12, 40);
	TileCollisionBox510Bottom.setHeight(TileCollisionBox510Bottom.getHeight() / 1.5);
	TileCollisionBox510Bottom.setWidth(TileCollisionBox510Bottom.getWidth() / 1.5);

	TileCollisionBox615.Init_CollisionBox_With_LevelMap(LevelMap, 6, 15); 
	TileCollisionBox616.Init_CollisionBox_With_LevelMap(LevelMap, 6, 16);	
	TileCollisionBox710.Init_CollisionBox_With_LevelMap(LevelMap, 7, 10); // restrict jump
	TileCollisionBox711.Init_CollisionBox_With_LevelMap_And_OffsetX(LevelMap, 7, 11, 20); // reset jump
	TileCollisionBox715.Init_CollisionBox_With_LevelMap(LevelMap, 7, 15);
	TileCollisionBox716.Init_CollisionBox_With_LevelMap(LevelMap, 7, 16);

	if (coin17 != nullptr) {
		Coin17_CollisionBox.Init_CollisionBox_With_Object(coin17);
	}
	if (coin44 != nullptr) {
		Coin44_CollisionBox.Init_CollisionBox_With_Object(coin44);
	}

	if (coin47 != nullptr) {
		Coin47_CollisionBox.Init_CollisionBox_With_Object(coin47);
	}
	if (coin42 != nullptr) {
		Coin42_CollisionBox.Init_CollisionBox_With_Object(coin42);
	}
	if (coin410 != nullptr) {
		Coin410_CollisionBox.Init_CollisionBox_With_Object(coin410);
	}
	if (coin515 != nullptr) {
		Coin515_CollisionBox.Init_CollisionBox_With_Object(coin515);
	}
}

void Game::LevelOneCollision()
{
	Setup_LevelOneCollision();
	PlayerCollisionBox.CollisionCheck_PlayerAndEnemy_FallingDeath(SpiderCollisionBox, player, Spider1, playerDiesSound);
	//player collision with block at destRect[2][7]	
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerOnTop(TileCollisionBox27, player, LevelMap, 2, 7);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_HitBlockForItem(TileCollisionBox27Bottom, player, greenMushroom, LevelMap, BumpSound, ObjectAppearSound, 2, 7);
	PlayerCollisionBox.CollisionCheck_PlayerAndObject_CollectSpecialItem(GreenMushroomCollisionBox, player, greenMushroom, objectCollectedSound);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_ReleaseJumpRestriction(TileCollisionBox46, LevelMap, 4, 6);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_JumpRestriction(TileCollisionBox47,LevelMap, 4, 7);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_ReleaseJumpRestriction(TileCollisionBox48, LevelMap, 4, 8);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerOnTop(TileCollisionBox54, player, LevelMap, 5, 4);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_HitBlockForItem(TileCollisionBox54Bottom, player, mushroom, LevelMap, BumpSound, ObjectAppearSound, 5, 4);
	PlayerCollisionBox.CollisionCheck_PlayerAndObject_CollectSpecialItem(MushroomCollisionBox, player, mushroom, objectCollectedSound);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerOnTop(TileCollisionBox57, player, LevelMap, 5, 7);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_HitBlockForItem(TileCollisionBox57Bottom, player, redMushroom, LevelMap, BumpSound, ObjectAppearSound, 5, 7);
	PlayerCollisionBox.CollisionCheck_PlayerAndObject_CollectSpecialItem(RedMushroomCollisionBox, player, redMushroom, objectCollectedSound);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerOnTop(TileCollisionBox510, player, LevelMap, 5, 10);	
	
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_HitBlockForItem(TileCollisionBox510Bottom, player, mushroomGreen, LevelMap, BumpSound, ObjectAppearSound, 5, 10);
	
	PlayerCollisionBox.CollisionCheck_PlayerAndObject_CollectSpecialItem(GreenMushroomCollisionBox2, player, mushroomGreen, objectCollectedSound);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_ReleaseJumpRestriction(TileCollisionBox73, LevelMap, 7, 3);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_JumpRestriction(TileCollisionBox74, LevelMap, 7, 4);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_ReleaseJumpRestriction(TileCollisionBox75, LevelMap, 7, 5);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_JumpRestriction(TileCollisionBox77, LevelMap, 7, 7);	
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_ReleaseJumpRestriction(TileCollisionBox78, LevelMap, 7, 8);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerOnTop(TileCollisionBox615, player, LevelMap, 6, 15);	// Pipe
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerOnTop(TileCollisionBox616, player, LevelMap, 6, 16);	// Pipe
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_JumpRestriction(TileCollisionBox710, LevelMap, 7, 10);	
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_ReleaseJumpRestriction(TileCollisionBox711, LevelMap, 7, 11);		
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerToLeft(TileCollisionBox715, player, LevelMap, 7, 15);
	SpiderCollisionBox.CollisionCheck_EnemyAndTile_EnemyToLeft(TileCollisionBox715, Spider1, LevelMap, 7, 15);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerToRight(TileCollisionBox716, player, LevelMap, 7, 16);	

	if (coin17 != nullptr) {
		PlayerCollisionBox.CollisionCheck_PlayerAndObject_CollectItem(Coin17_CollisionBox, player, coin17, coinCollectSound);
	}
	if (coin44 != nullptr) {
		PlayerCollisionBox.CollisionCheck_PlayerAndObject_CollectItem(Coin44_CollisionBox, player, coin44, coinCollectSound);
	}
	if (coin47 != nullptr) {
		PlayerCollisionBox.CollisionCheck_PlayerAndObject_CollectItem(Coin47_CollisionBox, player, coin47, coinCollectSound);
	}
	if (coin42 != nullptr)
	{
		PlayerCollisionBox.CollisionCheck_PlayerAndObject_CollectItem(Coin42_CollisionBox, player, coin42, coinCollectSound);
	}
	if (coin410 != nullptr) {
		PlayerCollisionBox.CollisionCheck_PlayerAndObject_CollectItem(Coin410_CollisionBox, player, coin410, coinCollectSound);
	}
	if (coin515 != nullptr) {
		PlayerCollisionBox.CollisionCheck_PlayerAndObject_CollectItem(Coin515_CollisionBox, player, coin515, coinCollectSound);
	}
}

void Game::Setup_LevelTwoCollision()
{
	PlayerCollisionBox.Init_CollisionBox_With_Player(player);
	
	SpiderCollisionBox.Init_CollisionBox_With_Enemy(Spider1);
	SpiderCollisionBox.setWidth(SpiderCollisionBox.getWidth() / 1.4);
	SpiderCollisionBox.setHeight(SpiderCollisionBox.getHeight() / 1.4);
	SpiderCollisionBox.setPosX(SpiderCollisionBox.getPosX() + 17);
	SpiderCollisionBox.setPosY(SpiderCollisionBox.getPosY() + 25);

	MovingFloorCollisionBox.Init_CollisionBox_With_Object(movingFloor);
	MushroomCollisionBox.Init_CollisionBox_With_Object(mushroom);
	GreenMushroomCollisionBox.Init_CollisionBox_With_Object(greenMushroom);
	RedMushroomCollisionBox.Init_CollisionBox_With_Object(redMushroom);
	
	if (coin17 != nullptr) {
		Coin17_CollisionBox.Init_CollisionBox_With_Object(coin17);
	}
	if (coin44 != nullptr) {
		Coin44_CollisionBox.Init_CollisionBox_With_Object(coin44);
	}

	if (coin47 != nullptr) {
		Coin47_CollisionBox.Init_CollisionBox_With_Object(coin47);
	}
	if (coin42 != nullptr) {
		Coin42_CollisionBox.Init_CollisionBox_With_Object(coin42);
	}
	if (coin410 != nullptr) {
		Coin410_CollisionBox.Init_CollisionBox_With_Object(coin410);
	}
	if (coin515 != nullptr) {
		Coin515_CollisionBox.Init_CollisionBox_With_Object(coin515);
	}
	
	TileCollisionBox55.Init_CollisionBox_With_LevelMap(LevelMap, 5, 5); // block location	
	TileCollisionBox55Bottom.Init_CollisionBox_With_LevelMap_And_OffsetX_And_OffsetY(LevelMap, 5, 5, 12, 40);
	TileCollisionBox55Bottom.setHeight(TileCollisionBox55Bottom.getHeight() / 1.5);
	TileCollisionBox55Bottom.setWidth(TileCollisionBox55Bottom.getWidth() / 1.5);

	TileCollisionBox74.Init_CollisionBox_With_LevelMap_And_OffsetX(LevelMap, 7, 4, -10); // reset jump
	TileCollisionBox70.Init_CollisionBox_With_LevelMap(LevelMap, 7, 0); // Stop Player from free falling
	TileCollisionBox75.Init_CollisionBox_With_LevelMap(LevelMap, 7, 5); // restrict jump	
	TileCollisionBox77.Init_CollisionBox_With_LevelMap(LevelMap, 7, 7); // stop from falling
	TileCollisionBox78.Init_CollisionBox_With_LevelMap(LevelMap, 7, 8); // Stop Enemy from going
	TileCollisionBox76.Init_CollisionBox_With_LevelMap_And_OffsetX(LevelMap, 7, 6, 25); // reset jump
	TileCollisionBox86.Init_CollisionBox_With_LevelMap(LevelMap, 8, 6); // Stop Player from free falling
	
	TileCollisionBox88.Init_CollisionBox_With_LevelMap_And_OffsetX(LevelMap, 8, 8, 60);//so player does not fall right away.
	TileCollisionBox89.Init_CollisionBox_With_LevelMap(LevelMap, 8, 9);

	TileCollisionBox216.Init_CollisionBox_With_LevelMap(LevelMap, 2, 16); 
	TileCollisionBox216Bottom.Init_CollisionBox_With_LevelMap_And_OffsetX_And_OffsetY(LevelMap, 2, 16, 12, 40);
	TileCollisionBox216Bottom.setHeight(TileCollisionBox216Bottom.getHeight() / 1.5);
	TileCollisionBox216Bottom.setWidth(TileCollisionBox216Bottom.getWidth() / 1.5);
	
	TileCollisionBox414.Init_CollisionBox_With_LevelMap(LevelMap, 4, 14); 	
	TileCollisionBox416.Init_CollisionBox_With_LevelMap(LevelMap, 4, 16);
	TileCollisionBox418.Init_CollisionBox_With_LevelMap(LevelMap, 4, 18);	
	TileCollisionBox515.Init_CollisionBox_With_LevelMap(LevelMap, 5, 15);

	TileCollisionBox516.Init_CollisionBox_With_LevelMap(LevelMap, 5, 16);
	TileCollisionBox516Bottom.Init_CollisionBox_With_LevelMap_And_OffsetX_And_OffsetY(LevelMap, 5, 16, 12, 40);
	TileCollisionBox516Bottom.setHeight(TileCollisionBox516Bottom.getHeight() / 1.5);
	TileCollisionBox516Bottom.setWidth(TileCollisionBox516Bottom.getWidth() / 1.5);

	TileCollisionBox517.Init_CollisionBox_With_LevelMap(LevelMap, 5, 17);
	TileCollisionBox617.Init_CollisionBox_With_LevelMap(LevelMap, 6, 17); // restrict jump
	
	TileCollisionBox714.Init_CollisionBox_With_LevelMap(LevelMap, 7, 14);

	TileCollisionBox714_B.Init_CollisionBox_With_LevelMap_And_OffsetX(LevelMap, 7, 14, -20);// invisible not render. Reset Jump

	TileCollisionBox715.Init_CollisionBox_With_LevelMap(LevelMap, 7, 15);
	TileCollisionBox717.Init_CollisionBox_With_LevelMap(LevelMap, 7, 17);	
	TileCollisionBox718.Init_CollisionBox_With_LevelMap_And_OffsetX(LevelMap, 7, 18, 25); // reset jump
	TileCollisionBox724.Init_CollisionBox_With_LevelMap(LevelMap, 7, 24);// reset jump
	TileCollisionBox810.Init_CollisionBox_With_LevelMap(LevelMap, 8, 10);
	TileCollisionBox811.Init_CollisionBox_With_LevelMap(LevelMap, 8, 11);
	TileCollisionBox812.Init_CollisionBox_With_LevelMap(LevelMap, 8, 12);
	TileCollisionBox813.Init_CollisionBox_With_LevelMap_And_OffsetX(LevelMap, 8, 13, -60);
	TileCollisionBox814.Init_CollisionBox_With_LevelMap(LevelMap, 8, 14);	

}

void Game::LevelTwoCollision()
{
	Setup_LevelTwoCollision();	
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerOnTop(TileCollisionBox55, player, LevelMap, 5, 5);	
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_HitBlockForItem(TileCollisionBox55Bottom, player, mushroom, LevelMap, BumpSound, ObjectAppearSound, 5, 5);
	PlayerCollisionBox.CollisionCheck_PlayerAndObject_CollectSpecialItem(MushroomCollisionBox, player, mushroom, objectCollectedSound);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_ReleaseJumpRestriction(TileCollisionBox70, LevelMap, 7, 0);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_ReleaseJumpRestriction(TileCollisionBox74, LevelMap, 7, 4);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_JumpRestriction(TileCollisionBox75, LevelMap, 7, 5);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_ReleaseJumpRestriction(TileCollisionBox76, LevelMap, 7, 6);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_StopFreeFall(TileCollisionBox77, LevelMap, 7, 7);	

	PlayerCollisionBox.CollisionCheck_PlayerAndObject_PlayerToTop(MovingFloorCollisionBox, player, movingFloor);
	MovingFloorCollisionBox.CollisionCheck_ObjectAndTile_ObjectToRight(TileCollisionBox77, movingFloor, LevelMap, 7, 7);
	MovingFloorCollisionBox.CollisionCheck_ObjectAndTile_ObjectToLeft(TileCollisionBox714, movingFloor, LevelMap, 7, 14);

	SpiderCollisionBox.CollisionCheck_EnemyAndTile_EnemyToLeft(TileCollisionBox78, Spider1, LevelMap, 7, 8);
	PlayerCollisionBox.CollisionCheck_PlayerAndEnemy_FallingDeath(SpiderCollisionBox, player, Spider1, playerDiesSound);

	PlayerCollisionBox.CollisionCheck_PlayerAndTile_StopFreeFall(TileCollisionBox86, LevelMap, 8, 6);		
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_StopFreeFall(TileCollisionBox87, LevelMap, 8, 7);	
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_FallingDeath(TileCollisionBox88, player, LevelMap, playerDiesSound, 8, 8);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_FallingDeath(TileCollisionBox89, player, LevelMap, playerDiesSound, 8, 9);

	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerOnTop(TileCollisionBox216, player, LevelMap, 2, 16);
	
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_HitBlockForItem(TileCollisionBox216Bottom, player, greenMushroom, LevelMap, BumpSound, ObjectAppearSound, 2, 16);	
	PlayerCollisionBox.CollisionCheck_PlayerAndObject_CollectSpecialItem(GreenMushroomCollisionBox, player, greenMushroom, objectCollectedSound);

	PlayerCollisionBox.CollisionCheck_PlayerAndTile_ReleaseJumpRestriction(TileCollisionBox414, LevelMap, 4, 14);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_JumpRestriction(TileCollisionBox416, LevelMap, 4, 16);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_ReleaseJumpRestriction(TileCollisionBox418, LevelMap, 4, 18);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerOnTop(TileCollisionBox515, player, LevelMap, 5, 15);

	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerOnTop(TileCollisionBox516, player, LevelMap, 5, 16);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_HitBlockForItem(TileCollisionBox516Bottom, player, redMushroom, LevelMap, BumpSound, ObjectAppearSound, 5, 16);
	PlayerCollisionBox.CollisionCheck_PlayerAndObject_CollectSpecialItem(RedMushroomCollisionBox, player, redMushroom, objectCollectedSound);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerOnTop(TileCollisionBox517, player, LevelMap, 5, 17);	

	PlayerCollisionBox.CollisionCheck_PlayerAndTile_ReleaseJumpRestriction(TileCollisionBox714_B, LevelMap, 7, 14);// invisible not render
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_JumpRestriction(TileCollisionBox715, LevelMap, 7, 15);	
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_JumpRestriction(TileCollisionBox717, LevelMap, 7, 17);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_ReleaseJumpRestriction(TileCollisionBox718, LevelMap, 7, 18);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_ReleaseJumpRestriction(TileCollisionBox724, LevelMap, 7, 24);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_FallingDeath(TileCollisionBox810, player, LevelMap, playerDiesSound, 8, 10);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_FallingDeath(TileCollisionBox811, player, LevelMap, playerDiesSound, 8, 11);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_FallingDeath(TileCollisionBox812, player, LevelMap, playerDiesSound, 8, 12);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_FallingDeath(TileCollisionBox813, player, LevelMap, playerDiesSound, 8, 13);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_StopFreeFall(TileCollisionBox814, LevelMap, 8, 14);
	

	if (coin17 != nullptr) {
		PlayerCollisionBox.CollisionCheck_PlayerAndObject_CollectItem(Coin17_CollisionBox, player, coin17, coinCollectSound);
	}
	if (coin44 != nullptr) {
		PlayerCollisionBox.CollisionCheck_PlayerAndObject_CollectItem(Coin44_CollisionBox, player, coin44, coinCollectSound);
	}
	if (coin47 != nullptr) {
		PlayerCollisionBox.CollisionCheck_PlayerAndObject_CollectItem(Coin47_CollisionBox, player, coin47, coinCollectSound);
	}
	if (coin42 != nullptr)
	{
		PlayerCollisionBox.CollisionCheck_PlayerAndObject_CollectItem(Coin42_CollisionBox, player, coin42, coinCollectSound);
	}
	if (coin410 != nullptr) {
		PlayerCollisionBox.CollisionCheck_PlayerAndObject_CollectItem(Coin410_CollisionBox, player, coin410, coinCollectSound);
	}
	if (coin515 != nullptr) {
		PlayerCollisionBox.CollisionCheck_PlayerAndObject_CollectItem(Coin515_CollisionBox, player, coin515, coinCollectSound);
	}
}

void Game::Setup_LevelThreeCollision()
{
	PlayerCollisionBox.Init_CollisionBox_With_Player(player);	
	
	SpiderCollisionBox.Init_CollisionBox_With_Enemy(Spider1);
	SpiderCollisionBox.setWidth(SpiderCollisionBox.getWidth() / 1.4);
	SpiderCollisionBox.setHeight(SpiderCollisionBox.getHeight() / 1.4);
	SpiderCollisionBox.setPosX(SpiderCollisionBox.getPosX() + 17);
	SpiderCollisionBox.setPosY(SpiderCollisionBox.getPosY() + 25);

	MovingFloorCollisionBox.Init_CollisionBox_With_Object(movingFloor);
	RedMushroomCollisionBox.Init_CollisionBox_With_Object(redMushroom);
	TileCollisionBox39.Init_CollisionBox_With_LevelMap(LevelMap, 3, 9);
	TileCollisionBox39Bottom.Init_CollisionBox_With_LevelMap_And_OffsetX_And_OffsetY(LevelMap, 3, 9, 12, 35);
	TileCollisionBox39Bottom.setHeight(TileCollisionBox39Bottom.getHeight() / 1.5);
	TileCollisionBox39Bottom.setWidth(TileCollisionBox39Bottom.getWidth() / 1.5);
	TileCollisionBox49.Init_CollisionBox_With_LevelMap(LevelMap, 4, 9);
	TileCollisionBox57.Init_CollisionBox_With_LevelMap(LevelMap, 5, 7);
	TileCollisionBox59.Init_CollisionBox_With_LevelMap(LevelMap, 5, 9);
	TileCollisionBox64.Init_CollisionBox_With_LevelMap(LevelMap, 6, 4);
	TileCollisionBox65.Init_CollisionBox_With_LevelMap(LevelMap, 6, 5);
	TileCollisionBox68.Init_CollisionBox_With_LevelMap(LevelMap, 6, 8);
	TileCollisionBox69.Init_CollisionBox_With_LevelMap(LevelMap, 6, 9);
	TileCollisionBox74.Init_CollisionBox_With_LevelMap(LevelMap, 7, 4);
	TileCollisionBox75.Init_CollisionBox_With_LevelMap(LevelMap, 7, 5);
	TileCollisionBox78.Init_CollisionBox_With_LevelMap(LevelMap, 7, 8);
	TileCollisionBox79.Init_CollisionBox_With_LevelMap(LevelMap, 7, 9);
	TileCollisionBox89.Init_CollisionBox_With_LevelMap(LevelMap, 8, 9);
	TileCollisionBox417.Init_CollisionBox_With_LevelMap(LevelMap, 4, 17);
	TileCollisionBox615.Init_CollisionBox_With_LevelMap(LevelMap, 6, 15);
	TileCollisionBox616.Init_CollisionBox_With_LevelMap(LevelMap, 6, 16);
	TileCollisionBox715.Init_CollisionBox_With_LevelMap(LevelMap, 7, 15);
	TileCollisionBox716.Init_CollisionBox_With_LevelMap(LevelMap, 7, 16);
	TileCollisionBox724.Init_CollisionBox_With_LevelMap(LevelMap, 7, 24);
	TileCollisionBox810.Init_CollisionBox_With_LevelMap(LevelMap, 8, 10);
	TileCollisionBox811.Init_CollisionBox_With_LevelMap(LevelMap, 8, 11);
	TileCollisionBox812.Init_CollisionBox_With_LevelMap(LevelMap, 8, 12);
	TileCollisionBox813.Init_CollisionBox_With_LevelMap(LevelMap, 8, 13);
	TileCollisionBox814.Init_CollisionBox_With_LevelMap(LevelMap, 8, 14);
	
	if (coin17 != nullptr) {
		Coin17_CollisionBox.Init_CollisionBox_With_Object(coin17);
	}
	if (coin44 != nullptr) {
		Coin44_CollisionBox.Init_CollisionBox_With_Object(coin44);
	}
	if (coin47 != nullptr) {
		Coin47_CollisionBox.Init_CollisionBox_With_Object(coin47);
	}
	if (coin42 != nullptr) {
		Coin42_CollisionBox.Init_CollisionBox_With_Object(coin42);
	}
	if (coin410 != nullptr) {
		Coin410_CollisionBox.Init_CollisionBox_With_Object(coin410);
	}
	if (coin515 != nullptr) {
		Coin515_CollisionBox.Init_CollisionBox_With_Object(coin515);
	}
}

void Game::LevelThreeCollision()
{
	Setup_LevelThreeCollision();
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerOnTop(TileCollisionBox39, player, LevelMap, 3, 9);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_HitBlockForItem(TileCollisionBox39Bottom, player, redMushroom, LevelMap, BumpSound, ObjectAppearSound, 3, 9);
	PlayerCollisionBox.CollisionCheck_PlayerAndObject_CollectSpecialItem(RedMushroomCollisionBox, player, redMushroom, objectCollectedSound);
	MovingFloorCollisionBox.CollisionCheck_ObjectAndTile_ObjectToRight(TileCollisionBox49, movingFloor, LevelMap, 4, 9);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_ReleaseJumpRestriction(TileCollisionBox57, LevelMap, 5, 7);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_JumpRestriction(TileCollisionBox59, LevelMap, 5, 9);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerOnTop(TileCollisionBox64, player, LevelMap, 6, 4); // Pipe
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerOnTop(TileCollisionBox65, player, LevelMap, 6, 5); // Pipe	
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerOnTop(TileCollisionBox68, player, LevelMap, 6, 8); // Pipe
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerOnTop(TileCollisionBox69, player, LevelMap, 6, 9); // Pipe
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerToLeft(TileCollisionBox74, player, LevelMap, 7, 4);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerToRight(TileCollisionBox75, player, LevelMap, 7, 5);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerToLeft(TileCollisionBox78, player, LevelMap, 7, 8);	
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerToRight(TileCollisionBox79, player, LevelMap, 7, 9);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_StopFreeFall(TileCollisionBox89, LevelMap, 8, 9);
	MovingFloorCollisionBox.CollisionCheck_ObjectAndTile_ObjectToLeft(TileCollisionBox417, movingFloor, LevelMap, 4, 17);
	PlayerCollisionBox.CollisionCheck_PlayerAndObject_PlayerToTop(MovingFloorCollisionBox, player, movingFloor);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerOnTop(TileCollisionBox615, player, LevelMap, 6, 15); // Pipe
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerOnTop(TileCollisionBox616, player, LevelMap, 6, 16); // Pipe
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerToLeft(TileCollisionBox715, player, LevelMap, 7, 15);
	
	SpiderCollisionBox.CollisionCheck_EnemyAndTile_EnemyToRight(TileCollisionBox716, Spider1, LevelMap, 7, 16);
	PlayerCollisionBox.CollisionCheck_PlayerAndEnemy_FallingDeath(SpiderCollisionBox, player, Spider1, playerDiesSound);
	
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerToRight(TileCollisionBox716, player, LevelMap, 7, 16);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_ReleaseJumpRestriction(TileCollisionBox724, LevelMap, 7, 24);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_FallingDeath(TileCollisionBox810, player, LevelMap, playerDiesSound, 8, 10);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_FallingDeath(TileCollisionBox811, player, LevelMap, playerDiesSound, 8, 11);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_FallingDeath(TileCollisionBox812, player, LevelMap, playerDiesSound, 8, 12);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_FallingDeath(TileCollisionBox813, player, LevelMap, playerDiesSound, 8, 13);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_FallingDeath(TileCollisionBox814, player, LevelMap, playerDiesSound, 8, 14);

	if (coin17 != nullptr) {
		PlayerCollisionBox.CollisionCheck_PlayerAndObject_CollectItem(Coin17_CollisionBox, player, coin17, coinCollectSound);
	}
	if (coin44 != nullptr) {
		PlayerCollisionBox.CollisionCheck_PlayerAndObject_CollectItem(Coin44_CollisionBox, player, coin44, coinCollectSound);
	}
	if (coin47 != nullptr) {
		PlayerCollisionBox.CollisionCheck_PlayerAndObject_CollectItem(Coin47_CollisionBox, player, coin47, coinCollectSound);
	}
	if (coin42 != nullptr)
	{
		PlayerCollisionBox.CollisionCheck_PlayerAndObject_CollectItem(Coin42_CollisionBox, player, coin42, coinCollectSound);
	}
	if (coin410 != nullptr) {
		PlayerCollisionBox.CollisionCheck_PlayerAndObject_CollectItem(Coin410_CollisionBox, player, coin410, coinCollectSound);
	}
	if (coin515 != nullptr) {
		PlayerCollisionBox.CollisionCheck_PlayerAndObject_CollectItem(Coin515_CollisionBox, player, coin515, coinCollectSound);
	}
}

void Game::Setup_LevelFourCollision() 
{
	PlayerCollisionBox.Init_CollisionBox_With_Player(player);

	SpiderCollisionBox.Init_CollisionBox_With_Enemy(Spider1);
	SpiderCollisionBox.setWidth(SpiderCollisionBox.getWidth() / 1.4);
	SpiderCollisionBox.setHeight(SpiderCollisionBox.getHeight() / 1.4);
	SpiderCollisionBox.setPosX(SpiderCollisionBox.getPosX() + 17);
	SpiderCollisionBox.setPosY(SpiderCollisionBox.getPosY() + 25);
	
	TileCollisionBox39.Init_CollisionBox_With_LevelMap(LevelMap, 3, 9);
	TileCollisionBox39Left.Init_CollisionBox_With_LevelMap_And_OffsetX(LevelMap, 3, 9, -2);
	TileCollisionBox39Left.setPosY(LevelMap->getTilePositionY(3, 9) + 10);
	TileCollisionBox48.Init_CollisionBox_With_LevelMap(LevelMap, 4, 8);
	TileCollisionBox48Left.Init_CollisionBox_With_LevelMap_And_OffsetX(LevelMap, 4, 8, -2);
	TileCollisionBox48Left.setPosY(LevelMap->getTilePositionY(4, 8) + 10);
	TileCollisionBox57.Init_CollisionBox_With_LevelMap(LevelMap, 5, 7);
	TileCollisionBox57Left.Init_CollisionBox_With_LevelMap_And_OffsetX(LevelMap, 5, 7, -2);
	TileCollisionBox57Left.setPosY(LevelMap->getTilePositionY(5, 7) + 10);
	TileCollisionBox66.Init_CollisionBox_With_LevelMap(LevelMap, 6, 6);
	TileCollisionBox66Left.Init_CollisionBox_With_LevelMap_And_OffsetX(LevelMap, 6, 6, -2);
	TileCollisionBox66Left.setPosY(LevelMap->getTilePositionY(6, 6) + 10);
	TileCollisionBox75.Init_CollisionBox_With_LevelMap(LevelMap, 7, 5);
	TileCollisionBox84.Init_CollisionBox_With_LevelMap_And_OffsetX(LevelMap, 8, 4, 40);
	TileCollisionBox210.Init_CollisionBox_With_LevelMap_And_OffsetX(LevelMap, 2, 10, 8);
	TileCollisionBox210Left.Init_CollisionBox_With_LevelMap_And_OffsetX(LevelMap, 2, 10, -2);
	TileCollisionBox210Left.setPosY(LevelMap->getTilePositionY(2, 10) + 10);
	TileCollisionBox211.Init_CollisionBox_With_LevelMap(LevelMap, 2, 11);
	TileCollisionBox311.Init_CollisionBox_With_LevelMap(LevelMap, 3, 11);
	TileCollisionBox411.Init_CollisionBox_With_LevelMap(LevelMap, 4, 11);
	TileCollisionBox511.Init_CollisionBox_With_LevelMap(LevelMap, 5, 11);
	TileCollisionBox611.Init_CollisionBox_With_LevelMap(LevelMap, 6, 11);
	TileCollisionBox711.Init_CollisionBox_With_LevelMap(LevelMap, 7, 11);
	TileCollisionBox717.Init_CollisionBox_With_LevelMap_And_OffsetX(LevelMap, 7, 17, 8);
	TileCollisionBox717.setWidth(TileCollisionBox717.getWidth() / 1.2);
	TileCollisionBox717Left.Init_CollisionBox_With_LevelMap_And_OffsetX(LevelMap, 7, 17, -2);
	TileCollisionBox717Left.setPosY(LevelMap->getTilePositionY(7, 17) + 10);
	TileCollisionBox717Right.Init_CollisionBox_With_LevelMap(LevelMap, 7, 17);
	TileCollisionBox717Right.setPosY(LevelMap->getTilePositionY(7, 17) + 10);
}

void Game::LevelFourCollision()
{
	Setup_LevelFourCollision();
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerToLeft(TileCollisionBox39Left, player, LevelMap, 3, 9);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerToLeft(TileCollisionBox48Left, player, LevelMap, 4, 8);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerToLeft(TileCollisionBox57Left, player, LevelMap, 5, 7);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerToLeft(TileCollisionBox66Left, player, LevelMap, 6, 6);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerToLeft(TileCollisionBox210Left, player, LevelMap, 2, 10);
	
	SpiderCollisionBox.CollisionCheck_EnemyAndTile_EnemyToRight(TileCollisionBox717Right, Spider1, LevelMap, 7, 17);	
	PlayerCollisionBox.CollisionCheck_PlayerAndEnemy_FallingDeath(SpiderCollisionBox, player, Spider1, playerDiesSound);

	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerOnTop(TileCollisionBox39, player, LevelMap, 3, 9);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerOnTop(TileCollisionBox48, player, LevelMap, 4, 8);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerOnTop(TileCollisionBox57, player, LevelMap, 5, 7);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerOnTop(TileCollisionBox66, player, LevelMap, 6, 6);	
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerToLeftStop(TileCollisionBox66, player, LevelMap, 6,6);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerOnTop(TileCollisionBox75, player, LevelMap, 7, 5);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_FallingDeath(TileCollisionBox84, player, LevelMap, playerDiesSound, 8, 4);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerOnTop(TileCollisionBox210, player, LevelMap, 2, 10);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerOnTop(TileCollisionBox211, player, LevelMap, 2, 11);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerToRight(TileCollisionBox311, player, LevelMap, 3, 11);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerToRight(TileCollisionBox411, player, LevelMap, 4, 11);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerToRight(TileCollisionBox511, player, LevelMap, 5, 11);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerToRight(TileCollisionBox611, player, LevelMap, 6, 11);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerToRight(TileCollisionBox711, player, LevelMap, 7, 11);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerOnTop(TileCollisionBox717, player, LevelMap, 7, 17);
	PlayerCollisionBox.CollisionCheck_PlayerAndTile_PlayerToLeft(TileCollisionBox717Left, player, LevelMap, 7, 17);	
}

void Game::EnemyMovementAI()
{
	if (player->getPlayerPositionX() < Spider1->getEnemyPositionX() && isSpiderChasePlayer == true)
	{
		Spider1->moveLeft();
	}

	if (player->getPlayerPositionX() > Spider1->getEnemyPositionX() && isSpiderChasePlayer == true)
	{
		Spider1->moveRight();
	}

	if (isSpiderChasePlayer == false)
	{
		Spider1->moveLeft();
		if (Spider1->getEnemyPositionX() < 0)
		{
			Spider1->setEnemyPositionX(0);
			isSpiderChasePlayer = true;
		}
	}

	if (PlayerCollisionBox.getPosX() < TileCollisionBox715.getPosX())
	{
		isSpiderChasePlayer = true;
	}
	Spider1->Update();
}

void Game::GameOver()
{
	int texW = 0;
	int texH = 0;
	Uint32 wrapLength = 1700;
	SDL_Color textColor = { 0, 0, 0, 255 };
	text = TTF_RenderText_Solid_Wrapped(font, "GAME OVER!! Thank you for playing this game and your supports. Press Escape to Exit the game. Thank you!!", 
		textColor, wrapLength);
	// text = TTF_RenderText_Solid(font, "GAME OVER!! Please Close The Application", textColor);
	if (!text)
	{
		std::cout << "\x1b[1;31mFAILED to create TTF_RenderText_Solid(): " << TTF_GetError() << "\033[0m" << std::endl;
	}
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(GameRenderer, text);
	SDL_FreeSurface(text);
	SDL_QueryTexture(textTexture, NULL, NULL, &texW, &texH);
	if (!textTexture)
	{
		std::cout << "\x1b[1;31mFAILED to create SDL_CreateTextureFromSurface(): " << SDL_GetError() << "\033[0m" << std::endl;
	}
	SDL_Rect textRect = { 50, LevelMap->getTilePositionY(0,4), texW, texH};

	SDL_RenderCopy(GameRenderer, textTexture, NULL, &textRect);
	SDL_DestroyTexture(textTexture);
}


void Game::Update()
{
	//If we are too fast, waste some time until we reach the MILLISECS_PER_FRAME
	int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
	if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
		SDL_Delay(timeToWait);
	}
	// The difference in ticks since the last frame, converted to seconds
	deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;

	// Store the "previous" frame time
	millisecsPreviousFrame = SDL_GetTicks();
	
	// Map1: Updating player and enemy on Map1
	if (Game::playerFinishedMap1 == false)
	{	
		LevelOneCollision();
		player->Update(sdlEvent);	
		star->Falling(10.0);
		star->Update();

		if (Game::isFallFromSky == true && !player->getFallingAnimationStatus())
		{
			player->setPlayerTexture("./assets/player-falling-Sheet.png", 16, 16, 0, 0, 4, SDL_FLIP_NONE);
			// Added flag to prevent recreating Falling Animation texture in a loop.
			player->setFallingAnimationToTrue();
		}
		if (Game::isFallFromSky == true) {
			player->Falling(Game::fGravityDeathFalling);
		}

		if (coin17 != nullptr) {
			coin17->Update();
		}
		if (coin44 != nullptr) {
			coin44->Update();
		}
		if (coin47 != nullptr) {
			coin47->Update();
		}
		if (coin42 != nullptr) {
			coin42->Update();
		}
		if (coin410 != nullptr) {
			coin410->Update();
		}
		if (coin515 != nullptr) {
			coin515->Update();
		}

		if (mushroom->getIsObjectAppears())	{
			mushroom->setApplyGravity(true);
			mushroom->moveToRight();
		}
		if (greenMushroom->getIsObjectAppears()) {
			greenMushroom->setApplyGravity(true);
			greenMushroom->moveToRight();
		}
		if (redMushroom->getIsObjectAppears()) {
			redMushroom->setApplyGravity(true);
			redMushroom->moveToRight();
		}
		if (mushroomGreen->getIsObjectAppears()) {
			mushroomGreen->setApplyGravity(true);
			mushroomGreen->moveToRight();
		}
		mushroom->Update();
		greenMushroom->Update();
		redMushroom->Update();
		mushroomGreen->Update();
		Game::EnemyMovementAI();
	}

	// Loading Map 2: Map1 completed move on to map2. Player reached the end of windowWidth of map1
	if (player->getPlayerPositionX() + player->getPlayerWidth() > Game::windowWidth &&
		Game::playerFinishedMap1 == false)
	{
		SetupLevel2();
		Game::playerFinishedMap1 = true;
		LevelMap->Update();
	}

	// Map2: Updating player and enemy on Map2
	if (Game::playerFinishedMap2 == false && Game::playerFinishedMap1 == true)
	{
		player->Update(sdlEvent);
		LevelTwoCollision();

		star->Falling(2.00);
		star->Update();
		Game::EnemyMovementAI();

		// checking the condition for movingFloor to move right and left. 
		if (is_movingFloorMoveToLeft)
		{
			movingFloor->moveToLeft();
		}
		else
		{
			movingFloor->moveToRight();

		}
		movingFloor->Update();

		if (Game::isFallFromSky == true && !player->getFallingAnimationStatus())
		{
			player->setPlayerTexture("./assets/player-falling-Sheet.png", 16, 16, 0, 0, 4, SDL_FLIP_NONE);
			// Added flag to prevent recreating Falling Animation texture in a loop.
			player->setFallingAnimationToTrue();
		}
		if (Game::isFallFromSky == true) {
			player->Falling(Game::fGravityDeathFalling);
		}

		if (coin17 != nullptr) {
			coin17->Update();
		}
		if (coin44 != nullptr) {
			coin44->Update();
		}
		if (coin47 != nullptr) {
			coin47->Update();
		}
		if (coin42 != nullptr) {
			coin42->Update();
		}
		if (coin410 != nullptr) {
			coin410->Update();
		}
		if (coin515 != nullptr) {
			coin515->Update();
		}

		if (mushroom->getIsObjectAppears())
		{
			mushroom->setApplyGravity(true);
			mushroom->moveToRight();
		}
		mushroom->Update();
			
		if(greenMushroom->getIsObjectAppears()) 
		{
			greenMushroom->setApplyGravity(true);
			greenMushroom->moveToRight();
		}
		greenMushroom->Update();
		
		if (redMushroom->getIsObjectAppears())
		{
			redMushroom->setApplyGravity(true);
			redMushroom->moveToRight();
		}
		redMushroom->Update();
	}

	// Loading Map 3: map2 completed move on to map3. Player reached the end of windowWidth of map2
	if (player->getPlayerPositionX() + player->getPlayerWidth() > Game::windowWidth && 
	Game::playerFinishedMap1 == true && Game::playerFinishedMap2 == false)
	{
		SetupLevel3();		
		Game::playerFinishedMap2 = true; // Player completed map2 set to true
		LevelMap->Update();
	}

	// Map3: Updating player and enemy on Map3
	if (Game::playerFinishedMap3 == false && Game::playerFinishedMap2 == true && 
		Game::playerFinishedMap1 == true )
	{
		player->Update(sdlEvent);
		LevelThreeCollision();
		Game::EnemyMovementAI();
		star->Update();
		
		if (is_movingFloorMoveToLeft) {
			movingFloor->moveToLeft();
		}
		else{
			movingFloor->moveToRight();
		}
		movingFloor->Update();

		if (Game::isFallFromSky == true && !player->getFallingAnimationStatus())
		{
			player->setPlayerTexture("./assets/player-falling-Sheet.png", 16, 16, 0, 0, 4, SDL_FLIP_NONE);
			// Added flag to prevent recreating Falling Animation texture in a loop.
			player->setFallingAnimationToTrue();
		}
		if (Game::isFallFromSky == true)
		{
			player->Falling(Game::fGravityDeathFalling);
			player->Update(sdlEvent);
		}

		if (coin17 != nullptr) {
			coin17->Update();
		}
		if (coin44 != nullptr) {
			coin44->Update();
		}
		if (coin47 != nullptr) {
			coin47->Update();
		}
		if (coin42 != nullptr) {
			coin42->Update();
		}
		if (coin410 != nullptr) {
			coin410->Update();
		}
		if (coin515 != nullptr) {
			coin515->Update();
		}
				
		if (redMushroom->getIsObjectAppears()){
			redMushroom->setApplyGravity(true);
			redMushroom->moveToRight();
		}
		redMushroom->Update();
	}

	// Loading Map4: map3 completed move on to map4. Player reached the end of windowWidth of map3
	if (player->getPlayerPositionX() + player->getPlayerWidth() > Game::windowWidth && Game::playerFinishedMap1 == true && 
		Game::playerFinishedMap2 == true && Game::playerFinishedMap3 == false)
	{
		SetupLevel4();
		Game::playerFinishedMap3 = true; 
		LevelMap->Update();
	}

	// Map4: Updating player and enemy on Map4
	if (Game::playerFinishedMap4 == false && Game::playerFinishedMap3 == true) {		
		LevelFourCollision();
		Game::EnemyMovementAI();
		star->Falling(1.0);
		star->Update();
		
		player->Update(sdlEvent);				

		if (Game::isFallFromSky == true && !player->getFallingAnimationStatus())
		{
			player->setPlayerTexture("./assets/player-falling-Sheet.png", 16, 16, 0, 0, 4, SDL_FLIP_NONE);
			// Added flag to prevent recreating Falling Animation texture in a loop.
			player->setFallingAnimationToTrue();
		}
		if (Game::isFallFromSky == true)
		{
			player->Falling(Game::fGravityDeathFalling);			
		}
	}

	// Loading Map1: map4 completed return to map1. Player reached the end of windowWidth of map4
	if (player->getPlayerPositionX() + player->getPlayerWidth() > Game::windowWidth && Game::playerFinishedMap1 == true && 
		Game::playerFinishedMap2 == true && Game::playerFinishedMap3 == true && Game::playerFinishedMap4 == false)
	{
		SetupLevel1();
		Game::playerFinishedMap4 = true;
		LevelMap->Update();
	}

	// RETURN TO Map1: Updating player and enemy on Map
	if (Game::playerFinishedMap4 == true && Game::playerFinishedMap3 == true) 
	{		
		Game::playerFinishedMap1 = false;
		Game::playerFinishedMap2 = false;
		Game::playerFinishedMap3 = false;		
		Game::playerFinishedMap4 = false;
	}
}

void Game::Render()
{
	// Map1: Render Entities and Objects on Map1
	if (Game::playerFinishedMap1 == false)
	{	
		playerScore->setTextLocation(0, LevelMap->getTilePositionY(9, 9) + 190);

		int texW = 0;
		int texH = 0;
		SDL_Color textColor = { 255, 255, 255, 255 };
		Uint32 wrapLength = 1700;
		text = TTF_RenderText_Solid_Wrapped(font, "Press Space To Jump AND Press C to Run. Press Escape to Exit the game. Movement Right and Left Arrow. ",
			textColor, wrapLength);
		// TTF_RenderText_Solid(font, "Press Space To Jump AND Press C to Run, Movement Right and Left Arrow", textColor);
		if(!text)
		{
			std::cout << "\x1b[1;31mFAILED to create TTF_RenderText_Solid(): " << TTF_GetError() << "\033[0m" << std::endl;
		}
		SDL_Texture *textTexture = SDL_CreateTextureFromSurface(GameRenderer, text);
		SDL_FreeSurface(text);
		SDL_QueryTexture(textTexture, NULL, NULL, &texW, &texH);
		if(!textTexture)
		{
			std::cout << "\x1b[1;31mFAILED to create SDL_CreateTextureFromSurface(): " << SDL_GetError() << "\033[0m" << std::endl;
		}
		SDL_Rect textRect = { 0, LevelMap->getTilePositionY(9, 9) + 75, texW, texH };

		SDL_RenderClear(GameRenderer);
		SDL_RenderCopy(GameRenderer, mBackground, NULL, &backgroundRect);
		LevelMap->Render();
	
		if (isDebug) {
			SDL_SetRenderDrawColor(GameRenderer, 255, 0, 0, 255);
			SpiderCollisionBox.Render();			
			PlayerCollisionBox.Render();
			TileCollisionBox27.Render();	
			TileCollisionBox27Bottom.Render();
			TileCollisionBox46.Render();
			TileCollisionBox47.Render();
			TileCollisionBox48.Render();
			TileCollisionBox54.Render();
			TileCollisionBox54Bottom.Render();	
			TileCollisionBox57.Render();
			TileCollisionBox57Bottom.Render();
			TileCollisionBox510.Render();
			TileCollisionBox510Bottom.Render();
			TileCollisionBox73.Render();
			TileCollisionBox74.Render();
			TileCollisionBox75.Render();
			TileCollisionBox77.Render();
			TileCollisionBox78.Render();
			TileCollisionBox615.Render();
			TileCollisionBox616.Render();
			TileCollisionBox710.Render();
			TileCollisionBox711.Render();
			TileCollisionBox715.Render();
			TileCollisionBox716.Render();
			
			if (!coin17->getIsObjectCollected()) {
				Coin17_CollisionBox.Render();
			}
			if (!coin44->getIsObjectCollected()) {
				Coin44_CollisionBox.Render();
			}
			if (!coin47->getIsObjectCollected()) {
				Coin47_CollisionBox.Render();
			}
			if (!coin42->getIsObjectCollected())
			{
				Coin42_CollisionBox.Render();
			}
			if (!coin410->getIsObjectCollected())
			{
				Coin410_CollisionBox.Render();
			}
			if (!coin515->getIsObjectCollected())
			{
				Coin515_CollisionBox.Render();
			}

			if (mushroom->getIsObjectAppears() && !mushroom->getIsObjectCollected()){

				MushroomCollisionBox.Render();
			}	
			if (greenMushroom->getIsObjectAppears() && !greenMushroom->getIsObjectCollected()) {
				GreenMushroomCollisionBox.Render();
			}
			if (redMushroom->getIsObjectAppears() && !redMushroom->getIsObjectCollected())
			{
				RedMushroomCollisionBox.Render();
			}
			if (mushroomGreen->getIsObjectAppears() && !mushroomGreen->getIsObjectCollected())
			{
				GreenMushroomCollisionBox2.Render();
			}

			//ImGUI
			//myDebugGUI.DebugInput();
			//myDebugGUI.RenderDebugDemo();
		}

		if (!coin17->getIsObjectCollected()){
			coin17->Render();
		}
		if (!coin44->getIsObjectCollected()){
			coin44->Render();
		}		
		if (!coin47->getIsObjectCollected()) {
			coin47->Render();
		}		
		if (!coin42->getIsObjectCollected())
		{
			coin42->Render();
		}
		if (!coin410->getIsObjectCollected())
		{
			coin410->Render();
		}
		if (!coin515->getIsObjectCollected()){
			coin515->Render();
		}

		if (mushroom->getIsObjectAppears() && 
			!mushroom->getIsObjectCollected()){
			mushroom->Render();
		}

		if (greenMushroom->getIsObjectAppears() && 
			!greenMushroom->getIsObjectCollected())
		{
			greenMushroom->Render();
		}

		if (redMushroom->getIsObjectAppears() && 
			!redMushroom->getIsObjectCollected())
		{
			redMushroom->Render();
		}

		if (mushroomGreen->getIsObjectAppears() && 
			!mushroomGreen->getIsObjectCollected())
		{
			mushroomGreen->Render();
		}

		star->Render();
		Spider1->Render();	
		player->Render();
		playerScore->setTotalScore(tmpTotalScore);
		playerScore->UpdateScoreSystem();
		playerScore->DrawScoreBox();
		SDL_RenderCopy(GameRenderer, textTexture, NULL, &textRect);
		SDL_DestroyTexture(textTexture);
		
		if (Game::IsGameOver) {
			GameOver();
		}

		SDL_RenderPresent(GameRenderer);
	}

	// Map2: Render Entities and Objects on Map2
	if (Game::playerFinishedMap2 == false && Game::playerFinishedMap1 == true)
	{
		SDL_RenderClear(GameRenderer);
		SDL_RenderCopy(GameRenderer, mBackground, NULL, &backgroundRect);
		LevelMap->Render();

		if (isDebug) {
			SDL_SetRenderDrawColor(GameRenderer, 255, 0, 0, 255);
			PlayerCollisionBox.Render();
			MovingFloorCollisionBox.Render();
			SpiderCollisionBox.Render();
			TileCollisionBox55.Render();
			TileCollisionBox55Bottom.Render();
			TileCollisionBox70.Render();
			TileCollisionBox74.Render();
			TileCollisionBox75.Render();	
			TileCollisionBox76.Render();
			TileCollisionBox77.Render();	
			TileCollisionBox78.Render();	
			TileCollisionBox86.Render();
			TileCollisionBox88.Render();	
			TileCollisionBox89.Render();						
			TileCollisionBox216.Render();
			TileCollisionBox216Bottom.Render();
			TileCollisionBox414.Render();
			TileCollisionBox416.Render();
			TileCollisionBox418.Render();
			TileCollisionBox515.Render();
			TileCollisionBox516.Render();
			TileCollisionBox516Bottom.Render();
			TileCollisionBox517.Render();
			TileCollisionBox714.Render();
			TileCollisionBox714_B.Render();
			TileCollisionBox715.Render();
			TileCollisionBox717.Render();
			TileCollisionBox718.Render();
			TileCollisionBox724.Render();
			TileCollisionBox810.Render();
			TileCollisionBox811.Render();
			TileCollisionBox812.Render();
			TileCollisionBox813.Render();
			TileCollisionBox814.Render();

			if (!coin17->getIsObjectCollected()) {
				Coin17_CollisionBox.Render();
			}
			if (!coin44->getIsObjectCollected()) {
				Coin44_CollisionBox.Render();
			}
			if (!coin47->getIsObjectCollected()) {
				Coin47_CollisionBox.Render();
			}
			if (!coin42->getIsObjectCollected())
			{
				Coin42_CollisionBox.Render();
			}
			if (!coin410->getIsObjectCollected())
			{
				Coin410_CollisionBox.Render();
			}
			if (!coin515->getIsObjectCollected())
			{
				Coin515_CollisionBox.Render();
			}

			if (mushroom->getIsObjectAppears() && 
				!mushroom->getIsObjectCollected()) {
				MushroomCollisionBox.Render();
			}

			if (greenMushroom->getIsObjectAppears() &&
				!greenMushroom->getIsObjectCollected()) {
				GreenMushroomCollisionBox.Render();
			}

			if (redMushroom->getIsObjectAppears() &&
				!redMushroom->getIsObjectCollected())
			{
				RedMushroomCollisionBox.Render();
			}
		}

		if (!coin17->getIsObjectCollected()) {
			coin17->Render();
		}
		if (!coin44->getIsObjectCollected()) {
			coin44->Render();
		}
		if (!coin47->getIsObjectCollected()) {
			coin47->Render();
		}
		if (!coin42->getIsObjectCollected()){
			coin42->Render();
		}
		if (!coin410->getIsObjectCollected())
		{
			coin410->Render();
		}
		if (!coin515->getIsObjectCollected()) {
			coin515->Render();
		}

		if (mushroom->getIsObjectAppears() && !mushroom->getIsObjectCollected())
		{
			mushroom->Render();
		}
		if (greenMushroom->getIsObjectAppears() && !greenMushroom->getIsObjectCollected())
		{
			greenMushroom->Render();
		}
		if (redMushroom->getIsObjectAppears() && !redMushroom->getIsObjectCollected())
		{
			redMushroom->Render();
		}

		movingFloor->Render();
		star->Render();
		Spider1->Render();
		player->Render();

		playerScore->setTextLocation(0, LevelMap->getTilePositionY(9, 9) + 200);
		playerScore->setTotalScore(tmpTotalScore);
		playerScore->UpdateScoreSystem();
		playerScore->DrawScoreBox();

		if (Game::IsGameOver) {
			GameOver();
		}
		SDL_RenderPresent(GameRenderer);
	}
	
	// Map3: Render Entities and Objects on Map3
	if (Game::playerFinishedMap3 == false && Game::playerFinishedMap2 == true && Game::playerFinishedMap1 == true)
	{
		SDL_RenderClear(GameRenderer);
		SDL_RenderCopy(GameRenderer, mBackground, NULL, &backgroundRect);
		LevelMap->Render();
		if (isDebug) {
			SDL_SetRenderDrawColor(GameRenderer, 255, 0, 0, 255);			
			PlayerCollisionBox.Render();
			MovingFloorCollisionBox.Render();
			SpiderCollisionBox.Render();	
			TileCollisionBox39.Render();
			TileCollisionBox39Bottom.Render();
			TileCollisionBox49.Render();
			TileCollisionBox57.Render();	
			TileCollisionBox59.Render();
			TileCollisionBox64.Render();
			TileCollisionBox65.Render();
			TileCollisionBox68.Render();
			TileCollisionBox69.Render();		
			TileCollisionBox74.Render();
			TileCollisionBox75.Render();	
			TileCollisionBox78.Render();
			TileCollisionBox79.Render();	
			TileCollisionBox89.Render();
			TileCollisionBox417.Render();
			TileCollisionBox615.Render();	
			TileCollisionBox616.Render();	
			TileCollisionBox715.Render();	
			TileCollisionBox716.Render();
			TileCollisionBox724.Render();
			TileCollisionBox810.Render();
			TileCollisionBox811.Render();
			TileCollisionBox812.Render();
			TileCollisionBox813.Render();
			TileCollisionBox814.Render();			
		}

		if (!coin17->getIsObjectCollected()) {
			coin17->Render();
		}
		if (!coin44->getIsObjectCollected()) {
			coin44->Render();
		}
		if (!coin47->getIsObjectCollected()) {
			coin47->Render();
		}

		if (!coin42->getIsObjectCollected())
		{
			coin42->Render();
		}
		if (!coin410->getIsObjectCollected())
		{
			coin410->Render();
		}
		if (!coin515->getIsObjectCollected()) {
			coin515->Render();
		}

		if (redMushroom->getIsObjectAppears() && !redMushroom->getIsObjectCollected())
		{
			redMushroom->Render();
		}

		star->Render();
		movingFloor->Render();
		Spider1->Render();
		player->Render();

		playerScore->setTextLocation(0, LevelMap->getTilePositionY(9, 9) + 200);
		playerScore->setTotalScore(tmpTotalScore);
		playerScore->UpdateScoreSystem();
		playerScore->DrawScoreBox();

		if (Game::IsGameOver) {
			GameOver();
		}
		SDL_RenderPresent(GameRenderer);
	}

	// Map4: Render Entities and Objects on Map4
	if (Game::playerFinishedMap4 == false && Game::playerFinishedMap3 == true && Game::playerFinishedMap2 == true && Game::playerFinishedMap1 == true)
	{
		SDL_RenderClear(GameRenderer);
		SDL_RenderCopy(GameRenderer, mBackground, NULL, &backgroundRect);
		LevelMap->Render();
		

		if (isDebug) {
			SDL_SetRenderDrawColor(GameRenderer, 255, 0, 0, 255);
			PlayerCollisionBox.Render();
			SpiderCollisionBox.Render();
			TileCollisionBox39.Render();
			TileCollisionBox48.Render();
			TileCollisionBox57.Render();
			TileCollisionBox66.Render();
			TileCollisionBox39Left.Render();
			TileCollisionBox48Left.Render();
			TileCollisionBox57Left.Render();
			TileCollisionBox66Left.Render();
			TileCollisionBox210Left.Render();
			TileCollisionBox75.Render();
			TileCollisionBox84.Render();
			TileCollisionBox210.Render();
			TileCollisionBox211.Render();
			TileCollisionBox311.Render();
			TileCollisionBox411.Render();
			TileCollisionBox511.Render();
			TileCollisionBox611.Render();
			TileCollisionBox711.Render();
			TileCollisionBox717.Render();
			TileCollisionBox717Left.Render();
			TileCollisionBox717Right.Render();
		}

		Spider1->Render();
		player->Render();

		playerScore->setTextLocation(0, LevelMap->getTilePositionY(9, 9) + 200);
		playerScore->setTotalScore(tmpTotalScore);
		playerScore->UpdateScoreSystem();
		playerScore->DrawScoreBox();
		
		if (Game::IsGameOver) {
			GameOver();
		}
		
		SDL_RenderPresent(GameRenderer);
	}		
}

void Game::CleanUP()
{	
	SDL_DestroyTexture(mBackground);
	SDL_DestroyRenderer(GameRenderer);
	SDL_DestroyWindow(mGameWindow);

	Mix_FreeMusic(Game_Music);	
	Mix_FreeChunk(JumpSound);
	Mix_FreeChunk(ObjectAppearSound);
	Mix_FreeChunk(coinCollectSound);
	Mix_FreeChunk(BumpSound);
	Mix_FreeChunk(objectCollectedSound);
	Mix_FreeChunk(playerDiesSound);
	Game_Music = NULL;
	JumpSound = NULL;
	ObjectAppearSound = NULL;
	coinCollectSound = NULL;
	BumpSound = NULL;
	objectCollectedSound = NULL;
	Mix_CloseAudio();
	Mix_Quit();

	TTF_CloseFont(font);
	TTF_Quit();

	/*
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	*/

	delete player;
	player = nullptr;
	delete LevelMap;
	LevelMap = nullptr;
	delete Spider1;
	Spider1 = nullptr;
	delete movingFloor;
	movingFloor = nullptr,
	delete star;
	star = nullptr;
	delete coin17;
	coin17 = nullptr;
	delete coin44;
	coin44 = nullptr;
	delete coin47;
	coin47 = nullptr;
	delete coin42;
	coin42 = nullptr;
	delete coin410;
	coin410 = nullptr;
	delete coin515;
	coin515 = nullptr;
	delete playerScore;
	playerScore = nullptr;
	delete mushroom;
	mushroom = nullptr;
	delete greenMushroom;
	greenMushroom = nullptr;
	delete redMushroom;
	redMushroom = nullptr;
	delete mushroomGreen;
	mushroomGreen = nullptr;

   	SDL_Quit();
		
	printf("\033[1;35mCleaning Data is Complete!\n \x1b[0m");
}
