#include <iostream>
#include <algorithm>
#include <math.h>
#include "eigen.hpp"
#define sig(input) (1.0 / (1.0 + exp(-(input))))

using namespace std;

int is, hs, os;
VectorXd I, H, O;   // is*1, hs*1, os*1
MatrixXd ih, ho;    // is*hs, hs*os
VectorXd eH, eO;    // hs*1, os*1
VectorXd B;         // Teacher vector, os*1
double alpha = 1.5;

void forprop(VectorXd input) 
{
    I = input;
    H = ih.transpose() * I;
    rep (i, H.rows()) H(i) = sig(H(i));
    O = ho.transpose() * H;
    rep (i, O.rows()) O(i) = sig(O(i));
}

void backprop(VectorXd teacher_in, VectorXd teacher_out)
{
    forprop(teacher_in);
    B = teacher_out;
    eO = ((O - B).array() * O.array() * (ArrayXXd::Ones(O.rows(), 1) - O.array())).matrix();
    ho = ho - alpha * H * eO.transpose();
    eH = ((ho * eO).array() * H.array() * (ArrayXXd::Ones(H.rows(), 1) - H.array())).matrix();
    ih = ih - alpha * I * eH.transpose();
}

void init(int _is, int _hs, int _os)
{
    is = _is, hs = _hs; os = _os;

    ih = MatrixXd::Random(_is, _hs);
    rep (i, _is) rep (j, _hs)
        ih(i, j) = abs(ih(i, j));

    ho = MatrixXd::Random(_hs, _os);
    rep (i, _hs) rep (j, _os)
        ho(i, j) = abs(ho(i, j));
}

int main(void)
{
    srand((unsigned int)time(NULL));
    init(2, 4, 1);

    MatrixXd zerozero = MatrixXd(2, 1); zerozero << 0, 0;
    MatrixXd zeroone = MatrixXd(2, 1); zeroone << 0, 1;
    MatrixXd onezero = MatrixXd(2, 1); onezero << 1, 0;
    MatrixXd oneone = MatrixXd(2, 1); oneone << 1, 1;
    MatrixXd zero = MatrixXd(1, 1); zero << 0;
    MatrixXd one = MatrixXd(1, 1); one << 1;

    rep (i, 10000) {
        backprop(zerozero, one);
        backprop(zeroone, zero);
        backprop(onezero, zero);
        backprop(oneone, one);

        forprop(zerozero); cout << O(0, 0) << " ";
        forprop(zeroone); cout << O(0, 0) << " ";
        forprop(onezero); cout << O(0, 0) << " ";
        forprop(oneone); cout << O(0, 0) << " ";
        cout << endl;
    }

    rep (i, 10000) {
        VectorXd r = VectorXd::Random(2);
        rep (j, 2) r(j) = abs(r(j));
        forprop(r);
        cout << "#XY " << r(0) << " " << r(1) << " " << O(0) << " " << (O(0) > 0.5 ? 1 : 0) << endl;

    }

    return 0;
}
