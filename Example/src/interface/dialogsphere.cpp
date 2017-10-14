#include "dialogsphere.h"
#include "ui_dialogsphere.h"

DialogSphere::DialogSphere(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSphere)
{
    ui->setupUi(this);
}

DialogSphere::~DialogSphere()
{
    delete ui;
}

double DialogSphere::getX()
{
    return ui->doubleSpinBox_X->value();
}

double DialogSphere::getY()
{
    return ui->doubleSpinBox_Y->value();
}

double DialogSphere::getZ()
{
    return ui->doubleSpinBox_Z->value();
}

double DialogSphere::getR()
{
    return ui->doubleSpinBox_R->value();
}

int DialogSphere::getDisc()
{
    return ui->spinBox_Disc->value();
}
