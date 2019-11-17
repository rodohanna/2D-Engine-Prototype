#include "GUI.h"
#include "Events.h"
#include "Assets.h"
#include "Camera.h"

Text::Text(){};
Text::Text(EventBus *e, size_t font_index, std::string texture_key) : event_bus(e), font_index(font_index), texture_key(texture_key){};

void Text::set_text(std::string text)
{
    if (this->text != text)
    {
        auto info = Assets::create_texture_from_text(
            this->event_bus->renderer,
            this->font_index,
            this->texture_key,
            text,
            {0xFF, 0xFF, 0xFF, 0xFF});
        this->text = text;
        this->texture_index = info.texture_index;
        this->dimensions = info.dimensions;
    }
}

void Text::update(double ts, size_t z_index)
{
    this->event_bus->publish_render_event(
        Events::createRenderTextureEvent(
            this->texture_index,
            this->position,
            &this->overflow_clip,
            1,
            z_index));
}

void UIPanel::update_rect()
{
    Rect camera = Camera::get_camera();
    int x, y;
    if (this->anchor_horizontal.type == AnchorType::RIGHT)
    {
        x = camera.w - this->dimensions.x - this->anchor_horizontal.offset;
    }
    else
    {
        x = this->anchor_horizontal.offset;
    }

    if (this->anchor_vertical.type == AnchorType::TOP)
    {
        y = this->anchor_vertical.offset;
    }
    else
    {
        y = camera.h - this->dimensions.y - this->anchor_vertical.offset;
    }
    this->rect = {
        x,
        y,
        this->dimensions.x,
        this->dimensions.y};
    for (Text &t : this->panel_text)
    {
        t.overflow_clip = this->rect;
    }
};

void UIPanel::update(double ts)
{
    this->update_rect();
    int last_text_height = 0;
    for (Text &t : this->panel_text)
    {
        t.position = {
            this->rect.x + (this->rect.w - t.dimensions.x) / 2,
            this->rect.y + last_text_height + 10};
        last_text_height += t.dimensions.y;
        t.update(ts, this->z_index + 1);
    }
    this->event_bus->publish_render_event(
        Events::createRenderRectangleEvent(this->rect, {0x11, 0x11, 0x11, 0xF0}, true, this->z_index));
    this->event_bus->publish_render_event(
        Events::createRenderRectangleEvent(this->rect, {0xFF, 0xFF, 0xFF, 0xF0}, false, this->z_index + 1));
}

GUI::GUI(EventBus *e) : event_bus(e)
{
    this->inventory_panel.event_bus = e;
    this->inventory_panel.dimensions = {275, 175};
    this->inventory_panel.anchor_horizontal = {AnchorType::RIGHT, 10};
    this->inventory_panel.anchor_vertical = {AnchorType::BOTTOM, 10};
    this->inventory_panel.z_index = 4;
    Text t = Text(e, 0, "test-text");
    t.set_text("Inventory");
    Text t2 = Text(e, 0, "test-text2");
    t2.set_text("Rusty Sword");
    this->inventory_panel.panel_text.push_back(t);
    this->inventory_panel.panel_text.push_back(t2);
}

void GUI::update(double ts)
{
    this->inventory_panel.update(ts);
};