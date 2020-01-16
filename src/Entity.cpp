#include "Entity.h"
#include "Window.h"
#include "Input.h"
#include "Physics.h"
#include "MessageBus.h"
#include "Assets.h"
#include <assert.h>
#include <stdio.h>

static int RENDER_FLAGS = (1 << ECS::RENDER) | (1 << ECS::POSITION);

ECS::Entity::Entity()
{
    this->components = new ECS::Component[ECS::NUM_COMPONENT_TYPES];
    this->component_length = 0;
    this->component_flags = 0;
}
void ECS::Entity::add_component(ECS::Component *c)
{
    if (this->component_length >= ECS::NUM_COMPONENT_TYPES - 1)
    {
        printf("WARNING: Entity component list is full and should not be full\n");
        return;
    }
    this->components[this->component_length++] = *c;
    this->component_flags |= (1 << c->type);
}
ECS::Entity::~Entity()
{
    // Can't free in destructor because entities get copied around a lot.
    // delete[] this->components;
}
ECS::Component *ECS::Entity::get_component(ECS::Type type)
{
    for (int i = 0; i < this->component_length; ++i)
    {
        if (this->components[i].type == type)
        {
            return &this->components[i];
        }
    }
    return nullptr;
}

ECS::Map::Map() : mouse_data_cached(false), hovered_cell_cached(false){};

void ECS::Map::update(double ts)
{
    this->mouse_data_cached = false;
    this->hovered_cell_cached = false;
}

void ECS::Map::recalculate_mouse_positions()
{
    Rect *camera = Window::get_camera();
    V2 *mouse_position = Window::get_mouse_position();
    this->mouse_world_position = {mouse_position->x + camera->x, mouse_position->y + camera->y};
    this->mouse_grid_position = {
        this->mouse_world_position.x / this->cell_size,
        this->mouse_world_position.y / this->cell_size};
    this->mouse_data_cached = true;
}

Rect ECS::Map::get_hovered_grid_cell()
{
    if (!this->hovered_cell_cached)
    {
        if (!this->mouse_data_cached)
        {
            this->recalculate_mouse_positions();
        }
        Rect *camera = Window::get_camera();
        this->hovered_grid_cell = {
            (static_cast<int>(this->mouse_grid_position.x) * this->cell_size) - camera->x,
            (static_cast<int>(this->mouse_grid_position.y) * this->cell_size) - camera->y,
            this->cell_size,
            this->cell_size};
        this->hovered_cell_cached = true;
    }
    return this->hovered_grid_cell;
}

V2 ECS::Map::get_mouse_grid_position()
{
    if (!this->mouse_data_cached)
    {
        this->recalculate_mouse_positions();
    }
    return this->mouse_grid_position;
};
V2 ECS::Map::get_mouse_world_position()
{
    if (!this->mouse_data_cached)
    {
        this->recalculate_mouse_positions();
    }
    return this->mouse_world_position;
};

// Systems

bool ECS::render_system(Entity *e)
{
    if ((e->component_flags & RENDER_FLAGS) == RENDER_FLAGS)
    {
        ECS::Component *render_ptr = e->get_component(ECS::Type::RENDER);
        ECS::Component *position_ptr = e->get_component(ECS::Type::POSITION);
        assert(render_ptr != nullptr && position_ptr != nullptr);
        auto render_component = render_ptr->data.r;
        auto position_component = position_ptr->data.p;
        if (!render_component.has_clip)
        {
            render_component.clip = {};
        }
        Rect *camera = Window::get_camera();
        Rect entity_rect = {position_component.position.x, position_component.position.y, render_component.clip.w, render_component.clip.h};
        if (Physics::check_collision(camera, &entity_rect))
        {
            V2 render_position = {position_component.position.x - camera->x, position_component.position.y - camera->y};
            Render::render_texture(
                render_component.layer,
                render_component.texture_index,
                render_component.clip,
                render_position,
                nullptr,
                render_component.scale,
                render_component.z_index);
            return true;
        }
    }
    return false;
}

void ECS::camera_system(Entity *e)
{
    auto camera_ptr = e->get_component(ECS::Type::CAMERA);
    auto position_ptr = e->get_component(ECS::Type::POSITION);
    if (camera_ptr != nullptr && position_ptr != nullptr)
    {
        auto position = position_ptr->data.p.position;
        Window::set_camera_position(position);
    }
}

