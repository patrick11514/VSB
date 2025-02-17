from tkinter import *
from tkinter import filedialog, messagebox, ttk


class myApp:

    searchEntries: dict[str, Entry] = {}
    exampleData = [
        {
            "name": "Date A Live IV - Hoshimiya Mukuro",
            "manufacturer": "Taito",
            "size": "180mm",
            "price": "20$",
            "weight": "235g",
            "images": {
                "main": "items/1/main.png",
                "other": ["items/1/1.png", "items/1/2.png"],
            },
        },
        {
            "name": "Sono Bisque Doll wa Koi o Suru - Kitagawa Marin",
            "manufacturer": "Sega",
            "size": "190mm",
            "price": "19.41$",
            "weight": "245g",
            "images": {
                "main": "items/2/main.png",
                "other": [
                    "items/2/1.png",
                    "items/2/2.png",
                    "items/2/3.png",
                    "items/2/4.png",
                ],
            },
        },
        {
            "name": "Hatsune Miku",
            "manufacturer": "Taito",
            "size": "230mm",
            "price": "30$",
            "weight": "422g",
            "images": {
                "main": "items/3/main.png",
                "other": [
                    "items/3/1.png",
                    "items/3/2.png",
                    "items/3/3.png",
                ],
            },
        },
        {
            "name": "Black ★ Rock Shooter - Pop Up Parade",
            "manufacturer": "Good Smile Company",
            "size": "170mm",
            "price": "44$",
            "weight": "340g",
            "images": {
                "main": "items/4/main.png",
                "other": [
                    "items/4/1.png",
                    "items/4/2.png",
                    "items/4/3.png",
                    "items/4/4.png",
                ],
            },
        },
    ]

    infoElements: dict[str, Label] = {}

    infoColumns = {
        "id": "Id",
        "name": "Jméno",
        "manufacturer": "Výrobce",
        "size": "Velikost",
        "weight": "Váha",
        "price": "Cena",
    }

    columns = {
        "id": {"name": "ID", "width": 10},
        "name": {"name": "Jméno figurky", "width": 350},
        "manufacturer": {"name": "Výrobce", "width": 180},
        "weight": {"name": "Váha", "width": 50},
        "size": {"name": "Velikost", "width": 80},
        "price": {"name": "Cena", "width": 80},
    }

    previewId: int = None
    imagesList: list[int] = []
    imagelist: list[PhotoImage] = []

    selected: int | None = None

    def __init__(self, master: Tk):
        self.master = master
        self.master.configure(bg="white")
        # icon and title
        self.master.title("Evidence anime figurek (MIN0150)")
        icon = PhotoImage(file="icon.png")
        # https://stackoverflow.com/a/11180300
        self.master.call("wm", "iconphoto", root._w, icon)

        self.master.minsize(1500, 800)

        # change default font
        self.style = ttk.Style()
        self.style.configure(".", font=("Ubuntu", 12))

        # menu
        self.menu = Menu(self.master)

        self.file = Menu(self.menu, tearoff=0)
        self.file.add_command(label="Nový", command=self.add)
        self.file.add_command(label="Smazat", command=self.remove)
        self.file.add_command(label="Ukončit", command=self.master.destroy)
        self.menu.add_cascade(label="Soubor", menu=self.file)

        self.other = Menu(self.menu, tearoff=0)
        self.other.add_command(label="O programu", command=self.about)
        self.menu.add_cascade(label="Další", menu=self.other)

        self.master.config(menu=self.menu)

        # search
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

        self.listTree = ttk.Treeview(
            self.list,
            columns=tuple(self.columns.keys()),
            show="headings",
        )
        self.listTree.bind("<ButtonRelease-1>", self.selectItem)

        for key, value in self.columns.items():
            self.listTree.heading(key, text=value["name"])
            self.listTree.column(key, width=value["width"])

        self.listTree.pack(side="left", fill="both", expand=True)
        self.scrollbar = ttk.Scrollbar(
            self.list, orient=VERTICAL, command=self.listTree.yview
        )
        self.listTree.configure(yscroll=self.scrollbar.set)
        self.scrollbar.pack(side="right", fill="y")
        self.list.pack(side="left", fill="both", expand=True, padx="20", pady="20")

        # fill example data
        self.fillData()

        self.view = Frame(self.master)
        self.topView = Frame(self.view)
        self.notebook = ttk.Notebook(self.topView)

        self.info = Frame(self.notebook)
        self.infoLabel = Label(
            self.info, text="Vyber produkt pro zobrazení informací", font=("Ubuntu", 14)
        )
        self.infoLabel.pack(side="top", fill="both", expand=True)
        self.notebook.add(self.info, text="Informace o produktu")

        self.moreImages = Frame(self.notebook)
        self.images = Canvas(
            self.moreImages, width=400, height=400, background="darkgray"
        )
        self.images.pack(side="top")
        self.notebook.add(self.moreImages, text="Více obrázků")

        self.notebook.pack(side="left", anchor="n", fill="y", padx="20", pady="20")

        self.preview = Canvas(
            self.topView,
            width=200,
            height=200,
            background="darkgray",
        )
        self.preview.pack(side="right", anchor="n")

        self.topView.pack(side="top", fill="both")

        self.addItem = Button(
            self.view,
            text="Přidat",
            background="darkgray",
            font=("Ubuntu", 16),
            borderwidth=0,
            command=self.add,
        )
        self.addItem.pack(side="left", anchor="s", padx="20", pady="20")

        self.editItem = Button(
            self.view,
            text="Upravit",
            background="darkgray",
            font=("Ubuntu", 16),
            borderwidth=0,
        )
        self.editItem.pack(side="left", anchor="s", padx="20", pady="20")

        self.removeItem = Button(
            self.view,
            text="Odebrat",
            background="darkgray",
            font=("Ubuntu", 16),
            borderwidth=0,
            command=self.remove,
        )
        self.removeItem.pack(side="left", anchor="s", padx="20", pady="20")

        self.view.pack(side="right", fill="both", expand=True, padx="20", pady="20")

    def about(self):
        window = Toplevel()
        window.grab_set()
        window.title("O programu")

        frame = Frame(window)
        label = Label(frame, text="Evidence Anime Figurek", font=("Ubuntu", 20))
        label.pack(side="top")
        text = Label(
            frame, text="Vytvořil Patrik Mintěl (MIN0150)", font=("Ubuntu", 14)
        )
        text.pack(side="top", fill="both")
        button = Button(frame, text="Ok", command=window.destroy, font=("Ubuntu", 14))
        button.pack(side="top", expand=True)
        frame.pack(side="top", expand=True, fill="both", padx="40", pady="40")

    def unselectItem(self):
        if self.selected == None:
            return

        self.selected = None

        for element in self.infoElements.values():
            element.destroy()

        self.infoElements.clear()
        self.infoLabel = Label(
            self.info, text="Vyber produkt pro zobrazení informací", font=("Ubuntu", 14)
        )
        self.infoLabel.pack(side="top", fill="both", expand=True)

        if self.previewId != None:
            self.preview.delete(self.previewId)
            self.previewId = None

        for imgId in self.imagesList:
            self.images.delete(imgId)
        self.imagesList.clear()

    def selectItem(self, _):
        selectedItem = self.listTree.focus()

        if selectedItem == "":
            return

        # remove old elements from UI
        for element in self.infoElements.values():
            element.destroy()

        self.infoElements.clear()
        self.infoLabel.destroy()

        selectedData = self.exampleData[int(selectedItem) - 1]
        self.selected = int(selectedItem) - 1

        for col, name in self.infoColumns.items():
            label = Label(
                self.info,
                font=("Ubuntu", 12),
            )
            if col == "id":
                label.configure(text=f"{name}: {selectedItem}")
            else:
                label.configure(text=f"{name}: {selectedData[col]}")
            label.pack(side="top", anchor="w", padx="10")
            self.infoElements[col] = label

        self.image = PhotoImage(file=selectedData["images"]["main"])
        if self.previewId != None:
            self.preview.delete(self.previewId)
        self.previewId = self.preview.create_image(100, 100, image=self.image)

        self.imagelist.clear()

        for imgId in self.imagesList:
            self.images.delete(imgId)
        self.imagesList.clear()

        moreImages = selectedData["images"]["other"]
        i = 0
        x = 0
        y = 0
        for path in moreImages:
            self.imagelist.append(PhotoImage(file=path))
            self.imagesList.append(
                self.images.create_image(
                    x * 200 + 100, y * 200 + 100, image=self.imagelist[i]
                )
            )
            x += 1
            i += 1
            if x > 1:
                y += 1
                x = 0

    def add(self):
        window = Toplevel()
        window.grab_set()
        window.title("Přidání nové figurky")

        label = Label(window, text="Přidání nové figurky", font=("Ubuntu", 20))
        label.pack(side="top", anchor="w", padx="20", pady="20")

        frame = Frame(window)
        inputs = Frame(frame)

        names = ["Jméno", "Výrobce", "Velikost", "Cena"]

        for name in names:
            inputFrame = Frame(inputs)
            label = Label(inputFrame, text=name, font=("Ubuntu", 14))
            label.pack(side="top", anchor="w")
            entry = Entry(
                inputFrame, borderwidth=0, background="darkgray", font=("Ubuntu", 14)
            )
            entry.pack(side="bottom")
            inputFrame.pack(side="top")

        inputs.pack(side="left", padx="40", pady="40")

        upload = Button(
            frame,
            width=25,
            height=10,
            text="Nahrát obrázky",
            font=("Ubuntu", 18),
            command=self.upload,
            borderwidth=0,
            background="darkgray",
        )
        upload.pack(side="right", padx="40", pady="40")

        buttons = Frame(frame)

        add = Button(
            buttons,
            text="Přidat",
            background="darkgray",
            borderwidth=0,
            font=("Ubuntu", 18),
        )
        add.pack(side="left", padx="10", pady="10")

        cancel = Button(
            buttons,
            text="Zrušit",
            background="darkgray",
            borderwidth=0,
            command=window.destroy,
            font=("Ubuntu", 18),
        )
        cancel.pack(side="right", padx="10", pady="10")

        buttons.pack(side="bottom")

        frame.pack(side="top", fill="both", expand=True)

    def upload(self):
        file = filedialog.askopenfiles(
            parent=None,
            title="Vyber prosím obrázky",
            filetypes=[("Obrázky PNG", "*.png")],
        )
        if file == None:
            return

        print(file.name)

    def remove(self):
        if self.selected == None:
            messagebox.showerror(title="Error", message="Nevybral jsi žádnou položku")
            return

        result = messagebox.askyesno(
            title="Smazání položky", message="Opravdu chceš smazat tuto položku?"
        )

        if not result:
            return

        for i in range(len(self.exampleData)):
            self.listTree.delete(i + 1)

        self.exampleData.pop(self.selected)

        self.fillData()
        self.unselectItem()

    def fillData(self):
        i = 1
        for data in self.exampleData:
            vals = []
            for key in self.columns.keys():
                if key == "id":
                    vals.append(i)
                    continue
                vals.append(data[key])
            self.listTree.insert("", END, values=tuple(vals), iid=i, text=i)

            i += 1


root = Tk()
app = myApp(root)
root.mainloop()
