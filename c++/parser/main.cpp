#include <iostream>
#include <string>
#include <stack>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "flex_header.hpp"
#include "lex.yy.hpp"

//#define PRINT_PARSER

using namespace std;
#define is_number_char(p) ('0' <= p && p <= '9')

#ifdef PRINT_PARSER
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
#endif

#define OPERATOR_NUM 6
char operator_list[OPERATOR_NUM] = {'(', '^', '*', '+', '-', ')'};
char operator_point[OPERATOR_NUM] = {5, 4, 3, 2, 2, 1}; // must be more than 0

// xの優先度を返す
int get_priority(char x)
{
    int xi;
    for (int i = 0; i < OPERATOR_NUM; i++) 
        if (x == operator_list[i])
            xi = operator_point[i];

    return xi;
}

void pop_operate_plus(stack <double> &st, stack <char> &op)
{
    double post = st.top(); st.pop();
    double pre = st.top(); st.pop();
    op.pop();

    st.push(pre + post);
}
void pop_operate_mult(stack <double> &st, stack <char> &op)
{
    double post = st.top(); st.pop();
    double pre = st.top(); st.pop();
    op.pop();

    st.push(pre * post);
}
void pop_operate_minus(stack <double> &st, stack <char> &op)
{
    double post = st.top(); st.pop();
    double pre = st.top(); st.pop();
    op.pop();

    st.push(pre - post);
}
void pop_operate_pow(stack <double> &st, stack <char> &op)
{
    double post = st.top(); st.pop();
    double pre = st.top(); st.pop();
    op.pop();

    st.push(pow(pre, post));
}

void pop_operate(char op_c, stack <double> &st, stack <char> &op)
{
    if (op_c == '+') 
        pop_operate_plus(st, op);
    else if (op_c == '-')
        pop_operate_minus(st, op);
    else if (op_c == '*')
        pop_operate_mult(st, op);
    else if (op_c == '^')
        pop_operate_pow(st, op);
}

double get_parsed(const string expression)
{
    stack <double> st;
    stack <char> op;
    char* expr = strdup(expression.c_str());
    char* tok;

    int return_code;
    yy_scan_string(expr);
    while (return_code = yylex()) {
        tok = yytext; // 名前を変えているだけ 
        if (is_number_char(tok[0])) { // オペランド
            char** e = NULL;
            st.push(strtof(tok, e));
        } else { // オペレータ
            char op_c = tok[0];
#ifdef PRINT_PARSER
            cerr << "operator: token " << op_c << endl;
#endif

            while (get_priority(op_c) <= (op.empty() ? 0 : get_priority(op.top())) // Tokenの優先順位 <= Stackの優先順位
                    && !(!op.empty() && op.top() == '(')) { // Stackが(でない
#ifdef PRINT_PARSER
                cerr << "operate: " << op.top() << endl;
#endif
                pop_operate(op.top(), st, op);
            }
            if (op_c != ')')  // Tokenが)でないなら普通にpush
                op.push(op_c);
            else // Tokenが)なら，Stackの一番上にあるはずの(とTokenを破棄する
                op.pop();
        }
#ifdef PRINT_PARSER
        cerr << "var: "; print_double_stack(st);
        cerr << "ope: "; print_char_stack(op);
        cerr << endl;
#endif
    }

    while (!op.empty())
        pop_operate(op.top(), st, op);

    double ret = st.top(); st.pop();

    free(expr);
    return ret;
}

int main(int argc, char** argv)
{
    string str;
    if (argc == 1)
        str = "(1 + 2) * 4 ^ 2";
    else if (argc == 2)
        str = argv[1];
    else 
        return 1;


    cerr << get_parsed(str) << endl;
    
    return 0;
}
