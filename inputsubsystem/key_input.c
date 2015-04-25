#include <stdio.h>
#include <stdlib.h>
#include <linux/input.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

int main(void)
{
    //      int fd = open("/dev/input/event0", O_RDONLY); // blocking
    int fd = open("/dev/input/event2", O_NONBLOCK); // non-blocking

    while (1) {
        // Read
        struct input_event event;

        int a = read(fd, &event, sizeof(event));
//        printf("%d\n", a);
        if (a < 0 && errno == EAGAIN) {
            // No Input
        } else if (a < 0) {
            fprintf(stderr, "read error\n");
            return 1;
        } else {
            // Refer
            if (event.type == EV_KEY) // EV_KEY: Keyboard, EV_REL: Mouse movement, EV_ABS: joystick/touch panel
                if (event.value == 1)  // 1: pushed, 0: released, 2: machinegun
                    printf("%d pushed\n", event.code); // code: info to detect!
        }
    }

}
