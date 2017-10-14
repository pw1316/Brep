#ifndef DIALOGCORNER_H
#define DIALOGCORNER_H

#include <QDialog>

namespace Ui {
class DialogCorner;
}

class DialogCorner : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogCorner(QWidget *parent = 0);
    ~DialogCorner();
    
    double getMinX();
    double getMinY();
    double getMinZ();
    double getMidX();
    double getMidY();
    double getMidZ();
    double getMaxX();
    double getMaxY();
    double getMaxZ();
private:
    Ui::DialogCorner *ui;
};

#endif // DIALOGCORNER_H
