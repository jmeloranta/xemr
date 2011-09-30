manual.pdf: manual.dvi
	dvipdf manual.dvi

manual.dvi: manual.tex manual.bib *.eps
	latex manual
	bibtex manual
	latex manual
	latex manual
clean:
	-rm manual.dvi manual.pdf manual.ps manual.log *~ *.backup *.aux *.blg *.bbl *.out


