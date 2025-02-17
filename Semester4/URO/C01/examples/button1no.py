#------------------------------------------------------------------------------#
# Obycejne tlacitko, aplikace zde neni trida                                   #
#------------------------------------------------------------------------------#
from tkinter import *

top = Tk()
b = Button(top, text="Konec", command=top.quit)
top.title("Button 1")
b.pack()

top.mainloop()
#------------------------------------------------------------------------------#


