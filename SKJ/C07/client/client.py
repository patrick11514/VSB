import random
import xml.etree.ElementTree as ET
import xmlrpc.client

from visualizer import Visualizer


class Agent:
    login: str
    data: list[str] = []
    visualizer: Visualizer
    gameserver: xmlrpc.client.ServerProxy

    def __init__(self, filename: str):
        # TODO 1     - load data from the XML configuration file (filename is in the parameter)
        # TODO 2     - create instance variables (login, data, visualizer, gameserver)
        # login      - load from XML file (tag 'login')
        # data       - an empty list, where data from server will be stored
        # visualizer - instance of the visuzlizer.Visualizer class
        # gameserver - connect to the remote XML-RPC server (url is provided in the XML file, tag 'url')
        # TODO 3     - call method 'add_player' on the server with login as the parameter (use instance variable 'self.login')

        tree = ET.parse(filename)
        config = tree.getroot()

        login = config.find("login")
        if login is None:
            print("Unable to find login in config")
            return

        self.login = login.text

        url = config.find("url")
        if url is None:
            print("Unable to find url in config")

        self.visualizer = Visualizer()
        self.gameserver = xmlrpc.client.ServerProxy(url.text)
        self.gameserver.add_player(self.login)

    def action(self):
        # TODO 4 - Call 'make_action' method on the XML-RPC server.
        # The method has 3 parameters (login, action_name, parameters).
        # All three parameters are strings. Call the 'look' method on the server,
        # to take a look around the player, without NY parameter (empty string).
        # Every action returns a list of strings, where each row represents one
        # row from the surrounding area of the player.
        # Each string is 11-characters in length and there are 22 rows.
        # First 11 elements of the list represent the agent's environment
        # and other 11 elements of his surroundings
        # (so far, only "p" character is present to represent other agents).
        # The player is in this surrounding at the position [5][5] (5th row, 5th character).
        # Objects on the same position can be sought at the coordinates [5 + 11] [5].
        # "~" water
        # " " grass
        # "*" road
        # "t" tree
        # "o" rock (wall)
        # "f" tiled floor
        # "p" player
        self.data = self.gameserver.make_action(self.login, "look", "")
        self.visualizer.visualize(self.data)

    def __repr__(self):
        # TODO 5 - Returns human readable representation of stored data form 'self.data' variable.
        return "\n".join(self.data)

    def save_data(self):
        # TODO 6 - Store data into the 'data.txt' filename.
        with open("data.txt", "w") as file:
            file.write("\n".join(self.data))


class AgentRandom(Agent):
    # TODO 7 - This agent will extend from the previous agent and
    # reimplement (override) the 'action' method so that the action will be 'move' and
    # the passed parameter will be one the directions: 'north', 'west', 'south', 'east'.
    # These directions will be randomly selected
    # (find the appropriate method from the random package).
    def action(self):
        direction = random.choices(["north", "west", "south", "east"])

        self.data = self.gameserver.make_action(self.login, "move", direction[0])
        self.visualizer.visualize(self.data)


class AgentLeftRight(Agent):
    # TODO 8 - This agent will be moving just to the left until it hits a barrier.
    # Then it rurn to the right and moves until it hits the barrier again.
    # It repeats such behavior.
    left = True

    def _getPlayerPosition(self):
        if len(self.data) == 0:
            return None

        for y in range(11, len(self.data)):
            for x in range(len(self.data[y])):
                if self.data[y][x] == "p":
                    return [y, x]

    def action(self):
        direction = "east"

        if self.left == True:
            direction = "west"

        self.data = self.gameserver.make_action(self.login, "move", direction)
        self.visualizer.visualize(self.data)

        pos = self._getPlayerPosition()
        if pos == None:
            return

        [y, x] = pos
        y = y - 11

        if self.left == True:
            if x - 1 <= 0 or (self.data[y][x - 1] not in " *"):
                self.left = False
        else:
            if x + 1 >= len(self.data[y]) or self.data[y][x + 1] not in " *":
                self.left = True


def main():
    agent = None
    try:
        # agent = Agent("config.xml")
        # agent = AgentRandom("config.xml")
        agent = AgentLeftRight("config.xml")
        while agent.visualizer.running:
            agent.action()
            print(agent)
        else:
            agent.gameserver.make_action(agent.login, "exit", "")

        agent.save_data()
    except KeyboardInterrupt:
        agent.gameserver.make_action(agent.login, "exit", "")


if __name__ == "__main__":
    main()
