CC = g++

HEADS = 
CFLAGS = -g -Wall `xml2-config --cflags`
LFLAGS = `xml2-config --libs` -lcurl

all: simple curl_xml curl_html

simple: simple.o
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

curl_xml: curl_xml.o
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

curl_html: curl_html.o
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

.SUFFIXES: .c .o .cpp
.c.o:
	$(CC) $(CFLAGS) -c $<  $(LFLAGS)
.cpp.o:
	$(CC) $(CFLAGS) -c $<  $(LFLAGS)

$(OBJS): $(HEADS)

clean: 
	rm $(OBJS) a.out
