#pragma once

class MouseUI {
private:
    int fd;
    void click_with_button(int button, int value);
public:
    MouseUI(int fd_in);
    void move_rel(int rel_x, int rel_y);
    void click(int value);
    void right_click(int value);
    void middle_click(int value);
};
