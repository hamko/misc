#include "twitterGetTimeline.hpp"
#include "parseTwitterHome.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <queue>
#include <algorithm>
#include "config.hpp"

#define _TWITCURL_TEST_

using namespace std;

twitCurl twitterObj;
queue<tweetinfo*> g_tweets; // これがポインターだと解放らへんでめんどい，popした瞬間に解放するのが今のところいいか
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void getTweetsLoop(void)
{
    cout << "called getTweetsLoop" << endl;
    string latest_tweet("");
    while (1) {
        cout << "Loop getTweetsLoop" << endl;
        /* Get public timeline */
        string replyMsg = "";
        printf( "\nGetting friend timeline\n" );
        if( twitterObj.timelineHomeGet(latest_tweet) ) {
            twitterObj.getLastWebResponse( replyMsg );

            vector<tweetinfo*> newtweets = get_tweets(replyMsg.c_str());
            if (newtweets.size()) 
                latest_tweet = newtweets[0]->m_st["/id"];
            for (int i = 0; i < (int)newtweets.size(); i++) 
                cout << "\x1b[41m" << newtweets[i]->m_st["/user/name"] << "\x1b[49m" << newtweets[i]->m_st["/text"] << endl;

            reverse(newtweets.begin(), newtweets.end());
            for (int i = 0; i < (int)newtweets.size(); i++) {
                pthread_mutex_lock(&mutex); // lock for g_tweet 
                g_tweets.push(newtweets[i]);
                pthread_mutex_unlock(&mutex); // unlock for g_tweet 
            }

            // How to get tweet from Twitter
            /*
            pthread_mutex_lock(&mutex);
            while (!g_tweets.empty()) {
                tweetinfo* it = g_tweets.front();
                g_tweets.pop();
                cout << "#queue " << it->m_st["/user/name"] << endl;
                delete it;
            }
            pthread_mutex_unlock(&mutex);
            */
        } else {
            twitterObj.getLastCurlError( replyMsg );
            printf( "\ntwitterClient:: twitCurl::timelinePublicGet error:\n%s\n", replyMsg.c_str() );
        }
        sleep(20);
    }
}

void* juwitter_get_main(void* argv)
{
    cout << "called juwitter_get_main" << endl;

    std::string tmpStr;
    std::string replyMsg;
    char tmpBuf[1024];

    /* Set twitter username and password */
    std::string userName( USER_NAME );
    std::string passWord( PASSWORD );
    twitterObj.setTwitterUsername( userName );
    twitterObj.setTwitterPassword( passWord );

    /* OAuth flow begins */
    /* Step 0: Set OAuth related params. These are got by registering your app at twitter.com */
    twitterObj.getOAuth().setConsumerKey( std::string( CONSUMERKEY ) );
    twitterObj.getOAuth().setConsumerSecret( std::string( CONSUMERSECRET ) );

    /* Step 1: Check if we alredy have OAuth access token from a previous run */
    std::string myOAuthAccessTokenKey("");
    std::string myOAuthAccessTokenSecret("");
    std::ifstream oAuthTokenKeyIn;
    std::ifstream oAuthTokenSecretIn;

    oAuthTokenKeyIn.open( "twitterClient_token_key.txt" );
    oAuthTokenSecretIn.open( "twitterClient_token_secret.txt" );

    memset( tmpBuf, 0, 1024 );
    oAuthTokenKeyIn >> tmpBuf;
    myOAuthAccessTokenKey = tmpBuf;

    memset( tmpBuf, 0, 1024 );
    oAuthTokenSecretIn >> tmpBuf;
    myOAuthAccessTokenSecret = tmpBuf;

    oAuthTokenKeyIn.close();
    oAuthTokenSecretIn.close();

    if( myOAuthAccessTokenKey.size() && myOAuthAccessTokenSecret.size() )
    {
        /* If we already have these keys, then no need to go through auth again */
        printf( "\nUsing:\nKey: %s\nSecret: %s\n\n", myOAuthAccessTokenKey.c_str(), myOAuthAccessTokenSecret.c_str() );

        twitterObj.getOAuth().setOAuthTokenKey( myOAuthAccessTokenKey );
        twitterObj.getOAuth().setOAuthTokenSecret( myOAuthAccessTokenSecret );
    } else {
        /* Step 2: Get request token key and secret */
        std::string authUrl;
        twitterObj.oAuthRequestToken( authUrl );

        /* Step 3: Get PIN  */
        memset( tmpBuf, 0, 1024 );
        printf( "\nDo you want to visit twitter.com for PIN (0 for no; 1 for yes): " );
        gets( tmpBuf );
        tmpStr = tmpBuf;
        if( std::string::npos != tmpStr.find( "1" ) )
        {
            /* Ask user to visit twitter.com auth page and get PIN */
            memset( tmpBuf, 0, 1024 );
            printf( "\nPlease visit this link in web browser and authorize this application:\n%s", authUrl.c_str() );
            printf( "\nEnter the PIN provided by twitter: " );
            gets( tmpBuf );
            tmpStr = tmpBuf;
            twitterObj.getOAuth().setOAuthPin( tmpStr );
        } else {
            /* Else, pass auth url to twitCurl and get it via twitCurl PIN handling */
            twitterObj.oAuthHandlePIN( authUrl );
        }

        /* Step 4: Exchange request token with access token */
        twitterObj.oAuthAccessToken();

        /* Step 5: Now, save this access token key and secret for future use without PIN */
        twitterObj.getOAuth().getOAuthTokenKey( myOAuthAccessTokenKey );
        twitterObj.getOAuth().getOAuthTokenSecret( myOAuthAccessTokenSecret );

        /* Step 6: Save these keys in a file or wherever */
        std::ofstream oAuthTokenKeyOut;
        std::ofstream oAuthTokenSecretOut;

        oAuthTokenKeyOut.open( "twitterClient_token_key.txt" );
        oAuthTokenSecretOut.open( "twitterClient_token_secret.txt" );

        oAuthTokenKeyOut.clear();
        oAuthTokenSecretOut.clear();

        oAuthTokenKeyOut << myOAuthAccessTokenKey.c_str();
        oAuthTokenSecretOut << myOAuthAccessTokenSecret.c_str();

        oAuthTokenKeyOut.close();
        oAuthTokenSecretOut.close();
    }
    /* OAuth flow ends */

    getTweetsLoop();

    return 0;
}
