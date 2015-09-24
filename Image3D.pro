
QT       += core gui opengl

TARGET = Image3D

CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH = include
INCLUDEPATH += src


SOURCES += src/main.cpp \
    src/Visu3D.cpp \
    src/Image3D.cpp


HEADERS += \
    include/Visu3D.h \
    include/Image3D.h

RESOURCES += \
    ressource.qrc


