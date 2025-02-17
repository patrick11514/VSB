#------------------------------------------------------------------------------#
# Listbox s rolovaci listou, formatovani pomoci grid                           #
#------------------------------------------------------------------------------#
from tkinter import *

class myApp:
  def __init__(self, master):
    self.fr = Frame(master)

    self.li = Listbox(self.fr, width=15, height=5)
    self.sy = Scrollbar(self.fr, orient="vertical", command=self.li.yview)  
    self.la = Label(self.fr, text="nic", foreground="red")
    self.li.configure(yscrollcommand=self.sy.set)
 
    self.li.insert(0, "Tatra", "Areo", "Skoda", "Wikov", "Jawa", "Praga", "Z")

    self.fr.pack()
    self.li.grid(row=0, column=0)
    self.sy.grid(row=0, column=1, sticky="ns")
    self.la.grid(row=1, column=0, columnspan=2)
    self.fr.master.title("Listbox 2")
    self.li.bind("<Double-B1-ButtonRelease>", self.liTakeOver)
  
  def liTakeOver(self, event):
    self.la.configure(text=self.li.get("active"))

root = Tk()
app = myApp(root)
root.mainloop()
#------------------------------------------------------------------------------#
