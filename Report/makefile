#
# Makefile for this assignment
#
# pdf 		- to create LaTeX outputs
# clean		- tidy up
#
# 2011 - Philipp Fonteyn
#  

TEXFILE = cg-project-report

pdf : $(TEXFILE).tex

# Create the pdf file
$(TEXFILE).tex : $(TEXFILE).toc $(TEXFILE).bib
	pdflatex $(TEXFILE).tex
	pdflatex $(TEXFILE).tex

# Create the table of contents
$(TEXFILE).toc :
	pdflatex $(TEXFILE).tex

# Compile the references
$(TEXFILE).bib : $(TEXFILE).aux
	bibtex $(TEXFILE)

# Call this with "make clean" to clean up
clean:
	rm -f *.ps *.aux *.toc *.bbl *.blg *.log *.dvi
