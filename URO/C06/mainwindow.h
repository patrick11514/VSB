#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QGridLayout>
#include <QJsonDocument>
#include <QByteArray>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QTableView>
#include <QStandardItemModel>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
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
