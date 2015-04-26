#include <stdio.h>
#include <stdlib.h>
#include <linux/input.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>

int fd;

void get(int& value, int& code) 
{
    fd_set set;
    /* Initialize the file descriptor set. */
    FD_ZERO (&set);
    FD_SET (fd, &set);

    int available = TEMP_FAILURE_RETRY (select (FD_SETSIZE, &set, NULL, NULL, NULL));
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
    // Refer
    if (event.type == EV_KEY) { // EV_KEY: Keyboard, EV_REL: Mouse movement, EV_ABS: joystick/touch panel
        value = event.value;
        code = event.code;
    }
}


int main(void)
{
    fd = open("/dev/input/event2", O_RDWR); // non-blocking

    while (1) {
        int v, c;
        get(v, c);
        if (v != -1)
            printf("%d %d\n", v, c);
    }

}
