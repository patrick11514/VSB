#------------------------------------------------------------------------------#
# Jednoduche menu                                                              #
#------------------------------------------------------------------------------#
from tkinter import *

class myApp:
  def __init__(self, master):
  
    self.menubar = Menu(master)
    
    self.filemenu = Menu(self.menubar, tearoff=0)
    self.filemenu.add_command(label="Neco", command=self.neco)
    self.filemenu.add_separator()
    self.filemenu.add_command(label="Quit", command=master.quit)
    self.menubar.add_cascade(label="File", menu=self.filemenu)
    
    self.editmenu = Menu(self.menubar, tearoff=0)
    self.editmenu.add_command(label="Nic", command=self.nic)
    self.menubar.add_cascade(label="Edit", menu=self.editmenu)
    
    master.title("Menu1")
    master.config(menu=self.menubar)
  def neco(self):
    pass 
  def nic(self):
    pass 
     
root = Tk()
app = myApp(root)
root.mainloop()
#------------------------------------------------------------------------------#

