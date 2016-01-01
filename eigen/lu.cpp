#include <iostream>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

void func43(void)
{
  // Jc in nxm (n>m)の行列に対して，独立行の抽出Jcm
  // Jcmに対して，独立列の抽出Js

  MatrixXd m(4, 3);
  m << 1, 0, 0, 0, 0, 1, 1, 0, 2, 4, 0, 3;
//  m << 0, 0, 1, 0, 0, 1, 0, 0, 3, 0, 0, 4;
  cout << "Here is the matrix m:" << endl << m << endl;

  Eigen::FullPivLU<MatrixXd> lu(m);
  cout << "Here is, up to permutations, its LU decomposition matrix:"
         << endl << lu.matrixLU() << endl;

  cout << "Here is the L part:" << endl;
  MatrixXd l = MatrixXd::Identity(4, 4);
  l.block<4,3>(0,0).triangularView<StrictlyLower>() = lu.matrixLU();
  cout << l << endl;

  cout << "Here is the U part:" << endl;
  MatrixXd u = lu.matrixLU().triangularView<Upper>();
  cout << u << endl;

  cout << "Let us now reconstruct the original matrix m:" << endl;
  cout << lu.permutationP().inverse() * l * u * lu.permutationQ().inverse() << endl;

  MatrixXd P; P = lu.permutationP();
  cout << "Here is the permutationP:" << endl;
  cout << P << endl;

  MatrixXd Q; Q = lu.permutationQ();
  cout << "Here is the permutationQ:" << endl;
  cout << Q << endl;
  
  cout << "Here is the permutationQ represented with a sparce matrix:" << endl;
  auto Qperm = lu.permutationQ();
  cout << Qperm.indices()[0] << endl;;
  cout << Qperm.indices()[1] << endl;;
  cout << Qperm.indices()[2] << endl;;

  cout << "Here is the image:" << endl;
  cout << lu.image(m)<<endl; // 選択後行列
  cout << "Here is indice list from the image to the matrix" << endl;
  for (int i = 0; i < lu.rank(); i++) 
    cout << Qperm.indices()[i] << " ";
  cout << endl;

  cout << "-------------" << endl;

}

void func34(void)
{
  MatrixXd m(3, 4);
//  m << 0, 1, 1, 0, 1, 1, 9.54053e-317, 0, 1, 1, 1, -1;
  m <<     1 ,1.84147,0.841471  ,     0
    ,  1 ,      1  ,      1 ,      -1
    ,0 ,0.540302 ,0.540302    ,    0;

  cout << "Here is the matrix m:" << endl << m << endl;

  Eigen::FullPivLU<MatrixXd> lu(m);
  cout << "Here is, up to permutations, its LU decomposition matrix:"
    << endl << lu.matrixLU() << endl;

  MatrixXd P; P = lu.permutationP();
  cout << "Here is the permutationP:" << endl;
  cout << P << endl;

  MatrixXd Q; Q = lu.permutationQ();
  cout << "Here is the permutationQ:" << endl;
  cout << Q << endl;

  cout << "Here is the permutationQ represented with a sparce matrix:" << endl;
  auto Qperm = lu.permutationQ();
  cout << Qperm.indices()[0] << endl;;
  cout << Qperm.indices()[1] << endl;;
  cout << Qperm.indices()[2] << endl;;

  cout << "Here is the image:" << endl;
  cout << lu.image(m)<<endl; // 選択後行列
  cout << "Here is indice list from the image to the matrix" << endl;
  for (int i = 0; i < lu.rank(); i++) 
    cout << Qperm.indices()[i] << " ";
  cout << endl;
  cout << "-------------" << endl;
}

int main(int argc, char** argv)
{
  func34();
  //  func43();
  return 0;
}
