#
# Makefile for pLaTeX
#
# Nov.18 2009. Ryuma NIIYAMA(c)
# ISI lab, University of Tokyo
#

# ファイル名の設定
MAIN = thesis
BIB = $(shell ls ./bibTeX/*.bib)

# インクルードされているファイル
CHAPTER=$(shell ls *.tex)
CHAPAUX=$(shell ls *.aux)

# 拡張子の登録
.SUFFIXES: .tex .dvi .aux .log .toc .lof .lot .pdf .ps .bbl .bib .blg

# 実行プログラムとオプションの設定
TEX = platex
TEX_OPT = -kanji=utf8
#TEX_OPT = -kanji=euc-jp
BIBTEX = pbibtex
#BIBTEX = jbibtex
BIBTEX_OPT =  -kanji=utf8
#BIBTEX_OPT =  -kanji=euc-jp
DVIPS = dvips
DVIPS_OPT = -t a4 -Ppdf
DVIPDF = dvipdfmx
CONV = ps2ps-utf_for-bookmarks-jp.pl
CONVOPT = utf-8
SUF = _bookmarks

# ターゲットとルール
all: $(MAIN).dvi

pdf: $(MAIN).pdf
dvi: $(MAIN).dvi
aux: $(MAIN).aux
ps:  $(MAIN).ps


# 依存関係
# 1. .tex -(platex)-> .aux (AUXiliary file)
#                  -> .toc(Table Of Contents), .log(LOG file)
#                  -> .lof(List Of Figure), .lot(List Of Table)
# 2. .aux .bib -(jbibtex)-> .bbl (BiBliography List), .blg(BibTeX LoG file)
# 3. .tex .bbl -(platex)-> .aux
# 4. .tex .bbl .aux -(platex)-> .dvi (DeVice Independent format)
$(MAIN).dvi:$(MAIN).tex $(MAIN).bbl $(MAIN).aux $(CHAPTER) $(CHAPAUX) $(BIB)
$(MAIN).bbl:$(MAIN).aux $(BIB)
$(MAIN).aux:$(MAIN).tex $(CHAPTER)

$(MAIN).pdf:$(MAIN).dvi
$(MAIN).ps:$(MAIN).dvi


# 実際の生成コマンド
.dvi.ps:
	$(DVIPS) $^
#	-cat $@ | perl $(CONV) $(CONVOPT) > $*$(SUF).ps
.ps.pdf:
	ps2pdf $<
.tex.aux:
	$(TEX) $(TEX_OPT) $*
	@echo "=================.tex -> .aux=========================="
	@echo ""
.tex.dvi:
	$(BIBTEX) $(BIBTEX_OPT) $*
	@echo "=================.aux -> .bbl=========================="
	@echo ""
	$(TEX) $(TEX_OPT) $*
	@echo "=================.tex .bbl -> .aux====================="
	@echo ""
	$(TEX) $(TEX_OPT) $*
	@echo "=================.tex .bbl .aux -> .dvi================"
	@echo ""


clean:
	rm -f *~ $(MAIN).aux $(MAIN).toc $(MAIN).log\
		 $(MAIN).lof $(MAIN).lot\
		 $(MAIN).dvi $(MAIN).pdf $(MAIN).ps $(MAIN)$(SUF).ps\
		 $(MAIN).bbl $(MAIN).blg $(MAIN).out
	rm -f $(CHAPAUX) *.pbm
