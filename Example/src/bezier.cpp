#include "bezier.h"
#include "cmath"
#include <GL/glu.h>

Bezier::Bezier()
{
}

Bezier::Bezier(double x1, double y1, double z1, double x2, double y2, double z2)
{
    Point3D p1 = Point3D(x1, y1, z1);
    Point3D p4 = Point3D(x2, y2, z2);

    Point3D p2 = Point3D( (x1+x2)/4.0, (y1+y2)/4.0, (z1+z2)/4.0);
    Point3D p3 = Point3D( (3.0*(x1+x2))/4.0, (3.0*(y1+y2))/4.0, (3.0*(z1+z2))/4.0);

    point_vector.push_back(p1);
    point_vector.push_back(p2);
    point_vector.push_back(p3);
    point_vector.push_back(p4);

    order = 3;
}

Bezier::Bezier(Point3D p2, Point3D p3)
{
    Point3D p1 = Point3D(-20.0, 0.0, 0.0);

    point_vector.push_back(p1);

    point_vector.push_back(p2);
    point_vector.push_back(p3);

    Point3D p4 = Point3D(20.0, 0.0, 0.0);

    point_vector.push_back(p4);

    order = 3;
}

Bezier::Bezier(Point3D p1, Point3D p2, Point3D p3, Point3D p4)
{
    point_vector.push_back(p1);
    point_vector.push_back(p2);
    point_vector.push_back(p3);
    point_vector.push_back(p4);

    order = 3;
}

void Bezier::insertPoint(double x, double y, double z)
{
    std::vector<Point3D> points;

    bool inserted = false;

    for(int i=0; i<=order; i++)
    {
        if(!inserted)
        {
            if( x < point_vector[i].getX() )
            {
                points.push_back( Point3D(x, y, z) );
                points.push_back(point_vector[i]);
                inserted = true;
            }
            else
                points.push_back(point_vector[i]);
        }
        else
            points.push_back(point_vector[i]);
    }

    if(inserted == false)
        points.push_back( Point3D(x, y, z) );

    order++;

    point_vector = points;
}

static int fat(int x)
{
    if(x==0) return 1;

    int value = x;

    for(int i=1; i<x; i++)
    {
        value *= i;
    }

    return value;
}

Point3D Bezier::calculeP(double u)
{
    std::vector<Point3D> points = point_vector;

    for(int i=0; i <=order; i++)
    {
        double x = (fat(order) / ( fat(i) * fat(order-i) )) * pow(u,i) * pow(1.0-u, order-i);

        points[i].esc(x);
    }

    for(int x=1; x<=order; x++)
    {
        points[x].sum(points[x-1]);
    }

    return points[order];
}

void Bezier::draw()
{
    if(point_vector.size() == 0)
        return;

    glColor3f(0, 1, 0);

    glPointSize(4);
    glBegin(GL_POINTS);
    for(int x=0; x<=order; x++)
    {
        glVertex3f(point_vector[x].getX(), point_vector[x].getY(), point_vector[x].getZ());
    }
    glEnd();

    glLineWidth(1.0);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(4, 0x5555);
    glColor4f(0, 1, 0, 0.4);
    glBegin(GL_LINES);
    for(int x=1; x<=order; x++)
    {
        glVertex3f(point_vector[x].getX(), point_vector[x].getY(), point_vector[x].getZ());
        glVertex3f(point_vector[x-1].getX(), point_vector[x-1].getY(), point_vector[x-1].getZ());
    }
    glEnd();
    glDisable(GL_LINE_STIPPLE);

    Point3D p, p2;
    double v = 0.05;
    for(double u = 0.0; u < 1.0; u += v)
    {
        p = calculeP(u);
        p2 = calculeP(u+v);

        glColor3f(1, 1, 1);
        glLineWidth(2.5);

        glBegin(GL_LINES);
            glVertex3f(p.getX(), p.getY(), p.getZ());
            glVertex3f(p2.getX(), p2.getY(), p2.getZ());
        glEnd();
    }

}
