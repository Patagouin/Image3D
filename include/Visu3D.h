#ifndef VISU3D_H
#define VISU3D_H

#include <QtOpenGL>
#include "Image3D.h"


class Visu3D : public QGLWidget
{
    Q_OBJECT
public:
    // Passage des données par pointeur
    Visu3D(QVector<QVector3D> &_verticeData, QVector<int> &_greyData, int _lvlGreyMax, size_t sizeGrid[DIM], QWidget *parent = 0);
    ~Visu3D();


protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void makeLandMark(QMatrix4x4 mTransformation);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:

    QVector<QVector3D> verticesData;
    QVector<int> greyData;

    QGLBuffer verticesDataBuffer;
    QGLBuffer greyDataBuffer;


    QMatrix4x4 pMatrix;
    QGLShaderProgram shaderProgram;


    double lightAngle;
    double alpha;
    double beta;
    double distance;
    int lvlOfGreyMax;
    QPoint lastMousePosition;


};

#endif // VISU3D_H
