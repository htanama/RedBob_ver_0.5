#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "Game.h"

class TextureManager
{
public:
    TextureManager() = default;
    
    static SDL_Texture* LoadTexture(const char *filename);
   	static void Draw(SDL_Texture* texture, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip);

};
#endif 