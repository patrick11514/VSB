from tkinter import *
from tkinter import font


class myApp:
    top = None

    def __init__(self, master: Tk):
        self.master = master
        self.master.title("Ultra Super nejlepší převodník - MIN0150")

        # change default font
        self.defaultFont = font.nametofont("TkDefaultFont")
        self.defaultFont.configure(size=16)

        self.mainFrame = LabelFrame(self.master, text="Main frame", relief="flat")
        self.mainFrame.pack(expand=True, fill="both", side="top")

        self.name = Label(self.mainFrame, font=25, text="Patrik Mintěl (MIN0150)")
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
        self.inputValue = StringVar()
        self.input = Entry(self.inputFrame, textvariable=self.inputValue)
        self.input.grid(row=1, column=0)

        self.outputLabel = Label(self.inputFrame, text="Zadejte hodnotu v °C:")
        self.outputLabel.grid(row=2, column=0)
        self.outputValue = StringVar()
        self.output = Entry(
            self.inputFrame, state="readonly", textvariable=self.outputValue
        )
        self.output.grid(row=3, column=0)

        self.button = Button(self.inputFrame, text="Vypočítat", command=self.calculate)
        self.button.grid(row=4, column=0)

        self.temp = Canvas(self.rightFrame, width=300, height=400)
        self.image = PhotoImage(file="th_empty.png")
        self.temp.create_image(150, 200, image=self.image)
        self.temp.pack()

    def update(self):
        if self.mode.get() == 0:
            self.inputLabel.config(text="Zadejte hodnotu v °C:")
            self.outputLabel.config(text="Výstupní hodnota v °F:")
        else:
            self.inputLabel.config(text="Zadejte hodnotu v °F:")
            self.outputLabel.config(text="Výstupní hodnota v °C:")

        data = self.inputValue.get()

        if len(data) > 0:
            self.calculate()

    def easterEgg(self, _):
        self.name.config(text="Never gonna give you up, never gonna let you down...")

    def calculate(self):
        try:
            input = int(self.inputValue.get())

            if self.mode.get() == 0:
                if input > 50 or input < -20:
                    raise Exception("Invalid input range")

                output = input * 1.8 + 32
                celsius = input
            else:
                if input > 122 or input < -4:
                    raise Exception("Invalid input range")

                output = (input - 32) / 1.8
                celsius = output

            self.outputValue.set(output)

            # 10°C = 30px
            # 10.....30px
            # X.......?px
            diff = 30 * celsius / 10

            self.temp.delete("rect")

            self.temp.create_rectangle(
                149,
                300,
                151,
                230 - diff,
                fill="#01011e",
                outline="#01011e",
                tags="rect",
            )

        except Exception as e:
            print(e)
            if self.top != None:
                self.top.destroy()

            self.top = Toplevel(self.master)
            self.top.geometry("280x65")
            self.top.title = "Chyba"

            text = ""
            if len(self.input.get()) == 0:
                text = "Nezadal jsi žádnou hodnotu"
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
