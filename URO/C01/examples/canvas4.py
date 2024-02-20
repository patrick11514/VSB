#------------------------------------------------------------------------------#
# Platno s pohyblivymi objekty                                                 #
#------------------------------------------------------------------------------#
from tkinter import *

class myApp:
  def __init__(self, master):
    self.fr = Frame(master)
    self.ca = Canvas(self.fr, width=250, height=150)
    self.fr.master.title("Canvas3")
    self.li = self.ca.create_line(25, 25, 125, 125, width=2, fill="red")
    self.ov = self.ca.create_oval(125, 25,  225, 125, fill="blue")
    self.fr.pack()
    self.ca.pack()
    self.ca.tag_bind(self.li, "<B1-Motion>", self.callback1)
    self.ca.tag_bind(self.ov, "<B1-Motion>", self.callback2)
  def callback1(self, event):
    self.ca.coords(self.li, event.x-50, event.y-50, event.x+50, event.y+50)
  def callback2(self, event):
    self.ca.coords(self.ov, event.x-50, event.y-50, event.x+50, event.y+50)

root = Tk()
app = myApp(root)
root.mainloop()

#------------------------------------------------------------------------------#
