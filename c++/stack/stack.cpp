#include <iostream>
#include <stack>

using namespace std;

void print_double_stack(stack <double> st)
{
    while (!st.empty()) {
        cerr << st.top() << endl; 
        st.pop();
    }
}

int main(int argc, char** argv)
{
    stack<double> st;
    st.push(1);
    st.push(2);
    st.push(3);
    st.push(4);
    st.push(5);
    print_double_stack(st);

    return 0;
}
