#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <fcntl.h>

#include "MouseUI.hpp"
MouseUI::MouseUI(int fd_in) : fd(fd_in)
{ 
}

void MouseUI::move_rel(int rel_x, int rel_y)
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

void MouseUI::click_with_button(int button, int value)
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

void MouseUI::click(int value)
{
    click_with_button(BTN_LEFT, value);
}
void MouseUI::right_click(int value)
{
    click_with_button(BTN_RIGHT, value);
}
void MouseUI::middle_click(int value)
{
    click_with_button(BTN_MIDDLE, value);
}

