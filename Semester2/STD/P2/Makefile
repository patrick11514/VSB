COMPILER=latexmk
FILE=test.tex

compile:
	$(COMPILER) -pdf $(FILE)
watch:
	$(COMPILER) -pdf -pvc $(FILE)
clean:
	rm -f *.aux *.fdb_latexmk *.fls *.log *.out *.synctex.gz *.tocx