#ifndef SESSION_LOCKER_H
#define SESSION_LOCKER_H

#include "monitor.h"
#include "registry.h"

extern struct state state;
extern size_t monitor_count;
extern struct monitor monitors[MAX_MONITORS];

void lock_session(const struct state *state);
void unlock_session();

#endif // SESSION_LOCKER_H