void ECS::input_system(ECS::Map *map, Entity *e, double ts)
{
    double speed = 500;
    auto player_input_ptr = e->get_component(ECS::Type::PLAYER_INPUT);
    auto position_ptr = e->get_component(ECS::Type::POSITION);
    if (player_input_ptr != nullptr && position_ptr != nullptr)
    {
        auto w_pressed = Input::is_input_active(Input::Event::W_KEY_DOWN);
        auto a_pressed = Input::is_input_active(Input::Event::A_KEY_DOWN);
        auto s_pressed = Input::is_input_active(Input::Event::S_KEY_DOWN);
        auto d_pressed = Input::is_input_active(Input::Event::D_KEY_DOWN);
        auto position = &position_ptr->data.p.position;
        int vel = speed * ts;
        if (w_pressed)
        {
            position->y -= vel;
        }
        if (s_pressed)
        {
            position->y += vel;
        }
        if (a_pressed)
        {
            position->x -= vel;
        }
        if (d_pressed)
        {
            position->x += vel;
        }

        V2 *window = Window::get_window();
        Rect *camera = Window::get_camera();
        V2 window_dimensions_with_camera_zoom = {
            window->x - (window->x - camera->w),
            window->y - (window->y - camera->h)};

        if (position->x < 0)
        {
            position->x = 0;
        }
        if (position->x + window_dimensions_with_camera_zoom.x > map->pixel_dimensions.x)
        {
            position->x = map->pixel_dimensions.x - window_dimensions_with_camera_zoom.x;
        }
        if (position->y < 0)
        {
            position->y = 0;
        }
        if (position->y + window_dimensions_with_camera_zoom.y > map->pixel_dimensions.y)
        {
            position->y = map->pixel_dimensions.y - window_dimensions_with_camera_zoom.y;
        }
        if (window_dimensions_with_camera_zoom.x > map->pixel_dimensions.x)
        {
            position->x = -((window_dimensions_with_camera_zoom.x - map->pixel_dimensions.x) / 2);
        }
        if (window_dimensions_with_camera_zoom.y > map->pixel_dimensions.y)
        {
            position->y = -((window_dimensions_with_camera_zoom.y - map->pixel_dimensions.y) / 2);
        }
    }
}

void ECS::process_map(ECS::Map *m, double ts)
{
    int tiles_rendered = 0;
    for (int i = 0; i < m->dimensions.x; ++i)
    {
        for (int j = 0; j < m->dimensions.y; ++j)
        {
            ECS::Cell *cell = &m->grid[i][j];
            if (!cell->tile.empty)
            {
                if (ECS::render_system(&cell->tile.tile_entity))
                {
                    ++tiles_rendered;
                }
            }
        }
    }
    {
        // DEBUG
        MBus::Message debug;
        debug.type = MBus::TILES_RENDERED;
        debug.data.er.num = tiles_rendered;
        MBus::send_debug_message(&debug);
    }
}

ECS::Manager::Manager() : player_entity_index(-1){};

void ECS::Manager::update_player(double ts)
{
    if (this->player_entity_index == -1)
    {
        for (unsigned int i = 0; i < this->entities.size(); ++i)
        {
            Entity *e = &this->entities[i];
            auto input_component_ptr = e->get_component(ECS::Type::PLAYER_INPUT);
            auto camera_component_ptr = e->get_component(ECS::Type::CAMERA);
            if (input_component_ptr != nullptr && camera_component_ptr != nullptr)
            {
                this->player_entity_index = i;
                break;
            }
        }
        if (this->player_entity_index == -1)
        {
            printf("WARNING: Could not find player entity\n");
            return;
        }
    }
    ECS::input_system(&this->map, &this->entities[this->player_entity_index], ts);
    ECS::camera_system(&this->entities[this->player_entity_index]);
}

void ECS::Manager::update(double ts)
{
    int entities_rendered = 0;
    this->map.update(ts);
    for (Entity &e : this->entities)
    {
        if (ECS::render_system(&e))
        {
            ++entities_rendered;
        }
    }
    {
        // DEBUG
        MBus::Message debug;
        debug.type = MBus::ENTITIES_RENDERED;
        debug.data.er.num = entities_rendered;
        MBus::send_debug_message(&debug);

        debug.type = MBus::ENTITIES_PROCESSED;
        debug.data.er.num = this->entities.size();
        MBus::send_debug_message(&debug);
    }
};

