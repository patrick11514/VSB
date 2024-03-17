from tkinter import *
from tkinter import font, messagebox, ttk


class myApp:
    formData = [
        {"name": "John", "surname": "Hammond", "birth_number": "045216/1512"},
        {"name": "Harry", "surname": "Potter", "birth_number": "901121/7238"},
        {"name": "Patrik", "surname": "Mintěl", "birth_number": "050301/4567"},
    ]

    addresses = [
        {
            "street": "Časlavská",
            "number": "465",
            "town": "Praha",
            "postal_number": 42000,
        },
        {
            "street": "Přímá",
            "number": "61",
            "town": "Bradavice",
            "postal_number": 61234,
        },
        {"street": "Rovná", "number": "66", "town": "Ostrava", "postal_number": 70800},
    ]

    notes = ["Super týpek", "Čáry máry fuk", "Ten kdo to vytvořil"]

    labels: dict[str, ttk.Label] = {}
    inputs: dict[str, ttk.Entry] = {}

    selectedId: int | None = None

    def __init__(self, master: Tk):
        self.master = master
        self.master.title("Formulář (MIN0150)")

        # change default font
        self.defaultFont = font.nametofont("TkDefaultFont")

        # menu
        self.menu = Menu(self.master)
        # soubor
        self.fileMenu = Menu(self.menu, tearoff=0)
        self.fileMenu.add_command(label="Konec", command=master.quit)
        self.menu.add_cascade(label="Soubor", menu=self.fileMenu)
        # nastavení
        self.something = Menu(self.menu, tearoff=0)
        self.something.add_command(label="Něco", command=self.settings)
        self.menu.add_cascade(label="Nastavení", menu=self.something)
        # nápověda
        self.help = Menu(self.menu, tearoff=0)
        self.help.add_command(label="Pomoc", command=self.callHelp)
        self.menu.add_cascade(label="Nápověda", menu=self.help)
        #
        self.master.config(menu=self.menu)

        # TreeView
        self.treeview = ttk.Frame(self.master)
        self.data = ttk.Treeview(
            self.treeview, columns=("name", "surname", "birth_number"), show="headings"
        )
        self.data.bind("<ButtonRelease-1>", self.selectItem)
        self.data.heading("name", text="First Name")
        self.data.heading("surname", text="Last Name")
        self.data.heading("birth_number", text="Birth Number")

        self.data.pack(side="left", expand=True, fill="both")

        self.scrollbar = ttk.Scrollbar(
            self.treeview, orient=VERTICAL, command=self.data.yview
        )
        self.data.configure(yscroll=self.scrollbar.set)
        self.scrollbar.pack(side="right", fill="y")

        self.treeview.pack(
            side="top",
            fill="both",
        )

        # Fill example data
        i = 0
        for item in self.formData:
            self.data.insert("", END, values=tuple(item.values()), iid=i, text=i)
            i += 1

        # Entries
        self.entries = ttk.Frame(self.master)
        self.entries.columnconfigure(2)

        inputs = [
            {"label": "Jméno:", "name": "name"},
            {"label": "Příjmení:", "name": "surname"},
            {"label": "Rodné číslo:", "name": "birth_number"},
        ]

        i = 0

        for item in inputs:
            self.labels[item["name"]] = ttk.Label(self.entries, text=item["label"])
            self.labels[item["name"]].grid(
                row=i, column=1, sticky="e", pady="2px", padx="4px"
            )

            self.inputs[item["name"]] = ttk.Entry(self.entries, name=item["name"])
            self.inputs[item["name"]].grid(row=i, column=2, sticky="w", pady="2px")

            i += 1

        self.entries.pack(side="top", expand=True, pady="2px")

        # notebook
        self.notebook = ttk.Notebook(self.master)

        self.addressFrame = ttk.Frame(self.notebook)
        # adress
        self.addressMidFrame = ttk.Frame(self.addressFrame)
        # street
        self.street = ttk.Label(self.addressMidFrame, text="Ulice:")
        self.street.grid(row=0, column=0, sticky="e", pady="2px")
        self.steetEntry = ttk.Entry(self.addressMidFrame)
        self.steetEntry.grid(row=0, column=1, sticky="w", pady="2px")
        # house number
        self.houseNumber = ttk.Label(self.addressMidFrame, text="č.p.:")
        self.houseNumber.grid(row=0, column=2, sticky="e", pady="2px", padx="4px")
        self.houseNumberEntry = ttk.Entry(self.addressMidFrame)
        self.houseNumberEntry.grid(row=0, column=3, sticky="w", pady="2px")
        # town
        self.town = ttk.Label(self.addressMidFrame, text="Město:")
        self.town.grid(row=1, column=0, sticky="e", pady="2px", padx="4px")
        self.townEntry = ttk.Entry(self.addressMidFrame)
        self.townEntry.grid(row=1, column=1, columnspan=3, sticky="we", pady="2px")
        # postal number
        self.postalNumber = ttk.Label(self.addressMidFrame, text="PSČ:")
        self.postalNumber.grid(row=2, column=0, sticky="e", pady="2px", padx="4px")
        self.postalNumberEntry = ttk.Entry(self.addressMidFrame, width=10)
        self.postalNumberEntry.grid(row=2, column=1, sticky="w", pady="2px")
        self.addressMidFrame.pack(side="top", expand=True)

        self.notebook.add(self.addressFrame, text="Adresa")

        # notes
        self.notesFrame = ttk.Frame(self.notebook)
        self.notesMidFrame = ttk.Frame(self.notesFrame)
        self.note = Text(self.notesMidFrame, height=10)
        self.note.pack()

        self.notesMidFrame.pack(side="top", expand=True)
        self.notebook.add(self.notesFrame, text="Poznámky")

        self.notebook.pack(side="top", expand=True, fill="both")

        # buttons
        self.buttonsFrame = ttk.Frame(self.master)
        self.cancelButton = ttk.Button(
            self.buttonsFrame, text="Zrušit", command=self.cancel
        )
        self.cancelButton.pack(side="left", padx="2px")
        self.newButton = ttk.Button(
            self.buttonsFrame, text="Nový záznam", command=self.new
        )
        self.newButton.pack(side="left", padx="2px")
        self.saveButton = ttk.Button(
            self.buttonsFrame, text="Uložit záznam", command=self.save
        )
        self.saveButton.pack(side="left", padx="2px")
        self.deleteButton = ttk.Button(
            self.buttonsFrame, text="Smazat záznam", command=self.delete
        )
        self.deleteButton.pack(side="left", padx="2px")
        self.buttonsFrame.pack(side="top", expand=True, pady="2px")

        # name
        self.name = ttk.Label(self.master, text="Patrik Mintěl (MIN0150)")
        self.name.pack(side="left")

    def settings(self):
        print("Nastavení")

    def callHelp(self):
        print("Pomoc už je na cestě")

    def cancel(self):
        # clear all inputs
        self.data.selection_remove(self.selectedId)

        for input in self.inputs.values():
            input.delete(0, END)

        inputs = [
            self.steetEntry,
            self.houseNumberEntry,
            self.townEntry,
            self.postalNumberEntry,
        ]

        for input in inputs:
            input.delete(0, END)

        self.note.delete(0.0, END)
        self.selectedId = None

    def new(self):
        # append empty data
        data = {"name": "", "surname": "", "birth_number": ""}
        self.formData.append(data)
        self.addresses.append(
            {
                "street": "",
                "number": "",
                "town": "",
                "postal_number": "",
            }
        )
        self.notes.append("")

        # put into treeview
        self.selectedId = len(self.formData) - 1
        self.data.insert(
            "",
            END,
            values=tuple(data.values()),
            iid=self.selectedId,
            text=self.selectedId,
        )
        # select in treeview + fill inputs
        self.data.selection_set(self.selectedId)
        self.select()

    def save(self):
        if self.selectedId == None:
            return

        # personal data
        inputs = ["name", "surname", "birth_number"]
        i = 0
        for input in inputs:
            value = self.inputs[input].get()
            # data
            self.formData[self.selectedId][input] = value
            # treeview
            self.data.set(self.selectedId, i, value=value)
            i += 1

        # address info
        inputs = [
            self.steetEntry,
            self.houseNumberEntry,
            self.townEntry,
            self.postalNumberEntry,
        ]
        keys = self.addresses[self.selectedId].keys()
        i = 0
        for key in keys:
            self.addresses[self.selectedId][key] = inputs[i].get()
            i += 1

        # notes
        self.notes[self.selectedId] = self.note.get(0.0, END)

    def selectItem(self, _):
        selectedItem = self.data.focus()
        if selectedItem == "":
            return
        dict = self.data.item(selectedItem)
        self.selectedId = int(dict["text"])
        self.select()

    def select(self):
        # put to fields
        inputs = ["name", "surname", "birth_number"]

        for name in inputs:
            self.inputs[name].delete(0, END)
            self.inputs[name].insert(0, self.formData[self.selectedId][name])

        # address
        address = list(self.addresses[self.selectedId].values())
        inputs = [
            self.steetEntry,
            self.houseNumberEntry,
            self.townEntry,
            self.postalNumberEntry,
        ]
        i = 0
        for input in inputs:
            input.delete(0, END)
            input.insert(0, address[i])
            i += 1

        # notes
        self.note.delete(0.0, END)
        self.note.insert(0.0, self.notes[self.selectedId])

    def itterate(self, list: list[any], exclude: int):
        i = 0
        for item in list:
            if i != exclude:
                yield item
            i += 1

    def delete(self):
        if self.selectedId == None:
            return

        result = messagebox.askyesno(
            title="Smazání záznamu", message="Opravdu chceš smazat tento záznam?"
        )

        if not result:
            return

        oldSize = len(self.formData)

        self.formData = list(self.itterate(self.formData, self.selectedId))
        self.addresses = list(self.itterate(self.addresses, self.selectedId))
        self.notes = list(self.itterate(self.notes, self.selectedId))

        self.cancel()
        # rerender list
        for i in range(oldSize):
            self.data.delete(i)
        # add back

        for i in range(len(self.formData)):
            self.data.insert(
                "",
                END,
                values=tuple(self.formData[i].values()),
                iid=i,
                text=i,
            )


root = Tk()
app = myApp(root)
root.mainloop()
