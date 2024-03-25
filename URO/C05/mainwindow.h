#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>

enum Operation
{
    PLUS,
    MINUS,
    MUL,
    DIV
};

class MainWindow : public QWidget
{
    Q_OBJECT

    double currentValue;
    double prevValue;
    Operation operation;

    QLineEdit *screen;

    QPushButton *zero;
    QPushButton *one;
    QPushButton *two;
    QPushButton *three;
    QPushButton *four;
    QPushButton *five;
    QPushButton *six;
    QPushButton *seven;
    QPushButton *eight;
    QPushButton *nine;

    QPushButton *plus;
    QPushButton *minus;
    QPushButton *multiplication;
    QPushButton *division;
    QPushButton *modulo;

    QPushButton *dot;

    QPushButton *eq;
    QPushButton *reset;
    QPushButton *swap;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
