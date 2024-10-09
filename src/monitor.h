#ifndef MONITOR_H
#define MONITOR_H

#include <wayland-client.h>

#define MAX_NAME_SIZE 512
#define MAX_MONITORS 42

struct monitor {
    int x, y;
    int width, height;
    int refresh_rate, scale;
    char name[MAX_NAME_SIZE];
    struct wl_output *wl_output;
};

size_t getMonitors(struct monitor **monitorList);
extern const struct wl_output_listener monitor_listener;

#endif //MONITOR_H