void ECS::Manager::process_messages()
{
    MBus::MessageQueue queue = MBus::get_queue(MBus::QueueType::ECS);
    for (int i = 0; i < queue.length; ++i)
    {
        MBus::Message message = queue.queue[i];
        if (message.type == MBus::CREATE_PLANT_ENTITY)
        {
            Entity e;
            Component render_component;
            render_component.type = RENDER;
            render_component.strings.push_back("tilesheet-transparent");
            render_component.data.r = {
                {32, 0, 32, 32},
                Render::Layer::WORLD_LAYER,
                Assets::get_texture_index("tilesheet-transparent"),
                static_cast<int>(render_component.strings.size() - 1),
                1,
                1,
                true};
            Component position_component;
            position_component.type = POSITION;
            position_component.data.p = {message.data.cpe.grid_position.x * 32, message.data.cpe.grid_position.y * 32};
            e.add_component(&position_component);
            e.add_component(&render_component);
            this->map.grid[message.data.cpe.grid_position.x][message.data.cpe.grid_position.y].has_entity = true;
            this->map.grid[message.data.cpe.grid_position.x][message.data.cpe.grid_position.y].entity_id = this->entities.size();
            this->entities.push_back(e);
        }
        else if (message.type == MBus::CREATE_TILE)
        {
            V2 grid_position = message.data.ct.grid_position;
            assert(grid_position.x >= 0 &&
                   grid_position.x < static_cast<int>(this->map.dimensions.x) &&
                   grid_position.y >= 0 &&
                   grid_position.y < static_cast<int>(this->map.dimensions.y));
            Entity tile_entity;
            Component render_component;
            render_component.type = ECS::RENDER;
            render_component.strings.push_back("tilesheet-transparent");
            render_component.data.r = {
                {0, 0, 32, 32},
                Render::WORLD_LAYER,
                Assets::get_texture_index("tilesheet-transparent"),
                static_cast<int>(render_component.strings.size() - 1),
                1,
                0,
                true};
            Component position_component;
            position_component.type = ECS::POSITION;
            position_component.data.p.position = {
                grid_position.x * this->map.cell_size,
                grid_position.y * this->map.cell_size};
            tile_entity.add_component(&render_component);
            tile_entity.add_component(&position_component);
            this->map.grid[grid_position.x][grid_position.y].tile.empty = false;
            this->map.grid[grid_position.x][grid_position.y].tile.tile_entity = tile_entity;
        }
        else if (message.type == MBus::HANDLE_CAMERA_RESIZE_FOR_PLAYER)
        {
            if (this->player_entity_index != -1)
            {
                Entity *player = &this->entities[this->player_entity_index];
                auto player_position_ptr = player->get_component(ECS::Type::POSITION);
                if (player_position_ptr != nullptr)
                {
                    V2 old_camera_dimensions = message.data.hcrfp.old_camera_dimensions;
                    V2 new_camera_dimensions = message.data.hcrfp.new_camera_dimensions;
                    V2 *current_player_position = &player_position_ptr->data.p.position;
                    *current_player_position = {
                        current_player_position->x + (old_camera_dimensions.x - new_camera_dimensions.x) / 2,
                        current_player_position->y + (old_camera_dimensions.y - new_camera_dimensions.y) / 2};
                }
            }
        }
    }
}

void jsonize_component_strings(picojson::object *object, ECS::Component *component)
{
    assert(component != nullptr);
    assert(object != nullptr);
    if (component->strings.size() > 0)
    {
        picojson::array strings_array;
        for (std::string &s : component->strings)
        {
            strings_array.push_back(picojson::value(s));
        }
        (*object)["strings"] = picojson::value(strings_array);
    }
}

std::vector<std::string> componentize_json_strings(picojson::object *object)
{
    assert(object != nullptr);
    std::vector<std::string> strings;
    if ((*object)["strings"].is<picojson::array>())
    {
        picojson::array strings_array = (*object)["strings"].get<picojson::array>();
        for (picojson::value::array::const_iterator obj_it = strings_array.begin(); obj_it != strings_array.end(); ++obj_it)
        {
            if (obj_it->is<std::string>())
            {
                strings.push_back(obj_it->get<std::string>());
            }
            else
            {
                printf("ERROR: componentize_json_strings 'strings' array contains non-string item\n");
            }
        }
    }
    return strings;
}

