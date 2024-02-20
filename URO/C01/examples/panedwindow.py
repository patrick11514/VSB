#------------------------------------------------------------------------------#
# Pouziti OptionMenu                                                           #
#------------------------------------------------------------------------------#
from tkinter import *

class myApp:

  def fce(self, var):
    self.la.configure(text=self.prom.get())
    
  def __init__(self, master):


    self.m1 = PanedWindow(master, sashrelief= RAISED, width= 200)
    self.m1.pack(fill=BOTH, expand=1)

    self.a1 = Label(self.m1, text="Levy panel")
    self.m1.add(self.a1)

    self.a2 = Label(self.m1, text="Pravy panel")
    self.m1.add(self.a2)

    

root = Tk()
app = myApp(root)
root.mainloop()
#------------------------------------------------------------------------------#


