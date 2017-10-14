#ifndef DIALOGNORMA_H
#define DIALOGNORMA_H

#include <QDialog>

namespace Ui {
class DialogNorma;
}

class DialogNorma : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogNorma(QWidget *parent = 0);
    ~DialogNorma();

    double getSize();
    
private:
    Ui::DialogNorma *ui;
};

#endif // DIALOGNORMA_H
