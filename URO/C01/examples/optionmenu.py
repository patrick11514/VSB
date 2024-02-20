#------------------------------------------------------------------------------#
# Pouziti OptionMenu                                                           #
#------------------------------------------------------------------------------#
from tkinter import *

class myApp:

  def fce(self, var):
    self.la.configure(text=self.prom.get())
    
  def __init__(self, master):

    self.la = Label(master, text="nic", foreground="red")
    self.la.pack()
    self.prom = StringVar(master)
    self.option = OptionMenu(master, self.prom, "bílá", "žlutá", "červená", "černá", command=self.fce)
    self.option.pack()
    self.prom.set("červená")

root = Tk()
app = myApp(root)
root.mainloop()
#------------------------------------------------------------------------------#


