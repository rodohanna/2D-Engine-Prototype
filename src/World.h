#ifndef WORLD_h_
#define WORLD_h_

#include "GameTypes.h"
#include "MapGen.h"
#include "Entity.h"
#include "EventBus.h"
#include <vector>
#include <memory>

struct Chunk
{
    V2 world_coords;
    Palette palette;
    ProcGenRules rules;
    std::vector<std::shared_ptr<IEntity>> entities;
};

struct World
{
    World(Palette &p, V2 &dimensions, size_t chunk_size);
    void generate_world(); // needs proc gen rules
    V2 dimensions;
    std::shared_ptr<Palette> palette;
    std::vector<std::vector<Chunk>> world_chunks;
};

struct ChunkManager : IDebugEventSubscriber
{
    ChunkManager(EventBus *e, World &world, size_t chunk_size);
    ~ChunkManager();
    void sync_chunks_to_world_position(V2 &world_position, bool force = false);
    void update_chunks(double ts);
    void handle_debug_events(const Events::DebugEvent *, size_t);
    // Active chunks include the chunk the player
    // is currently at, as well as all of the surrounding
    // chunks.
    Chunk *chunk_add_buffer[10];
    World world;
    V2 last_world_position_sync;
    EventBus *event_bus;
    std::vector<Chunk> active_chunks;
    size_t chunk_size;

    // debug flags
    bool DEBUG_show_chunk_boundary;
};

#endif