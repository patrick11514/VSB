#include "mainwindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    this->resize(1550, 700);
    this->setMinimumSize(1550, 700);
    this->setFont(QFont{"Sans Serif", 13});
    this->setWindowIcon(QIcon{"../assets/icon.png"});

    this->setWindowTitle("Evidence anime figurek");
    QGridLayout *layout = new QGridLayout(this);

    // load data
    QFile loadFile("../data.json");
    if (loadFile.open(QIODevice::ReadOnly))
    {
        QByteArray data = loadFile.readAll();
        QJsonDocument arr = QJsonDocument::fromJson(data);
        QJsonArray array = arr.array();

        for (auto item : array)
        {
            QJsonObject obj = item.toObject();
            Item i;
            i.name = obj.find("name")->toString();
            i.manufacturer = obj.find("manufacturer")->toString();
            i.size = obj.find("size")->toString();
            i.price = obj.find("price")->toString();
            i.weight = obj.find("weight")->toString();
            QJsonObject images = obj.find("images")->toObject();
            i.previewPath = images.find("main")->toString();
            QJsonArray otherImages = images.find("other")->toArray();

            int index = 0;
            for (auto item : otherImages)
            {
                if (index >= 4)
                {
                    break;
                }
                i.images[index] = item.toString();
                ++index;
            }

            this->mainData.push_back(i);
        }
    }

    // menu
    QMenuBar *bar = new QMenuBar();

    QMenu *fileMenu = new QMenu(tr("Soubor"));
    QAction *newItem = new QAction(tr("&Nový"));
    connect(newItem, SIGNAL(triggered()), this, SLOT(newItem()));
    fileMenu->addAction(newItem);

    QAction *removeItem = new QAction(tr("Smazat"));
    connect(removeItem, SIGNAL(triggered()), this, SLOT(removeItem()));
    fileMenu->addAction(removeItem);

    QAction *exit = new QAction(tr("Ukončit"));
    connect(exit, SIGNAL(triggered()), this, SLOT(exitProgram()));
    fileMenu->addAction(exit);
    bar->addMenu(fileMenu);

    QMenu *other = new QMenu(tr("Další"));
    QAction *about = new QAction(tr("O programu"));
    connect(about, SIGNAL(triggered()), this, SLOT(aboutProgram()));
    other->addAction(about);

    bar->addMenu(other);

    this->layout()->setMenuBar(bar);

    QList<QList<QStandardItem *>> rows = this->transformRows();

    model = new QStandardItemModel(0, 5, layout);
    model->setHeaderData(0, Qt::Orientation::Horizontal, "Jméno");
    model->setHeaderData(1, Qt::Orientation::Horizontal, "Výrobce");
    model->setHeaderData(2, Qt::Orientation::Horizontal, "Velikost");
    model->setHeaderData(3, Qt::Orientation::Horizontal, "Cena");
    model->setHeaderData(4, Qt::Orientation::Horizontal, "Váha");

    for (const auto &row : rows)
    {
        model->appendRow(row);
    }

    view = new QTableView();
    view->setFont(QFont{"Sans Serif", 13});
    view->setModel(model);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view->resizeColumnsToContents();

    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(onSelect(QModelIndex)));

    layout->addWidget(view, 0, 0);

    QGridLayout *rightLayout = new QGridLayout(this);

    QTabWidget *tabs = new QTabWidget();
    tabs->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QFrame *infoTab = new QFrame();
    this->data = new QFormLayout();

    QLineEdit *id = new QLineEdit();
    id->setReadOnly(true);
    this->data->addRow("&Id", id);

    QLineEdit *name = new QLineEdit();
    name->setReadOnly(true);
    this->data->addRow("&Jméno", name);

    QLineEdit *manufacturer = new QLineEdit();
    manufacturer->setReadOnly(true);
    this->data->addRow("&Výrobce", manufacturer);

    QLineEdit *size = new QLineEdit();
    size->setReadOnly(true);
    this->data->addRow("&Velikost", size);

    QLineEdit *weight = new QLineEdit();
    weight->setReadOnly(true);
    this->data->addRow("&Váha", weight);

    QLineEdit *price = new QLineEdit();
    price->setReadOnly(true);
    this->data->addRow("&Cena", price);
    infoTab->setLayout(this->data);

    tabs->addTab(infoTab, "Informace o produktu");

    QFrame *images = new QFrame();
    QGridLayout *imagesLayout = new QGridLayout();

    for (int y = 0; y < 2; ++y)
    {
        for (int x = 0; x < 2; ++x)
        {
            QGraphicsScene *singleImage = new QGraphicsScene();
            QGraphicsView *graphicView = new QGraphicsView(singleImage);

            QPixmap image("../assets/blank.png");
            singleImage->addPixmap(image);

            graphicView->setSceneRect(singleImage->sceneRect());

            imagesLayout->addWidget(graphicView, y, x);

            this->images[y * 2 + x] = singleImage;
            this->imagesViews[y * 2 + x] = graphicView;
        }
    }

    images->setLayout(imagesLayout);

    tabs->addTab(images, "Další obrázky");

    rightLayout->addWidget(tabs, 0, 0, 2, 1);

    QPixmap map("../assets/blank.png");
    this->preview = new QGraphicsScene();
    this->previewView = new QGraphicsView(this->preview);
    this->preview->addPixmap(map);
    this->previewView->setSceneRect(this->preview->sceneRect());

    rightLayout->addWidget(this->previewView, 0, 1, Qt::AlignTop);

    QHBoxLayout *buttons = new QHBoxLayout();
    QPushButton *addButton = new QPushButton("Přidat");
    buttons->addWidget(addButton);
    connect(addButton, SIGNAL(clicked()), this, SLOT(newItem()));

    QPushButton *editButton = new QPushButton("Upravit");
    buttons->addWidget(editButton);
    connect(editButton, SIGNAL(clicked()), this, SLOT(editItem()));

    QPushButton *removeButton = new QPushButton("Odebrat");
    buttons->addWidget(removeButton);
    connect(removeButton, SIGNAL(clicked()), this, SLOT(removeItem()));

    rightLayout->addLayout(buttons, 2, 0, 1, 2);

    layout->addLayout(rightLayout, 0, 1);
    setLayout(layout);
}

