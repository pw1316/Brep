#ifndef DIALOGROTATE_H
#define DIALOGROTATE_H

#include <QDialog>

namespace Ui {
class DialogRotate;
}

class DialogRotate : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogRotate(QWidget *parent = 0);
    ~DialogRotate();
    
    double getX();
    double getY();
    double getZ();
    int getAngle();

private:
    Ui::DialogRotate *ui;
};

#endif // DIALOGROTATE_H

