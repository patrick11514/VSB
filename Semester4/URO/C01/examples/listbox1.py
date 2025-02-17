#------------------------------------------------------------------------------#
# Listbox nejobycejnejsi                                                       #
#------------------------------------------------------------------------------#
from tkinter import *

class myApp:
  def __init__(self, master):
    self.fr = Frame(master)
    self.li = Listbox(self.fr, width=15, height=3)
    self.la = Label(self.fr, text="nic", foreground="red")
    self.bu = Button(self.fr, text="Dopln", command=self.dopln)
    self.li.insert(0, "Tatra", "Areo", "Skoda")
    self.fr.pack()
    self.li.pack()
    self.la.pack(side="left")
    self.bu.pack(side="right")
    self.fr.master.title("Listbox 1")
    self.li.bind("<Double-B1-ButtonRelease>", self.liTakeOver)
  
  def dopln(self):
    self.li.insert(END, "Wikov", "Jawa")
    self.li.configure(height=self.li.cget("height")+2)
    self.bu.configure(state="disabled")

  def liTakeOver(self, event):
    self.la.configure(text=self.li.get("active"))

root = Tk()
app = myApp(root)
root.mainloop()
#------------------------------------------------------------------------------#

