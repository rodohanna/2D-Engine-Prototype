#include "Texture.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

std::unique_ptr<Texture> loadTextureFromFile(std::string path, SDL_Renderer *renderer)
{
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        printf("Unable to load asset %s. SDL_image Error: %s\n", path.c_str(), IMG_GetError());
        return NULL;
    }

    SDL_Texture *newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (newTexture == NULL)
    {
        printf("Unable to create texture from %s. SDL Error: %s\n", path.c_str(), SDL_GetError());
        return NULL;
    }

    Texture texture;
    texture.texture = newTexture;
    texture.width = loadedSurface->w;
    texture.height = loadedSurface->h;

    SDL_FreeSurface(loadedSurface);
    return std::make_unique<Texture>(texture);
}

std::unique_ptr<Texture> createTextureFromText(std::string textureText, SDL_Color textColor, TTF_Font *font, SDL_Renderer *renderer)
{
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
    if (textSurface == NULL)
    {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", IMG_GetError());
        return nullptr;
    }

    SDL_Texture *newTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (newTexture == NULL)
    {
        printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        return nullptr;
    }
    Texture texture;
    texture.texture = newTexture;
    texture.width = textSurface->w;
    texture.height = textSurface->h;

    SDL_FreeSurface(textSurface);
    return std::make_unique<Texture>(texture);
}

void renderTexture(Texture const *texture, SDL_Renderer *renderer, int x, int y, SDL_Rect *clip, double angle, SDL_Point *center, SDL_RendererFlip flip)
{
    if (texture == NULL)
    {
        printf("Warning! A null texture render was attempted.\n");
        return;
    }
    SDL_Rect renderQuad = {x, y, texture->width, texture->height};
    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    SDL_RenderCopyEx(renderer, texture->texture, clip, &renderQuad, angle, center, flip);
}

void freeTexture(std::unique_ptr<Texture> &texture)
{
    if (texture != NULL)
    {
        SDL_DestroyTexture(texture->texture);
        texture.reset();
    }
}