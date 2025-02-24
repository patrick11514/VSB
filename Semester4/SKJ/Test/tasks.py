from typing import List, Dict, Tuple

"""
Úkol 1 (15 bodů)

Jednotka SG-11 se ztratila! Je třeba pro ni vyslat záchrannou misi.
Bohužel není jasné, kde přesně se ztratila, k dispozici jsou pouze útržky nouzových signálů z různých bodů,
kde by jednotka mohla být.

Pomozte najít jednotku SG-11 tím, že naimplementujete funkci spocti_krychli.
Funkce ze souboru na zadané cestě načte 3D souřadnice bodů a najde nejmenší krychli, která ohraničuje tyto
souřadnice (stačí naleznout minimální/maximální souřadnice na všech osách).
Každý řádek v souboru reprezentuje jeden 3D bod, jednotlivé souřadnice jsou odděleny čárkou.
Funkce poté vrátí objem tohoto krychle, aby šlo zjistit, jak velký prostor je nutný prohledat k nalezení jednotky.

Příklad (ukázka souboru soubor_souradnice):
1,2,3
-5,8,-20
2,6,0

spocti_krychli("souradnice_test.txt") # 966
# minimum první souřadnice je -5, maximum je 2, takže první rozměr je 7, obdobně lze naleznout zbylé rozměry
"""


def spocti_krychli(soubor_souradnice: str):
    with open(soubor_souradnice) as file:
        lines = map(lambda line: line.strip(), file.readlines())

        xes: List[str] = []
        yes: List[str] = []
        zes: List[str] = []

        for line in lines:
            [x, y, z] = line.split(",")
            xes.append(int(x))
            yes.append(int(y))
            zes.append(int(z))

        x = max(xes) - min(xes)
        y = max(yes) - min(yes)
        z = max(zes) - min(zes)

        return x * y * z


"""
Úkol 2 (10 bodů)

Pro nalezení SG-11 je třeba vyřešit další problém - nefunguje ovládací panel k Hvězdné bráně.
Naštěstí Samantha Carterová navrhla dočasné řešení - pomozte jí vytvořit softwarový modul pro ovládání brány.

Naimplementujte třídu OvladaciPanel, která obdrží seznam znaků na panelu.
Znaky jsou uspořádané do kruhu, jeden ze znaků je vždy aktivní (na začátku to bude nultý znak v seznamu).
Uživatel může měnit aktivní znak pomocí pohybu doleva nebo doprava.
Nezapomeňte, že znaky jsou uspořádány do kruhu, lze je tak všechny projet pohybem pouze doleva nebo pouze doprava.

Uživatel může aktivní znak zadat, čímž dojde k přidání aktivního znaku do adresy.
Po přidání všech požadovaných znaků může vytočit adresu (při vytočení panel vrátí seznam všech navolených
znaků v pořadí, ve kterém byly navoleny).

Příklad:
panel = OvladaciPanel(["A", "B", "C"]) # nejprve je aktivní znak "A"
panel.posun_doprava() # nyní je aktivní znak "B"
panel.zadej_znak()    # znak "B" je přidán do vytočené adresy
panel.posun_doprava() # nyní je aktivní znak "C"
panel.posun_doprava() # nyní je aktivní znak "A"
panel.zadej_znak()    # znak "A" je přidán do vytočené adresy
panel.posun_doleva()  # nyní je aktivní znak "C"
panel.zadej_znak()    # znak "C" je přidán do vytočené adresy
panel.vytoc_adresu()  # vrátí ["B", "A", "C"]
"""


class OvladaciPanel:
    current_symbols: List[str]
    current_index: int
    address: List[str]

    def __init__(self, symbols: List[str]):
        self.current_index = 0
        self.address = []
        self.current_symbols = symbols

    def posun_doprava(self):
        self.current_index += 1
        if self.current_index >= len(self.current_symbols):
            self.current_index = 0

    def posun_doleva(self):
        self.current_index -= 1
        if self.current_index < 0:
            self.current_index = len(self.current_symbols) - 1

    def zadej_znak(self):
        self.address.append(self.current_symbols[self.current_index])

    def vytoc_adresu(self):
        return self.address


"""
Úkol 3 (15 bodů)

Nyní už víme přibližnou lokaci SG-11 a máme funkční ovládací panel, zbývá určit přesnou polohu jednotky.
V operačním deníku jednotky jsou adresy planet, které SG-11 navštívila, z těchto informací a přibližné lokace
jednotky by mělo jít dohledat, kde se jednotka přesně nachází.

Naimplementujte funkci nejcastejsi_glyfy.
Funkce ze souboru na zadané cestě načte adresy planet, kam SG-11 cestovala.
Každá adresa (řádek v souboru) je tvořena několika slovy (glyfy) oddělenými mezerou.
Spočítejte, kolikrát se jednotlivé glyfy vyskytují v souboru a vraťte z funkce seznam dvojic (glyf, počet výskytů)
seřazený sestupně dle počtu výskytů jednotlivých glyfů.
Pokud budou mít dva nebo více glyfů stejný počet výskytů, seřaďte je lexikograficky vzestupně dle jejich názvu
("dle abecedy").
V jedné adrese se konkrétní glyf může vyskytovat maximálně jednou.

Příklad (ukázka souboru soubor_adresy):
Crater Taurus Virgo Capricornus Auriga Eridanus Gemini
Taurus Crater Lynx Hydra Auriga Sagittarius Orion
Crater Aries Taurus Scutum Sagittarius Gemini Norma

nejcastejsi_glyfy("planety_test.txt")
# [
#  ('Crater', 3), ('Taurus', 3), ('Auriga', 2), ('Gemini', 2), ('Sagittarius', 2), ('Aries', 1),
#  ('Capricornus', 1), ('Eridanus', 1), ('Hydra', 1), ('Lynx', 1), ('Norma', 1), ('Orion', 1), ('Scutum', 1),
#  ('Virgo', 1)
# ]
"""


def do_sort(a: Tuple[str, int], b: Tuple[str, int]):
    return a[0] > b[0]


def nejcastejsi_glyfy(soubor_adresy: str):
    with open(soubor_adresy) as file:
        lines = map(lambda line: line.strip(), file.readlines())

        planets: Dict[str, int] = {}

        for line in lines:
            for planet in line.split(" "):
                if planet in planets:
                    planets[planet] += 1
                else:
                    planets[planet] = 1

        # sort

        tuples: List[Tuple[str, int]] = list(planets.items())
        tuples.sort(key=lambda x: (-1 * x[1], x[0]), reverse=False)

        return tuples
