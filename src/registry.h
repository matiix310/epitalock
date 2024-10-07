#ifndef REGISTRY_H
#define REGISTRY_H

#include "wayland-client.h"

struct state {
    struct wl_compositor *compositor;
    struct ext_session_lock_manager_v1 *session_lock_manager;
    struct wl_shm *shm;
};

int setup_registry(struct state *state);

#endif //REGISTRY_H
