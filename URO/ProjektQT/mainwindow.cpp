#include "mainwindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    this->resize(1512, 650);
    this->setMinimumSize(1512, 650);
    this->setFont(QFont{"Sans Serif", 13});
    this->setWindowIcon(QIcon{"../assets/icon.png"});

    this->setWindowTitle("Evidence anime figurek");
    QGridLayout *layout = new QGridLayout(this);

    QList<QList<QStandardItem *>> rows;

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

            this->mainData.push_back(i);

            QList<QStandardItem *> row = {
                new QStandardItem(i.name),
                new QStandardItem(i.manufacturer),
                new QStandardItem(i.size),
                new QStandardItem(i.price),
                new QStandardItem(i.weight)};

            rows.push_back(row);
        }
    }

    QStandardItemModel *model = new QStandardItemModel(0, 5, layout);
    model->setHeaderData(0, Qt::Orientation::Horizontal, "Jméno");
    model->setHeaderData(1, Qt::Orientation::Horizontal, "Výrobce");
    model->setHeaderData(2, Qt::Orientation::Horizontal, "Velikost");
    model->setHeaderData(3, Qt::Orientation::Horizontal, "Cena");
    model->setHeaderData(4, Qt::Orientation::Horizontal, "Váha");

    for (const auto &row : rows)
    {
        model->appendRow(row);
    }

    QTableView *view = new QTableView();
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
            QLabel *singleImage = new QLabel();
            QPixmap image("../assets/blank.png");
            singleImage->setPixmap(image);

            imagesLayout->addWidget(singleImage, y, x);

            this->images[y * 2 + x] = singleImage;
        }
    }

    images->setLayout(imagesLayout);

    tabs->addTab(images, "Další obrázky");

    rightLayout->addWidget(tabs, 0, 0, 2, 1);

    this->map = new QPixmap("../assets/blank.png");
    this->preview = new QLabel();
    this->preview->setPixmap(*this->map);

    rightLayout->addWidget(this->preview, 0, 1);

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
}

void MainWindow::openNewWindow(bool edit = false)
{
    if (edit && this->currentIndex == -1)
    {
        return;
    }

    QDialog *window = new QDialog();
    this->opened = window;
    window->setWindowTitle(edit ? "Úprava položky" : "Vytváření položky");
    QFormLayout *inputs = new QFormLayout();

    inputs->addRow("&Id", new QLineEdit());
    inputs->addRow("&Jméno", new QLineEdit());
    inputs->addRow("&Výrobce", new QLineEdit());
    inputs->addRow("&Velikost", new QLineEdit());
    inputs->addRow("&Váha", new QLineEdit());
    inputs->addRow("&Cena", new QLineEdit());

    QPushButton *confirm = new QPushButton(edit ? "Upravit" : "Přidat");
    connect(confirm, SIGNAL(clicked()), this, SLOT(confirmWindow()));

    QPushButton *cancel = new QPushButton("Zrušit");
    connect(cancel, SIGNAL(clicked()), this, SLOT(closeWindow()));

    inputs->addRow(confirm, cancel);

    if (edit)
    {
        const Item &row = this->mainData[this->currentIndex];

        QLineEdit *id = dynamic_cast<QLineEdit *>(inputs->itemAt(1)->widget());
        if (id != nullptr)
        {
            id->setText(QString::fromStdString(std::to_string(this->currentIndex + 1)));
        }

        QLineEdit *name = dynamic_cast<QLineEdit *>(inputs->itemAt(3)->widget());
        if (name != nullptr)
        {
            name->setText(row.name);
        }

        QLineEdit *manufacturer = dynamic_cast<QLineEdit *>(inputs->itemAt(5)->widget());
        if (manufacturer != nullptr)
        {
            manufacturer->setText(row.manufacturer);
        }

        QLineEdit *size = dynamic_cast<QLineEdit *>(inputs->itemAt(7)->widget());
        if (size != nullptr)
        {
            size->setText(row.size);
        }

        QLineEdit *weight = dynamic_cast<QLineEdit *>(inputs->itemAt(9)->widget());
        if (weight != nullptr)
        {
            weight->setText(row.weight);
        }

        QLineEdit *price = dynamic_cast<QLineEdit *>(inputs->itemAt(11)->widget());
        if (price != nullptr)
        {
            price->setText(row.price);
        }
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

    // POPUP to confirm
    // then remove

    qInfo("TODO");
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
    qInfo("TODO");

    this->closeWindow();
}
