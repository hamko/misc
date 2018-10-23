dot2tex -f tikz --tikzedgelabels --graphstyle=automaton --crop -w --nominsize --autosize  testTex.dot > testTex.tex
texi2pdf testTex.tex
pdf2ps testTex.pdf
ps2eps -f testTex.ps

