#------------------------------------------------------------------------------#
# Checkbutton                                                                  #
#------------------------------------------------------------------------------#
from tkinter import *
from tkinter import font

class myApp:
  def __init__(self, master):
    self.we = StringVar()
    self.sl = StringVar()
    self.la = Label(master, text="Two check buttons")
    self.c1 = Checkbutton(master, text="Bold", variable=self.we,
              onvalue="bold", offvalue="normal")
    self.c2 = Checkbutton(master, text="Italics", variable=self.sl,
              onvalue="italic", offvalue="roman")
    self.bu = Button(text="Ukaz", command=self.ukaz)

    master.title("Checkbutton 1")
    self.la.pack()
    self.c1.pack(side="left")
    self.c2.pack(side="left")
    self.bu.pack()
    self.c1.select()
    self.c2.deselect()
    self.ukaz()

  def ukaz (self):
    self.newFont = font.Font(family="Helvetica", 
                   weight=self.we.get(), slant=self.sl.get())
    self.la.configure(font=self.newFont)  
     
root = Tk()
app = myApp(root)
root.mainloop()
#------------------------------------------------------------------------------#

