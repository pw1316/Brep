#ifndef DIALOGSPHERE_H
#define DIALOGSPHERE_H

#include <QDialog>

namespace Ui {
class DialogSphere;
}

class DialogSphere : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogSphere(QWidget *parent = 0);
    ~DialogSphere();

    double getX();
    double getY();
    double getZ();
    double getR();
    int getDisc();
private:
    Ui::DialogSphere *ui;
};

#endif // DIALOGSPHERE_H
