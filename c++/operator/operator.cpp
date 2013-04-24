#include "operator.hpp"

int* Test::getNumPointer(void)
{
    return p;
}

int Test::getNum(void) const
{
    return *p;
}

Test::Test(int num)
{
    p = new int(num);
}

Test::Test(const Test& obj)
{
    p = new int(obj.getNum());
}

Test::~Test(void)
{
    delete p;
}

Test Test::operator+(const Test& obj)
{
    return Test(*(this->p) + *(obj.p));
}

// &を付けることで高速化が可能
// 付けなくても動くが，コピーが一回発生する
// 引数の参照は，引数の元に無理やり&をつけて，関数内部に無理やり*を付ける感じ
// 戻り値の参照は，関数内に&をつけて，呼出元に*を付けるのと同じ．
Test& Test::operator=(const Test& obj)
{
    *(this->p) = *(obj.p);
    return *this;
}

Test Test::operator[](const Test& obj)
{
    return Test(*(this->p) * *(obj.p));
}

Test Test::operator[](const int num)
{
    return Test(*(this->p) * num);
}
