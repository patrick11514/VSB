#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    QGridLayout* layout = new QGridLayout(this);

    screen = new QLineEdit();
    layout->addWidget(screen, 1, 1, 1, 4);


    setLayout(layout);

}

MainWindow::~MainWindow() {}
