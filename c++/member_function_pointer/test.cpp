#include <iostream>
using namespace std;
class A;

// typedef int (A::*memFun)(); // メンバ関数ポインタの型定義

class A
{
  int P(){return 1;}
  int Q(){return 2;}
  int R(){return 3;}

//  int Z(memFun f1, memFun f2)
  int Z(int (A::*f1)(), int (A::*f2)()) // メンバ関数ポインタの定義
  {
    return (this->*f1)() + (this->*f2)(); // メンバ関数ポインタの呼び出し
  }
  public: 
  int run();
};

int A::run()
{
  return Z(&A::P, &A::Q); // メンバ関数ポインタの渡し方
}
int main()
{
  A a;
  cout << a.run() << endl;
}
