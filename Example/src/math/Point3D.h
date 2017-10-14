#ifndef _DATA_POINT3D_H_
#define _DATA_POINT3D_H_

#include "Point.h"

class Point3D : public Point
{
public:

    Point3D(double x = 0.0, double y = 0.0, double z = 0.0);
    Point3D(const Point &p);
    Point3D(const Point *p);
    virtual ~Point3D();

    virtual void setPosition(double x, double y, double z);

    virtual void setX(double x);
    virtual double getX() const;

    virtual void setY(double y);
    virtual double getY() const;

    virtual void setZ(double z);
    virtual double getZ() const;

    virtual unsigned int dimension() const;

    using Point::match;
    virtual bool match(double x, double y, double z);

    using Point::equal;
    virtual bool equal(double x, double y, double z);

#if USE_ARRAY
    using Point::operator=;
#endif //#if USE_ARRAY


    using Point::draw;
    virtual void draw() const;

    //virtual  unsigned int fill(GLdouble *coord = NULL, GLdouble *color = NULL, GLdouble *normal = NULL) const;
};


#endif //#ifndef _DATA_POINT3D_H_
