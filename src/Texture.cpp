#include "Texture.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

Texture::Texture(SDL_Texture *texture, int width, int height) : mTexture(texture), mWidth(width), mHeight(height) {}

Texture::~Texture()
{
    if (mTexture != NULL)
    {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
    }
}

std::unique_ptr<Texture> Texture::makeTextureFromFile(std::string path, SDL_Renderer *renderer)
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

    Texture *texture = new Texture(newTexture, loadedSurface->w, loadedSurface->h);
    SDL_FreeSurface(loadedSurface);
    return std::unique_ptr<Texture>(texture);
}

std::unique_ptr<Texture> Texture::makeTextureFromText(std::string textureText, SDL_Color textColor, TTF_Font *font, SDL_Renderer *renderer)
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
    Texture *texture = new Texture(newTexture, textSurface->w, textSurface->h);
    SDL_FreeSurface(textSurface);
    return std::unique_ptr<Texture>(texture);
}

void Texture::render(SDL_Renderer *renderer, int x, int y, SDL_Rect *clip, int scale, double angle, SDL_Point *center, SDL_RendererFlip flip)
{
    if (mTexture == NULL)
    {
        printf("Warning! A null texture render was attempted.\n");
        return;
    }
    SDL_Rect renderQuad = {x, y, mWidth * scale, mHeight * scale};
    if (clip != NULL)
    {
        renderQuad.w = clip->w * scale;
        renderQuad.h = clip->h * scale;
    }
    SDL_RenderCopyEx(renderer, mTexture, clip, &renderQuad, angle, center, flip);
}