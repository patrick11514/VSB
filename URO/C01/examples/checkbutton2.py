#------------------------------------------------------------------------------#
# Checkbutton - totez jako Checkbutton 1, ale trochu jinak                     #
#------------------------------------------------------------------------------#
from tkinter import *
from tkinter import font

class myApp:
  def __init__(self, master):
    self.bo = IntVar()
    self.it = IntVar()
    self.la = Label(master, text="Two check buttons")
    self.c1 = Checkbutton(master, text="Bold", variable=self.bo, onvalue=1, offvalue=0)
    self.c2 = Checkbutton(master, text="Italics", variable=self.it)
    self.bu = Button(text="Show", command=self.ukaz)

    master.title("Checkbutton 2")
    self.la.pack()
    self.c1.pack(side="left")
    self.c2.pack(side="left")
    self.bu.pack()
    self.ukaz()

  def ukaz (self):
    if (self.bo.get() == 1): nweight = "bold"
    else:                    nweight = "normal"
    if (self.it.get() == 1): nslant  = "italic"
    else:                    nslant  = "roman"
    self.newFont = font.Font(family="Helvetica", weight=nweight, slant=nslant )
    self.la.configure(font=self.newFont)  
     
root = Tk()
app = myApp(root)
root.mainloop()
#------------------------------------------------------------------------------#
