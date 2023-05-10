# Průměr a poloměr grafu
## Zadání
![Project info](https://upload.patrick115.eu/.storage/ALG_projekt.png)

## Spuštění
### Build + spuštění
```BASH
make #build
./main graf.txt #soubor s grafem
make clean #smazání binárních souborů
make all #clean + build
```
### Doxygen dokumentace
```BASH
doxygen #vygeneruje dokumentaci v LaTeXu a HTML
cd latex && make #vygeneruje pdf dokumentaci
```

- PDF dokumentace: latex/refman.pdf
- HTML dokumentace: html/index.html

## Ukázkový graf
```TXT
0 1
1 2
2 3
3 4
4 0
```