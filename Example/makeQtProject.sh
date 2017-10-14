#!/bin/bash

qmake -project -r "CONFIG(debug, debug|release) {
    DESTDIR=bin/debug
    OBJECTS_DIR=obj/debug
} else {
    DESTDIR=bin/release
    OBJECTS_DIR=obj/release
}" UI_DIR=temp RCC_DIR=temp MOC_DIR=temp QMAKE_CXXFLAGS+=-std=c++0x QT+=opengl LIBS+=-lGLU
qmake *.pro