MainWindow::~MainWindow() {}

void MainWindow::onSelect(QModelIndex index)
{
    if (!index.isValid())
    {
        return;
    }

    auto &row = this->mainData[index.row()];

    this->currentIndex = index.row();

    // fill inputs
    QLineEdit *id = dynamic_cast<QLineEdit *>(this->data->itemAt(1)->widget());
    if (id != nullptr)
    {
        id->setText(QString::fromStdString(std::to_string(index.row() + 1)));
    }

    QLineEdit *name = dynamic_cast<QLineEdit *>(this->data->itemAt(3)->widget());
    if (name != nullptr)
    {
        name->setText(row.name);
    }

    QLineEdit *manufacturer = dynamic_cast<QLineEdit *>(this->data->itemAt(5)->widget());
    if (manufacturer != nullptr)
    {
        manufacturer->setText(row.manufacturer);
    }

    QLineEdit *size = dynamic_cast<QLineEdit *>(this->data->itemAt(7)->widget());
    if (size != nullptr)
    {
        size->setText(row.size);
    }

    QLineEdit *weight = dynamic_cast<QLineEdit *>(this->data->itemAt(9)->widget());
    if (weight != nullptr)
    {
        weight->setText(row.weight);
    }

    QLineEdit *price = dynamic_cast<QLineEdit *>(this->data->itemAt(11)->widget());
    if (price != nullptr)
    {
        price->setText(row.price);
    }

    // preview image
    QPixmap preview(row.previewPath);
    this->preview->clear();
    this->preview->addPixmap(preview);
    this->previewView->setSceneRect(preview.rect());

    // other images
    for (int i = 0; i < 4; ++i)
    {
        auto item = row.images[i];

        auto img = this->images[i];
        img->clear();

        if (item.size() == 0)
        {
            continue;
        }

        QPixmap image(item);
        img->addPixmap(image);
        this->imagesViews[i]->setSceneRect(image.rect());
    }
}

