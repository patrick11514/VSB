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
                        "    font-weight: bold;"
                        "}"
                        "QPushButton#light {"
                        "    background: #ccc4c4;"
                        "    color: black;"
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

    // signal mappers
    QSignalMapper *signalMapper = new QSignalMapper(this);
    QSignalMapper *signalMapperOp = new QSignalMapper(this);

    // numbers
    zero = new QPushButton("0");
    layout->addWidget(zero, 6, 1, 1, 2);
    connect(zero, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(zero, "0");

    one = new QPushButton("1");
    layout->addWidget(one, 5, 1);
    connect(one, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(one, "1");

    two = new QPushButton("2");
    layout->addWidget(two, 5, 2);
    connect(two, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(two, "2");

    three = new QPushButton("3");
    layout->addWidget(three, 5, 3);
    connect(three, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(three, "3");

    four = new QPushButton("4");
    layout->addWidget(four, 4, 1);
    connect(four, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(four, "4");

    five = new QPushButton("5");
    layout->addWidget(five, 4, 2);
    connect(five, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(five, "5");

    six = new QPushButton("6");
    layout->addWidget(six, 4, 3);
    connect(six, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(six, "6");

    seven = new QPushButton("7");
    layout->addWidget(seven, 3, 1);
    connect(seven, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(seven, "7");

    eight = new QPushButton("8");
    layout->addWidget(eight, 3, 2);
    connect(eight, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(eight, "8");

    nine = new QPushButton("9");
    layout->addWidget(nine, 3, 3);
    connect(nine, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(nine, "9");

    // modifiers
    reset = new QPushButton("AC");
    reset->setObjectName("light");
    layout->addWidget(reset, 2, 1);
    connect(reset, SIGNAL(clicked()), this, SLOT(clear()));

    swap = new QPushButton("+/-");
    swap->setObjectName("light");
    layout->addWidget(swap, 2, 2);
    connect(swap, SIGNAL(clicked()), this, SLOT(toggleNegative()));

    modulo = new QPushButton("%");
    modulo->setObjectName("light");
    layout->addWidget(modulo, 2, 3);
    connect(modulo, SIGNAL(clicked()), signalMapperOp, SLOT(map()));
    signalMapperOp->setMapping(modulo, "%");

    dot = new QPushButton(".");
    layout->addWidget(dot, 6, 3);
    connect(dot, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(dot, ",");

    // symbols

    plus = new QPushButton("+");
    plus->setObjectName("orange");
    layout->addWidget(plus, 2, 4);
    connect(plus, SIGNAL(clicked()), signalMapperOp, SLOT(map()));
    signalMapperOp->setMapping(plus, "+");

    minus = new QPushButton("-");
    minus->setObjectName("orange");
    layout->addWidget(minus, 3, 4);
    connect(minus, SIGNAL(clicked()), signalMapperOp, SLOT(map()));
    signalMapperOp->setMapping(minus, "-");

    multiplication = new QPushButton("*");
    multiplication->setObjectName("orange");
    layout->addWidget(multiplication, 4, 4);
    connect(multiplication, SIGNAL(clicked()), signalMapperOp, SLOT(map()));
    signalMapperOp->setMapping(multiplication, "*");

    division = new QPushButton("/");
    division->setObjectName("orange");
    layout->addWidget(division, 5, 4);
    connect(division, SIGNAL(clicked()), signalMapperOp, SLOT(map()));
    signalMapperOp->setMapping(division, "/");

    eq = new QPushButton("=");
    layout->addWidget(eq, 6, 4);
    connect(eq, SIGNAL(clicked()), this, SLOT(solve()));

    connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(addNumber(QString)));
    connect(signalMapperOp, SIGNAL(mapped(QString)), this, SLOT(setOperation(QString)));

    setLayout(layout);
}

MainWindow::~MainWindow() {}

void MainWindow::setDisplay()
{
    QString str;
    if (this->negative)
    {
        str += "-";
    }
    str += this->currentValue;
    this->screen->setText(str);
}

void MainWindow::addNumber(QString n)
{
    this->currentValue += n;
    this->setDisplay();
}

void MainWindow::setOperation(QString o)
{
    if (this->operation != Operation::NONE)
    {
        this->solve();
    }

    if (o == "+")
    {
        this->operation = Operation::PLUS;
    }
    else if (o == "-")
    {
        this->operation = Operation::MINUS;
    }
    else if (o == "*")
    {
        this->operation = Operation::MUL;
    }
    else if (o == "/")
    {
        this->operation = Operation::DIV;
    }
    else if (o == "%")
    {
        this->operation = Operation::MODULO;
    }
    else
    {
        return;
    }

    this->prevValue = this->currentValue;
    this->currentValue = "";
    this->negative = false;
    this->setDisplay();
}

void MainWindow::clear()
{
    this->currentValue = "";
    this->setDisplay();
}

void MainWindow::toggleNegative()
{
    this->negative = !this->negative;
    this->setDisplay();
}

void MainWindow::solve()
{
    if (this->operation == Operation::NONE)
    {
        return;
    }

    QString first = this->prevValue;
    if (first.size() == 0)
    {
        first = "0";
    }

    QString second = this->currentValue;
    if (second.size() == 0)
    {
        second = "0";
    }

    try
    {
        double firstNumber = std::stod(first.toStdString());
        double secondNumber = std::stod(second.toStdString());
        double result;

        switch (this->operation)
        {
        case Operation::PLUS:
            result = firstNumber + secondNumber;
            break;
        case Operation::MINUS:
            result = firstNumber - secondNumber;
            break;
        case Operation::MUL:
            result = firstNumber * secondNumber;
            break;
        case Operation::DIV:
            result = firstNumber / secondNumber;
            break;
        case Operation::MODULO:
            result = std::fmod(firstNumber, secondNumber);
            break;
        }

        // round to 2 decimal numbers
        std::string resultString = std::to_string(std::round(result * 100.0) / 100.0);
        this->currentValue = QString::fromStdString(resultString);
        // remove last zeroes from number
        while (true)
        {
            auto end = this->currentValue.end() - 1;
            auto value = *end;

            // if zero, remove and continue
            if (value == '0')
            {
                this->currentValue.chop(1);
                continue;
            }

            // if decimal point, remove and stop
            if (value == ',')
            {
                this->currentValue.chop(1);
                break;
            }

            // other number/symbol stop
            break;
        }
    }
    catch (std::exception &)
    {
        this->currentValue = "ERROR";
    }

    this->operation = Operation::NONE;
    this->setDisplay();
}
