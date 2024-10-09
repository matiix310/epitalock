#include "session-locker.h"

#include <stdio.h>
#include <string.h>
#include <wayland-client.h>
#include <sys/mman.h>

#include "ext-session-lock.h"
#include "monitor.h"
#include "surface.h"

static struct ext_session_lock_v1 *session_lock = NULL;

static void HandleSessionLockLocked(void *data, struct ext_session_lock_v1 *ext_session_lock_v1) {
    printf("LOCKED\n");
}

static void HandleSessionLockFinished(void *data, struct ext_session_lock_v1 *ext_session_lock_v1) {
    printf("FINISHED\n");
}

static struct ext_session_lock_v1_listener session_lock_listener = {
    .locked = HandleSessionLockLocked,
    .finished = HandleSessionLockFinished
};

void lock_session(const struct state *state) {
    session_lock = ext_session_lock_manager_v1_lock(state->session_lock_manager);

    ext_session_lock_v1_add_listener(session_lock, &session_lock_listener, NULL);

    struct monitor *monitors;
    size_t monitor_count = getMonitors(&monitors);

    printf("MONITOR COUNT = %lu\n", monitor_count);

    // create a lock surface
    for (size_t i = 0; i < monitor_count; i++) {
        struct wl_surface *surface = wl_compositor_create_surface(state->compositor);

        const int width = monitors[i].width, height = monitors[i].height;
        const int stride = width * 4;
        const int shm_pool_size = height * stride * 2;

        int fd = allocate_shm_file(shm_pool_size);
        uint8_t *pool_data = mmap(NULL, shm_pool_size,
            PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

        struct wl_shm_pool *pool = wl_shm_create_pool(state->shm, fd, shm_pool_size);

        int index = 0;
        int offset = height * stride * index;
        struct wl_buffer *buffer = wl_shm_pool_create_buffer(pool, offset,
            width, height, stride, WL_SHM_FORMAT_XRGB8888);

        uint32_t *pixels = (uint32_t *)&pool_data[offset];
        memset(pixels, 0, width * height * 4);

        wl_surface_attach(surface, buffer, 0, 0);
        wl_surface_damage(surface, 0, 0, UINT32_MAX, UINT32_MAX);
        wl_surface_commit(surface);

        ext_session_lock_v1_get_lock_surface(session_lock, surface, monitors[i].wl_output);
    }
}

void unlock_session() {
    if (session_lock == NULL)
        return;

    ext_session_lock_v1_unlock_and_destroy(session_lock);
}