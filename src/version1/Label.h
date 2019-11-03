#ifndef LABEL_h_
#define LABEL_h_

#include <SDL2/SDL.h>
#include "Texture.h"

struct Label
{
    Label();
    ~Label();
    Texture *setText(SDL_Renderer *renderer, SDL_Color color, std::string &text);
    void render(SDL_Renderer *renderer, SDL_Rect &box);
    std::string mText;
    std::unique_ptr<Texture> mTexture;
};

#endif