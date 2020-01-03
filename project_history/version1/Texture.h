#ifndef TEXTURE_h_
#define TEXTURE_h_
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class Texture
{
public:
    Texture(SDL_Texture *texture, int width, int height);
    ~Texture();
    static std::unique_ptr<Texture> makeTextureFromFile(std::string path, SDL_Renderer *renderer);
    static std::unique_ptr<Texture> makeTextureFromText(std::string textureText, SDL_Color textColor, TTF_Font *font, SDL_Renderer *renderer);
    void render(SDL_Renderer *renderer, int x, int y, SDL_Rect *clip = NULL, int scale = 1, double angle = 0.0, SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
    SDL_Texture *mTexture;
    int mWidth, mHeight;
};

#endif