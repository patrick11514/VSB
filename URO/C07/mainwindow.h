#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QFormLayout>
#include <QDialog>

#include <QJsonDocument>
#include <QByteArray>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QTableView>

#include <QSignalMapper>

#include <QStandardItemModel>
#include <QTabWidget>
#include <QPixmap>

#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

struct Item;
class MainWindow : public QWidget
{
    Q_OBJECT

    QFormLayout *data;
    QPixmap *map;
    QLabel *preview;
    std::array<QLabel *, 4> images;
    std::vector<Item> mainData;
    int currentIndex = -1;

    QDialog *opened = nullptr;

    void openNewWindow(bool edit);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onSelect(QModelIndex index);

    void newItem();
    void editItem();
    void removeItem();

    void closeWindow();
    void confirmWindow();
};

struct Item
{
    QString name;
    QString manufacturer;
    QString size;
    QString price;
    QString weight;
};

#endif // MAINWINDOW_H
