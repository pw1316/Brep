#include "dialogrotate.h"
#include "ui_dialogrotate.h"

DialogRotate::DialogRotate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogRotate)
{
    ui->setupUi(this);
}

DialogRotate::~DialogRotate()
{
    delete ui;
}

double DialogRotate::getX()
{
    return ui->doubleSpinBox_X->value();
}

double DialogRotate::getY()
{
    return ui->doubleSpinBox_Y->value();
}

double DialogRotate::getZ()
{
    return ui->doubleSpinBox_Z->value();
}

int DialogRotate::getAngle()
{
    return ui->spinBox_Angle->value();
}
