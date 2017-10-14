#ifndef DIALOGCUBE_H
#define DIALOGCUBE_H

#include <QDialog>

namespace Ui {
class DialogCube;
}

class DialogCube : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogCube(QWidget *parent = 0);
    ~DialogCube();

    double getX();
    double getY();
    double getZ();
    double getSize();

private:
    Ui::DialogCube *ui;
};

#endif // DIALOGCUBE_H
