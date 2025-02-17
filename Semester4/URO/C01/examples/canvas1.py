#------------------------------------------------------------------------------#
# Platno s geometrickymi objekty                                               #
#------------------------------------------------------------------------------#
from tkinter import *

class myApp:
  def __init__(self, master):
    self.fr = Frame(master)
    self.ca = Canvas(self.fr, width=250, height=150)
    self.fr.master.title("Canvas1")
    self.ca.create_line(25, 25, 125, 125, width=2, fill="red")
    self.ca.create_oval(125, 25,  225, 125, fill="blue")
    self.fr.pack()
    self.ca.pack()

root = Tk()
app = myApp(root)
root.mainloop()

#------------------------------------------------------------------------------#
