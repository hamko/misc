#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <fcntl.h>

void write_mouse_xy(int rel_x, int rel_y, int fd)
{
    struct input_event event;
    if (fd <= 0) {
        return;
    }
    gettimeofday(&event.time, NULL);
    event.type = EV_REL;
    event.code = REL_X;
    event.value = rel_x;
    write(fd, &event, sizeof(event));

    event.type = EV_REL;
    event.code = REL_Y;
    event.value = rel_y;
    write(fd, &event, sizeof(event));

    event.type = EV_SYN;
    event.code = SYN_REPORT;
    event.value = 0;
    write(fd, &event, sizeof(event));
}

void write_mouse_click(int button, int value, int fd)
{
    struct input_event event;
    if (fd <= 0) {
        return;
    }
    gettimeofday(&event.time, NULL);
    event.type = EV_KEY;
    event.code = button;
    event.value = value;
    write(fd, &event, sizeof(event));

    event.type = EV_SYN;
    event.code = SYN_REPORT;
    event.value = 0;
    write(fd, &event, sizeof(event));
}


int main(void)
{
    int fd = open("/dev/input/event3", O_WRONLY);

    while (1) {
        int d = 10;
        write_mouse_xy(d, 0, fd); usleep(30000);
        write_mouse_xy(0, d, fd); usleep(30000);
        write_mouse_xy(-d, 0, fd); usleep(30000);
        write_mouse_xy(0, -d, fd); usleep(30000);

        write_mouse_click(BTN_LEFT, 1, fd);
        write_mouse_click(BTN_LEFT, 0, fd); // LEFTの他にMIDDLE, RIGHTなど
    }

    return 0;
}
