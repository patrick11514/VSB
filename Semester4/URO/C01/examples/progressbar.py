#------------------------------------------------------------------------------#
# Progressbar v ttk                                                            #
#------------------------------------------------------------------------------#
from tkinter import *
from tkinter import ttk

class myApp:

  def update(self):
    self.btn.configure(state=DISABLED)
    i = self.val.get()
    i += 2
    print(i)
    if(i >= 100): 
        self.val.set(0)
        self.btn.configure(state=NORMAL)
        return
    self.val.set(i)
    self.root.after( 100, self.update )
    
  def __init__(self, master):
    self.root = master
    self.val = IntVar()
    self.val.set(0)
    self.prb = ttk.Progressbar(master, orient="horizontal", variable=self.val, mode='determinate')
    self.prb.pack()
    
    self.btn = Button(master, text="CLICK", command=self.update)    
    self.btn.pack()



root = Tk()
app = myApp(root)
root.mainloop()
#------------------------------------------------------------------------------#


