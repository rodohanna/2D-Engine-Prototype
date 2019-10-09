#ifndef ASSETS_h_
#define ASSETS_h_
#include "Texture.h"
#include <string>

bool loadImageTexture(std::string texturePath, std::string textureKey, SDL_Renderer *renderer);
Texture *getTexture(std::string textureKey);

#endif