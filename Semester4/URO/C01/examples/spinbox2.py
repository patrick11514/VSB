#------------------------------------------------------------------------------#
# Spinbox s cisly po 0.1                                                       #
#------------------------------------------------------------------------------#
from tkinter import *

class myApp:
  def __init__(self, master):
    self.fr = Frame(master)
    self.cislo = IntVar()
    self.sb = Spinbox(self.fr, fr=2.0, to=3.0, increment=0.1, textvariable=self.cislo)
    self.la = Label(self.fr, foreground="red")
    self.bu = Button(self.fr, text="OK", command=self.buok)
    self.fr.master.title("Spinbox 2")
    self.fr.pack()
    self.sb.pack()
    self.la.pack(side="left", padx=8, pady=10)
    self.bu.pack(side="right", padx=8)
    self.cislo.set(2.5)   
  def buok(self):
    self.la.configure(text=self.sb.get())

root = Tk()
app = myApp(root)
root.mainloop()
#------------------------------------------------------------------------------#


