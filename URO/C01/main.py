from tkinter import *
from tkinter import ttk


class myApp:
  def __init__(self, master: Tk):
    master.title("10/10 UI")

    self.button1 = Button(master, text=f"Čudlík 1")
    self.button1.pack(side="left")

    self.button2 = Button(master, text=f"Čudlík 2")
    self.button2.pack(side="right")

    self.button3 = Button(master, text=f"Čudlík 3")
    self.button3.pack(side="top")

    self.button4 = Button(master, text=f"Čudlík 4")
    self.button4.pack(side="bottom")



root = Tk()
app = myApp(root)
root.mainloop()
