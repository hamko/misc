#include <Eigen/Dense>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
using namespace Eigen;
int main(void) {
  Matrix<double, 4, 3> Jc;
  Jc  << 1, 0, 1,
         1, 0, 0,
         3, 0, 5,
         4, 0, 7;

  FullPivLU<MatrixXd> lu_cm(Jc.transpose());
  MatrixXd Jcm = lu_cm.image(Jc.transpose()).transpose(); 
  cout << "Jcm" << endl << Jcm << endl;

  FullPivLU<MatrixXd> lu_s(Jcm);
  MatrixXd m_Js;
  m_Js= lu_s.image(Jcm);
  cout << "Js" << endl << m_Js << endl;
  MatrixXd Jg = MatrixXd(Jcm.rows(), 0);
  int j = 0;
  auto Qperm = lu_s.permutationQ();
  vector<int> QpermList;
  for (int i = 0; i < lu_s.rank(); i++)
    QpermList.push_back(Qperm.indices()[i]);
  sort(QpermList.begin(), QpermList.end());
  for (int i = 0; i < Jcm.cols(); i++) {
    if (j < QpermList.size() && QpermList[j] == i) {
      j++;
    } else {
      Jg.conservativeResize(Jg.rows(), Jg.cols()+1);
      Jg.col(Jg.cols()-1) = Jcm.col(i);
    }
  }

  cout << "Js_inv" << endl << -m_Js.inverse() << endl;
  MatrixXd tmp =  -m_Js.inverse();
  cout << "Jg" << endl << Jg << endl;
  MatrixXd m_H = tmp * Jg;
  cout << "H" << endl <<  m_H << endl;
//  cout << "H" << endl << -m_Js.inverse() * Jg << endl; // NG

  // OK
  auto A = MatrixXd::Identity(3,3);
  auto B = 2 * MatrixXd::Ones(3,3);
  cout << A.inverse() * B << endl;
  

  return 0;
}
