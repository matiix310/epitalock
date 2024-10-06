#ifndef SESSION_LOCKER_H
#define SESSION_LOCKER_H

#include "registry.h"

void lock_session(const struct state *state);
void unlock_session();

#endif //SESSION_LOCKER_H
