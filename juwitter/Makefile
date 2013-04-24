CC=g++
SRC = julius-simple.cpp twitterPostTweet.cpp twitterGetTimeline.cpp parseTwitterHome.cpp initJuwitter.cpp

#### When using system-installed libraries
CPPFLAGS= -g -Wall -O3 `libjulius-config --cflags` `libsent-config --cflags` `xml2-config --cflags`
LDFLAGS=`libjulius-config --libs` `libsent-config --libs` `xml2-config --libs` -ltwitcurl -lpthread

all: julius-simple 

julius-simple: $(SRC)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $(SRC) $(LDFLAGS)

clean:
	$(RM) *.o *.bak *~ core TAGS 
	$(RM) julius-simple
