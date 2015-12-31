#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Eigen>

using namespace std;
using namespace Eigen;

int main(void)
{
  MatrixXd m(2, 2);
  m(0, 0) = 1;
  m(1, 0) = 2;
  m(0, 0) = 3;
  m(0, 1) = 4;

  std::cout << m << std::endl;

  Matrix4d H = Matrix4d::Zero();
  cout << H << endl;
  Matrix3d I3 = Matrix3d::Identity();
  H.block<3, 3>(0, 0)  = I3;
  cout << H << endl;
  Vector3d v3z = Vector3d::UnitZ();
  H.block<3, 1>(1, 0)  = v3z;
  cout << H << endl;

  return 0;

}
