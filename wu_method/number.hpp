#ifndef __NUMBER_HPP_INCLUDED__
#define __NUMBER_HPP_INCLUDED__

#include "operand.hpp"

class Number : public Operand {
private:
    double m_num;
public:
    Number(int num);
    void setNum(int num);
    int getNum(void);
};

#endif
