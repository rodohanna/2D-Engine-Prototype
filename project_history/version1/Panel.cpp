#include "Panel.h"
#include "Physics.h"
#include "Input.h"

Panel::Panel(SDL_Rect box) : mBox(box) {}
Panel::~Panel(){};
void Panel::render(SDL_Renderer *renderer, SDL_Rect &camera)
{
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderDrawRect(renderer, &mBox);
}
bool Panel::update(GameState &state)
{
    mBox.x = (state.camera.w - mBox.w) / 2;
    mBox.y = (state.camera.h - mBox.h) / 2;
    if (checkPointInRect(state.mouseCoords, mBox) && isInputActive(LEFT_MOUSE_JUST_PRESSED))
    {
        clearInput(LEFT_MOUSE_JUST_PRESSED);
    }
    return true;
}
void Panel::handleEvent(GameEvent *e, GameState *state) {}