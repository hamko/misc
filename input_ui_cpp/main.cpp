#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <fcntl.h>

#include "MouseUI.hpp"
#include "KeyboardUI.hpp"

int main(void)
{
    int keyboard_fd = open("/dev/input/event2", O_RDWR);
    int mouse_fd = open("/dev/input/event3", O_RDWR);

    MouseUI* mouse = new MouseUI(mouse_fd);
    KeyboardUI* keyboard = new KeyboardUI(keyboard_fd);

    int flag = 0;
    while (1) {
        int v, c;
        keyboard->get(v, c);
        if (v == 1 && c == KEY_B)
            printf("flag: %d v: %d c: %d\n", flag, v, c);
        if (v == 0 && c == KEY_B)
            printf("flag: %d v: %d c: %d\n", flag, v, c);

        if (v == 1 && c == KEY_B) 
            flag = 1 - flag;
        if (flag) {
            mouse->click(1);
            mouse->click(0);
//            keyboard->type(1, KEY_A);
//            keyboard->type(0, KEY_A);
        }
        usleep(20000);
    }

    return 0;
}