void MainWindow::deselect()
{
    this->currentIndex = -1;

    QLineEdit *id = dynamic_cast<QLineEdit *>(this->data->itemAt(1)->widget());
    if (id != nullptr)
    {
        id->setText("");
    }

    QLineEdit *name = dynamic_cast<QLineEdit *>(this->data->itemAt(3)->widget());
    if (name != nullptr)
    {
        name->setText("");
    }

    QLineEdit *manufacturer = dynamic_cast<QLineEdit *>(this->data->itemAt(5)->widget());
    if (manufacturer != nullptr)
    {
        manufacturer->setText("");
    }

    QLineEdit *size = dynamic_cast<QLineEdit *>(this->data->itemAt(7)->widget());
    if (size != nullptr)
    {
        size->setText("");
    }

    QLineEdit *weight = dynamic_cast<QLineEdit *>(this->data->itemAt(9)->widget());
    if (weight != nullptr)
    {
        weight->setText("");
    }

    QLineEdit *price = dynamic_cast<QLineEdit *>(this->data->itemAt(11)->widget());
    if (price != nullptr)
    {
        price->setText("");
    }

    this->preview->clear();

    for (auto item : this->images)
    {
        item->clear();
    }
}

void MainWindow::openNewWindow(bool edit = false)
{
    if (edit && this->currentIndex == -1)
    {
        return;
    }

    this->isEdit = edit;

    QDialog *window = new QDialog();
    this->opened = window;
    window->setFont(QFont{"Sans Serif", 13});
    window->setWindowTitle(edit ? "Úprava položky" : "Vytváření položky");
    QFormLayout *inputs = new QFormLayout();
    this->openedData = inputs;

    inputs->addRow("&Jméno", new QLineEdit());
    inputs->addRow("&Výrobce", new QLineEdit());
    inputs->addRow("&Velikost", new QLineEdit());
    inputs->addRow("&Váha", new QLineEdit());
    inputs->addRow("&Cena", new QLineEdit());

    QPushButton *preview = new QPushButton("Vybrat náhled");
    connect(preview, SIGNAL(clicked()), this, SLOT(selectPreview()));

    QPushButton *images = new QPushButton("Vybrat obrázky");
    connect(images, SIGNAL(clicked()), this, SLOT(selectImages()));

    inputs->addRow(preview, images);

    QPushButton *confirm = new QPushButton(edit ? "Upravit" : "Přidat");
    connect(confirm, SIGNAL(clicked()), this, SLOT(confirmWindow()));

    QPushButton *cancel = new QPushButton("Zrušit");
    connect(cancel, SIGNAL(clicked()), this, SLOT(closeWindow()));

    inputs->addRow(confirm, cancel);

    if (edit)
    {
        const Item &row = this->mainData[this->currentIndex];

        QLineEdit *name = dynamic_cast<QLineEdit *>(inputs->itemAt(1)->widget());
        if (name != nullptr)
        {
            name->setText(row.name);
        }

        QLineEdit *manufacturer = dynamic_cast<QLineEdit *>(inputs->itemAt(3)->widget());
        if (manufacturer != nullptr)
        {
            manufacturer->setText(row.manufacturer);
        }

        QLineEdit *size = dynamic_cast<QLineEdit *>(inputs->itemAt(5)->widget());
        if (size != nullptr)
        {
            size->setText(row.size);
        }

        QLineEdit *weight = dynamic_cast<QLineEdit *>(inputs->itemAt(7)->widget());
        if (weight != nullptr)
        {
            weight->setText(row.weight);
        }

        QLineEdit *price = dynamic_cast<QLineEdit *>(inputs->itemAt(9)->widget());
        if (price != nullptr)
        {
            price->setText(row.price);
        }

        this->currentPreviewPath = row.previewPath;
        this->currentImagesPaths = row.images;
    }

    window->setLayout(inputs);
    window->exec();
}

void MainWindow::newItem()
{
    this->openNewWindow(false);
}

void MainWindow::editItem()
{
    this->openNewWindow(true);
}

void MainWindow::removeItem()
{
    if (this->currentIndex == -1)
    {
        return;
    }

    QMessageBox::StandardButton clicked = QMessageBox::question(this, "Smazání položky", "Opravdu chcete smazat tuto položku?");

    if (clicked == QMessageBox::StandardButton::No)
    {
        return;
    }

    mainData.erase(mainData.begin() + currentIndex);

    this->rerenderTable(false);
    this->deselect();
}

