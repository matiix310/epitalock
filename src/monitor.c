#include "monitor.h"

#include <assert.h>
#include <string.h>

size_t monitor_count = 0;
struct monitor monitors[MAX_MONITORS];

static void HandleMonitorGeometry(void *data, struct wl_output *wl_output,
                                  int32_t x, int32_t y, int32_t physical_width,
                                  int32_t physical_height, int32_t subpixel,
                                  const char *make, const char *model,
                                  int32_t transform)
{
    monitors[monitor_count].x = x;
    monitors[monitor_count].y = y;
}

static void HandleMonitorPixelContents(void *data, struct wl_output *wl_output,
                                       uint32_t flags, int32_t width,
                                       int32_t height, int32_t refresh)
{
    monitors[monitor_count].width = width;
    monitors[monitor_count].height = height;
}

static void HandleMonitorInformationSent(void *data,
                                         struct wl_output *wl_output)
{
    monitors[monitor_count].wl_output = wl_output;
    monitor_count++;
    assert(monitor_count < MAX_MONITORS);
}

static void HandleMonitorScale(void *data, struct wl_output *wl_output,
                               int32_t factor)
{
    monitors[monitor_count].scale = factor;
}

static void HandleMonitorName(void *data, struct wl_output *wl_output,
                              const char *name)
{
    strncpy(monitors[monitor_count].name, name, MAX_NAME_SIZE);
}

static void HandleMonitorDescription(void *data, struct wl_output *wl_output,
                                     const char *description)
{
    // No operation.
    create_surface();
}

const struct wl_output_listener monitor_listener = {
    HandleMonitorGeometry,
    HandleMonitorPixelContents,
    HandleMonitorInformationSent,
    HandleMonitorScale,
    HandleMonitorName,
    HandleMonitorDescription
};
