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

void UIPanel::update(double ts)
{
    int last_text_height = 0;
    for (Text &t : this->panel_text)
    {
        t.position = {
            this->rect.x + (this->rect.w - t.dimensions.x) / 2,
            this->rect.y + last_text_height + 10};
        last_text_height = t.dimensions.y;
        t.update(ts);
    }
}

GUI::GUI(EventBus *e) : event_bus(e)
{
    this->inventory_panel.rect = {
        (800 - (275 + 10)),
        (640 - (175 + 10)),
        275,
        175};
    Text t = Text(e, 0, "test-text");
    t.set_text("Inventory");
    Text t2 = Text(e, 0, "test-text2");
    t2.set_text("Rusty Sword");
    this->inventory_panel.panel_text.push_back(t);
    this->inventory_panel.panel_text.push_back(t2);
}

void GUI::update(double ts)
{
    this->event_bus->publish_render_event(
        Events::createRenderRectangleEvent(this->inventory_panel.rect, {0x11, 0x11, 0x11, 0xF0}, true, 4));
    this->event_bus->publish_render_event(
        Events::createRenderRectangleEvent(this->inventory_panel.rect, {0xFF, 0xFF, 0xFF, 0xF0}, false, 5));
    this->inventory_panel.update(ts);
};