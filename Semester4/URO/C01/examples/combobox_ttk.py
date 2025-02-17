#------------------------------------------------------------------------------#
# Combobox v ttk                                                               #
#------------------------------------------------------------------------------#
from tkinter import *
from tkinter import ttk

class myApp:

  def fce(self, var):
    self.la.configure(text=self.cmb.get())
    
  def __init__(self, master):

    self.la = Label(master, text="nic", foreground="red")
    self.la.pack()
    vals=("Option 1", "Option 2", "Option 3", "Option 4")
    self.cmb = ttk.Combobox(master,values=vals, state="readonly", validatecommand=self.fce)
    self.cmb.pack()
    self.cmb.bind("<<ComboboxSelected>>", self.fce)



root = Tk()
app = myApp(root)
root.mainloop()
#------------------------------------------------------------------------------#


