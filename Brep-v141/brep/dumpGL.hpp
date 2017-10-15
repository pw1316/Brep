//#pragma once
//
//#include <Windows.h>
//#include <gl/GL.h>
//#include <corecrt_math_defines.h>
//#include <set>
//
//
//__forceinline float degrees(float radians)
//{
//    return 180.0 * radians * M_1_PI;
//}
//
//__forceinline float radians(float degrees)
//{
//    return degrees * M_PI / 180.0;
//}
//
//enum Mode { EDIT, DRAW };
//
//class GLWidget
//{
//public:
//    GLWidget();
//    ~GLWidget();
//
//    void switchMode();
//    void setMeshSelect();
//    void setFaceSelect();
//    void setEdgeSelect();
//    void setVertexSelect();
//    void setStartLineLoop() { startLineLoop = !startLineLoop; }
//    void setStartBezier() { startBezier = !startBezier; }
//
//    void doneDrawing();
//
//    void sweep(float dX, float dY, float dZ);
//
//    int scene;//Scene
//    int drawScene;//Scene
//
//    int curve;//Bezier
//
//    // public slots:
//    void showGrid(bool);
//    void setPerspective(bool);
//
//protected:
//    void initializeGL();
//    void paintGL();
//    void resizeGL(int width, int height);
//
//    void mousePressEvent();
//    void mouseMoveEvent();
//    void mouseReleaseEvent();
//    void wheelEvent();
//    void keyPressEvent();
//
//    // protected Q_SLOTS:
//    void deleteMesh();
//    void move();
//    void moveNormal();
//    void neighborhood();
//    void rotate();
//    void scale();
//    void translate();
//    void inface();
//
//private:
//    void drawGrid();
//    void drawAxis(GLfloat *mat);
//    void drawBackground();
//    void drawEditBackground();
//    void shotPicking(int x, int y);
//    void loadProjection();
//
//    bool show_grid;
//    bool perspective;
//    bool selectionMode;
//
//
//    //Camera
//    float zoom_fat;
//    float pivot[3];
//    float zoomDepth;
//    float curquat[4];
//    float lastquat[4];
//    bool spinning, moving;
//    int beginx, beginy;
//    float clipClose;
//    float fovy;
//    float panFactor;
//
//    enum
//    {
//        SPIN_BUTTON = 0,//Left
//        PICK_BUTTON = 1//Right
//    };
//
//    Mode currentMode;
//    bool selectMesh, selectFace, selectEdge, selectVertex;
//    bool startLineLoop;
//    bool startBezier;
//    int numPts;
//
//    std::set<BSolid*> meshList;
//    std::set<BLoop*> faceList;
//    std::set<BEdge*> edgeList;
//    std::set<BVertex*> vertexList;
//
//    double curveP1[3];
//
//    void draw(int x, int y);
//};
