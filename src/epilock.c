#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "registry.h"
#include "session-locker.h"

struct state state = { 0 };

int main(void)
{
    sleep(1);

    // get all the objects from the wayland server
    if (!setup_registry())
        errx(EXIT_FAILURE,
             "Can't fetch the required interfaces from your wayland server!");

    // lock the session
    sleep(2);
    printf("Locking the session\n");
    lock_session(&state);

    // unlock the session
    sleep(5);
    unlock_session();
    printf("Unlocking the session\n");

    // ending the program
    sleep(2);
}
