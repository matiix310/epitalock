#ifndef SURFACE_H
#define SURFACE_H

#include <stddef.h>

#include "ext-session-lock.h"
#include "src/monitor.h"

extern struct state state;

struct custom_surface
{
    struct wl_surface *surface;
    struct ext_session_lock_surface_v1 *session_lock_surface;
    struct wl_buffer *buffer;
};

int allocate_shm_file(size_t size);
void create_surface(struct monitor *monitor);

#endif // SURFACE_H
