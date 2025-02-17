#------------------------------------------------------------------------------#
# Obycejne tlacitko odsazene od okolnich objektu                               #
#------------------------------------------------------------------------------#
from tkinter import *

class myApp:
  def __init__(self, master):
    self.fr = Frame(master, relief="sunken", borderwidth=2)
    self.bu = Button(self.fr, text="Konec", width=10, height=2, command=self.fr.quit)
    self.fr.master.title("Button2")
    self.fr.pack()
    self.bu.pack(padx=32, pady=32)

root = Tk()
app = myApp(root)
root.mainloop()

#------------------------------------------------------------------------------#
