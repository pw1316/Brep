#ifndef DIALOGSWEEPING_H
#define DIALOGSWEEPING_H

#include <QDialog>

namespace Ui {
class DialogSweeping;
}

class DialogSweeping : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogSweeping(QWidget *parent = 0);
    ~DialogSweeping();

    double getX();
    double getY();
    double getZ();
private:
    Ui::DialogSweeping *ui;
};

#endif // DIALOGSWEEPING_H
