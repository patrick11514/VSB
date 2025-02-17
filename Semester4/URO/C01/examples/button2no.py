#------------------------------------------------------------------------------#
# Obycejne tlacitko odsazene od okolnich objektu, aplikace neni trida          #
#------------------------------------------------------------------------------#
from tkinter import *

top = Tk()
f = Frame(top, relief="sunken", borderwidth=2)
b = Button(f, text="Konec", width=10, height=2, command=f.quit)
top.title("Button 2")
f.pack()
b.pack(padx=32, pady=32)

top.mainloop()
#------------------------------------------------------------------------------#
