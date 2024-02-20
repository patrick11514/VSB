#------------------------------------------------------------------------------#
# Pouziti OptionMenu                                                           #
#------------------------------------------------------------------------------#
from tkinter import *


class myApp:

  def __init__(self, master):

   self.fr = LabelFrame(master, text="Poznamka", padx=4, pady=4)
   self.fr.pack(expand=1, fill=BOTH, padx=10, pady=10)
   self.pozn= Text(self.fr, width=50, height=5)
   self.scrol = Scrollbar(self.fr)
   self.scrol.pack(side=RIGHT, fill=Y)
   self.pozn.pack(expand=1, fill=BOTH)
   self.pozn.focus_set()
   self.scrol.config(command=self.pozn.yview)
   self.pozn.config(yscrollcommand=self.scrol.set)



root = Tk()
app = myApp(root)
root.mainloop()



