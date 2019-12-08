#ifndef DEBUG_h_
#define DEBUG_h_

#include "Events.h"

namespace Debug
{
void handle_debug_events(Events::DebugEvent *, size_t);
bool check_debug_event(Events::DebugEventType);
}; // namespace Debug

#endif