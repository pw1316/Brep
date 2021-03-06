######################################################################
# Automatically generated by qmake (2.01a) Thu Jan 24 14:47:50 2013
######################################################################

CONFIG(debug, debug|release) {
    DESTDIR=bin/debug
    OBJECTS_DIR=obj/debug
} else {
    DESTDIR=bin/release
    OBJECTS_DIR=obj/release
}
UI_DIR=temp
RCC_DIR=temp
MOC_DIR=temp
QMAKE_CXXFLAGS+=-std=c++0x
QT+=opengl
LIBS+=-lGLU
TEMPLATE = app
TARGET = 
DEPENDPATH += . src data/icon src/interface src/math
INCLUDEPATH += . src src/math src/interface

# Input
HEADERS += src/BoundingBox.h \
           src/Edge.h \
           src/Face.h \
           src/HalfEdge.h \
           src/Loop.h \
           src/Mesh.h \
           src/Scene.h \
           src/Vertex.h \
           src/interface/GLWidget.h \
           src/interface/mainwindow.h \
           src/interface/trackball.h \
           src/math/Quat.h \
           src/math/Vec3.h \
    src/interface/dialogcube.h \
    src/interface/dialogcorner.h \
    src/interface/dialogcylinder.h \
    src/interface/dialogsphere.h \
    src/interface/dialogsweeping.h \
    src/interface/dialogedit.h \
    src/interface/dialogrotate.h \
    src/bezier.h \
    src/math/Point3D.h \
    src/math/Point.h \
    src/interface/dialognorma.h
FORMS += src/interface/mainwindow.ui \
    src/interface/dialogcube.ui \
    src/interface/dialogcorner.ui \
    src/interface/dialogcylinder.ui \
    src/interface/dialogsphere.ui \
    src/interface/dialogsweeping.ui \
    src/interface/dialogedit.ui \
    src/interface/dialogrotate.ui \
    src/interface/dialognorma.ui
SOURCES += \
           src/Edge.cpp \
           src/Face.cpp \
           src/HalfEdge.cpp \
           src/Loop.cpp \
           src/Mesh.cpp \
           src/Scene.cpp \
           src/Vertex.cpp \
           src/interface/GLWidget.cpp \
           src/interface/main.cpp \
           src/interface/mainwindow.cpp \
           src/interface/trackball.cpp \
           src/math/Quat.cpp \
           src/math/Vec3.cpp \
    src/interface/dialogcube.cpp \
    src/interface/dialogcorner.cpp \
    src/interface/dialogcylinder.cpp \
    src/interface/dialogsphere.cpp \
    src/interface/dialogsweeping.cpp \
    src/interface/dialogedit.cpp \
    src/interface/dialogrotate.cpp \
    src/bezier.cpp \
    src/math/Point3D.cpp \
    src/math/Point.cpp \
    src/interface/dialognorma.cpp
RESOURCES += data/icon/icons.qrc
