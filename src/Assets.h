#ifndef ASSETS_h_
#define ASSETS_h_
#include "Texture.h"
#include <SDL2/SDL_ttf.h>
#include <string>

bool loadImageTexture(std::string texturePath, std::string textureKey, SDL_Renderer *renderer);
bool loadFont(std::string fontPath, std::string fontKey, Uint32 fontSize);
Texture *getTexture(std::string textureKey);
TTF_Font *getFont(std::string fontKey);

#endif