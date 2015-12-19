#include <iostream>

using namespace std;
// 線形独立な列を抽出するプログラム
int main(int argc, char** argv)
{
  // Jc in nxm (n>m)の行列に対して，独立行の抽出Jcm
  // Jcmに対して，独立列の抽出Js

  typedef Matrix<double, 4, 3> Matrix4x3;
  typedef Matrix<double, 4, 4> Matrix4x4;

  Matrix4x3 m;
  m << 1, 0, 0, 0, 0, 1, 1, 0, 2, 4, 0, 3;
//  m << 0, 0, 1, 0, 0, 1, 0, 0, 3, 0, 0, 4;
  cout << "Here is the matrix m:" << endl << m << endl;

  Eigen::FullPivLU<Matrix4x3> lu(m);
  cout << "Here is, up to permutations, its LU decomposition matrix:"
         << endl << lu.matrixLU() << endl;

  cout << "Here is the L part:" << endl;
  Matrix4x4 l = Matrix4x4::Identity();
  l.block<4,3>(0,0).triangularView<StrictlyLower>() = lu.matrixLU();
  cout << l << endl;

  cout << "Here is the U part:" << endl;
  Matrix4x3 u = lu.matrixLU().triangularView<Upper>();
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

  cout << "-------------" << endl;
  cout << "Here is the image:" << endl;
  cout << lu.image(m)<<endl; // 選択後行列
  cout << "Here is indice list from the image to the matrix" << endl;
  for (int i = 0; i < lu.rank(); i++) 
    cout << Qperm.indices()[i] << " ";
  cout << endl;

  
  return 0;

}
