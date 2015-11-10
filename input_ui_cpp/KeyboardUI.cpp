#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>

#include "KeyboardUI.hpp"

KeyboardUI::KeyboardUI(int fd_in) : fd(fd_in)
{
}

void KeyboardUI::type(int value, int code)
{
    struct input_event key_event;

    gettimeofday(&key_event.time, NULL);
    key_event.type = EV_KEY;
    key_event.code = code;
    key_event.value = value;
    write(fd, &key_event, sizeof(key_event));

    key_event.type = EV_SYN;
    key_event.code = SYN_REPORT;
    key_event.value = 0;
    write(fd, &key_event, sizeof(key_event));
}

void KeyboardUI::get(int& value, int& code) 
{
    fd_set set;
    struct timeval timeout;
    /* Initialize the file descriptor set. */
    FD_ZERO (&set);
    FD_SET (fd, &set);

    /* Initialize the timeout data structure. */
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    int available = TEMP_FAILURE_RETRY (select (FD_SETSIZE, &set, NULL, NULL, &timeout));
    if (available == -1) {
        perror("select");
        value = -1;
        return;
    }
    if (available == 0) {
        value = -1;
        return;
    }

    // Read
    struct input_event event;

    int a = read(fd, &event, sizeof(event));
    if (a < 0) {
        perror("read");
        value = -1;
        return;
    }

    if (event.type == EV_KEY) {
        value = event.value;
        code = event.code;
    } else { // REPORT避け
        value = -1;
        code = -1;
    }
}


