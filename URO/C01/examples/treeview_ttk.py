#------------------------------------------------------------------------------#
# Treeview                                                                     #
#------------------------------------------------------------------------------#
from tkinter import *
from tkinter import ttk

class myApp:
  def __init__(self, master):
    master.title("Treeview")
    self.treeview = ttk.Treeview(master, columns=("Size"))
    self.treeview.heading("#0", text="Path")
    self.treeview.heading("Size", text="Size")
    self.treeview.column("Size", width=100)
    self.treeview.insert("", 0, iid="rc", text="C:/")
    self.treeview.insert("", 1, iid="rd", text="D:/")
    self.treeview.insert("rc", 0, iid="c_pf", text="Program Files/")    
    self.treeview.insert("rc", 'end', iid="c_win", text="Windows/")
    self.treeview.insert("c_pf", 0, iid="c_games", text="Games/")    
    self.treeview.insert("c_pf", 'end', text="Data/")
    self.treeview.insert("c_games", 0, text="Mafia.exe", values=("34MB"))
    
    self.treeview.insert("rd", 'end', iid="c_movies", text="Movies/")
    self.treeview.insert("c_movies", 'end', text="Postřižiny.avi", values=("1.24GB"))
    self.treeview.pack()

root = Tk()
app = myApp(root)
root.mainloop()
root.destroy()
#------------------------------------------------------------------------------#
