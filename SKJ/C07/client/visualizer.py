import pygame
from pygame.locals import *
import time

testovaci = [
    "ooooooooooo",
    "ottt  * ffo",
    "ott  ** ffo",
    "ot~  * fffo",
    "o~~  *  ffo",
    "o~~~**  tfo",
    "oooo*~~~~~o",
    "offo* ~~~~o",
    "offf*  ~~to",
    "offo  ~~tto",
    "ooooooooooo",
    "           ",
    "           ",
    "      p    ",
    "           ",
    "           ",
    "           ",
    "           ",
    "    p      ",
    "           ",
    "           ",
    "           "
]

class Visualizer(object):
    def __init__(self):
        pygame.init()
        self.display = pygame.display.set_mode((330,330), pygame.HWSURFACE)
        self.grass = pygame.image.load("images/trava.png").convert()
        self.tree = pygame.image.load("images/strom.png").convert()
        self.rock = pygame.image.load("images/skala.png").convert()
        self.road = pygame.image.load("images/cesta.png").convert()
        self.floor = pygame.image.load("images/podlaha.png").convert()
        self.water = pygame.image.load("images/voda.png").convert()
        self.player = pygame.image.load("images/player.png").convert_alpha()
        self.running = True
    def visualize(self,data):
        for y in range(11):
            for x in range(11):
                ox = x*30
                oy = y*30
                map_element = data[y][x]
                if map_element == " ":
                    self.display.blit(self.grass,(ox,oy))
                elif map_element == "t":
                    self.display.blit(self.tree,(ox,oy))
                elif map_element == "o":
                    self.display.blit(self.rock,(ox,oy))
                elif map_element == "~":
                    self.display.blit(self.water,(ox,oy))
                elif map_element == "f":
                    self.display.blit(self.floor,(ox,oy))
                elif map_element == "*":
                    self.display.blit(self.road,(ox,oy))
                else:
                    self.display.blit(self.rock,(ox,oy))
        if len(data)>11:
            for y in range(11):
                for x in range(11):
                    ox = x*30
                    oy = y*30
                    if data[y+11][x] == "p":
                        self.display.blit(self.player,(ox,oy))
        #self.display.blit(self.player,(5*30,5*30))
        pygame.display.flip()
        for event in pygame.event.get():
            if event.type == QUIT:
                self.running = False
    def __enter__(self):
        return self
    def __exit__(self, otype, value, traceback):
        pygame.quit()

if __name__ == "__main__":
    with Visualizer() as vis:
        while vis.running:
            vis.visualize(testovaci)
            time.sleep(0.1)
