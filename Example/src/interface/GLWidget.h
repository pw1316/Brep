#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtOpenGL>
#include "Scene.h"
#include "bezier.h"
#include <set>


inline float degrees(float radians)
{ return 180.0 * radians * M_1_PI; }

inline float radians(float degrees)
{ return degrees * M_PI/180.0; }

enum Mode {EDIT, DRAW};
class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    void switchMode();
    void setMeshSelect();
    void setFaceSelect();
    void setEdgeSelect();
    void setVertexSelect();
    void setStartLineLoop() {startLineLoop=!startLineLoop;}
    void setStartBezier() {startBezier=!startBezier;}

    void doneDrawing();

    void sweep(float dX, float dY, float dZ);

    Scene scene;
    Scene drawScene;

    Bezier curve;

public slots:
    void showGrid(bool);
    void setPerspective(bool);

protected:
    void initializeGL();
	void paintGL();
    void resizeGL(int width, int height);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent * event);
    void wheelEvent(QWheelEvent* event);
    void keyPressEvent(QKeyEvent *);

protected Q_SLOTS:
    void deleteMesh();
    void move();
    void moveNormal();
    void neighborhood();
    void rotate();
    void scale();
    void translate();
    void inface();

private:
	void drawGrid();
	void drawAxis(GLfloat *mat);
	void drawBackground();
    void drawEditBackground();
    void shotPicking(int x, int y);
	void loadProjection();

	bool show_grid;
	bool perspective;
	bool selectionMode;


    //Camera
    float zoom_fat;
    float pivot[3];
    float zoomDepth;
    float curquat[4];
    float lastquat[4];
    bool spinning, moving;
    int beginx, beginy;
	float clipClose;
    float fovy;
    float panFactor;

    enum
    {
		SPIN_BUTTON = Qt::LeftButton,
		PICK_BUTTON = Qt::RightButton
	};

    Mode currentMode;
    bool selectMesh, selectFace, selectEdge, selectVertex;
    bool startLineLoop;
    bool startBezier;
    int numPts;

    std::set<Mesh*> meshList;
    std::set<Loop*> faceList;
    std::set<Edge*> edgeList;
    std::set<Vertex*> vertexList;

    double curveP1[3];

    void draw(int x, int y);
};

#endif
