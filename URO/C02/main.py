from tkinter import *
from tkinter import ttk


class myApp:
    top = None

    def __init__(self, master: Tk):
        self.master = master
        self.master.title("Ultra Super nejlepší převodník - MIN0150")

        self.mainFrame = LabelFrame(self.master, text="Main frame", relief="flat")
        self.mainFrame.pack(expand=True, fill="both", side="top")

        self.name = Label(self.mainFrame, text="Patrik Mintěl (MIN0150)")
        self.name.pack(side="bottom", anchor="w")
        self.name.bind("<Button-1>", self.easterEgg)

        self.leftFrame = LabelFrame(self.mainFrame, text="Left frame", relief="flat")
        self.leftFrame.pack(expand=True, fill="both", side="left")

        self.rightFrame = LabelFrame(self.mainFrame, text="Right frame", relief="flat")
        self.rightFrame.pack(expand=True, fill="both", side="right")

        self.selectFrame = LabelFrame(
            self.leftFrame, text="Select frame", relief="flat"
        )
        self.selectFrame.pack(expand=True, fill="x", side="top")

        self.mode = IntVar()
        self.mode.set(0)

        self.celsiusRadio = Radiobutton(
            self.selectFrame,
            text="°C -> °F",
            command=self.update,
            variable=self.mode,
            value=0,
        )
        self.celsiusRadio.grid(row=0, column=0)

        self.celsiusRadio = Radiobutton(
            self.selectFrame,
            text="°F -> °C",
            command=self.update,
            variable=self.mode,
            value=1,
        )
        self.celsiusRadio.grid(row=0, column=1)

        self.inputFrame = LabelFrame(self.leftFrame, text="Input frame", relief="flat")
        self.inputFrame.pack(
            expand=True,
            fill="x",
        )

        self.inputLabel = Label(self.inputFrame, text="Zadejte hodnotu v °C:")
        self.inputLabel.grid(row=0, column=0)
        self.input = Entry(self.inputFrame)
        self.input.grid(row=1, column=0)

        self.outputLabel = Label(self.inputFrame, text="Zadejte hodnotu v °C:")
        self.outputLabel.grid(row=2, column=0)
        self.output = Entry(self.inputFrame, state="readonly")
        self.output.grid(row=3, column=0)

        self.button = Button(self.inputFrame, text="Vypočítat", command=self.calculate)
        self.button.grid(row=4, column=0)

    def update(self):
        if self.mode.get() == 0:
            self.inputLabel.config(text="Zadejte hodnotu v °C:")
            self.outputLabel.config(text="Výstupní hodnota v °F:")
        else:
            self.inputLabel.config(text="Zadejte hodnotu v °F:")
            self.outputLabel.config(text="Výstupní hodnota v °C:")

    def easterEgg(self, _):
        self.name.config(text="Never gonna give you up, never gonna let you down...")

    def calculate(self):
        try:
            print(int(self.input.get()))
        except:
            if self.top != None:
                self.top.destroy()

            self.top = Toplevel(self.master)
            self.top.geometry("250x40")
            self.top.title = "Chyba"

            text = ""
            if len(self.input.get()) == 0:
                text = "Nezadal jsi žádný text"
            else:
                text = "Zadal jsi neplatnou hodnotu"

            label = Label(self.top, text=text)
            label.pack(expand=True, fill="both", side="top")

            button = Button(self.top, text="Zavřít okno", command=self.closeError)
            button.pack(expand=True, fill="both", side="bottom")

    def closeError(self):
        self.top.destroy()


root = Tk()
app = myApp(root)
root.mainloop()
