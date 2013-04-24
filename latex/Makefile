PROD =  test.pdf

all: $(PROD)

.SUFFIXES: .tex .dvi .pdf
.tex.dvi:
	platex `basename $^ .tex`

.dvi.pdf:
	dvipdfmx `basename $^ .dvi`


.PHONY: clean
clean:
	-rm *.euc *.dvi *.pdf *.aux *.log
