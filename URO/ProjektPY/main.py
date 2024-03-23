from tkinter import *
from tkinter import font, ttk


class myApp:

    searchEntries: dict[str, Entry] = {}
    exampleData = []

    def __init__(self, master: Tk):
        self.master = master
        self.master.configure(bg="white")
        # icon and title
        self.master.title("Evidence anime figurek (MIN0150)")
        icon = PhotoImage(file="icon.png")
        # https://stackoverflow.com/a/11180300
        self.master.call("wm", "iconphoto", root._w, icon)

        # change default font
        self.defaultFont = font.nametofont("TkDefaultFont")

        self.search = Frame(self.master)

        self.searchInner = Frame(self.search)

        self.searchLabel = Label(
            self.searchInner, text="Vyhledávání", font=("Ubuntu", 18)
        )
        self.searchLabel.pack(side="top", anchor="w")

        entries = {
            "name": "Jméno",
            "manufacturer": "Výrobce",
            "size": "Velikost",
            "price": "Cena",
        }

        i = 0
        for name, title in entries.items():

            frame = Frame(self.searchInner)
            label = Label(frame, text=title, font=("Ubuntu", 14))
            label.pack(side="top", anchor="w")
            entry = Entry(frame, bg="darkgray", borderwidth=0, font=("Ubuntu", 14))
            entry.pack(side="bottom")
            if i == 0:
                frame.pack(side="left")
            else:
                frame.pack(side="left", padx="20")

            self.searchEntries[name] = entry
            i += 1

        self.searchSearch = Button(
            self.searchInner,
            text="Vyhledat",
            font=("Ubuntu", 14),
            borderwidth=0,
            bg="darkgray",
        )
        self.searchSearch.pack(side="right")

        self.searchReset = Button(
            self.searchInner,
            text="Resetovat",
            font=("Ubuntu", 14),
            borderwidth=0,
            bg="darkgray",
        )
        self.searchReset.pack(side="right", padx="20")

        self.searchInner.pack(
            side="top", fill="both", expand=True, padx="20", pady="20"
        )
        self.search.pack(side="top", fill="x", padx="20", pady="20")

        self.list = Frame(self.master)
        columns = {
            "id": "ID",
            "name": "Jméno figurky",
            "manufacturer": "Výrobce",
            "weight": "Váha",
            "size": "Velikost",
            "price": "Cena",
        }
        self.listTree = ttk.Treeview(
            self.list,
            columns=tuple(columns.keys()),
            show="headings",
        )
        self.listTree.bind("<ButtonRelease-1>", print)

        for key, value in columns.items():
            self.listTree.heading(key, text=value)

        self.listTree.pack(side="top", fill="both", expand=True)
        self.list.pack(side="left", fill="both", expand=True, padx="20", pady="20")

        self.view = Frame(self.master)
        self.view.pack(side="right", fill="both", expand=True, padx="20", pady="20")


root = Tk()
app = myApp(root)
root.mainloop()
