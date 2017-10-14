#ifndef BEZIER_H
#define BEZIER_H

#include "Point3D.h"
#include <vector>

class Bezier
{
public:
    Bezier();
    Bezier(double x1, double y1, double z1, double x2, double y2, double z2);
    Bezier(Point3D p2, Point3D p3);
    Bezier(Point3D p1, Point3D p2, Point3D p3, Point3D p4);

    void insertPoint(double x, double y, double z);

    void setP2(double x, double y, double z) {point_vector[1].setPosition(x, y, z);}
    void setP3(double x, double y, double z) {point_vector[2].setPosition(x, y, z);}

    void setP2(Point3D p) {point_vector[1] = p;}
    void setP3(Point3D p) {point_vector[2] = p;}

    Point3D getP2() {return point_vector[1];}
    Point3D getP3() {return point_vector[2];}

    std::vector<Point3D>& getPoints() {return point_vector;}

    Point3D calculeP(double u);

    void draw();

private:
    std::vector<Point3D> point_vector;

    int order;
};


#endif // BEZIER_H
