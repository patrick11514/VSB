#------------------------------------------------------------------------------#
# Notebook v ttk                                                               #
#------------------------------------------------------------------------------#
from tkinter import *
from tkinter import ttk

class myApp:
  def __init__(self, master):
    self.nb = ttk.Notebook(master)
    self.p1 = Frame(self.nb)
    self.p2 = Frame(self.nb)
    self.nb.add(self.p1, text="page1")
    self.nb.add(self.p2, text="page2")


    self.nb.pack(expand=1, fill=BOTH)
    
    #A1 
    self.la1 = Label(self.p1, text="Prvni okno")
    self.la1.pack()

    #B1
    self.la2 = Label(self.p2, text="Druhe okno")
    self.la2.pack()

root = Tk()
app = myApp(root)
root.mainloop()
root.destroy()
#------------------------------------------------------------------------------#
