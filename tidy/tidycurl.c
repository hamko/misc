/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2011, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at http://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/ 
/* Download a document and use libtidy to parse the HTML.
 * Written by Jeff Pohlmeyer
 *
 * LibTidy => http://tidy.sourceforge.net
 *
 * gcc -Wall -I/usr/local/include tidycurl.c -lcurl -ltidy -o tidycurl
 *
 */ 

#include <stdio.h>
#include <tidy/tidy.h>
#include <tidy/buffio.h>
#include <curl/curl.h>

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

/* Traverse the document tree */ 
void extractStockInfo(TidyDoc doc, TidyNode tnod, int indent )
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
                // Stock Price
                if (name && !strcmp(name, "td") && !strcmp(tidyAttrName(attr), "class") && !strcmp(tidyAttrValue(attr), "stoksPrice")) {
                    child = tidyGetChild(child);
                    TidyBuffer buf;
                    tidyBufInit(&buf);
                    tidyNodeGetText(doc, child, &buf);
                    printf("price is %s\n", buf.bp?(char *)buf.bp:""); // Price
                    printf("price is %f\n", comma_atof(buf.bp)); // Price
                }
            }
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
        extractStockInfo( doc, child, indent + 4 ); /* recursive */ 
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
    if ( argc == 2) {
        curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL, argv[1]);
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
                        extractStockInfo( tdoc, tidyGetRoot(tdoc), 0 ); /* walk the tree */ 
                        fprintf(stderr, "%s\n", tidy_errbuf.bp); /* show errors */ 
                    }
                }
            }
        }
        else
            fprintf(stderr, "%s\n", curl_errbuf);

        /* clean-up */ 
        curl_easy_cleanup(curl);
        tidyBufFree(&docbuf);
        tidyBufFree(&tidy_errbuf);
        tidyRelease(tdoc);
        return(err);

    }
    else
        printf( "usage: %s <url>\n", argv[0] );

    return(0);
}

