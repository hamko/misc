#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <fcntl.h>

void write_key_event(int code, int value, int fd)
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


int main(void)
{
    int fd = open("/dev/input/event2", O_WRONLY); 

    while (1) {
        write_key_event(KEY_A, 1, fd);
        write_key_event(KEY_A, 0, fd);

        write_key_event(KEY_LEFTSHIFT, 2, fd);
        write_key_event(KEY_B, 1, fd);
        write_key_event(KEY_B, 0, fd);
        write_key_event(KEY_LEFTSHIFT, 0, fd);

        sleep(1);
    }

    return 0;
}
