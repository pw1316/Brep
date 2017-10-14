#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_sweepingButton_released();
    void on_doneButton_released();
    void on_lineButton_released();
    void on_bezierButton_released();

    void zoomIn();
    void zoomOut();
    void zoom();
    void newModel();
    void vertexmode();
    void edgemode();
    void facemode();
    void meshmode();
    void r2d2();

    void on_tabWidget_currentChanged(int index);

    void on_cubeButton_released();

    void on_cornerButton_released();

    void on_cylinderButton_released();

    void on_sphereButton_released();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
