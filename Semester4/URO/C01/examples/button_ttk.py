#------------------------------------------------------------------------------#
# Tlacitko v ttk                                                               #
#------------------------------------------------------------------------------#
from tkinter import *
from tkinter import ttk

class myApp:
  def __init__(self, master):
    self.style = ttk.Style()
    self.style.configure("TButton", font=('Helvetica', 10), relief="flat", foreground="blue", background="white")
    self.bu = ttk.Button(master, text="Konec", style="TButton", command=master.quit)
    self.bu.pack()

root = Tk()
app = myApp(root)
root.mainloop()
#------------------------------------------------------------------------------#

