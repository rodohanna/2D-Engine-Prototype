#ifndef TEXTURE_h_
#define TEXTURE_h_
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

typedef struct Texture
{
    SDL_Texture *texture;
    int width, height;
} ETexture;

Texture *loadTextureFromFile(std::string path, SDL_Renderer *renderer);
Texture *createTextureFromText(std::string textureText, SDL_Color textColor, TTF_Font *font, SDL_Renderer *renderer);
void renderTexture(Texture *texture, SDL_Renderer *renderer, int x, int y, SDL_Rect *clip = NULL, double angle = 0.0, SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
void freeTexture(Texture *texture);

#endif