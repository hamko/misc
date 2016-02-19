#include <iostream>
#include "tbb/task_scheduler_init.h" 
#include "tbb/blocked_range.h" 
#include "tbb/concurrent_vector.h" 
#include "tbb/parallel_scan.h" 
using namespace std; 
using namespace tbb;  

#define DATA_COUNT 100000

#define IdentityElement (0)
//#define IdentityElement (inf)
//#define IdentityElement (-inf)
long long op(long long a, long long b)
{
  return a + b;
//  return min(a, b);
//  return max(a, b);
}

class ParallelScanSum {
  public:
  long long reduced_result;    
  concurrent_vector<long long> inputData;
  static concurrent_vector<long long> results;

  long long get_reduced_result() const { return this->reduced_result; }
  concurrent_vector<long long> get_results() const { return this->results; }
  static ParallelScanSum pararell_scan(concurrent_vector<long long> inputData)
  {
    ParallelScanSum body(inputData);
    parallel_scan(blocked_range<long long>(0, inputData.size()), body, auto_partitioner());
    return body;
  }


  // 5 Necessary Functions to use scan: 
  // for input and reduce:  Constructer, Split Constructer
  // for reduce:            assign, reverse_join where "this" is in the right
  // for scan:              operator()
  ParallelScanSum(concurrent_vector<long long> inputData) : inputData(inputData), reduced_result(0) { }
  ParallelScanSum(ParallelScanSum& obj, split) : inputData(obj.inputData), reduced_result(0) { }
  void assign(ParallelScanSum& obj) { this->reduced_result = obj.reduced_result; }
  void reverse_join(ParallelScanSum& obj) { this->reduced_result = op(obj.reduced_result, this->reduced_result); }
  template< typename Tag >
    void operator()(const blocked_range<long long>& range, Tag) 
    {
      long long tmp = this->reduced_result;
      for (size_t i = range.begin(); i != range.end(); i++) {
        tmp = op(tmp, inputData[ i ]);
        if (Tag::is_final_scan()) 
          results[i] = tmp;
      }
      this->reduced_result = tmp;
    }
};
concurrent_vector<long long> ParallelScanSum::results(DATA_COUNT, (long long)0);

int main()
{
  task_scheduler_init init;

  concurrent_vector<long long> data;
  for (size_t i = 0; i < DATA_COUNT; i++) data.push_back(i);
//  copy(data.begin(), data.end(), ostream_iterator<long long>(cout, " ")); cout << endl;

  ParallelScanSum result = ParallelScanSum::pararell_scan(data);

  concurrent_vector<long long> output = result.get_results();
//  copy(output.begin(), output.end(), ostream_iterator<long long>(cout, " ")); cout << endl;

  cout << result.reduced_result << endl;

  return 0;
}

