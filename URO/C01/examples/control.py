#------------------------------------------------------------------------------#
# Pouziti OptionMenu                                                           #
#------------------------------------------------------------------------------#
from tkinter import *
from tkinter import tix

class myApp:

  def fce(self, prom):
    self.la.configure(text=prom)
    
  def __init__(self, master):

    self.la = Label(master, text="nic", foreground="red")
    self.la.pack()
    self.prom = StringVar(master)
    self.option = tix.Control(master,label="volba:",integer=TRUE, min=0, max=50, command = self.fce)
    self.option.pack()
    self.bu = Button(master, text="Button")
    self.bu.pack()



root = tix.Tk()
app = myApp(root)
root.mainloop()

#------------------------------------------------------------------------------#



