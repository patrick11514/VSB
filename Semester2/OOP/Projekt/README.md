# OOP Projekt - Pizzerie

## Popis
V projektu je hlavní třída Pizzerie, ta obsahuje:
- pole ukazatelů na objekty třídy Zaměstnanec
- pole ukazatelů na objekty třídy Objednávka
- jmeno pizzerie

Třída zaměstnanec obsahuje:
- jmeno
- příjmení

Ze třídy zaměstnanec dědí poté:
- Chef - pizzař
- Service - obsluha

Chef má navíc:
- pole ukazatelů na objekty třídy Objednávka

Třída Objednávka obsahuje:
- pole ukazatelů na objekty třídy Pizza
- jmeno zákazníka
- adresa zákazníka
- telefon na zákazníka

Z třídy Objednávka dědí poté:
- Online Objednávka
- Osobní Objednávka

Online Objednávka má navíc:
- cena za dovoz

Třída Pizza obsahuje:
- jmeno pizzy
- cena pizzy
- pole ukazatelů na objekty třídy Ingredience

Z třídy Pizza dědí poté:
- Custom (Vlastní) Pizza
- Vegetarianská Pizza

Třída Custom Pizza má navíc:
- metoda na přidání sýru
- metoda na přidání masa
- metoda na přidání zeleniny
- každá metoda poté navyšuje cenu pizzy

Třida Vegetarianská Pizza má navíc:
- překrytí metody, která do pizzy dá výchozí suroviny
- metoda na přidání zeleniny

Třída Ingredience obsahuje:
- jmeno ingredience
- kvanitita ingredience

Z třídy Ingredience dědí poté:
- Sýr
- Maso
- Zelenina

Třída Sýr má navíc:
- typ sýru

**V main souboru je poté ukázka základního fungování restaurace**
**Podrobnější popis tříd je v hlavičkových souborech, případně pdf soubory vygenerovaný Doxygenem**

## Zadání

1. Alespoň 7 rozdílných tříd. Třídy mají atributy a/nebo metody, konstruktor a destruktor. **0,5b**
Třídy jsou v samostatných .h a .cpp souborech, v hlavním .cpp souboru je pouze funkce main(). **0,5b**

2. V main() funkci je vytvořeno několik instancí tříd a jsou volány metody, které demonstrují funkci vytvořených struktur a dávají smysl v kontextu projektu. **1b**

3. Alespoň jedna třída má implementováno dynamické pole ukazatelů na objekty jiné třídy. **0,5b**
Jsou implementovány metody pro přidání a mazání prvků pole (včetně přeskládání prázdného místa). **0,5b**

4. Alespoň jedna třída má přetížený konstruktor. **1b**

5. Alespoň jedna třída má třídní proměnnou (static), která udržuje počet instancí třídy, včetně implementace do konstruktoru a destruktoru. **1b**

6. V projektu existuje dědičná hiearchie obsahující alespoň 3 třídy. **0,5b**

7. Dědičnost bude obsahovat a používat rozšíření (dat i metod), změna chování s využitou pozdní vazbou **0,5b**

8. K projektu je napsána průvodní dokumentace. **0,5b**

9. V projektu je implementován protected atribut. **0,5b**

10. V projektu je implementována alespoň jedna ryze abstraktní třída. **1b**

11. V projektu je využit polymorfismus (např. v dynamickém poli ukazatelů na objekty různých tříd). **1b**

- Projekt je odevzdán do 8.5. 2023 23:59 na emailovou adresu martin.radvansky1@vsb.cz
- Projekt je v jednom souboru .cpp nebo v archivu se soubory .cpp a .h
- Soubory nebo archiv obsahují v názvu vaše univerzitní ID (aaa0000)
- Je přiložen soubor s dokumentací k projektu (.docx, .pdf, .txt)
- Projekt je možné zkompilovat a spustit v MS Visual Studio (pokud programujete- v jiném programu nebo pod Linuxem, ověřte si to! Zvláště si pohlídejte- správnou inicializaci proměnných a polí)
- Projekt obsahuje pouze aktivní kód a komentáře, usnadňující orientaci či- vysvětlující funkci částí kódu (žádné bloky "vypnutého" kódu)
- Projekt má textový výstup do konzole, který je srozumitelný i bez náhledu do- kódu
- Všechny třídy a metody mají název v angličtině
- **Pokud nebude projekt splňovat výše uvedené body, pak ho ani nebudu opravovat**

**Minimální počet bodů pro udělení zápočtu a úspěšně obhájený projekt je 5 z 9 bodů.**

### Checklist Zadání 
1.
    1.1. **Ok** 
    1.2. **Ok**
2. **Ok**
3. 
    3.1. **Ok**
    3.2. **Ok**
4. **Ok**
5. **Ok**
6. **Ok**
7. **Ok**
8. **Ok**
9. **Ok**
10. **Ok**
11. **Ok**

## Příkazy
- **Build projektu**
```BASH
make
```
- **Spuštění projektu**
```BASH
./main
```
- **Build dokumentace**
```BASH
doxygen #vygeneruje dokumentaci podle Doxyfile
cd latex
make
```