#include "Label.h"
#include "Assets.h"

Label::Label() {}
Label::~Label() {}
Texture *Label::setText(SDL_Renderer *renderer, SDL_Color color, std::string &text)
{
    if (text != mText)
    {
        mText = text;
        mTexture = Texture::makeTextureFromText(mText, color, getFont("standard_font"), renderer);
    }
    return mTexture.get();
}
void Label::render(SDL_Renderer *renderer, SDL_Rect &box)
{
    if (mTexture != NULL)
    {
        mTexture->render(renderer, box.x + ((box.w - mTexture->mWidth) / 2), box.y + ((box.h - mTexture->mHeight) / 2));
    }
}