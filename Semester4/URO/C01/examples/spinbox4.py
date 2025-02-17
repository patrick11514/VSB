#------------------------------------------------------------------------------#
# Spinbox se seznamem                                                          #
#------------------------------------------------------------------------------#
from tkinter import *

class myApp:
  def __init__(self, master):
    self.fr = Frame(master)
    self.var = StringVar()
    self.sb = Spinbox(self.fr, textvariable= self.var, values="leden unor brezen duben kveten cerven")
    self.la = Label(self.fr, foreground="red")
    self.bu = Button(self.fr, text="OK", command=self.buok)

    self.fr.master.title("Spinbox 1")
    self.fr.pack()
    self.sb.pack()
    self.la.pack(side="left", pady=10)
    self.bu.pack(side="right")
    self.var.set("brezen")
  def buok(self):
    self.la.configure(text=self.sb.get())

root = Tk()
app = myApp(root)
root.mainloop()
#------------------------------------------------------------------------------#


