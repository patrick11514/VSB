#------------------------------------------------------------------------------#
# ColorChooser and Dialog                                                      #
#------------------------------------------------------------------------------#
from tkinter import *
from tkinter import messagebox, colorchooser, filedialog    

class myApp:
  def __init__(self, master):
    self.fr = Frame(master)
    self.en = Entry(self.fr)
    self.bv = Button(self.fr, text="Barva", width=10, command=self.barva)
    self.bz = Button(self.fr, text="Zprava", width=10, command=self.msg)
    self.bs = Button(self.fr, text="Soubor", width=10, command=self.soubor)
    self.fr.pack()
    self.en.pack(side="top", padx=4, pady=4, fill="x")
    self.bv.pack(side="left", pady=4)
    self.bz.pack(side="left", pady=4)
    self.bs.pack(side="right", pady=4)
    self.fr.master.title("Color")
  
  def barva(self):
    self.color=colorchooser.askcolor()
    self.en.delete(0,END)
    self.en.insert(0,self.color)
  
  def msg(self):
    messagebox.showerror('Spam', "Barva")
    
  def soubor(self):
    filedialog.askopenfilename(filetypes = (("JPG images", "*.jpg;*.jpeg")
                                                         ,("GIF files", "*.gif")
                                                         ,("All files", "*.*") ))
     
root = Tk()
app = myApp(root)
root.mainloop()
#------------------------------------------------------------------------------#

