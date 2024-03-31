#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSignalMapper>
#include <cmath>

enum Operation
{
    NONE,
    PLUS,
    MINUS,
    MUL,
    DIV,
    MODULO
};

class MainWindow : public QWidget
{
    Q_OBJECT

    QString currentValue;
    QString prevValue;
    bool negative = false;
    Operation operation = Operation::NONE;

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

    void setDisplay();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void addNumber(QString n);
    void setOperation(QString o);
    void clear();
    void toggleNegative();
    void solve();
};
#endif // MAINWINDOW_H
