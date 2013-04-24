PROD = command.term

all : $(PROD)


.SUFFIXES: .yomi .grammar .term
.grammar.term: 
.yomi.term: 
	./yomi2voca-utf.pl `basename $@ .term`.yomi > `basename $@ .term`.voca
	./mkdfa.pl `basename $@ .term`
	./generate `basename $@ .term`


.PHONE: clean
clean:
	rm *.voca *.term *.dfa *.dict
