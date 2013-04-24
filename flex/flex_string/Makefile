objs = main.o lex.yy.o
LEXFLAG = #-d

a.out : $(objs)
	$(CXX) -o $@ $(objs) -lfl

lex.yy.c : test.l
	flex $(LEXFLAG) test.l

clean:
	-rm $(objs) a.out lex.yy.c
