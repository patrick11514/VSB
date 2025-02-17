#------------------------------------------------------------------------------#
# Radio buttons                                                                #
#------------------------------------------------------------------------------#
from tkinter import *
from tkinter import font

class myApp:
  def __init__(self, master):
    self.fo = StringVar()
    self.la = Label(master, text="A pair of radio buttons")
    self.r1 = Radiobutton(master, text="Times", variable=self.fo,
              value="Times", command=self.ukaz)
    self.r2 = Radiobutton(master, text="Helvetica", variable=self.fo,
              value="Helvetica", command=self.ukaz)

    master.title("Radiobutton 2")
    self.la.pack()
    self.r1.pack(side="left")
    self.r2.pack(side="left")
    self.r1.select()
    self.ukaz()

  def ukaz (self):
    self.newFont = font.Font(family=self.fo.get(), weight="bold")
    self.la.configure(font=self.newFont)  
     
root = Tk()
app = myApp(root)
root.mainloop()
#------------------------------------------------------------------------------#
