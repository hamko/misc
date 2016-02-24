#include <string>
#include <queue>
#include <iostream>
using namespace std;

int main(void)
{
  queue<string> q;
  q.push("");
  while (!q.empty()) {
    string curr = q.front(); q.pop();

    cout << curr << endl;

    if (curr.length() < 10) 
      for (char c : {'A', 'C', 'G', 'T'}) 
        q.push(curr + c);
  }

  return 0;
}
