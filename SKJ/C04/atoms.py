import random

import playground

colors = ["blue", "red", "green", "yellow"]


class Atom:
    def __init__(self, x: int, y: int, speed_x: int, speed_y: int, rad: int, color):
        """
        Inicializator třídy Atom
        :param x: souřadnice X
        :param y: soouřadnice Y
        :param rad: poloměr
        :param color: barva
        """
        self.x = x
        self.speed_x = speed_x
        self.y = y
        self.speed_y = speed_y
        self.rad = rad
        self.color = color

    def to_tuple(self):
        """Vrátí n-tici hodnot
        příklad: x = 10, y = 12, rad = 15, color = 'green' -> (10,12,15,'green')
        """
        return (self.x, self.y, self.rad, self.color)

    def move(self, width: int, height: int, back: bool):
        self.x = self.x + self.speed_x
        self.y = self.y + self.speed_y

        if self.x + self.rad > width:
            self.x = width - self.rad
            self.speed_x = -1 * self.speed_x
            if back:
                self.speed_y = -1 * self.speed_y

        if self.y + self.rad > height:
            self.y = height - self.rad
            self.speed_y = -1 * self.speed_y
            if back:
                self.speed_x = -1 * self.speed_x

        if self.x <= self.rad:
            self.x = self.rad
            self.speed_x = -1 * self.speed_x
            if back:
                self.speed_y = -1 * self.speed_y

        if self.y <= self.rad:
            self.y = self.rad
            self.speed_y = -1 * self.speed_y
            if back:
                self.speed_x = -1 * self.speed_x


class FallDownAtom(Atom):
    g = 3.0
    damping = 0.8

    def move(self, width: int, height: int, back: bool):
        self.x = self.x + self.speed_x
        self.y = self.y + self.speed_y

        if self.x + self.rad > width:
            self.x = width - self.rad
            self.speed_x = -1 * self.speed_x
            if back:
                self.speed_y = -1

        if self.y + self.rad > height:
            self.y = height - self.rad
            self.speed_y = -1 * self.speed_y * self.damping
            if back:
                self.speed_x = -1 * self.speed_x * self.damping
            else:
                self.speed_x = self.speed_x * self.damping

        if self.x <= self.rad:
            self.x = self.rad
            self.speed_x = -1 * self.speed_x
            if back:
                self.speed_y = -1

        if self.y <= self.rad:
            self.y = self.rad
            self.speed_y = -1 * self.speed_y * self.damping
            if back:
                self.speed_x = -1 * self.speed_x * self.damping

        self.speed_y += self.g


class ExampleWorld(object):

    def __init__(self, size_x: int, size_y: int, count: int, fallingAtom: bool):
        self.width = size_x
        self.height = size_y
        self.fallingAtom = fallingAtom
        self.atoms = [self.random_atom() for _ in range(count)]

    def random_atom(self):
        radius = random.randint(5, 20)

        X = random.randint(0, self.width - radius)
        Y = random.randint(0, self.height - radius)
        color = colors[random.randint(0, len(colors) - 1)]

        speed_X = random.randint(1, 40)
        speed_Y = random.randint(1, 40)
        if self.fallingAtom:
            return FallDownAtom(X, Y, speed_X, speed_Y, radius, color)
        return Atom(X, Y, speed_X, speed_Y, radius, color)

    def tick(self):
        """This method is called by playground. Sends a tuple of atoms to rendering engine.

        :param size_x: world size x dimension
        :param size_y: world size y dimension
        :return: tuple of atom objects, each containing (x, y, radius) coordinates
        """

        i = 0
        for a in self.atoms:
            b = False
            if i > len(self.atoms) / 2:
                b = True

            a.move(self.width, self.height, b)
            i += 1

        return tuple(map(lambda a: a.to_tuple(), self.atoms))


if __name__ == "__main__":
    size_x, size_y = 700, 400

    world = ExampleWorld(size_x, size_y, 2, True)
    playground.run((size_x, size_y), world)
