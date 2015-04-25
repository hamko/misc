/*
 * mousemode version 0.2.0
 *
 * Copyright (c) 2012 epian
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

#define INPUT_DIR "/dev/input"
#define UINPUT_DEVICE1 "/dev/uinput"
#define UINPUT_DEVICE2 "/dev/input/uinput"
#define MOUSE_MODE_DEVICE "mouse_mode_input"
#define DEFAULT_MOVE_SPEED 15000
#define MOVE_DISTANCE 4
#define WHEEL_SPEED 30000

#define BITS_PER_LONG (sizeof(long) * 8)
#define NBITS(x) ((((x)-1)/BITS_PER_LONG)+1)
#define OFF(x)  ((x)%BITS_PER_LONG)
#define BIT(x)  (1UL<<OFF(x))
#define LONG(x) ((x)/BITS_PER_LONG)
#define test_bit(bit, array)    ((array[LONG(bit)] >> OFF(bit)) & 1)

static int uifd = 0;
static int rel_x = 0;
static int rel_y = 0;
static int rel_wheel = 0;
static int do_loop = 1;
static float speed_x = 1.0f;
static int up = 0;
static int down = 0;
static int left = 0;
static int right = 0;
static int wheel_up = 0;
static int wheel_down = 0;

void send_move_event(int rel_x, int rel_y)
{
    struct input_event event;
    if (uifd <= 0) {
        return;
    }
    gettimeofday(&event.time, NULL);
    event.type = EV_REL;
    event.code = REL_X;
    event.value = rel_x;
    write(uifd, &event, sizeof(event));

    event.type = EV_REL;
    event.code = REL_Y;
    event.value = rel_y;
    write(uifd, &event, sizeof(event));

    event.type = EV_SYN;
    event.code = SYN_REPORT;
    event.value = 0;
    write(uifd, &event, sizeof(event));
}

void send_wheel_event(int value)
{
    struct input_event event;
    if (uifd <= 0) {
        return;
    }
    gettimeofday(&event.time, NULL);
    event.type = EV_REL;
    event.code = REL_WHEEL;
    event.value = value;
    write(uifd, &event, sizeof(event));

    event.type = EV_SYN;
    event.code = SYN_REPORT;
    event.value = 0;
    write(uifd, &event, sizeof(event));
}

void send_key_event(int button, int value)
{
    struct input_event event;
    if (uifd <= 0) {
        return;
    }
    gettimeofday(&event.time, NULL);
    event.type = EV_KEY;
    event.code = button;
    event.value = value;
    write(uifd, &event, sizeof(event));

    event.type = EV_SYN;
    event.code = SYN_REPORT;
    event.value = 0;
    write(uifd, &event, sizeof(event));
}

void create_virtual_device()
{
    struct uinput_user_dev dev;
    memset(&dev, 0, sizeof(dev));
    strcpy(dev.name, MOUSE_MODE_DEVICE);
    write(uifd, &dev, sizeof(dev));

    ioctl(uifd, UI_SET_EVBIT, EV_REL);
    ioctl(uifd, UI_SET_RELBIT, REL_X);
    ioctl(uifd, UI_SET_RELBIT, REL_Y);
    ioctl(uifd, UI_SET_RELBIT, REL_WHEEL);
    ioctl(uifd, UI_SET_EVBIT, EV_KEY);
    ioctl(uifd, UI_SET_KEYBIT, KEY_S);
    ioctl(uifd, UI_SET_KEYBIT, BTN_LEFT);
    ioctl(uifd, UI_SET_KEYBIT, BTN_RIGHT);
    ioctl(uifd, UI_SET_KEYBIT, BTN_MIDDLE);
    ioctl(uifd, UI_SET_KEYBIT, BTN_SIDE);
    ioctl(uifd, UI_SET_KEYBIT, BTN_EXTRA);
    ioctl(uifd, UI_SET_KEYBIT, BTN_FORWARD);
    ioctl(uifd, UI_SET_KEYBIT, BTN_BACK);
    ioctl(uifd, UI_SET_KEYBIT, BTN_TASK);
    ioctl(uifd, UI_SET_KEYBIT, KEY_RIGHTCTRL);
    ioctl(uifd, UI_SET_KEYBIT, KEY_LEFTCTRL);
    ioctl(uifd, UI_SET_KEYBIT, KEY_RIGHTSHIFT);
    ioctl(uifd, UI_SET_KEYBIT, KEY_LEFTSHIFT);
    ioctl(uifd, UI_SET_KEYBIT, KEY_RIGHTALT);
    ioctl(uifd, UI_SET_KEYBIT, KEY_LEFTALT);
    ioctl(uifd, UI_SET_KEYBIT, KEY_TAB);
    ioctl(uifd, UI_SET_KEYBIT, KEY_ESC);
    ioctl(uifd, UI_SET_KEYBIT, KEY_BACKSPACE);
    ioctl(uifd, UI_SET_KEYBIT, KEY_MENU);
    ioctl(uifd, UI_SET_KEYBIT, KEY_PAGEUP);
    ioctl(uifd, UI_SET_KEYBIT, KEY_PAGEDOWN);
    ioctl(uifd, UI_SET_KEYBIT, KEY_HOME);
    ioctl(uifd, UI_SET_KEYBIT, KEY_END);

    ioctl(uifd, UI_DEV_CREATE, 0);
}

void destroy_virtual_device()
{
    ioctl(uifd, UI_DEV_DESTROY, 0);
}

void start_grab(int fd)
{
    ioctl(fd, EVIOCGRAB, 1);
}

void end_grab(int fd)
{
    ioctl(fd, EVIOCGRAB, 0);
}

void release_all_keys(int fd)
{
    struct input_event event;
    if (fd <= 0) {
        return;
    }
    gettimeofday(&event.time, NULL);
    event.type = EV_KEY;
    int j = 0;
    for (j = 0; j < 255; j++) {
        event.code = j;
        event.value = 0;
        write(fd, &event, sizeof(event));
    }
    event.type = EV_SYN;
    event.code = SYN_REPORT;
    event.value = 0;
    write(fd, &event, sizeof(event));
}

void *move_thread(void *arg)
{
    while (do_loop) {
        if (rel_x != 0 || rel_y != 0) {
            send_move_event(rel_x, rel_y);
        }
        usleep(DEFAULT_MOVE_SPEED * speed_x);
    }
    return (void *) NULL;
}

void *wheel_thread(void *arg)
{
    while (do_loop) {
        if (rel_wheel != 0) {
            send_wheel_event(rel_wheel);
        }
        usleep(WHEEL_SPEED * speed_x);
    }
    return (void *) NULL;
}

int handle_key_event(struct input_event* event) {
    if (event->type == EV_KEY && event->value < 2) {
        switch (event->code) {
            case KEY_Q:
                // Quit
                return 1;
            case KEY_F:
                // Speed Up to 0.25
                speed_x = event->value > 0 ? 0.25f : 1.0f;
                break;
            case KEY_D:
                // Speed Up to 0.5
                speed_x = event->value > 0 ? 0.5f : 1.0f;
                break;
            case KEY_S:
                // Speed Down to 2.0
                speed_x = event->value > 0 ? 2.0f : 1.0f;
                break;
            case KEY_A:
                // Speed Down to 4.0
                speed_x = event->value > 0 ? 4.0f : 1.0f;
                break;
            case KEY_KPSLASH:
                // Speed Down
                if (event->value == 1 && speed_x < 4.0f) {
                    speed_x = speed_x * 2.0f;
                }
                break;
            case KEY_KPASTERISK:
                // Speed Up
                if (event->value == 1 && speed_x > 0.25f) {
                    speed_x = speed_x / 2.0f;
                }
                break;
            case KEY_UP:
            case KEY_K:
            case KEY_KP8:
                // Move Up
                up = event->value;
                break;
            case KEY_DOWN:
            case KEY_J:
            case KEY_KP2:
                // Move Down
                down = event->value;
                break;
            case KEY_LEFT:
            case KEY_H:
            case KEY_KP4:
                // Move Left
                left = event->value;
                break;
            case KEY_RIGHT:
            case KEY_L:
            case KEY_KP6:
                // Move Right
                right = event->value;
                break;
            case KEY_SPACE:
            case KEY_ENTER:
            case KEY_KPENTER:
            case KEY_KP7:
                // Left Button
                send_key_event(BTN_LEFT, event->value);
                break;
            case KEY_E:
            case KEY_KP5:
                // Middle Button
                send_key_event(BTN_MIDDLE, event->value);
                break;
            case KEY_R:
            case KEY_MENU:
            case KEY_KP9:
                // Right Button
                send_key_event(BTN_RIGHT, event->value);
                break;
            case KEY_PAGEUP:
            case KEY_P:
            case KEY_KPMINUS:
                // Wheel Up
                wheel_up = event->value;
                break;
            case KEY_PAGEDOWN:
            case KEY_N:
            case KEY_KPPLUS:
                // Wheel Down
                wheel_down = event->value;
                break;
            case KEY_RIGHTCTRL:
            case KEY_LEFTCTRL:
            case KEY_RIGHTSHIFT:
            case KEY_LEFTSHIFT:
            case KEY_RIGHTALT:
            case KEY_LEFTALT:
            case KEY_TAB:
            case KEY_ESC:
            case KEY_BACKSPACE:
            case KEY_HOME:
            case KEY_END:
                // send as it is
                send_key_event(event->code, event->value);
                break;
            case KEY_CAPSLOCK:
                // no capslock
                send_key_event(KEY_LEFTCTRL, event->value);
                break;
            default:
                break;
        }
        rel_x = MOVE_DISTANCE * (right - left);
        rel_y = MOVE_DISTANCE * (down - up);
        rel_wheel = wheel_up - wheel_down;
    }
    return 0;
}

void key_event_loop(int* fds, int size)
{
    struct input_event event;
    fd_set fdset, key_fdset;
    int i = 0;
    int maxfd = 0;
    int ret = 0;

    FD_ZERO(&key_fdset);
    for (i = 0; i < size; i++) {
        FD_SET(fds[i], &key_fdset);
        if (maxfd < fds[i]) {
            maxfd = fds[i];
        }
    }

    while (1) {
        memcpy(&fdset, &key_fdset, sizeof(fd_set));
        ret = select(maxfd + 1, &fdset, NULL, NULL, NULL);
        if (ret <= 0) {
            return;
        }
        for (i = 0; i < size; i++) {
            if (FD_ISSET(fds[i], &fdset)
                    && read(fds[i], &event, sizeof(event)) == sizeof(event)
                    && handle_key_event(&event) > 0) {
                return;
            }
        }
    }
}

void run_mousemode(int fds[], int size)
{
    pthread_t thread1, thread2;
    int status;
    void* result;
    do_loop = 1;
    status = pthread_create(&thread1, NULL, move_thread, (void*)NULL);
    if (status == 0) {
        status = pthread_create(&thread2, NULL, wheel_thread, (void*)NULL);
        if (status == 0) {
            key_event_loop(fds, size);
            do_loop = 0;
            pthread_join(thread2, &result);
        } else {
            fprintf(stderr, "pthread_create : %s", strerror(status));
        }
        pthread_join(thread1, &result);
    } else {
        fprintf(stderr, "pthread_create : %s", strerror(status));
    }
}

int test_key_device(char* dev_file)
{
    int fd = open(dev_file, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open device file.");
        return -1;
    }
    unsigned long bit[NBITS(KEY_MAX)];
    memset(bit, 0, sizeof(bit));
    ioctl(fd, EVIOCGBIT(EV_KEY, KEY_MAX), bit);
    close(fd);
    if (test_bit(KEY_E, bit)
            && test_bit(KEY_P, bit)
            && test_bit(KEY_I, bit)
            && test_bit(KEY_A, bit)
            && test_bit(KEY_N, bit)
            && test_bit(KEY_Q, bit)
            && test_bit(KEY_ENTER, bit)) {
        return 0;
    }
    return -1;
}

int open_key_event_devices(int fds[])
{
    DIR* input_dir = opendir(INPUT_DIR);
    if (input_dir == NULL) {
        perror("Failed to open input dir.");
        return -1;
    }
    struct dirent* entry;
    struct stat st;
    char dev_file[1024];
    int fd = 0;
    int count = 0;
    while ((entry = readdir(input_dir)) != NULL) {
        snprintf(dev_file, 1024, "%s/%s", INPUT_DIR, entry->d_name);
        if (stat(dev_file, &st) == 0 && S_ISCHR(st.st_mode)) {
            if (test_key_device(dev_file) == 0) {
                printf("%s is keyboard event device file.\n", dev_file);
                fd = open(dev_file, O_RDWR);
                if (fd > 0) {
                    fds[count] = fd;
                    count++;
                } else {
                    perror("Error opening the keyboard event device.");
                }
            }
        }
    }
    return count;
}

int open_uinput() {
    struct stat st;
    if (stat(UINPUT_DEVICE1, &st) == 0 && S_ISCHR(st.st_mode)) {
        return open(UINPUT_DEVICE1, O_WRONLY);
    }
    if (stat(UINPUT_DEVICE2, &st) == 0 && S_ISCHR(st.st_mode)) {
        return open(UINPUT_DEVICE2, O_WRONLY);
    }
    return -1;
}

int get_lock() {
    char buf[1024];
    ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
    if (len > -1) {
        buf[len] = '\0';
    } else {
        perror("Error reading link the self executable file\n");
        return -1;
    }

    int fd = open(buf, O_RDONLY);
    if (fd <= 0) {
        perror("Error opening the self executable file\n");
        return -1;
    }

    int lock = flock(fd, LOCK_EX | LOCK_NB);
    if (lock != 0) {
        perror("Error get lock\n");
        close(fd);
        return -1;
    }

    return fd;
}

int free_lock(int fd) {
    flock(fd, LOCK_UN);
    close(fd);
}

int main(int argc, char *argv[])
{
    int lock_fd = get_lock();
    if (lock_fd < 0) {
        return -1;
    }

    uifd = open_uinput();
    if (uifd <= 0) {
        perror("Error opening the uinput device.");
        free_lock(lock_fd);
        return -1;
    }

    int key_fds[10];
    int size = open_key_event_devices(key_fds);
    if (size <= 0) {
        printf("No keyboard devices found.");
        close(uifd);
        free_lock(lock_fd);
        return -1;
    }

    int i = 0;
    for (i = 0; i < size; i++) {
        release_all_keys(key_fds[i]);
        start_grab(key_fds[i]);
    }

    create_virtual_device();

    run_mousemode(key_fds, size);

    destroy_virtual_device();

    for (i = 0; i < size; i++) {
        end_grab(key_fds[i]);
        close(key_fds[i]);
    }

    close(uifd);
    free_lock(lock_fd);
}
