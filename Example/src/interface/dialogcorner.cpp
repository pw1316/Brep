#include "dialogcorner.h"
#include "ui_dialogcorner.h"

DialogCorner::DialogCorner(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCorner)
{
    ui->setupUi(this);
}

DialogCorner::~DialogCorner()
{
    delete ui;
}

double DialogCorner::getMinX()
{
    return ui->doubleSpinBox_minx->value();
}

double DialogCorner::getMinY()
{
    return ui->doubleSpinBox_miny->value();
}

double DialogCorner::getMinZ()
{
    return ui->doubleSpinBox_minz->value();
}

double DialogCorner::getMidX()
{
    return ui->doubleSpinBox_midx->value();
}

double DialogCorner::getMidY()
{
    return ui->doubleSpinBox_midy->value();
}

double DialogCorner::getMidZ()
{
    return ui->doubleSpinBox_midz->value();
}

double DialogCorner::getMaxX()
{
    return ui->doubleSpinBox_maxx->value();
}

double DialogCorner::getMaxY()
{
    return ui->doubleSpinBox_maxy->value();
}

double DialogCorner::getMaxZ()
{
    return ui->doubleSpinBox_maxz->value();
}
