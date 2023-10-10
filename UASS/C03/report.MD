# Random graf (Erdős and Rényi model)

## Základní random graf
U random grafu můžeme vidět, že všechny nody se shlukují do jednoho středu, tedy jsou rovnoměrně rozděleny. To lze vidět i na tom, že většina nodů má uprostřed tmavější vrcholy.
![random.png](https://upload.patrick115.eu/screenshot/random2.png)
To nám dokazuje i graf, na kterém lze vidět rozpoložení hran
![random_graph.png](https://upload.patrick115.eu/screenshot/random_graph.png)
Toto chování v reálných sítích nenajdeme, proto do něho přidáme náhodné hrany.

## Přidané náhodné hrany
Přidáním náhodných hran se nám graf rozdělí na více shluků, ale stále je zde vidět, že se nody shlukují do jednoho středu, hlavně podle jeho barvného odstínu. Tmavé vrcholy mají více spojení, světlejší (bílé) míň.
![real.png](https://upload.patrick115.eu/screenshot/real2.png) 
To můžeme vidět i na grafu s rozložením hran
![real_graph.png](https://upload.patrick115.eu/screenshot/real_graph.png)

# Náhodná síť
## Údaje sítě:
- 200 vrcholů
- 986 hran

Když si nad sítí vypočítáme některé její vlastnosti, tak zjístíme, že síť má průměrný stupeň **9.86**, kdy minimální stupeň je **2** a maximální **22**. Hustota stíě je poté **0.049** a průměrný shlukovací koeficient je **0.046**.

## Surové data:
Sit ma 200 vrcholu a 986 hran.
Prumerny stupen je 9.86, min. stupen je 2 a max. stupen je 22
Hustota site je 0.04954773869346734
Prumerny shlukovaci koeficient je 0.046390608574432106

## Vypočítání hodnot pro síť
### Údaje sítě:
- 1200 vrcholů
- 1988 hran

Když si nad sítí vypočítáme některé její vlastnosti, tak zjístíme, že síť má průměrný stupeň **3.313**, kdy minimální stupeň je **1** a maximální **29**. Hustota stíě je poté **0.003** a průměrný shlukovací koeficient je **0.004**.

### Surové data:
Sit ma 1200 vrcholu a 1988 hran.
Prumerny stupen je 3.3133333333333335, min. stupen je 1 a max. stupen je 29
Hustota site je 0.0027634139560745066
Prumerny shlukovaci koeficient je 0.0036628994057019207

## Vybraná síť (Dolphins)
### Údaje sítě:
- 62 vrcholů
- 159 hran

Když si nad sítí vypočítáme některé její vlastnosti, tak zjístíme, že síť má průměrný stupeň **5.129**, kdy minimální stupeň je **1** a maximální **12**. Hustota stíě je poté **0.084** a průměrný shlukovací koeficient je **0.258**.

### Surové data:
Sit ma 62 vrcholu a 159 hran.
Prumerny stupen je 5.129032258064516, min. stupen je 1 a max. stupen je 12
Hustota site je 0.08408249603384453
Prumerny shlukovaci koeficient je 0.25895824605502027

## Srovnání

||Vrcholy|Hrany|Min. stupeň|Průměrný stupeň|Max. stupeň|Hustota|Průměrný shlukovací koeficient|
|---|:-----:|:---:|:--------:|:-------------:|:---------:|:-----:|:----------------------------:|
|**Náhodná síť**|200|986|2|9.86|22|0.049|0.046|
|**Preferenční síť**|1200|1988|1|3.3133333333333335|29|0.0027634139560745066|0.0036628994057019207|
|**Dolphins**|62|159|1|5.129032258064516|12|0.08408249603384453|0.25895824605502027|
|**Dolphins - Gelphi**|62|159|1|5.129|12|0.084|0.303

### Závěr
Jak můžeme vidět, tak výstupní hodnoty z Pythonu se s hodnotama v Delphi o moc neliší, až na Průměrný shlukovací koeficient. Ten udává, průměr všech shlukovacích koeficientů vrcholů, tedy když vezmeme vrchol a jeho sousedy, tak zjišťujeme, jak moc jsou daní sousedé propojení. 