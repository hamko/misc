#include <iostream>
#include <string>
#include <stack>
#include <cmath>
#include <cstdlib>
#include <cstdio>

using namespace std;
#define is_number_char(p) ('0' <= p && p <= '9')

void print_double_stack(stack <double> st)
{
    while (!st.empty()) {
        cerr << st.top() << " " ; st.pop();
    }
    cerr << endl;
}
void print_char_stack(stack <char> st)
{
    while (!st.empty()) {
        cerr << st.top() << " " ; st.pop();
    }
    cerr << endl;
}

static string tmp;
string getToken(string str)
{
    static int index;
    if (str != "") {
        string tmp = str;
    }

    while (tmp[index] == ' ') 
        index++;
    if (tmp[index] == '\0')
        return "";

    cerr << "index: " << index << endl;
    if (!is_number_char(tmp[index])) {
        string ret = "";
        ret += tmp[index];
        index++;
        cerr << "getTokenOperator : " << ret << endl;
        return ret;
    } else {
        char num[1024] = {};
        int i = 0;
        while (is_number_char(tmp[index])) {
            num[i] = tmp[index];
            i++; index++;
        }
        cerr << "getTokenNum : " << num << endl;
        return string(num);
    }
}

double get_parsed(const string expression)
{
    stack <double> st;
    stack <char> op;
    string expr = expression;
    string tok;

    for (tok = getToken(expr); tok != ""; tok = getToken("")) {
        // オペランド
        if (is_number_char(tok[0])) {
            char** e = NULL;
            st.push(strtof(tok.c_str(), e));
        } else { // オペレータ
            char op_c = tok[0];
            op.push(op_c);
        }

        cerr << expr << endl;
        cerr << tok << endl;
        cerr << "var: "; print_double_stack(st);
        cerr << "ope: "; print_char_stack(op);
        cerr << endl;
    }

    return 0;
}

int main(int argc, char** argv)
{
    string str = "1 + 3";

    cerr << getToken(str) << endl;
    cerr << getToken("") << endl;
    cerr << getToken("") << endl;
    cerr << get_parsed(str) << endl;
    
    return 0;
}
