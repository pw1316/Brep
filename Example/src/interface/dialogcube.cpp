#include "dialogcube.h"
#include "ui_dialogcube.h"

DialogCube::DialogCube(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCube)
{
    ui->setupUi(this);
}

DialogCube::~DialogCube()
{
    delete ui;
}

double DialogCube::getX()
{
    return ui->doubleSpinBox_X->value();
}

double DialogCube::getY()
{
    return ui->doubleSpinBox_Y->value();
}

double DialogCube::getZ()
{
    return ui->doubleSpinBox_Z->value();
}

double DialogCube::getSize()
{
    return ui->doubleSpinBox_size->value();
}
