#include "dialogsweeping.h"
#include "ui_dialogsweeping.h"

DialogSweeping::DialogSweeping(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSweeping)
{
    ui->setupUi(this);
}

DialogSweeping::~DialogSweeping()
{
    delete ui;
}

double DialogSweeping::getX()
{
    return ui->doubleSpinBox_X->value();
}

double DialogSweeping::getY()
{
    return ui->doubleSpinBox_Y->value();
}

double DialogSweeping::getZ()
{
    return ui->doubleSpinBox_Z->value();
}
