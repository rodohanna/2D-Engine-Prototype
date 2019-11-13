#include "Scenery.h"
#include "Camera.h"
#include "Physics.h"

BackGround::BackGround(EventBus *e, const Rect &clip, const V2 &position, size_t texture_index) : clip(clip), position(position), event_bus(e), texture_index(texture_index){};

void BackGround::update(double delta)
{
    RenderEvent e;
    e.type = RenderEventType::RENDER_TEXTURE;
    e.z_index = 0;
    Rect camera = Camera::get_camera();
    size_t scale = camera.w / this->clip.w;
    e.data.render_texture_event = {this->texture_index, this->clip, this->position, scale, true};
    this->event_bus->publish_render_event(e);
}

Tree::Tree(EventBus *e, const Rect &clip, const V2 &position, size_t texture_index) : clip(clip), position(position), event_bus(e), texture_index(texture_index){};

void Tree::update(double delta)
{
    RenderEvent e;
    e.type = RenderEventType::RENDER_TEXTURE;
    e.z_index = 1;
    e.data.render_texture_event = {this->texture_index, this->clip, this->position, 2, true};
    Rect box = {this->position.x, this->position.y, this->clip.w * 2, this->clip.h * 2};
    Rect camera = Camera::get_camera();
    if (Physics::checkCollision(camera, box))
    {
        this->event_bus->publish_render_event(e);
    }
}