picojson::object ECS::jsonize_component(Type type, Component *component)
{
    picojson::object component_object;
    switch (type)
    {
    case ECS::NUM_COMPONENT_TYPES:
    {
        break;
    }
    case ECS::Type::PLAYER_INPUT:
    {
        component_object["type"] = picojson::value("PLAYER_INPUT");
        break;
    }
    case ECS::Type::POSITION:
    {
        component_object["type"] = picojson::value("POSITION");
        component_object["x"] = picojson::value((double)component->data.p.position.x);
        component_object["y"] = picojson::value((double)component->data.p.position.y);
        break;
    }
    case ECS::Type::CAMERA:
    {
        component_object["type"] = picojson::value("CAMERA");
        break;
    }
    case ECS::Type::DUMB_AI_COMPONENT:
    {
        component_object["type"] = picojson::value("DUMB_AI_COMPONENT");
        break;
    }
    case ECS::Type::POSITION_ANIMATE:
    {
        component_object["type"] = picojson::value("POSITION_ANIMATE");
        break;
    }
    case ECS::Type::RENDER:
    {
        component_object["type"] = picojson::value("RENDER");
        component_object["has_clip"] = picojson::value(component->data.r.has_clip);
        component_object["layer"] = picojson::value((double)component->data.r.layer);
        component_object["scale"] = picojson::value((double)component->data.r.scale);
        component_object["texture_key_strings_index"] = picojson::value((double)component->data.r.texture_key_strings_index);
        component_object["z_index"] = picojson::value((double)component->data.r.z_index);
        picojson::object clip;
        clip["x"] = picojson::value((double)component->data.r.clip.x);
        clip["y"] = picojson::value((double)component->data.r.clip.y);
        clip["w"] = picojson::value((double)component->data.r.clip.w);
        clip["h"] = picojson::value((double)component->data.r.clip.h);
        component_object["clip"] = picojson::value(clip);
        break;
    }
    }
    jsonize_component_strings(&component_object, component);
    return component_object;
}

ECS::ComponentizeJsonResult ECS::componentize_json(picojson::object *object)
{
    assert(object != nullptr);
    ECS::ComponentizeJsonResult result;
    result.success = false;
    if (!(*object)["type"].is<std::string>())
    {
        printf("JSON serialize error: 'type' field not set\n");
        return result;
    }
    std::string type = (*object)["type"].get<std::string>();
    if (type == "PLAYER_INPUT")
    {
        result.component.type = ECS::PLAYER_INPUT;
        result.success = true;
    }
    else if (type == "POSITION")
    {
        result.component.type = ECS::POSITION;
        if ((*object)["x"].is<double>() && (*object)["y"].is<double>())
        {
            result.component.data.p.position = {
                static_cast<int>((*object)["x"].get<double>()),
                static_cast<int>((*object)["y"].get<double>())};
            result.success = true;
        }
        else
        {
            printf("JSON serialize error: position is missing 'x' or 'y' field on POSITION\n");
        }
    }
    else if (type == "CAMERA")
    {
        result.component.type = ECS::CAMERA;
        result.success = true;
    }
    else if (type == "DUMB_AI_COMPONENT")
    {
        result.component.type = ECS::DUMB_AI_COMPONENT;
        result.success = true;
    }
    else if (type == "POSITION_ANIMATE")
    {
        result.component.type = ECS::POSITION_ANIMATE;
        assert(false && "POSITION_ANIMATE componentization not implemented!");
    }
    else if (type == "RENDER")
    {
        result.component.type = ECS::RENDER;
        picojson::object render_comp_obj = *object;
        if (render_comp_obj["clip"].is<picojson::object>() && render_comp_obj["layer"].is<double>() &&
            render_comp_obj["texture_key_strings_index"].is<double>() &&
            render_comp_obj["scale"].is<double>() && render_comp_obj["z_index"].is<double>() &&
            render_comp_obj["has_clip"].is<bool>())
        {
            picojson::object clip = render_comp_obj["clip"].get<picojson::object>();
            if (clip["x"].is<double>() &&
                clip["y"].is<double>() &&
                clip["w"].is<double>() &&
                clip["h"].is<double>())
            {
                Render::Layer layer = (Render::Layer)render_comp_obj["layer"].get<double>();
                int texture_key_strings_index = static_cast<int>(render_comp_obj["texture_key_strings_index"].get<double>());
                int scale = static_cast<int>(render_comp_obj["scale"].get<double>());
                int z_index = static_cast<int>(render_comp_obj["z_index"].get<double>());
                int has_clip = static_cast<int>(render_comp_obj["has_clip"].get<bool>());
                Rect _clip = {
                    static_cast<int>(clip["x"].get<double>()),
                    static_cast<int>(clip["y"].get<double>()),
                    static_cast<int>(clip["w"].get<double>()),
                    static_cast<int>(clip["h"].get<double>())};
                result.component.strings = componentize_json_strings(&render_comp_obj);
                result.component.data.r.clip = _clip;
                result.component.data.r.has_clip = has_clip;
                result.component.data.r.layer = layer;
                result.component.data.r.scale = scale;
                result.component.data.r.texture_key_strings_index = texture_key_strings_index;
                result.component.data.r.texture_index = Assets::get_texture_index(result.component.strings[texture_key_strings_index]);
                result.component.data.r.z_index = z_index;
                result.success = true;
            }
            else
            {
                printf("JSON serialize error: RENDER component has malformed 'clip' field\n");
            }
        }
        else
        {
            printf("JSON serialize error: RENDER component missing field\n");
        }
    }
    else
    {
        printf("JSON serialize error: Unrecognized type %s\n", type.c_str());
    }
    return result;
}