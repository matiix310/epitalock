#include "surface.h"

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

#include "monitor.h"
#include "registry.h"

static void randname(char *buf)
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    long r = ts.tv_nsec;
    for (int i = 0; i < 6; ++i)
    {
        buf[i] = 'A' + (r & 15) + (r & 16) * 2;
        r >>= 5;
    }
}

static int create_shm_file(void)
{
    int retries = 100;
    do
    {
        char name[] = "/wl_shm-XXXXXX";
        randname(name + sizeof(name) - 7);
        --retries;
        int fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, 0600);
        if (fd >= 0)
        {
            shm_unlink(name);
            return fd;
        }
    } while (retries > 0 && errno == EEXIST);
    return -1;
}

int allocate_shm_file(size_t size)
{
    int fd = create_shm_file();
    if (fd < 0)
        return -1;
    int ret;
    do
    {
        ret = ftruncate(fd, size);
    } while (ret < 0 && errno == EINTR);
    if (ret < 0)
    {
        close(fd);
        return -1;
    }
    return fd;
}

void create_surface(struct monitor *monitor)
{
    const int stride = monitor->width * 4;
    const int shm_pool_size = monitor->height * stride * 2;

    int fd = allocate_shm_file(shm_pool_size);
    uint8_t *pool_data =
        mmap(NULL, shm_pool_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    struct wl_shm_pool *pool = wl_shm_create_pool(state.shm, fd, shm_pool_size);

    int index = 0;
    int offset = monitor->height * stride * index;
    struct wl_buffer *buffer =
        wl_shm_pool_create_buffer(pool, offset, monitor->width, monitor->height,
                                  stride, WL_SHM_FORMAT_XRGB8888);

    uint32_t *pixels = (uint32_t *)&pool_data[offset];
    memset(pixels, 0, monitor->width * monitor->height * 4);
}
