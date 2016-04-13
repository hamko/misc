#include <bits/stdc++.h> 
using namespace std; 
int main(void) 
{ 
    vector<pair<double, double> > p(4);
    vector<complex<double> > c(4);

    double x[4] = {4, 2, 3, 1.5};
    double y[4] = {1, 3, 4, 2.7};

    for (int i = 0; i < 4; i++) {
        p[i].first = x[i]; p[i].second = y[i]; 
        c[i] = complex<double>(x[i], y[i]);
    }
    
    sort(begin(p), end(p));
    sort(begin(c), end(c), [](const complex<double>& a, const complex<double>& b){return a.real() < b.real();});

    cout << "pair" << endl;
    for (int i = 0; i < 4; i++) {
        cout << p[i].first << " " << p[i].second << endl;
    }
    cout << "complex" << endl;
    for (int i = 0; i < 4; i++) {
        cout << c[i].real() << " " << c[i].imag() << endl;
    }

    return 0;
}
