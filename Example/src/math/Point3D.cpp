#include "Point3D.h"
#include <GL/gl.h>

//#include "State.h"

Point3D::Point3D(double x, double y, double z) :
	Point(3)
{
    this->coords.resize(3, 0.0);

	this->setPosition(x, y, z);
}

Point3D::Point3D(const Point &p) :
	Point(3)
{
    this->coords.resize(3, 0.0);

	this->setPosition(p.getCoord(0), p.getCoord(1), p.getCoord(2));
}

Point3D::Point3D(const Point *p) :
	Point(3)
{
    this->coords.resize(3, 0.0);

	this->setPosition(p->getCoord(0), p->getCoord(1), p->getCoord(2));
}



Point3D::~Point3D()
{

}

void Point3D::setPosition(double x, double y, double z)
{
	this->setX(x);
	this->setY(y);
	this->setZ(z);
}

void Point3D::setX(double x)
{
	//this->setCoord(0, x);
	this->coords[0] = x;
}

double Point3D::getX() const
{
	//return this->getCoord(0);
	return this->coords[0];
}

void Point3D::setY(double y)
{
	//this->setCoord(1, y);
	this->coords[1] = y;
}

double Point3D::getY() const
{
	//return this->getCoord(1);
	return this->coords[1];
}

void Point3D::setZ(double z)
{
	//this->setCoord(2, z);
	this->coords[2] = z;
}

double Point3D::getZ() const
{
	//return this->getCoord(2);
	return this->coords[2];
}

unsigned int Point3D::dimension() const
{
	return 3;
}

bool Point3D::match(double x, double y, double z)
{
	return Point::match(Point3D(x, y, z));
}

bool Point3D::equal(double x, double y, double z)
{
	return Point::equal(Point3D(x, y, z));
}


void Point3D::draw() const
{
    glPointSize(size);

	glColor3f(r, g, b);

	bool lighting = false;

	if (glIsEnabled(GL_LIGHTING))
	{
		glDisable(GL_LIGHTING);

		lighting = true;
	}

    glBegin(GL_POINTS);
        glVertex3f(this->coords[0], this->coords[1], this->coords[2]);
    glEnd();

	if (lighting)
	{
		glEnable(GL_LIGHTING);
	}
}
/*
unsigned int Point3D::fill(GLdouble *coord, GLdouble *color, GLdouble *) const
{
	if (!getState().isInVisibleSpace(this))
	{
		return 0;
	}

	if (color)
	{
		color[0] = static_cast<GLdouble>(this->getR());
		color[1] = static_cast<GLdouble>(this->getG());
		color[2] = static_cast<GLdouble>(this->getB());
	}

	coord[0] = static_cast<GLdouble>(0.0);
	coord[1] = static_cast<GLdouble>(0.0);
	coord[2] = static_cast<GLdouble>(0.0);

	this->explodeAndShrink(coord, *this, true);

	return 1;
}
*/
