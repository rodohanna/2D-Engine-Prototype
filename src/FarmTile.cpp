#include "FarmTile.h"
#include "Assets.h"
#include "Physics.h"

FarmTile::FarmTile(V2 position)
{
    mTexture = getTexture("plowed-soil");
    mBox.x = position.x;
    mBox.y = position.y;
    mBox.w = 32;
    mBox.h = 32;
}
FarmTile::~FarmTile() {}
bool FarmTile::update(GameState &state) { return true; }
void FarmTile::render(SDL_Renderer *renderer, SDL_Rect &camera)
{
    if (checkCollision(mBox, camera))
    {
        SDL_Rect clip = {0, 0, 32, 32};
        mTexture->render(renderer, mBox.x - camera.x, mBox.y - camera.y, &clip);
    }
}