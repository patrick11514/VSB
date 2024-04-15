all: install build

install:
	pnpm i

build:
	marp --engine engine.js presentation.md --allow-local-files --html true

buildPDF:
	marp --engine engine.js presentation.md --allow-local-files --html true  --pdf
clean:
	rm presentation.pdf
	rm presentation.html

cleanAll: clean
	rm -r node_modules