void MainWindow::closeWindow()
{
    if (this->opened != nullptr)
    {
        this->opened->close();
        this->opened = nullptr;
    }
}

void MainWindow::confirmWindow()
{
    Item item;
    if (this->isEdit) {
        item = this->mainData[this->currentIndex];
    }

    QLineEdit *name = dynamic_cast<QLineEdit *>(this->openedData->itemAt(1)->widget());
    if (name != nullptr)
    {
        item.name = name->text();
    }

    QLineEdit *manufacturer = dynamic_cast<QLineEdit *>(this->openedData->itemAt(3)->widget());
    if (manufacturer != nullptr)
    {
        item.manufacturer = manufacturer->text();
    }

    QLineEdit *size = dynamic_cast<QLineEdit *>(this->openedData->itemAt(5)->widget());
    if (size != nullptr)
    {
        item.size = size->text();
    }

    QLineEdit *weight = dynamic_cast<QLineEdit *>(this->openedData->itemAt(7)->widget());
    if (weight != nullptr)
    {
        item.weight = weight->text();
    }

    QLineEdit *price = dynamic_cast<QLineEdit *>(this->openedData->itemAt(9)->widget());
    if (price != nullptr)
    {
        item.price = price->text();
    }

    item.previewPath = this->currentPreviewPath;
    item.images = std::move(this->currentImagesPaths);

    if (isEdit) {
        this->mainData[this->currentIndex] = item;
    } else {
        this->mainData.push_back(item);
    }

    this->rerenderTable(true);
    this->rerenderImages();
    this->isEdit = false;

    this->closeWindow();
}

void MainWindow::selectPreview()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Otevřít náhled", "", "*.png");
    if (fileName.size() == 0)
    {
        return;
    }

    this->currentPreviewPath = fileName;

    // do something
}

void MainWindow::selectImages()
{
     QStringList list = QFileDialog::getOpenFileNames(this, "Otevřít soubory (max 4)", "", "*.png");
     if (list.size() > 4) {
         QMessageBox::warning(this, "Příliš mnoho obrázků", "Můžete vybrat maximálně 4 obrázky.");
         return;
     }

    for (size_t i = 0; i < 4; ++i) {
        this->currentImagesPaths[i] = "";
    }

     for (size_t i = 0; i < list.size(); ++i) {
         this->currentImagesPaths[i] = list[i];
     }
}

void MainWindow::rerenderTable(bool autoselect)
{
    // remove all entries from model
    int count = model->rowCount();

    for (int i = 0; i < count; ++i)
    {
        model->removeRow(0);
    }

    // add entries again

    QList<QList<QStandardItem *>> rows = this->transformRows();

    for (auto row : rows)
    {
        model->appendRow(row);
    }

    if (autoselect)
    {
        view->selectRow(currentIndex);
    }
}

QList<QList<QStandardItem *>> MainWindow::transformRows() const
{
    QList<QList<QStandardItem *>> rows;

    for (auto item : this->mainData)
    {
        QList<QStandardItem *> row = {
            new QStandardItem(item.name),
            new QStandardItem(item.manufacturer),
            new QStandardItem(item.size),
            new QStandardItem(item.price),
            new QStandardItem(item.weight)};

        rows.push_back(row);
    }

    return rows;
}

void MainWindow::exitProgram()
{
    this->close();
}

void MainWindow::aboutProgram()
{
    QMessageBox::information(this, "O programu", "Evodence anime figurem\nVytvořil Patrik Mintěl (MIN0150)");
}


void MainWindow::rerenderImages() const {
    if (this->currentIndex < 0) return;

    auto &row = this->mainData[this->currentIndex];

    // preview image
    QPixmap preview(row.previewPath);
    this->preview->clear();
    this->preview->addPixmap(preview);
    this->previewView->setSceneRect(preview.rect());

    // other images
    for (int i = 0; i < 4; ++i)
    {
        auto item = row.images[i];

        auto img = this->images[i];
        img->clear();

        if (item.size() == 0)
        {
            continue;
        }

        QPixmap image(item);
        img->addPixmap(image);
        this->imagesViews[i]->setSceneRect(image.rect());
    }
}
