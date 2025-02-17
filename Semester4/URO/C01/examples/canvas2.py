#------------------------------------------------------------------------------#
# Platno s obrazem a geometrickym objektem                                     #
#------------------------------------------------------------------------------#
from tkinter import *

class myApp:
  def __init__(self, master):
    self.fr = Frame(master)
    self.ca = Canvas(self.fr, width=384, height=288)
    self.fr.master.title("Canvas2")
    self.photo = PhotoImage(file="auto1.gif")
    self.ca.create_image(2, 2, anchor=NW, image=self.photo)
    self.ca.create_oval(250, 150,  350, 250, fill="blue")
    self.fr.pack()
    self.ca.pack()

root = Tk()
app = myApp(root)
root.mainloop()

#------------------------------------------------------------------------------#



