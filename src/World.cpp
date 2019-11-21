#include "World.h"
#include "Window.h"

ChunkManager::ChunkManager(EventBus *e, World &world, size_t chunk_size) : world(world), event_bus(e), chunk_size(chunk_size){};

void ChunkManager::sync_chunks_to_world_position(V2 &world_position)
{
    int x_chunk = world_position.x / (int)(this->chunk_size * 32);
    int y_chunk = world_position.y / (int)(this->chunk_size * 32);
    if (last_world_position_sync.x == x_chunk && last_world_position_sync.y == y_chunk)
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
            if (chunk->entities.size() == 0)
            {
                chunk->entities = MapGen::generate_map(
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
    for (Chunk &chunk : this->active_chunks)
    {
        this->event_bus->publish_render_event(
            Events::create_render_rectangle_event({static_cast<int>((chunk.world_coords.x * 32) - Window::get_camera()->x),
                                                   static_cast<int>((chunk.world_coords.y * 32) - Window::get_camera()->y),
                                                   static_cast<int>(32 * this->chunk_size),
                                                   static_cast<int>(32 * this->chunk_size)},
                                                  {0xFF, 0xFF, 0xFF, 0xFF}));
        for (size_t i = 0; i < chunk.entities.size(); ++i)
        {
            chunk.entities[i].get()->update(ts);
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