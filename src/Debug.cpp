#include "Debug.h"
#include "MessageBus.h"
#include "Window.h"

Debug::Debug() : entities_rendered(0), messages_in_render_queue(0), tiles_rendered(0), entities_processed(0)
{
    this->debug_panel.rect_color = {0x11, 0x11, 0x11, 0xAF};
    this->debug_panel.outline_color = {0x00, 0x00, 0x00, 0xFF};
    this->debug_panel.z_index = 1;
    this->debug_panel.rect = {0, 5, 300, 100};

    this->entities_rendered_text.font_index = 0;
    this->entities_rendered_text.has_overflow_clip = false;
    this->entities_rendered_text.render_layer = Render::GUI_LAYER;
    this->entities_rendered_text.z_index = 2;
    this->entities_rendered_text.texture_key = "entities_rendered_text";

    this->messages_in_render_queue_text.font_index = 0;
    this->messages_in_render_queue_text.has_overflow_clip = false;
    this->messages_in_render_queue_text.render_layer = Render::GUI_LAYER;
    this->messages_in_render_queue_text.z_index = 2;
    this->messages_in_render_queue_text.texture_key = "messages_in_render_queue_text";

    this->tiles_rendered_text.font_index = 0;
    this->tiles_rendered_text.has_overflow_clip = false;
    this->tiles_rendered_text.render_layer = Render::GUI_LAYER;
    this->tiles_rendered_text.z_index = 2;
    this->tiles_rendered_text.texture_key = "tiles_rendered_text";

    this->entities_processed_text.font_index = 0;
    this->entities_processed_text.has_overflow_clip = false;
    this->entities_processed_text.render_layer = Render::GUI_LAYER;
    this->entities_processed_text.z_index = 2;
    this->entities_processed_text.texture_key = "entities_processed_text";
};
void Debug::update(double ts)
{
    this->debug_panel.rect.x = (Window::get_gui_camera()->w - this->debug_panel.rect.w) - 5;
    converter.str("");
    converter << "Entities Rendered: " << this->entities_rendered;
    this->entities_rendered_text.set_text(this->converter.str());
    converter.str("");
    converter << "In Render Queue: " << this->messages_in_render_queue;
    this->messages_in_render_queue_text.set_text(this->converter.str());
    converter.str("");
    converter << "Tiles Rendered: " << this->tiles_rendered;
    this->tiles_rendered_text.set_text(this->converter.str());
    converter.str("");
    converter << "Entities Processed: " << this->entities_processed;
    this->entities_processed_text.set_text(this->converter.str());

    this->entities_processed_text.position = {
        this->debug_panel.rect.x + 20,
        this->debug_panel.rect.y + 5};
    this->entities_rendered_text.position = {
        this->debug_panel.rect.x + 20,
        this->entities_processed_text.position.y + this->entities_processed_text.dimensions.y};
    this->tiles_rendered_text.position = {
        this->debug_panel.rect.x + 20,
        this->entities_rendered_text.position.y + this->entities_rendered_text.dimensions.y};
    this->messages_in_render_queue_text.position = {
        this->debug_panel.rect.x + 20,
        this->tiles_rendered_text.position.y + this->tiles_rendered_text.dimensions.y};

    this->debug_panel.rect.h = this->messages_in_render_queue_text.position.y + this->messages_in_render_queue_text.dimensions.y - this->entities_processed_text.position.y + 10;

    this->debug_panel.update(ts);
    this->entities_rendered_text.update(ts);
    this->messages_in_render_queue_text.update(ts);
    this->tiles_rendered_text.update(ts);
    this->entities_processed_text.update(ts);
}
void Debug::process_messages()
{
    this->entities_rendered = 0;
    this->messages_in_render_queue = 0;
    this->tiles_rendered = 0;
    this->entities_processed = 0;
    MBus::MessageQueue mq = MBus::get_queue(MBus::QueueType::DEBUG);
    for (int i = 0; i < mq.length; ++i)
    {
        MBus::Message m = mq.queue[i];
        switch (m.type)
        {
        case MBus::ENTITIES_RENDERED:
        {
            this->entities_rendered = m.data.er.num;
            break;
        }
        case MBus::MESSAGES_IN_RENDER_QUEUE:
        {
            this->messages_in_render_queue = m.data.mirq.num;
            break;
        }
        case MBus::TILES_RENDERED:
        {
            this->tiles_rendered = m.data.tr.num;
            break;
        }
        case MBus::ENTITIES_PROCESSED:
        {
            this->entities_processed = m.data.ep.num;
            break;
        }
        default:
        {
            // NOOP
            break;
        }
        }
    }
}