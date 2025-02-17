import xmlrpc.client
import socketserver
from xmlrpc.server import SimpleXMLRPCServer,SimpleXMLRPCRequestHandler
import string
import random
import time

class ThreadingXMLRPCServer(socketserver.ThreadingMixIn,SimpleXMLRPCServer):pass

class Player(object):

    def __init__(self, player_id, x, y, live, playground):
        self.x = x
        self.y = y
        self.live = live
        self.player_id = player_id
        self.playground = playground

    def move(self, dx, dy):
        item = self.playground.maplook_at(self.x+dx, self.y+dy)
        if item not in "toN~":
            self.x += dx
            self.y += dy
            #print "({0},{1})".format(self.x,self.y)

    def move_direction(self, direction):
        if direction == 'south':
            self.move(0,1)
        elif direction == 'north':
            self.move(0,-1)
        elif direction == 'west':
            self.move(-1,0)
        elif direction == 'east':
            self.move(1,0)

    def get_area(self):
        return self.playground.area_at(self.x,self.y,5)

    def __repr__(self):
        return 'ID: {}, pos: [{:02d}, {:02d}]'.format(self.player_id, self.x, self.y)

class Playground(object):

    def __init__(self, filename):
        self.map = []
        self.players = {}
        self.width = 0
        self.height = 0
        self.load_map(filename)
    
    def load_map(self, filename):
        orig = b"wase0123456789"
        tran = b"              "
        trant = bytes.maketrans(orig, tran)
        with open(filename, "r") as mapfile:
            for x in mapfile:
                linefiltered = x.translate(trant)[:len(x)-1]
                self.map.append(linefiltered)
                #print linefiltered
                self.height += 1
                self.width = len(linefiltered)
        #print self.width, self.height
    
    def maplook_at(self, x, y):
        if x>self.width-1 or y>self.height-1 or x<0 or y<0:
            return 'N'
        return self.map[y][x]
        
    def itemslook_at(self,x,y):
        for pid in self.players:
            if self.players[pid].x == x and self.players[pid].y == y:
                return "p"
        return " "
    
    def area_at(self,x,y,r):
        pole = []
        for i in range(-r,r+1):
            retstr = ""
            for j in range(-r,r+1):
                #print "({0},{1})".format(j,i),
                retstr += self.maplook_at(x+j,y+i)
            pole.append(retstr)
        for i in range(-r,r+1):
            retstr = ""
            for j in range(-r,r+1):
                #print "({0},{1})".format(j,i),
                retstr += self.itemslook_at(x+j,y+i)
            pole.append(retstr)
        return pole
    
    def __str__(self):
        ret = ""
        for x in self.map:
            ret += "{0}\n".format(x)
        ret += "width: {0} height: {1}".format(self.width,self.height)
        return ret
    
    def add_player(self, player_id):
        pos = (random.randint(1,self.width-1),random.randint(1,self.height-1))
        item = self.maplook_at(pos[0], pos[1])
        while item in "toN~":
            pos = (random.randint(1,self.width-1),random.randint(1,self.height-1))
            item = self.maplook_at(pos[0], pos[1])
        self.players[player_id] = Player(player_id, pos[0], pos[1], 100, self)

    def remove_player(self, player_id):
        if player_id in self.players:
            self.players.pop(player_id)

    def move_player(self, player_id, direction):
        if player_id in list(self.players.keys()):
            self.players[player_id].move_direction(direction)

class PlaygroundServer(object):
    def __init__(self, filename):
        self.playground = Playground(filename)
    
    def add_player(self, player_id):
        if player_id not in list(self.playground.players.keys()):
            self.playground.add_player(player_id)
        return "ok"
    
    def is_player(self, player_id):
        if player_id in list(self.playground.players.keys()):
            return True
        return False
    
    def look_around(self, player_id):
        if player_id in list(self.playground.players.keys()):
            return self.playground.players[player_id].get_area()
        return "fail"
    
    def make_action(self, player_id, action, parameter):
        print(self.playground.players)

        time.sleep(0.25)
        if player_id in list(self.playground.players.keys()):
            if action == "move":
                if parameter == "west":
                    self.playground.players[player_id].move_direction("west")
                elif parameter == "east":
                    self.playground.players[player_id].move_direction("east")
                elif parameter == "north":
                    self.playground.players[player_id].move_direction("north")
                elif parameter == "south":
                    self.playground.players[player_id].move_direction("south")
            elif action == "exit":
                self.playground.remove_player(player_id)
            return self.look_around(player_id)
        return "fail"

if __name__ == "__main__":
    pgserver = PlaygroundServer("map.txt")
    print(pgserver.playground)
    server = ThreadingXMLRPCServer(('0.0.0.0', 8080))
    server.register_instance(pgserver)
    server.register_introspection_functions()
    print("Server running! CTRL+C to shutdown server...")
    server.serve_forever()
    """
    print pgserver.playground
    print pgserver.add_player("karel")
    lst = pgserver.look_around("karel")
    while 1:
        direction = random.choice(["west","east","north","south"])
        print "--------------------------------"
        print direction
        lst = pgserver.make_action("karel", "move", direction)
        if lst != "fail":
            for l in lst:
                print " ".join([x for x in l])
    """
