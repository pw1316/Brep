#include "dialognorma.h"
#include "ui_dialognorma.h"

DialogNorma::DialogNorma(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNorma)
{
    ui->setupUi(this);
}

DialogNorma::~DialogNorma()
{
    delete ui;
}

double DialogNorma::getSize()
{
    return ui->doubleSpinBox_Size->value();
}
