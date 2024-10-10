#include "session-locker.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

#include "ext-session-lock.h"
#include "monitor.h"
#include "registry.h"
#include "render.h"
#include "wayland-client-core.h"

static struct ext_session_lock_v1 *session_lock = NULL;

static void
HandleSessionLockLocked(void *data,
                        struct ext_session_lock_v1 *ext_session_lock_v1)
{
    printf("LOCKED\n");
}

static void
HandleSessionLockFinished(void *data,
                          struct ext_session_lock_v1 *ext_session_lock_v1)
{
    printf("FINISHED\n");
}

static struct ext_session_lock_v1_listener session_lock_listener = {
    .locked = HandleSessionLockLocked,
    .finished = HandleSessionLockFinished
};

static void HandleSessionLockSurfaceConfigure(
    void *data, struct ext_session_lock_surface_v1 *session_lock_surface,
    uint32_t serial, uint32_t width, uint32_t height)
{
    printf("CONFIGURE\n");

    struct monitor *monitor = data;
    monitor->width = width;
    monitor->height = height;

    ext_session_lock_surface_v1_ack_configure(session_lock_surface, serial);

    render(monitor);
}

static struct ext_session_lock_surface_v1_listener
    session_lock_surface_listener = {
        .configure = HandleSessionLockSurfaceConfigure,
    };

void lock_session(const struct state *state)
{
    session_lock =
        ext_session_lock_manager_v1_lock(state->session_lock_manager);

    ext_session_lock_v1_add_listener(session_lock, &session_lock_listener,
                                     NULL);
    wl_display_roundtrip(state->display);

    printf("MONITOR COUNT = %lu\n", monitor_count);

    // create a lock surface
    for (size_t i = 0; i < monitor_count; i++)
    {
        struct wl_surface *surface =
            wl_compositor_create_surface(state->compositor);

        monitors[i].custom_surface->session_lock_surface =
            ext_session_lock_v1_get_lock_surface(session_lock, surface,
                                                 monitors[i].wl_output);

        ext_session_lock_surface_v1_add_listener(
            monitors[i].custom_surface->session_lock_surface,
            &session_lock_surface_listener, &monitors[i]);
    }

    wl_display_roundtrip(state->display);
}

void unlock_session()
{
    if (session_lock == NULL)
        return;

    ext_session_lock_v1_unlock_and_destroy(session_lock);

    // wait for the server to unlock
    wl_display_roundtrip(state.display);

    // destroy all surfaces
    for (size_t i = 0; i < monitor_count; i++)
        ext_session_lock_surface_v1_destroy(
            monitors[i].custom_surface->session_lock_surface);
}
