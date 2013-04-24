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
        if (p->type == XML_ELEMENT_NODE)// && p->children) 
            rmap[prefix + "/" + (const char*)p->name] = (char *)xmlNodeGetContent(p);

        if (p->children) 
            nodeToMap(p->children, rmap, prefix + "/" + (const char*)p->name);
        p = p->next;
    }
}

int main(void)
{
    vector<tweetinfo*> tweets;

    xmlDoc* doc = xmlReadFile("test", NULL, 0);
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

    for (int i = 0; i < (int)tweets.size(); i++) {
        cout << "#TWEET NO." << i << endl;
        for (map<string, string>::iterator it = tweets[i]->m_st.begin(); it != tweets[i]->m_st.end(); it++) 
            cout << it->first << " " << it->second << endl;
    }
    return 0;
}
