#include "c04.h"

C04::C04(QWidget *parent)
    : QMainWindow(parent)
{
    QGridLayout* layout = new QGridLayout(this);

    screen = new QLineEdit();
    layout->addWidget(screen, 1, 1, 1, 4);


    setLayout(layout);
}

C04::~C04()
{

}
