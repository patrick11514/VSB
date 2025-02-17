from tkinter import *
from tkinter import ttk


class myApp:
    def __init__(self, master: Tk):
        master.title("10/10 UI")

        self.button1 = Button(master, text="Konec :/", command=master.destroy)
        self.button1.pack(side="top", padx=50, pady=10, fill="both", expand=True)

        self.button2 = Button(master, text="Tlačítko", command=self.write)
        self.button2.pack(side="bottom")
    
    def write(self):
        print("Ahoj")

root = Tk()
app = myApp(root)
root.mainloop()
