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

VectorXd RungeKuttaExponential(VectorXd Q, double t)
{
  VectorXd Qd(Q.size());
  // Exp
  Qd = -Q;

  return Qd;
}

VectorXd RungeKuttaCos(VectorXd Q, double t)
{
  VectorXd Qd(2);
  // Sin
  Qd(0) = Q(1);
  Qd(1) = -Q(0);

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
  int i = 0;
  int cycle = 2000;
  double dt = 0.05;
  double t = 0;
   
  /*
  m_q = VectorXd::Zero(2);
  m_qd = VectorXd::Zero(2);
  m_qdd = VectorXd::Zero(2); m_qdd(0) = 1;
  
  double tn = 0;
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
  */

  // http://www.hakodate-ct.ac.jp/~moriya/class/ProIII/text010.pdf
  /*
  VectorXd Q = VectorXd::Ones(2);
//  for (int i = 0; i < cycle; i++) {
  while(1){
    i++; t = i * dt;

    VectorXd Qnext = RungeKutta(Q, RungeKuttaExponential, t, dt);
    cout << Q << "#Q" << endl << Qnext << "#Qnext" << endl;
    cout << VectorXd::Ones(2)*exp(-t) - Qnext << "#err" << endl;
    cout << endl;
    Q = Qnext;
  }
  */

  VectorXd Q = VectorXd::Zero(2); Q(0) = 1;
  double euler_q0 = 1; double euler_q1 = 0;
  while(1){
    cout << "i " << i << endl;
    i++; t = i * dt;

    // Runge-Kutta
    VectorXd Qnext = RungeKutta(Q, RungeKuttaCos, 0, dt);

    // Euler
    double prev_euler_q0 = euler_q0;
    euler_q0 += euler_q1 * dt;
    euler_q1 += -prev_euler_q0 * dt;

    cout << Q(0) << "#Q" << endl << Qnext(0) << "#Qnext" << endl;
    cout << prev_euler_q0 << "#Q" << endl << euler_q0 << "#Q euler" << endl;
    cout << cos(t) - Qnext(0) << "#err" << endl;
    cout << cos(t) - euler_q0 << "#err euler" << endl;
    cout << endl;
    Q = Qnext;
  }

  return 0;

}
