#include "Scenery.h"
#include "Camera.h"
#include "Physics.h"

Scenery::Scenery(EventBus *e, const Rect &clip, const V2 &position, size_t texture_index, size_t scale)
    : clip(clip), position(position), event_bus(e), texture_index(texture_index), scale(scale) {}

BackGround::BackGround(EventBus *e, const Rect &clip, const V2 &position, size_t texture_index, size_t scale)
    : Scenery(e, clip, position, texture_index, scale) {}

void BackGround::update(double delta)
{
    Rect camera = Camera::get_camera();
    size_t scale = (camera.w / this->clip.w);
    this->event_bus->publish_render_event(Events::createRenderTextureEvent(this->texture_index, this->clip, this->position, scale, 0u));
}

Dirt::Dirt(EventBus *e, const Rect &clip, const V2 &position, size_t texture_index, size_t scale)
    : Scenery(e, clip, position, texture_index, scale){};

void Dirt::update(double delta)
{
    Rect camera = Camera::get_camera();
    V2 render_position = {this->position.x - camera.x, this->position.y - camera.y};
    Rect box = {this->position.x, this->position.y, static_cast<int>(this->clip.w * this->scale), static_cast<int>(this->clip.h * this->scale)};
    if (Physics::checkCollision(camera, box))
    {
        this->event_bus->publish_render_event(Events::createRenderTextureEvent(this->texture_index, this->clip, render_position, this->scale, 1u));
    }
}

Tree::Tree(EventBus *e, const Rect &clip, const V2 &position, size_t texture_index, size_t scale)
    : Scenery(e, clip, position, texture_index, scale){};

void Tree::update(double delta)
{
    Rect camera = Camera::get_camera();
    V2 render_position = {this->position.x - camera.x, this->position.y - camera.y};
    Rect box = {this->position.x, this->position.y, static_cast<int>(this->clip.w * this->scale), static_cast<int>(this->clip.h * this->scale)};
    if (Physics::checkCollision(camera, box))
    {
        this->event_bus->publish_render_event(Events::createRenderTextureEvent(this->texture_index, this->clip, render_position, this->scale, 1u));
    }
}