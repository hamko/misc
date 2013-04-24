#ifndef PARSE_TWITTER_HPP_INCLUDED
#define PARSE_TWITTER_HPP_INCLUDED

#include <map>
#include <string>
#include <vector>

using namespace std;

class tweetinfo {
public:
    map<string, string> m_st;

    tweetinfo(map<string, string>& m);
    void print_tweetinfo(void);
};

extern vector<tweetinfo*> get_tweets(const char* input);

#endif
