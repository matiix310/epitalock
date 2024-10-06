#include "registry.h"

#include <stdio.h>
#include <string.h>
#include <wayland-client.h>

#include "monitor.h"
#include "ext-session-lock.h"

static struct wl_compositor *compositor = NULL;
static struct wl_shm *shm = NULL;
static struct ext_session_lock_manager_v1 *session_lock_manager = NULL;
static struct wl_output *output = NULL;

void registry_global_handler
(
    void *data,
    struct wl_registry *registry,
    uint32_t name,
    const char *interface,
    uint32_t version
) {
//    printf("%s\n", interface);
    if (strcmp(interface, wl_compositor_interface.name) == 0) {
        compositor = wl_registry_bind(registry, name,
            &wl_compositor_interface, version);
    } else if (strcmp(interface, wl_shm_interface.name) == 0) {
        shm = wl_registry_bind(registry, name,
            &wl_shm_interface, version);
    } else if (strcmp(interface, ext_session_lock_manager_v1_interface.name) == 0) {
        session_lock_manager = wl_registry_bind(registry, name,
            &ext_session_lock_manager_v1_interface, version);
    } else if (strcmp(interface, wl_output_interface.name) == 0) {
        struct wl_output *wl_output = wl_registry_bind(registry, name,
            &wl_output_interface, version);
        wl_output_add_listener(wl_output, &monitor_listener, NULL);
    }
}

void registry_global_remove_handler
(
    void *data,
    struct wl_registry *registry,
    uint32_t name
) {}

struct wl_registry_listener registry_listener = {
    .global = registry_global_handler,
    .global_remove = registry_global_remove_handler
};

int setup_registry(struct state *state)
{
    struct wl_display *display = wl_display_connect(NULL);
    struct wl_registry *registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, NULL);

    // wait for the "initial" set of globals to appear
    wl_display_roundtrip(display);

    state->compositor = compositor;
    state->session_lock_manager = session_lock_manager;
    state->shm = shm;

    return compositor && shm && session_lock_manager;
}