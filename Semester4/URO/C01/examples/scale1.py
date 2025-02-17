#------------------------------------------------------------------------------#
# Scale (stupnice)                                                             #
#------------------------------------------------------------------------------#
from tkinter import *

class myApp:
  def __init__(self, master):
    self.fr = Frame(master)
    self.sc = Scale(self.fr, orient="horizontal", length=200, fr=0, to=100,
                    tickinterval=20, command=self.scgetval)
    self.la = Label(self.fr, foreground="red")
    self.fr.master.title("Scale 1")
    self.fr.pack()
    self.sc.pack()
    self.la.pack(pady=8)
    self.sc.set(50)
   
  def scgetval(self, val):
    self.la.configure(text=val)

root = Tk()
app = myApp(root)
root.mainloop()
#------------------------------------------------------------------------------#
