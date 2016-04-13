#include <tuple>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char const *argv[]){
//    tuple<int, string, double> tupleValu = make_tuple(10, "test", 9.7); // OK
    auto tupleValue = make_tuple(10, "test", 9.7);

    cout << "Tupleの内容を確認" << endl;
    cout << get<0>(tupleValue) << " " << get<1>(tupleValue) << " " << get<2>(tupleValue) << endl;

    get<0>(tupleValue) = 0;
    get<1>(tupleValue) = "foo";
    get<2>(tupleValue) = 100.0;

    cout << "変更後の内容を確認" << endl;
//    cout << tupleValue << endl; // だめ
//    for (int i = 0; i < 3; i++)  // だめ。iはconstじゃないと
//        cout << get<i>(tupleValue) << endl;
    cout << get<0>(tupleValue) << " " << get<1>(tupleValue) << " " << get<2>(tupleValue) << endl;

    cout << "数を数えるための関数" << endl;
    cout << tuple_size<std::tuple<int, string, double> >::value << endl;
    cout << tuple_size<decltype(tupleValue)>::value << endl;

    int a = 100;
    int b = 99.0;
    string c = "bar";

    cout << "変数結合" << endl;
    auto new_taple = tie(a,b,c);

    cout << "Tuple 同士の結合" << endl;
    auto combine_taple = tuple_cat(tupleValue,new_taple);
    cout << tuple_size<decltype(combine_taple)>::value << endl;

    return 0;
} 
