#ifndef C04_H
#define C04_H

#include <QMainWindow>
#include <QPushButton>
#include <QGridLayout>
#include <QLineEdit>

class C04 : public QMainWindow
{
    Q_OBJECT
    QLineEdit *screen;

public:
    C04(QWidget *parent = nullptr);
    ~C04();
};
#endif // C04_H
