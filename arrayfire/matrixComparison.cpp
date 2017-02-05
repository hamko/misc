#include <arrayfire.h>
#include <iostream>
#include <Eigen/Core>

#define BITS 12

// Eigen
Eigen::MatrixXf A_Eigen;
Eigen::MatrixXf A_Eigen_trans;
int g_i, g_j;
static void calcEigen(void) {
    Eigen::MatrixXf dst = A_Eigen_trans * A_Eigen;
    assert(dst.rows() == dst.cols());
}

/*
// Eigen Parallel
static void calcEigenParallel(void) {
    Eigen::initParallel();
    Eigen::MatrixXf dst = A_Eigen * A_Eigen;
    assert(dst.rows() == dst.cols());
}
*/

// arrayfire
af::array A;
af::array A_trans;
static void calc(void)
{
    af::array dst = af::matmul(A_trans, A);
    dst.eval();
}

void ev(void (*func)(void))
{
#if 0
    af::timer start1 = af::timer::start();
    func();
    double time = af::timer::stop(start1);
    double flops = 2.0 * powf(g_i, 3) / (time * 1.0e9);
    std::cout << g_i << " * " << g_i << " time: " << time << ", GFlops: " << flops << std::endl;

#else
    double time = af::timeit(func);
    double flops = 2.0 * powf(g_i, 3) / (time * 1.0e9); // GFlops
    std::cout << g_i << " " << g_j << " " << time << " " << flops << std::endl;
#endif
}

int main()
{
    // ArrayFire
    std::cout << "#ArrayFire" << std::endl;
    for(g_i = 1; g_i <= (1 << BITS); g_i <<= 1) {
        g_j = g_i;
//        for(g_j = 1; g_j <= (1 << BITS); g_j <<= 1) {
            A = af::randu(g_i, g_j);
            A_trans = af::transpose(A);
            ev(calc);
//        }
    }


    // Eigen
    std::cout << "#Eigen" << std::endl;
    for(g_i = 1; g_i <= (1 << BITS); g_i <<= 1) {
        g_j = g_i;
//        for(g_j = 1; g_j <= (1 << BITS); g_j <<= 1) {
            A_Eigen = Eigen::MatrixXf::Random(g_i, g_j);
            A_Eigen_trans = A_Eigen.transpose();
            ev(calcEigen);
//        }
    }

    /*
    // Eigen Parallel
    for(g_i = 1; g_i <= (1 << BITS); g_i <<= 1) {
    A_Eigen = Eigen::MatrixXf::Random(g_i, g_i);
    ev(calcEigenParallel);
    }
    */

    return 0;

}
