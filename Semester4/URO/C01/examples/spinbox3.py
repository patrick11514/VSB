#------------------------------------------------------------------------------#
# Spinbox se seznamem, ktery si obsluhujeme sami                               #
#------------------------------------------------------------------------------#
from tkinter import *

class myApp:
  def __init__(self, master):

    self.fr = Frame(master)
    self.var = StringVar()
    self.predmety= ["java", "databaze", "fyzika", "C++"]
    self.sb = Spinbox(self.fr, command=self.change, textvariable = self.var, value=self.predmety)
    self.la = Label(self.fr, foreground="red")
    self.bu = Button(self.fr, text="OK", command=self.buok)

    self.fr.master.title("Spinbox 3")
    self.fr.pack()
    self.sb.pack()
    self.la.pack(side="left", pady=10)
    self.bu.pack(side="right")

   

   
  def change(self):
    print(self.sb.get())
  def buok(self):
    self.la.configure(text=self.sb.get())

root = Tk()
app = myApp(root)
root.mainloop()
#------------------------------------------------------------------------------#




