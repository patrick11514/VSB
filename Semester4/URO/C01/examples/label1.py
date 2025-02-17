#------------------------------------------------------------------------------#
# Popiska                                                                      #
#------------------------------------------------------------------------------#
from tkinter import *

class myApp:
  def __init__(self, master):
    self.fr = Frame(master)
    self.la = Label(self.fr, text="Toto je popiska.")
    self.fr.pack()
    self.la.pack()
    self.fr.master.title("Label1")

root = Tk()
app = myApp(root)
root.mainloop()
#------------------------------------------------------------------------------#


