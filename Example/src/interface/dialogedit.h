#ifndef DIALOGEDIT_H
#define DIALOGEDIT_H

#include <QDialog>

namespace Ui {
class DialogEdit;
}

class DialogEdit : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogEdit(QWidget *parent = 0);
    ~DialogEdit();

    double getX();
    double getY();
    double getZ();
    
private:
    Ui::DialogEdit *ui;
};

#endif // DIALOGEDIT_H
