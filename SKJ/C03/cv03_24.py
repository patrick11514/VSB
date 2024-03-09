# TODO Generátory
# TODO OOP


#! generátor - funkce s příkazem yield

# iterace - iterační protokol pracuje s iterovatelnymi objekty a s iterátory

# na iterovatelné objekty můžeme zavolat iter() -> vrací iterátor


x = iter([1, 2, 3])

# print(type(x))

# na iterátor můžeme volat next()

# print(next(x))
# print(next(x))
# print(next(x))

#! po vyčerpání dostaneme výjimku StopIteration

# print(next(x))


# ? for x in collection: ...

# iterator = iter(collection)
# while True:
#     try:
#         x = next(iterator)
#     except StopIteration:
#         break

#     ...  # tělo původního cyklu


#! generátory

# generátor je funkce vracející lazy iterátor
# příkaz yield indikuje, kdy se vrátí hodnota, narozdíl od returnu potom neopouštíš funkci

# místo toho je zapamatovaný stav funkce

a = range(5)

# print(list(a))


# ? co kdybychom chtěli vytisknout nekonečnou sekvenci?


def infinite_sequence():
    num = 0
    while True:
        yield num
        num += 1


# for i in infinite_sequence():
#     print(i, end=" ")


# i = infinite_sequence()

# print(next(i))
# print(next(i))
# print(next(i))
# print(next(i))


# ? načítání dlouhých souborů


def csv_reader(file_name):
    for row in open(file_name, "r"):
        yield row


# --------------------------------------------------#

#! OOP

# objekt: v Pythonu je objekt každá hodnota (něco, co můžeš uložit do proměnné, vrátít z funkce...)

# objekt obsahuje data a chování (instrukce/metody pracující s těmi daty)

x = 10

# print(type(x))

# ? Jak si nadefinovat vlastní třídu?


class Battlestar:
    """
    Represents a battlestar
    (its name and commander).
    """

    def __init__(self, name, commander):  # initializer
        self._name = name  # instance variable
        self._commander = commander
        self._x = 0
        self._y = 0

    def identify(self):  # method
        return "This is Battlestar {}, commanded by {}.".format(
            self._name, self._commander
        )

    @property
    def name(self):
        return self._name

    @property
    def commander(self):
        return self._commander

    @commander.setter
    def commander(self, name):
        if isinstance(name, str):
            self._name = name
        else:
            raise "Enter a string"

    @property
    def x(self):
        return self._x

    @x.setter
    def x(self, x):
        try:
            self._x = int(x)
        except:
            raise "Enter number"

    @property
    def y(self):
        return self._y

    @y.setter
    def y(self, y):
        try:
            self._y = int(y)
        except:
            raise "Enter number"


galactica = Battlestar("Galactica", "Bill Adama")
pegasus = Battlestar("Pegasus", "Helena Cain")

print(galactica.identify())

print(galactica.x)

print(f"{galactica.name}: [{galactica.x}, {galactica.y}]")

galactica.x = 10
galactica.y = 10

print(f"{galactica.name}: [{galactica.x}, {galactica.y}]")
print(pegasus.identify())


# TODO print only name


# ------------------------------------------------------------

# TODO zviratka
"""
Task:
Create Dog and Cat classes. Both classes will have the same 
constructor that will take one argument and name the animal.
Next, implement the "make_sound" method, which prints a string:
    "Name: MNAU!"
    or
    "Name: HAF!"
    depending on whether you are implementing a cat or a dog.
Finally, create a list in which you put several instances of classes
Dog and Cat. Using the for loop call the make_sound method.
In this way we find out that in dynamically typed languages ​​it is not necessary to 
implement a common ancestor or a common interface to obtain a polymorphism.
"""


class Animal:
    def __init__(self, name):
        self._name = name


class Dog(Animal):
    def __init__(self, name):
        super().__init__(name)

    def make_sound(self):
        print(f"{self._name}: HAF!")


class Cat(Animal):
    def __init__(self, name):
        super().__init__(name)

    def make_sound(self):
        print(f"{self._name}: MNAU!")


animals = [Dog("Lassie"), Cat("Mikes"), Dog("Zeryk")]

for animal in animals:
    animal.make_sound()
