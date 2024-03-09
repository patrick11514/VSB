class Kniha:
    def __init__(self, name: str, author: str, year: int, pages: int, isbn: str):
        self._name = name
        self._author = author
        self._year = year
        self._pages = pages
        self._isbn = isbn

    def __str__(self):
        return f"Název knihy: {self._name}, Autor: {self._author}"

    def vek_knihy(self):
        return 2024 - self._year


class Knihovna:
    _kniha: list[Kniha] = []

    def pridej_knihu(self, kniha: Kniha):
        self._kniha.append(kniha)

    def vypis_knihy(self, **kwargs: str):
        if "author" in kwargs:
            for k in self._kniha:
                if k._author == kwargs["author"]:
                    yield k
        if "name" in kwargs:
            for k in self._kniha:
                if k._name == kwargs["name"]:
                    yield k

        return []

    def odstran_knihu(self, isbn: str):
        self._kniha = list(filter(lambda k: k._isbn != isbn, self._kniha))

    def vypis(self):
        for k in self._kniha:
            print(k)


hp = Kniha("Harry Potter: Kámen Mudrců", "J.K. Rowlingová", 2004, 356, "1758197895")
pp = Kniha("Pán Prstenů", "Idk", 2012, 555, "58718951")

kn = Knihovna()

kn.pridej_knihu(hp)
kn.pridej_knihu(pp)


print("===============")
print("PRED:")
kn.vypis()
print("===============")
print("SEARCH:")
for k in kn.vypis_knihy(name="Pán Prstenů"):
    print(k)


kn.odstran_knihu("58718951")

print("===============")
print("PO:")
kn.vypis()

print("===============")
