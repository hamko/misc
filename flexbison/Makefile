PKG = a.out
CFLAGS = -g
objs = main.o lex.yy.o y.tab.o
FLEX_PROD = lex.yy.c
BISON_PROD = y.tab.c y.tab.h

all: a.out

lex.yy.o: lex.yy.c y.tab.h

a.out : $(objs)
	$(CXX) $(CFLAGS) -o $@ $(objs) -lfl

# -s: surpress default rule
# -d: debug mode
$(FLEX_PROD) : test.l
	flex -s test.l

# -y: POSIX Yaccモードで
# -d: %tokenのdefineをy.tab.hに出力する。Yaccモードじゃないと使えない。
# -t: debug mode
$(BISON_PROD) : syntax.y
	bison -ydt syntax.y

clean:
	-rm $(objs) $(PKG) $(FLEX_PROD) $(BISON_PROD)
