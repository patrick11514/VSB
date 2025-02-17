#------------------------------------------------------------------------------#
# Tlacitko s obrazkem                                                          #
#------------------------------------------------------------------------------#
from tkinter import *

class myApp:
  def __init__(self, master):
    self.fr = Frame(master)
    self.fr.master.title("Button4")
    self.photo = PhotoImage(file="exit.gif")
    self.bu = Button(self.fr, image=self.photo, command=self.fr.quit)
    self.fr.pack()
    self.bu.pack(padx=8, pady=8)
     
root = Tk()
app = myApp(root)
root.mainloop()

#------------------------------------------------------------------------------#

