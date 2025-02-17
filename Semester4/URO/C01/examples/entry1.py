#------------------------------------------------------------------------------#
# Vstup a spol.                                                                #
#------------------------------------------------------------------------------#
from tkinter import *

class myApp:
  def __init__(self, master):
    self.fr = Frame(master)
    self.en = Entry(self.fr)
    self.bv = Button(self.fr, text="Vymazat", width=10, command=self.vymaz)
    self.bz = Button(self.fr, text="Zpracovat", width=10, command=self.zpracuj)
    self.la = Label(self.fr, foreground="red")
    self.fr.pack()
    self.en.pack(side="top", padx=4, pady=4, fill="x")
    self.la.pack(side="bottom", pady=4)
    self.bv.pack(side="left", pady=4)
    self.bz.pack(side="right", pady=4)
    self.fr.master.title("Entry 1")
  def vymaz(self):
    self.en.delete(0, END)
  def zpracuj(self):   
    self.la.configure(text=self.en.get())
     
root = Tk()
app = myApp(root)
root.mainloop()
#------------------------------------------------------------------------------#
