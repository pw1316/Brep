#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogcube.h"
#include "dialogcorner.h"
#include "dialogcylinder.h"
#include "dialogsphere.h"
#include "dialogsweeping.h"

#include "iostream"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    meshmode();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_sweepingButton_released()
{
    DialogSweeping sweeping(this);
    if(sweeping.exec())
       ui->glwidget->sweep( sweeping.getX(), sweeping.getY(), sweeping.getZ() );

    ui->glwidget->updateGL();
}

void MainWindow::on_doneButton_released()
{
    ui->glwidget->doneDrawing();
    ui->glwidget->updateGL();
}

void MainWindow::on_lineButton_released()
{
    ui->bezierButton->setChecked(false);
    ui->glwidget->setStartLineLoop();
    ui->glwidget->updateGL();
}

void MainWindow::on_bezierButton_released()
{
    ui->lineButton->setChecked(false);
    ui->glwidget->setStartBezier();
    ui->glwidget->updateGL();
}

void MainWindow::zoomIn()
{

}

void MainWindow::zoomOut()
{
}

void MainWindow::zoom()
{
}

void MainWindow::newModel()
{
    ui->glwidget->scene.clear();
    ui->glwidget->updateGL();
}

void MainWindow::vertexmode()
{
    ui->actionEdgeMode->setChecked(false);
    ui->actionFaceMode->setChecked(false);
    ui->actionMeshMode->setChecked(false);
    ui->glwidget->setVertexSelect();
}

void MainWindow::edgemode()
{
    ui->actionVertexMode->setChecked(false);
    ui->actionFaceMode->setChecked(false);
    ui->actionMeshMode->setChecked(false);
    ui->glwidget->setEdgeSelect();
}

void MainWindow::facemode()
{
    ui->actionVertexMode->setChecked(false);
    ui->actionEdgeMode->setChecked(false);
    ui->actionMeshMode->setChecked(false);
    ui->glwidget->setFaceSelect();
}

void MainWindow::meshmode()
{
    ui->actionVertexMode->setChecked(false);
    ui->actionEdgeMode->setChecked(false);
    ui->actionFaceMode->setChecked(false);
    ui->glwidget->setMeshSelect();
}

void MainWindow::r2d2()
{
    ui->glwidget->scene.addR2D2();
    ui->glwidget->updateGL();
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    ui->glwidget->switchMode();
    ui->glwidget->updateGL();
}

void MainWindow::on_cubeButton_released()
{
    DialogCube cube(this);
    if(cube.exec())
       ui->glwidget->scene.addCube( cube.getX(), cube.getY(), cube.getZ(), cube.getSize() );

    ui->glwidget->updateGL();
}

void MainWindow::on_cornerButton_released()
{
    DialogCorner corner(this);
    if(corner.exec())
       ui->glwidget->scene.addCorner( corner.getMinX(), corner.getMinY(), corner.getMinZ(),
                                      corner.getMidX(), corner.getMidY(), corner.getMidZ(),
                                      corner.getMaxX(), corner.getMaxY(), corner.getMaxZ() );

    ui->glwidget->updateGL();
}

void MainWindow::on_cylinderButton_released()
{
    DialogCylinder cylinder(this);
    if(cylinder.exec())
        ui->glwidget->scene.addCylinder(cylinder.getX(), cylinder.getY(), cylinder.getZ(),
                                        cylinder.getR(), cylinder.getH(), cylinder.getDisc() );

    ui->glwidget->updateGL();
}

void MainWindow::on_sphereButton_released()
{
    DialogSphere sphere(this);
    if(sphere.exec())
        ui->glwidget->scene.addSphere( sphere.getX(), sphere.getY(), sphere.getZ(), sphere.getR(), sphere.getDisc() );

    ui->glwidget->updateGL();
}
