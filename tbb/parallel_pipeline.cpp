#include <iostream>
#include <vector>
#include <string>
#include <tbb/tbb.h>
#include <utility>
#include "tbb/task_scheduler_init.h"

using namespace std;
using namespace tbb;
typedef tbb::mutex::scoped_lock lock_t;

typedef vector<string>::iterator iterator_t;

// 1st-stage : おサシミを引く
class osashimi : public tbb::filter {
    iterator_t first_;
    iterator_t last_;
    public:
    osashimi(iterator_t f, iterator_t l) 
        : tbb::filter(tbb::filter::serial_in_order), first_(f), last_(l) {}
    void* operator()(void*) { // 初段なので引数は無視
        if ( first_ == last_ ) { // おサカナがなくなったら終了
            return 0;
        }
        string* result = new string(*first_ + "のおサシミ");
        ++first_;
        return result;
    }
};

// 2nd-stage : 皿に盛る
class moritsuke : public tbb::filter {
    public:
        moritsuke() : tbb::filter(tbb::filter::serial_in_order) {}
        void* operator()(void* p) {
            string* sp = static_cast<string*>(p);
            sp->append("を皿に盛り");
            return sp;
        }
};

// 3rd-stage : ツマを添える
class tsuma : public tbb::filter {
    public:
        tsuma() : tbb::filter(tbb::filter::serial_in_order) {}
        void* operator()(void* p) {
            string* sp = static_cast<string*>(p);
            sp->append("ツマを添えて");
            return sp;
        }
};

// 4th-stage : タンポポを乗せる
class tampopo : public tbb::filter {
    public:
        tampopo() : tbb::filter(tbb::filter::serial_in_order) {}
        void* operator()(void* p) {
            string* sp = static_cast<string*>(p);
            sp->append("タンポポ乗せる");
            return sp;
        }
};

// last-stage : ショーケースに並べる
class showcase : public tbb::filter {
    public:
        showcase() : tbb::filter(tbb::filter::serial_in_order) {}
        void* operator()(void* p) {
            string* sp = static_cast<string*>(p);
            cout << *sp << endl;
            delete sp;
            return 0;
        }
};


int main() {
    task_scheduler_init init;
    const char* table[] = { "マグロ", "ハマチ", "サワラ", "ホタテ" };
    vector<string> fish(table, table+4);

    // C++11の表記
    vector<string>::iterator first = fish.begin();
    vector<string>::iterator last  = fish.end();
    tbb::parallel_pipeline(1,
            // ---- 1st filter
            tbb::make_filter<void,string>(
                tbb::filter::serial_in_order, 
                [&](tbb::flow_control& fc) -> string {
                if ( first == last ) {
                    fc.stop();
                    return ""; // dummy value
                }
                return *first++ + "のおサシミ";
                })
            &
            // ---- 2nd filter
            tbb::make_filter<string,string>(
                tbb::filter::serial_in_order, 
                [](const string& s) -> string {
                    return s + "を皿に盛り";
                })
            &
            // ---- 3rd filter
            tbb::make_filter<string,string>(
                tbb::filter::serial_in_order, 
                [](const string& s) -> string {
                    return s + "ツマを添えて";
                })
            &
            // ---- 4th filter
            tbb::make_filter<string,string>(
                tbb::filter::serial_in_order,
                [](const string& s) -> string {
                return s + "タンポポ乗せる\n";
                })
            &
            // ---- last filter
            tbb::make_filter<string,void>(
                tbb::filter::serial_in_order,
                [](const string& s) -> void {
                cout << s;
                })
            );


    // C++11の表記
    tbb::mutex mtx;
    int count = 1;
    int limit = 20;
    tbb::parallel_pipeline(6, // ココを書き換えて遊んでみよう!
            // ---- 1st filter
            tbb::make_filter<void,int>(
                tbb::filter::serial_in_order, 
                [&](tbb::flow_control& fc) -> int { // void(はじめはflow_control) -> int
                if ( count == limit ) {
                    fc.stop();
                    return -1; // dummy value
                }
                lock_t lock(mtx); // ブロック抜けるまでロック？
                cout << string(count,' ') << "1st(" << count << ")\n";
                return count++;
                })
            &
            // ---- 2nd filter
            tbb::make_filter<int,pair<int,double>>(
                tbb::filter::serial_in_order, 
                [&](int n) -> pair<int,double> { // int -> pair
                lock_t lock(mtx);
                cout << string(n,' ') << "2nd(" << n << ")\n";
                return pair<int,double>(n, 1.0/n);
                })
            &
            // ---- 3rd filter
            tbb::make_filter<pair<int,double>,void>(
                tbb::filter::serial_in_order,
                [&](pair<int,double> p) -> void { // pair -> void
                lock_t lock(mtx);
                cout << string(p.first,' ') << "3rd(" << p.first << ") : " << p.second << endl;
                })
            );


    // C++99の表記
    // 入力系列とパイプラインを生成
    tbb::pipeline pipeline;

    // 各stageをパイプラインに接続
    osashimi  first_body(fish.begin(), fish.end());
    moritsuke second_body;
    tsuma     third_body;
    tampopo   fourth_body;
    showcase  last_body;

    pipeline.add_filter(first_body);
    pipeline.add_filter(second_body);
    pipeline.add_filter(third_body);
    pipeline.add_filter(fourth_body);
    pipeline.add_filter(last_body);

    pipeline.run(3);
    pipeline.clear();

    init.terminate();

    return 0;
}
