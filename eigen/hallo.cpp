#include <iostream>
#include <Eigen/Dense>

using namespace Eigen;

int main(void)
{
    MatrixXd m(2, 2);
    m(0, 0) = 1;
    m(1, 0) = 2;
    m(0, 0) = 3;
    m(0, 1) = 4;

    std::cout << m << std::endl;

    Matrix2d a;
    a << 1, 1, 1, 2;
    std::cout << a << std::endl;
}
