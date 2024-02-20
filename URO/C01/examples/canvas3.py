#------------------------------------------------------------------------------#
# Platno s rolovacimi listami                                                  #
#------------------------------------------------------------------------------#
from tkinter import *

class myApp:
  def __init__(self, master):
    self.fr = Frame(master)
    self.ca = Canvas(self.fr, width=200, height=100, scrollregion="0 0 400 250")
    self.sy = Scrollbar(self.fr, orient="vertical", command=self.ca.yview)  
    self.sx = Scrollbar(self.fr, orient="horizontal", command=self.ca.xview)
    self.ca.configure(xscrollcommand=self.sx.set, yscrollcommand=self.sy.set)
                     
    self.fr.master.title("Canvas 3")
    self.fr.pack()
    self.ca.grid(row=0, column=0)
    self.sy.grid(row=0, column=1, sticky="ns")
    self.sx.grid(row=1, column=0, sticky="we")
    
    self.li = self.ca.create_line(25, 25, 125, 125, width=2, fill="red")
    self.ov = self.ca.create_oval(125, 25, 225, 125, fill="blue")

root = Tk()
app = myApp(root)
root.mainloop()
#------------------------------------------------------------------------------#

