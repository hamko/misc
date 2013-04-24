#ifndef EIGEN_HPP_INCLUDED
#define EIGEN_HPP_INCLUDED

#include <Eigen/Core>
#include <Eigen/LU>

#define PRINT
#ifdef PRINT
#define PRINT_MAT(X) cout << #X << ":\n" << X << endl << endl
#define PRINT_MAT2(X,DESC) cout << DESC << ":\n" << X << endl << endl
#define PRINT_FNC    cout << "[" << __func__ << "]" << endl
#else
#define PRINT_MAT(X) 
#define PRINT_MAT2(X,DESC)
#define PRINT_FNC
#endif

#define rep(i, n) for(int i = 0; i < n; i++)
 
using namespace Eigen;

#endif
