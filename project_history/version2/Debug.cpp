#include "Debug.h"

#include <unordered_map>

std::unordered_map<Events::DebugEventType, bool> debug_map;

bool Debug::check_debug_event(Events::DebugEventType type)
{
    if (debug_map.find(type) != debug_map.end())
    {
        return debug_map[type];
    }
    return false;
}

void Debug::handle_debug_events(Events::DebugEvent *debug_events, size_t length)
{
    for (size_t i = 0; i < length; ++i)
    {
        Events::DebugEvent e = debug_events[i];
        switch (e.type)
        {
        case Events::DebugEventType::SHOW_CHUNK_BOUNDARY:
        {
            debug_map[Events::DebugEventType::SHOW_CHUNK_BOUNDARY] = true;
            break;
        }
        case Events::DebugEventType::HIDE_CHUNK_BOUNDARY:
        {
            debug_map[Events::DebugEventType::SHOW_CHUNK_BOUNDARY] = false;
            break;
        }
        case Events::DebugEventType::SHOW_TILE_GRID:
        {
            debug_map[Events::DebugEventType::SHOW_TILE_GRID] = true;
            break;
        }
        case Events::DebugEventType::HIDE_TILE_GRID:
        {
            debug_map[Events::DebugEventType::SHOW_TILE_GRID] = false;
            break;
        }
        }
    }
}