#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    QGridLayout *layout = new QGridLayout(this);
    layout->setSpacing(0);
    this->setStyleSheet("QPushButton {"
                        "    background: #5c5a5a;"
                        "    border-width: 1px;"
                        "    border-color: black;"
                        "    border-style: solid;"
                        "    border-radius: 0;"
                        "    width: 80px;"
                        "    height: 80px;"
                        "    padding: 0;"
                        "}"
                        "QPushButton#light {"
                        "    background: #ccc4c4;"
                        "}"
                        "QPushButton#orange {"
                        "    background: #eb9215;"
                        "}"
                        "QLineEdit {"
                        "    font-size: 40px;"
                        "    background: #292827;"
                        "    outline: 0"
                        "}");

    screen = new QLineEdit();
    screen->setReadOnly(true);
    layout->addWidget(screen, 1, 1, 1, 4);

    // numbers
    zero = new QPushButton("0");
    layout->addWidget(zero, 6, 1, 1, 2);

    one = new QPushButton("1");
    layout->addWidget(one, 5, 1);

    two = new QPushButton("2");
    layout->addWidget(two, 5, 2);

    three = new QPushButton("3");
    layout->addWidget(three, 5, 3);

    four = new QPushButton("4");
    layout->addWidget(four, 4, 1);

    five = new QPushButton("5");
    layout->addWidget(five, 4, 2);

    six = new QPushButton("6");
    layout->addWidget(six, 4, 3);

    seven = new QPushButton("7");
    layout->addWidget(seven, 3, 1);

    eight = new QPushButton("8");
    layout->addWidget(eight, 3, 2);

    nine = new QPushButton("9");
    layout->addWidget(nine, 3, 3);

    // modifiers
    reset = new QPushButton("AC");
    reset->setObjectName("light");
    layout->addWidget(reset, 2, 1);

    swap = new QPushButton("+/-");
    swap->setObjectName("light");
    layout->addWidget(swap, 2, 2);

    modulo = new QPushButton("%");
    modulo->setObjectName("light");
    layout->addWidget(modulo, 2, 3);

    dot = new QPushButton(".");
    layout->addWidget(dot, 6, 3);

    // symbols
    plus = new QPushButton("+");
    plus->setObjectName("orange");
    layout->addWidget(plus, 2, 4);

    minus = new QPushButton("-");
    minus->setObjectName("orange");
    layout->addWidget(minus, 3, 4);

    multiplication = new QPushButton("*");
    multiplication->setObjectName("orange");
    layout->addWidget(multiplication, 4, 4);

    division = new QPushButton("/");
    division->setObjectName("orange");
    layout->addWidget(division, 5, 4);

    eq = new QPushButton("=");
    layout->addWidget(eq, 6, 4);

    setLayout(layout);
}

MainWindow::~MainWindow() {}
