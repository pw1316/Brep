#include "dialogcylinder.h"
#include "ui_dialogcylinder.h"

DialogCylinder::DialogCylinder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCylinder)
{
    ui->setupUi(this);
}

DialogCylinder::~DialogCylinder()
{
    delete ui;
}

double DialogCylinder::getX()
{
    return ui->doubleSpinBox_X->value();
}

double DialogCylinder::getY()
{
    return ui->doubleSpinBox_Y->value();
}

double DialogCylinder::getZ()
{
    return ui->doubleSpinBox_Z->value();
}

double DialogCylinder::getR()
{
    return ui->doubleSpinBox_R->value();
}

double DialogCylinder::getH()
{
    return ui->doubleSpinBox_H->value();
}

int DialogCylinder::getDisc()
{
    return ui->spinBox_Disc->value();
}
