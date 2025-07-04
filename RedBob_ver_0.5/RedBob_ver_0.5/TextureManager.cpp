#include "Game.h"
#include "TextureManager.h"


SDL_Texture* TextureManager::LoadTexture(const char* filename)
{
    SDL_Surface* tempSurface = IMG_Load(filename);
    if (tempSurface == nullptr)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", filename, IMG_GetError());
    }
    SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(Game::GameRenderer, tempSurface);
    if (tempTexture == nullptr)
    {
		printf("Unable to create texture from %s! SDL Error: %s\n", filename, SDL_GetError());
	}
    SDL_FreeSurface(tempSurface);

    return tempTexture;

}

void TextureManager::Draw(SDL_Texture* texture, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip)
{
    SDL_RenderCopyEx(Game::GameRenderer, texture, &src, &dest, NULL, NULL, flip);
}
