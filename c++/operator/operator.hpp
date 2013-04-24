#ifndef __OPERATOR_HPP_INCLUDED__
#define __OPERATOR_HPP_INCLUDED__

class Test {
private:
    int* p;
public:
    // 始めのconstはポインタの指している値を書き換えられない
    // 真ん中のconstは返されたポインタ自体を書き換えられない
    // 最後のconstはメンバ関数の中で自分自身のインスタンスを書き換えられない
    int* getNumPointer(void); 
    int getNum(void) const; // コピーコンストラクタの引数がconstなので，これが変更されないことを保証しなければならないため，constが必要 

    Test(int num);
    Test(const Test& obj);
    ~Test(void);
    Test operator+(const Test& obj);
    Test& operator=(const Test& obj);
    // Test& operator+(const Test& obj);
    Test operator[](const Test& obj);
    Test operator[](const int num);
};

#endif
