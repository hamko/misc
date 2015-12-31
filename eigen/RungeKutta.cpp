#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Eigen>

using namespace std;
using namespace Eigen;

VectorXd m_q(2);
VectorXd m_qd(2);
VectorXd m_qdd(2);
VectorXd RungeKuttaEOM(VectorXd Q, double t)
{
  VectorXd Qd(Q.size());
  Qd.block(0, 0, Q.size()/2, 1) = m_qd;
  Qd.block(Q.size()/2, 0, Q.size()/2, 1) = m_qdd;

  return Qd;
}

// 運動方程式では大抵tnは要らないんだが…
VectorXd RungeKutta(VectorXd& Qn, VectorXd (*F)(VectorXd Q, double t), double tn, double h)
{
  VectorXd k1 = F(Qn, tn);
  VectorXd k2 = F(Qn+h/2*k1, tn+h/2);
  VectorXd k3 = F(Qn+h/2*k2, tn+h/2);
  VectorXd k4 = F(Qn+h*k3, tn+h);
  return Qn + (k1 + 2*k2 + 2*k3 + k4) * h / 6;
}

int main(void)
{
  m_q = VectorXd::Zero(2);
  m_qd = VectorXd::Zero(2);
  m_qdd = VectorXd::Zero(2); m_qdd(0) = 1;
  
  double tn = 0;
  double dt = 0.01;
  while (tn < 100) {
    VectorXd Q(m_q.size() * 2);
    Q.block(0, 0, m_q.size(), 1) = m_q; 
    Q.block(m_q.size(), 0, m_q.size(), 1) = m_qd; 
    VectorXd Qnext = RungeKutta(Q, RungeKuttaEOM, tn, dt);
    m_q = Qnext.block(0, 0, m_q.size(), 1);
    m_qd = Qnext.block(m_q.size(), 0, m_q.size(), 1);
    tn += dt;

    cout << tn << " " << endl << m_q.transpose() << "#m_q" << endl << m_qd.transpose() << "#m_qd" << endl << endl;
  }

  return 0;

}
