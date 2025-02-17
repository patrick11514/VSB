#------------------------------------------------------------------------------#
# Popiska - zmena textu                                                        #
#------------------------------------------------------------------------------#
from tkinter import *

class myApp:
  def __init__(self, master):
    self.fr = Frame(master)
    self.la = Label(self.fr, text="abc")
    self.fr.pack();self.la.pack()
    self.fr.master.title("Label 2")

root = Tk()
app = myApp(root)
app.la.configure(text="Zmena textu\npopisky.")
root.mainloop()
#------------------------------------------------------------------------------#
