#include "World.h"
#include "Window.h"
#include "Physics.h"
#include "Debug.h"
#include <sstream>

ChunkManager::ChunkManager(EventBus *e, World &world, size_t chunk_size) : world(world), event_bus(e), chunk_size(chunk_size)
{
    this->coordinate_text = std::unique_ptr<Text>(new Text(e, 1, "debug-coordinate-text"));
};

ChunkManager::~ChunkManager()
{
}

void ChunkManager::sync_chunks_to_world_position(V2 &world_position, bool force)
{
    int x_chunk = world_position.x / (int)(this->chunk_size * 16);
    int y_chunk = world_position.y / (int)(this->chunk_size * 16);
    if (last_world_position_sync.x == x_chunk && last_world_position_sync.y == y_chunk && !force)
    {
        return;
    }
    printf("World Position: %d %d\n", x_chunk, y_chunk);
    if (x_chunk < 0 || x_chunk > this->world.dimensions.x - 1 || y_chunk < 0 || y_chunk > this->world.dimensions.y - 1)
    {
        return;
    }
    last_world_position_sync.x = x_chunk;
    last_world_position_sync.y = y_chunk;

    // chunks that need to be loaded
    V2 top_left_chunk = {x_chunk - 1, y_chunk - 1};
    V2 top_chunk = {x_chunk, y_chunk - 1};
    V2 top_right_chunk = {x_chunk + 1, y_chunk - 1};
    V2 left_chunk = {x_chunk - 1, y_chunk};
    V2 active_player_chunk = {x_chunk, y_chunk};
    V2 right_chunk = {x_chunk + 1, y_chunk};
    V2 bottom_left_chunk = {x_chunk - 1, y_chunk + 1};
    V2 bottom_chunk = {x_chunk, y_chunk + 1};
    V2 bottom_right_chunk = {x_chunk + 1, y_chunk + 1};

    size_t chunk_add_buffer_index = 0;
    this->chunk_add_buffer[chunk_add_buffer_index] = &this->world.world_chunks[active_player_chunk.x][active_player_chunk.y];
    ++chunk_add_buffer_index;
    if (top_left_chunk.x >= 0 && top_left_chunk.y >= 0)
    {
        this->chunk_add_buffer[chunk_add_buffer_index] = &this->world.world_chunks[top_left_chunk.x][top_left_chunk.y];
        ++chunk_add_buffer_index;
    }
    if (top_chunk.y >= 0)
    {
        this->chunk_add_buffer[chunk_add_buffer_index] = &this->world.world_chunks[top_chunk.x][top_chunk.y];
        ++chunk_add_buffer_index;
    }
    if (top_right_chunk.x < this->world.dimensions.x && top_right_chunk.y >= 0)
    {
        this->chunk_add_buffer[chunk_add_buffer_index] = &this->world.world_chunks[top_right_chunk.x][top_right_chunk.y];
        ++chunk_add_buffer_index;
    }
    if (left_chunk.x >= 0)
    {
        this->chunk_add_buffer[chunk_add_buffer_index] = &this->world.world_chunks[left_chunk.x][left_chunk.y];
        ++chunk_add_buffer_index;
    }
    if (right_chunk.x < this->world.dimensions.x)
    {
        this->chunk_add_buffer[chunk_add_buffer_index] = &this->world.world_chunks[right_chunk.x][right_chunk.y];
        ++chunk_add_buffer_index;
    }
    if (bottom_left_chunk.x >= 0 && bottom_left_chunk.y < this->world.dimensions.y)
    {
        this->chunk_add_buffer[chunk_add_buffer_index] = &this->world.world_chunks[bottom_left_chunk.x][bottom_left_chunk.y];
        ++chunk_add_buffer_index;
    }
    if (bottom_chunk.y < this->world.dimensions.y)
    {
        this->chunk_add_buffer[chunk_add_buffer_index] = &this->world.world_chunks[bottom_chunk.x][bottom_chunk.y];
        ++chunk_add_buffer_index;
    }
    if (bottom_right_chunk.x < this->world.dimensions.x && bottom_right_chunk.y < this->world.dimensions.y)
    {
        this->chunk_add_buffer[chunk_add_buffer_index] = &this->world.world_chunks[bottom_right_chunk.x][bottom_right_chunk.y];
        ++chunk_add_buffer_index;
    }

    // remove inactive chunks
    for (auto it = this->active_chunks.begin(); it != this->active_chunks.end();)
    {
        Chunk &chunk = *it;
        if (std::abs((chunk.world_coords.x / (int)this->chunk_size) - x_chunk) > 1 || std::abs((chunk.world_coords.y / (int)this->chunk_size) - y_chunk) > 1)
        {
            printf("Unloading chunk at: %d %d\n", chunk.world_coords.x / this->chunk_size, chunk.world_coords.y / (int)this->chunk_size);
            this->active_chunks.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // add new chunks
    for (size_t i = 0; i < chunk_add_buffer_index; ++i)
    {
        Chunk *chunk = this->chunk_add_buffer[i];
        bool chunk_already_active = false;
        for (Chunk &c : this->active_chunks)
        {
            if (c.world_coords.x == chunk->world_coords.x && c.world_coords.y == chunk->world_coords.y)
            {
                chunk_already_active = true;
                break;
            }
        }
        if (!chunk_already_active)
        {
            printf("Loading chunk at: %d %d\n",
                   chunk->world_coords.x / this->chunk_size,
                   chunk->world_coords.y / this->chunk_size);
            if (chunk->map.grid.size() == 0)
            {
                chunk->map = MapGen::generate_map(
                    &chunk->palette,
                    &chunk->rules,
                    this->event_bus,
                    {static_cast<int>(this->chunk_size), static_cast<int>(this->chunk_size)},
                    chunk->world_coords);
                printf("Generating chunk\n");
            }
            else
            {
                printf("Used cached chunk\n");
            }
            this->active_chunks.push_back(*chunk);
        }
    }
}

void ChunkManager::update_chunks(double ts)
{
    bool show_chunk_boundary = Debug::check_debug_event(Events::DebugEventType::SHOW_CHUNK_BOUNDARY);
    bool show_tile_grid = Debug::check_debug_event(Events::DebugEventType::SHOW_TILE_GRID);
    Rect *camera = Window::get_camera();
    for (Chunk &chunk : this->active_chunks)
    {
        if (show_chunk_boundary)
        {
            this->event_bus->publish_render_event(
                Events::create_render_rectangle_event(
                    Events::RenderLayer::WORLD_LAYER,
                    {static_cast<int>((chunk.world_coords.x * 16) - Window::get_camera()->x),
                     static_cast<int>((chunk.world_coords.y * 16) - Window::get_camera()->y),
                     static_cast<int>((16 * this->chunk_size) + 1),
                     static_cast<int>((16 * this->chunk_size) + 1)},
                    {0xFF, 0xFF, 0xFF, 0xFF},
                    false,
                    3));
        }
        if (chunk.map.grid.size() > 0)
        {
            chunk.map.background->update(ts);
            for (size_t i = 0; i < chunk.map.grid.size(); ++i)
            {
                for (size_t j = 0; j < chunk.map.grid[i].size(); ++j)
                {
                    if (chunk.map.grid[i][j] != nullptr)
                    {
                        chunk.map.grid[i][j]->update(ts);
                    }
                    Rect r = {
                        static_cast<int>((i + chunk.world_coords.x) * 16),
                        static_cast<int>((j + chunk.world_coords.y) * 16),
                        16,
                        16};
                    if (Physics::check_collision(camera, &r))
                    {
                        Rect mouse_rect = {r.x - camera->x, r.y - camera->y, 16, 16};
                        if (Physics::check_point_in_rect(Window::get_mouse_position(), &mouse_rect))
                        {
                            this->event_bus->publish_render_event(
                                Events::create_render_rectangle_event(
                                    Events::RenderLayer::WORLD_LAYER,
                                    {r.x - camera->x, r.y - camera->y, r.w + 1, r.h + 1},
                                    {0xFF, 0xFF, 0xFF, 0xFF},
                                    false,
                                    3));
                            if (show_tile_grid)
                            {
                                std::stringstream ss("");
                                ss << i + chunk.world_coords.x << "," << j + chunk.world_coords.y;
                                this->coordinate_text->set_text(ss.str());
                                this->coordinate_text->overflow_clip = {0, 0, 800, 800};
                                this->coordinate_text->position = {
                                    (r.x + (r.w - this->coordinate_text->dimensions.x) / 2) - camera->x,
                                    (r.y - (this->coordinate_text->dimensions.y)) - camera->y};
                                this->coordinate_text->update(ts, 3, Events::RenderLayer::WORLD_LAYER);
                            }
                        }
                        if (show_tile_grid)
                        {
                            this->event_bus->publish_render_event(
                                Events::create_render_rectangle_event(
                                    Events::RenderLayer::WORLD_LAYER,
                                    {r.x - camera->x, r.y - camera->y, r.w + 1, r.h + 1},
                                    {0x00, 0x00, 0x00, 0xFF},
                                    false,
                                    2));
                        }
                    }
                }
            }
        }
    }
}

World::World(Palette &p, V2 &dimensions, size_t chunk_size) : dimensions(dimensions)
{
    this->palette = std::make_unique<Palette>(p);

    for (int i = 0; i < dimensions.x; ++i)
    {
        std::vector<Chunk> chunks;
        for (int j = 0; j < dimensions.y; j++)
        {
            chunks.push_back({{static_cast<int>(i * chunk_size), static_cast<int>(j * chunk_size)}});
        }
        this->world_chunks.push_back(chunks);
    }
}

void World::generate_world()
{
    for (std::vector<Chunk> &row : this->world_chunks)
    {
        for (Chunk &chunk : row)
        {
            chunk.rules = {static_cast<size_t>(rand() % 20 + 1), static_cast<size_t>(rand() % 20 + 1)};
            chunk.palette = *this->palette.get();
        }
    }
}