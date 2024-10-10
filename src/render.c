#include "monitor.h"
#include "surface.h"

void render(struct monitor *monitor)
{
    wl_surface_attach(monitor->custom_surface->surface,
                      monitor->custom_surface->buffer, 0, 0);
    wl_surface_damage(monitor->custom_surface->surface, 0, 0, UINT32_MAX,
                      UINT32_MAX);
    wl_surface_commit(monitor->custom_surface->surface);
}
