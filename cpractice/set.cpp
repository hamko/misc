#include <set>
#include <iostream>

using namespace std;

void print_set(set<int> a)
{
    set<int>::iterator it = a.begin();
    while (it != a.end()) {
        cout << *it << " ";
        it++;
    }
    cout << endl;
}

// A \subset B
// 0: A = B
// 1: A in B
// 2: A ni B
int subset(set<int> a, set<int> b)
{
    set<int>::iterator it_a = a.begin();
    while (it_a != a.end()) {
        set<int>::iterator it_b;
        if ((it_b = b.find(*it_a)) != b.end()) {
            a.erase(it_a++);
            b.erase(it_b++);
        } else {
            it_a++;
        }
    }

    if (a.empty() && b.empty())
        return 0;
    else if (a.empty())
        return 1;
    else if (b.empty())
        return 2;
    else 
        return 3;
}

int main()
{
    set<int> a;
    set<int> b;
    set<int> c;

    a.insert(100);
    a.insert(200);
    a.insert(300);
    a.insert(300);
    c = a;
    print_set(a);
    print_set(c);

    cout << subset(a, c) << endl;

    c.insert(400);
    print_set(a);
    print_set(c);

    cout << subset(a, c) << endl;
    cout << subset(c, a) << endl;

    a.insert(500);
    print_set(a);
    print_set(c);
    cout << subset(a, c) << endl;
    cout << subset(c, a) << endl;

    return 0;
}

