#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Eigen>
#include <stdlib.h>
#include <time.h>

using namespace std;
using namespace Eigen;

int main(void)
{
  srand((unsigned int)time(NULL));

  MatrixXd m(3, 3);

  int d = (int)(10.0*rand()/RAND_MAX)+1;
  while (1)  {
    m = m * MatrixXd::Random(3, d) * MatrixXd::Random(d, 3);
    cout << &m << " " << m.data() << endl;
  }

  return 0;

}
