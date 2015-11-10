#include <string>
#include <iostream>
#include <curl/curl.h>
#include <libxml/xpath.h>
#include <libxml/xmlreader.h>

using namespace std;

// 再帰的parse
int count = 0;
static void print_element_names(xmlNode * a_node)
{
    xmlNode *cur_node = NULL;

    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            for (int i = 0; i < count; i++) printf("    ");
            printf("node type: Element, name: %s, property: %s\n", cur_node->name, xmlGetProp(cur_node, (const xmlChar*)"content"));
        }
        if (cur_node->type == XML_TEXT_NODE) {
            for (int i = 0; i < count; i++) printf("    ");
            printf("node type: Text, content: %s\n", cur_node->content);
        }

        count++;
        print_element_names(cur_node->children);
        count--;
    }
}


size_t callBackFunk(char* ptr, size_t size, size_t nmemb, string* stream)
{
    int realsize = size * nmemb;
    stream->append(ptr, realsize);
    return realsize;
}

int main()
{
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    string chunk;
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://stocks.finance.yahoo.co.jp/stocks/detail/?code=8715.t");
//        curl_easy_setopt(curl, CURLOPT_URL, "http://dailynews.yahoo.co.jp/fc/rss.xml");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callBackFunk);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (string*)&chunk);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    if (res != CURLE_OK) {
        cout << "curl error" << endl;
        return 1;
    }

    cout << chunk << endl;

    xmlTextReader* reader = xmlReaderForDoc((xmlChar*)chunk.c_str(), NULL, NULL, 1);
    xmlTextReaderRead(reader);
    xmlTextReaderExpand(reader);
    xmlDocPtr doc = xmlTextReaderCurrentDoc(reader);
    xmlNode* root_element = xmlDocGetRootElement(doc);
    print_element_names(root_element); // parse
    xmlFreeDoc(doc);
    xmlCleanupParser();
}
