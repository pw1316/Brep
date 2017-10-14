#include "dialogedit.h"
#include "ui_dialogedit.h"

DialogEdit::DialogEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEdit)
{
    ui->setupUi(this);
}

DialogEdit::~DialogEdit()
{
    delete ui;
}

double DialogEdit::getX()
{
    return ui->doubleSpinBox_X->value();
}

double DialogEdit::getY()
{
    return ui->doubleSpinBox_Y->value();
}

double DialogEdit::getZ()
{
    return ui->doubleSpinBox_Z->value();
}
