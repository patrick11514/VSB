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
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>

#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

#include <QMessageBox>
#include <QFileDialog>

#include <QMenu>
#include <QMenuBar>
#include <QAction>

struct Item;
class MainWindow : public QWidget
{
    Q_OBJECT

    QFormLayout *data;
    QGraphicsScene *preview;
    QGraphicsView *previewView;
    std::array<QGraphicsScene *, 4> images;
    std::array<QGraphicsView *, 4> imagesViews;
    std::vector<Item> mainData;
    QStandardItemModel *model;
    QTableView *view;
    int currentIndex = -1;

    QDialog *opened = nullptr;

    void openNewWindow(bool edit);
    void rerenderTable(bool autoselect);

    void deselect();

    QList<QList<QStandardItem *>> transformRows() const;

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

    void selectPreview();
    void selectImages();

    void exitProgram();
    void aboutProgram();
};

struct Item
{
    QString name;
    QString manufacturer;
    QString size;
    QString price;
    QString weight;
    QString previewPath;
    std::array<QString, 4> images;
};

#endif // MAINWINDOW_H
