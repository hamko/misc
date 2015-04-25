class sample {
    unsigned int x_;
    public:
    void set(unsigned int x) {
        if (x == 0) { throw "exception."; }
        this->x_ = x - 1;
    }
    unsigned int get() {
        return this->x_;
    }
};
