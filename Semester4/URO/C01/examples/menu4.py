#------------------------------------------------------------------------------#
# Obrazky v menu                                                               #
#------------------------------------------------------------------------------#
from tkinter import *


class myApp:
  def __init__(self, master):
    self.menubar = Menu(master)
    
    self.cascmenu = Menu(self.menubar, tearoff=0)
    self.imgCopy = PhotoImage(file="copy.gif")
    self.cascmenu.add_command(compound="left", image=self.imgCopy,
         label=" Copy", underline=1, command=self.copy) 
    self.imgPrint = PhotoImage(file="print.gif")
    self.cascmenu.add_command(compound="left", image=self.imgPrint,
         label=" Print", command=self.nic) 

    self.filemenu = Menu(self.menubar, tearoff=0)
    self.filemenu.add_cascade(label="Kaskada...  ", underline=0, menu=self.cascmenu)
    self.filemenu.add_separator()
    self.imgQuit = PhotoImage(file="exits.gif")
    self.filemenu.add_command(image=self.imgQuit, command=master.quit)
    self.menubar.add_cascade(label="File", menu=self.filemenu)

    self.editmenu = Menu(self.menubar, tearoff=0)
    self.editmenu.add_command(label="Nic", command=self.nic)
    self.menubar.add_cascade(label="Edit", menu=self.editmenu)

    master.title("Menu2")
    master.config(menu=self.menubar)

  def copy(self):
    print("copy")

  def nic(self):
    print("nic")

     
root = Tk()
app = myApp(root)
root.mainloop()
#------------------------------------------------------------------------------#
