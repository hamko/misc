#include <iostream>
#include <assert.h>
#include <map>
#include <string>
#include <cstring>
#include <vector>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "parseTwitterHome.hpp"

using namespace std;

tweetinfo::tweetinfo(map<string, string>& m)
{
    m_st = m;
}

void nodeToMap(xmlNode* node, map<string, string>& rmap, string prefix)
{
    xmlNode* p = node;
    while (p != NULL) {
        if (p->type != XML_ELEMENT_NODE) {
            p = p->next;
            continue;
        }
        rmap[prefix + "/" + (const char*)p->name] = (char *)xmlNodeGetContent(p);
        if (p->children) 
            nodeToMap(p->children, rmap, prefix + "/" + (const char*)p->name);
        p = p->next;
    }
}

vector<tweetinfo*> get_tweets(const char* input)
{
    if (strstr(input, "created_at") == NULL) {
        return vector<tweetinfo*>();
    }

    vector<tweetinfo*> tweets;
//    xmlDoc* doc = xmlReadFile("test", NULL, 0);
    xmlDoc* doc = xmlReadDoc((const unsigned char*)input, NULL, NULL, 0);
    xmlNode* root = xmlDocGetRootElement(doc);

    xmlNode* p = root->children;
    while (p) {
        if (p->type != XML_ELEMENT_NODE) {
            p = p->next;
            continue;
        }
        map<string, string> m;
        nodeToMap(p->children, m, "");
        tweetinfo* t = new tweetinfo(m);
        tweets.push_back(t);

        p = p->next;
    }

    return tweets;
}

void tweetinfo::print_tweetinfo(void)
{
    for (map<string, string>::iterator it = m_st.begin(); it != m_st.end(); it++) 
        cout << it->first << " " << it->second << endl;
}
