#ifndef DIALOGCYLINDER_H
#define DIALOGCYLINDER_H

#include <QDialog>

namespace Ui {
class DialogCylinder;
}

class DialogCylinder : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogCylinder(QWidget *parent = 0);
    ~DialogCylinder();

    double getX();
    double getY();
    double getZ();
    double getR();
    double getH();
    int getDisc();
    
private:
    Ui::DialogCylinder *ui;
};

#endif // DIALOGCYLINDER_H
