#------------------------------------------------------------------------------#
# Entry vstup vystup                                                           #
#------------------------------------------------------------------------------#
from tkinter import *
import random, math

class myApp:
  def __init__(self, master):
    self.fr = Frame(master)
    self.en = Entry(self.fr)
    self.bv = Button(self.fr, text="Vymazat", width=10, command=self.vymaz)
    self.bz = Button(self.fr, text="Generuj", width=10, command=self.generuj)
    self.fr.pack()
    self.en.pack(side="top", padx=4, pady=4, fill="x")
    self.bv.pack(side="left", pady=4)
    self.bz.pack(side="right", pady=4)
    self.fr.master.title("Generator")
  def vymaz(self):
    self.en.delete(0, END)
  def generuj(self):
    self.en.insert(END,str(int (math.floor(random.random()*10))))
     
root = Tk()
app = myApp(root)
root.mainloop()
#------------------------------------------------------------------------------#

