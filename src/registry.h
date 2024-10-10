#ifndef REGISTRY_H
#define REGISTRY_H

#include <wayland-client.h>

#include "monitor.h"

struct state
{
    struct wl_compositor *compositor;
    struct ext_session_lock_manager_v1 *session_lock_manager;
    struct wl_shm *shm;
    struct wl_display *display;
};

extern struct state state;
extern size_t monitor_count;
extern struct monitor monitors[MAX_MONITORS];

int setup_registry();

#endif // REGISTRY_H
