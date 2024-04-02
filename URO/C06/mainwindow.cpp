#include "mainwindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
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
    view->setModel(model);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layout->addWidget(view, 0, 0);

    setLayout(layout);
}

MainWindow::~MainWindow() {}
