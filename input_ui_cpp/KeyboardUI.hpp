#pragma once
class KeyboardUI {
private:
    int fd;
public:
    KeyboardUI(int fd_in);
    void type(int value, int code); 
    void get(int& value, int& code); // non-blocking 
};
