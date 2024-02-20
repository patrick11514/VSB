#------------------------------------------------------------------------------#
# Kaskadove menu                                                               #
#------------------------------------------------------------------------------#
from tkinter import *

class myApp:
  def __init__(self, master):
    self.menubar = Menu(master)
    
    self.cascmenu = Menu(self.menubar, tearoff=0)
    self.cascmenu.add_command(label="Volba 1", underline=6, command=self.nic) 
    self.cascmenu.add_command(label="Volba 2", underline=6, state="disabled", command=self.nic) 

    self.filemenu = Menu(self.menubar, tearoff=0)
    self.filemenu.add_cascade(label="Kaskada... ", underline=0, menu=self.cascmenu)
    self.filemenu.add_separator()
    self.filemenu.add_command(label="Quit", underline=0, command=master.quit)
    self.menubar.add_cascade(label="File", menu=self.filemenu)

    self.editmenu = Menu(self.menubar, tearoff=0)
    self.editmenu.add_command(label="Nic", command=self.nic)
    self.menubar.add_cascade(label="Edit", menu=self.editmenu)

    master.title("Menu 2")
    master.config(menu=self.menubar)
  def nic(self):
    pass 
     
root = Tk()
app = myApp(root)
root.mainloop()
#------------------------------------------------------------------------------#


