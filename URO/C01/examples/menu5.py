#------------------------------------------------------------------------------#
# Obrazkova tlacitka v liste menu                                              #
#------------------------------------------------------------------------------#
from tkinter import *

class myApp:
  def __init__(self, master):
    master.title("Menu 5")
    self.menubar = Frame(master)
    self.menubar.pack(fill="x")
    
    self.imgFile = PhotoImage(file="file.gif")
    self.fileb = Menubutton(self.menubar, image=self.imgFile)
    self.fileb.pack(side="left", padx="2m")

    self.fileMenu = Menu(self.fileb, tearoff=0)
    self.fileMenu.add_command(label="Nic", underline=0)
    self.fileMenu.add_separator()
    self.imgQuit = PhotoImage(file="exits.gif")
    self.fileMenu.add_command(compound="left", image=self.imgQuit,
         label=" Quit", underline=1, command=master.quit)
    self.fileb.configure(menu=self.fileMenu)
    
    self.imgEdit = PhotoImage(file="edit.gif")
    self.editb = Menubutton(self.menubar, image=self.imgEdit)
    self.editb.pack(side="left", padx="2m")
  def nic(self):
    pass  
     
root = Tk()
app = myApp(root)
root.mainloop()
#------------------------------------------------------------------------------#
