#include "GUI.h"
#include "Events.h"
#include "Assets.h"

Text::Text(){};
Text::Text(EventBus *e, size_t font_index, std::string texture_key) : event_bus(e), font_index(font_index), texture_key(texture_key){};

void Text::set_text(std::string text)
{
    if (this->text != text)
    {
        // regenerate font texture
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

void Text::update(double ts)
{
    this->event_bus->publish_render_event(
        Events::createRenderTextureEvent(
            this->texture_index,
            this->position,
            1,
            5));
}

GUI::GUI(EventBus *e) : event_bus(e)
{
    this->right_panel = {200, 400};
    this->text = Text(e, 0, "test-text");
}

void GUI::update(double ts)
{
    Rect panel_rect = {
        (800 - (this->right_panel.dimensions.x + 10)),
        (640 - this->right_panel.dimensions.y) / 2,
        this->right_panel.dimensions.x,
        this->right_panel.dimensions.y};
    this->event_bus->publish_render_event(
        Events::createRenderRectangleEvent(panel_rect, {0x11, 0x11, 0x11, 0xF0}, true, 4));
    this->event_bus->publish_render_event(
        Events::createRenderRectangleEvent(panel_rect, {0xFF, 0xFF, 0xFF, 0xF0}, false, 5));
    this->text.set_text("Hello Chunky");
    this->text.position = {
        (panel_rect.x + (panel_rect.w - this->text.dimensions.x) / 2),
        (panel_rect.y + 10)};
    this->text.update(ts);
};