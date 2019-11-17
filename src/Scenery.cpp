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
    this->event_bus->publish_render_event(Events::createRenderTextureEvent(this->texture_index, this->clip, this->position, nullptr, scale, 0u));
}

void render_if_on_screen(EventBus *event_bus, size_t texture_index, V2 &position, Rect &clip, size_t scale)
{
    Rect camera = Camera::get_camera();
    V2 render_position = {position.x - camera.x, position.y - camera.y};
    Rect box = {position.x, position.y, static_cast<int>(clip.w * scale), static_cast<int>(clip.h * scale)};
    if (Physics::checkCollision(camera, box))
    {
        event_bus->publish_render_event(Events::createRenderTextureEvent(texture_index, clip, render_position, nullptr, scale, 1u));
    }
};

Dirt::Dirt(EventBus *e, const Rect &clip, const V2 &position, size_t texture_index, size_t scale)
    : Scenery(e, clip, position, texture_index, scale){};

void Dirt::update(double delta)
{
    render_if_on_screen(this->event_bus, this->texture_index, this->position, this->clip, this->scale);
}

Tree::Tree(EventBus *e, const Rect &clip, const V2 &position, size_t texture_index, size_t scale)
    : Scenery(e, clip, position, texture_index, scale){};

void Tree::update(double delta)
{
    render_if_on_screen(this->event_bus, this->texture_index, this->position, this->clip, this->scale);
}