/* 
 * gcc -Wall -I/usr/local/include tidycurl.c -lcurl -ltidy -o tidycurl
 */ 

#include <stdio.h>
#include <tidy/tidy.h>
#include <tidy/buffio.h>
#include <curl/curl.h>
#include <string>

/* curl write callback, to fill tidy's input buffer...  */ 
uint write_cb(char *in, uint size, uint nmemb, TidyBuffer *out)
{
    uint r;
    r = size * nmemb;
    tidyBufAppend( out, in, r );
    return(r);
}

double comma_atof(char* bp)
{
    char nocommaprice[32] = {};
    char* comma = strchr(bp, ',');
    if (comma) {
        strncpy(nocommaprice, bp, (size_t)comma-(size_t)bp);
        strncpy(nocommaprice+((size_t)comma-(size_t)bp), comma+1, strlen(bp)-((size_t)comma-(size_t)bp)-1);
    } else {
        strcpy(nocommaprice, bp);
    }

    return atof(nocommaprice);
}

/* Traverse the document tree */ 
void dumpNode(TidyDoc doc, TidyNode tnod, int indent )
{
    TidyNode child;
    for ( child = tidyGetChild(tnod); child; child = tidyGetNext(child) )
    {
        ctmbstr name = tidyNodeGetName( child );
        if ( name ) /* if it has a name, then it's an HTML tag ... */ 
        {
            TidyAttr attr;
            // タグ名
            int i;
            for (i = 0; i < indent; i++) printf(" ");
            printf("%s Tag (", name);
            // 属性名
            for ( attr=tidyAttrFirst(child); attr; attr=tidyAttrNext(attr) ) {
                printf("%s", tidyAttrName(attr));
                tidyAttrValue(attr)?printf("=\"%s\" ", tidyAttrValue(attr)):printf(" ");
            }
            printf(")\n");
        }
        else {
            /* if it doesn't have a name, then it's probably text, cdata, etc... */ 
            TidyBuffer buf;
            tidyBufInit(&buf);
            tidyNodeGetText(doc, child, &buf);
            int i;
            for (i = 0; i < indent; i++) printf(" ");
            printf("%s\n", buf.bp?(char *)buf.bp:"");
            tidyBufFree(&buf);
        }
        dumpNode( doc, child, indent + 4 ); /* recursive */ 
    }
}

double f_price;
/* Traverse the document tree */ 
void extractRealtimeStockInfo(TidyDoc doc, TidyNode tnod, int indent )
{
    TidyNode child;
    for ( child = tidyGetChild(tnod); child; child = tidyGetNext(child) )
    {
        ctmbstr name = tidyNodeGetName( child );
        if ( name ) // HTML tag 
        {
            TidyAttr attr;
            // タグ名
            int i;
            for (i = 0; i < indent; i++) printf(" ");
            printf("%s Tag (", name);
            // 属性名
            for ( attr=tidyAttrFirst(child); attr; attr=tidyAttrNext(attr) ) {
                printf("%s", tidyAttrName(attr));
                tidyAttrValue(attr)?printf("=\"%s\" ", tidyAttrValue(attr)):printf(" ");
                //********************************ここから**********************************
                // Stock Price ここには属性(class)が存在するもののみ検索可能
                // <td class="stoksPrice">の検索．name=td，attr=class, value=stoksPriceに相当
                if (name && !strcmp(name, "td") && !strcmp(tidyAttrName(attr), "class") && !strcmp(tidyAttrValue(attr), "stoksPrice")) {
                    TidyNode childchild = tidyGetChild(child);
                    TidyBuffer buf;
                    tidyBufInit(&buf);
                    tidyNodeGetText(doc, childchild, &buf);
                    f_price = comma_atof(buf.bp); // Price
                }
                //****************************ここまでが編集対象****************************
            }
                //********************************ここから**********************************
                // Stock Price ここには属性(class)が存在するもののみ検索可能
                // <td class="stoksPrice">の検索．name=td，attr=class, value=stoksPriceに相当
                if (name && !strcmp(name, "td")) {
                    printf("*");
                }
                //****************************ここまでが編集対象****************************

            printf(")");
            printf("\n");
        } else { /* text, cdata, etc... */ 
            TidyBuffer buf;
            tidyBufInit(&buf);
            tidyNodeGetText(doc, child, &buf);
            int i;
            for (i = 0; i < indent; i++) printf(" ");
            printf("%s\n", buf.bp?(char *)buf.bp:"");
            tidyBufFree(&buf);
        }
        extractRealtimeStockInfo( doc, child, indent + 4 ); /* recursive */ 
    }
}


int main(int argc, char **argv )
{
    CURL *curl;
    char curl_errbuf[CURL_ERROR_SIZE];
    TidyDoc tdoc;
    TidyBuffer docbuf = {0};
    TidyBuffer tidy_errbuf = {0};
    int err;

    
    char* url = "http://stocks.finance.yahoo.co.jp/stocks/chart/?code=8715.T";
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curl_errbuf);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);

    tdoc = tidyCreate();
    tidySetCharEncoding(tdoc, (const char*)"utf8");
    tidyOptSetBool(tdoc, TidyForceOutput, yes); /* try harder */ 
    tidyOptSetInt(tdoc, TidyWrapLen, 4096);
    tidySetErrorBuffer( tdoc, &tidy_errbuf );
    tidyBufInit(&docbuf);

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &docbuf);
    err=curl_easy_perform(curl);
    if ( !err ) {
        err = tidyParseBuffer(tdoc, &docbuf); /* parse the input */ 
        if ( err >= 0 ) {
            err = tidyCleanAndRepair(tdoc); /* fix any problems */ 
            if ( err >= 0 ) {
                err = tidyRunDiagnostics(tdoc); /* load tidy error buffer */ 
                if ( err >= 0 ) {
                    //                        dumpNode( tdoc, tidyGetRoot(tdoc), 0 ); /* walk the tree */ 
                    extractRealtimeStockInfo( tdoc, tidyGetRoot(tdoc), 0 ); /* walk the tree */ 
                    fprintf(stderr, "%s\n", tidy_errbuf.bp); /* show errors */ 
                }
            }
        }
    }
    else
        fprintf(stderr, "%s\n", curl_errbuf);


    printf("----------------\n");
    printf("price: %f\n", f_price);
    printf("----------------\n");

    /* clean-up */ 
    curl_easy_cleanup(curl);
    tidyBufFree(&docbuf);
    tidyBufFree(&tidy_errbuf);
    tidyRelease(tdoc);
    return(err);

    return(0);
}

