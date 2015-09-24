#include "Visu3D.h"


Visu3D::Visu3D(QVector<QVector3D> &_verticeData, QVector<int> &_greyData, QWidget *parent)
    : QGLWidget(QGLFormat(), parent), verticesData(_verticeData), greyData(_greyData)
{
    alpha = 25;
    beta = -25;
    distance = 1.5 * sizeGrid.z;

    updateGL();
}


Visu3D::~Visu3D()
{

}



void Visu3D::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    qglClearColor(QColor(Qt::black));


    shaderProgram.addShaderFromSourceFile(QGLShader::Vertex, ":/VertexShader.vsh");
    shaderProgram.addShaderFromSourceFile(QGLShader::Fragment, ":/FragmentShader.fsh");
    shaderProgram.link();


    verticesDataBuffer.create();
    verticesDataBuffer.bind();
    verticesDataBuffer.allocate(verticesData.size() * 3 * sizeof(GLfloat));
    verticesDataBuffer.write(0, verticesData.constData(), verticesData.size() * 3 * sizeof(GLfloat));
    verticesDataBuffer.release();

    greyDataBuffer.create();
    greyDataBuffer.bind();
    greyDataBuffer.allocate(greyData.size() * 1 * sizeof(GLfloat)); // 1 float par sommet de triangle
    greyDataBuffer.write(0, greyData.constData(), greyData.size() * 1 * sizeof(GLfloat));
    greyDataBuffer.release();

}

void Visu3D::resizeGL(int width, int height)
{
    if (height == 0) {
        height = 1;
    }

    pMatrix.setToIdentity();
    pMatrix.perspective(60.0, (float) width / (float) height, 0.001, 1000);
    glViewport(0, 0, width, height);
}

void Visu3D::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    QMatrix4x4 mMatrix;
    QMatrix4x4 vMatrix;

    QMatrix4x4 cameraTransformation;
    cameraTransformation.rotate(alpha, 0, 1, 0);
    cameraTransformation.rotate(beta, 1, 0, 0);

    QVector3D cameraPosition = cameraTransformation * QVector3D(0, 0, distance);
    QVector3D cameraUpDirection = cameraTransformation * QVector3D(0, 1, 0);

    vMatrix.lookAt(cameraPosition, QVector3D(0, 0, 0), cameraUpDirection);

    mMatrix.setToIdentity();
    QMatrix4x4 mvMatrix;
    mvMatrix = vMatrix * mMatrix;

    shaderProgram.bind();

    shaderProgram.setUniformValue("mvpMatrix", pMatrix * mvMatrix); // modele-vue-projection
    shaderProgram.setUniformValue("greyMax", (float) lvlOfGreyMax);

    verticesDataBuffer.bind();
    shaderProgram.setAttributeBuffer("vertex", GL_FLOAT, 0, 3, 0);
    shaderProgram.enableAttributeArray("vertex");
    verticesDataBuffer.release();

    greyDataBuffer.bind();
    shaderProgram.setAttributeBuffer("inGrey", GL_FLOAT, 0, 1, 0);
    shaderProgram.enableAttributeArray("inGrey");
    greyDataBuffer.release();

    glDrawArrays(GL_TRIANGLES, 0, verticesData.size());

    shaderProgram.disableAttributeArray("vertex");
    shaderProgram.disableAttributeArray("inGrey");
    shaderProgram.release();

    glClear(GL_DEPTH_BUFFER_BIT); // On annule le test de profondeur afin
                                  //que le repere soit devant
    QMatrix4x4 mvpMatrix = pMatrix * mvMatrix; // Matrice de transfo pour que le repere tourne
                                               //en meme temps que la scene

    makeLandMark(mvpMatrix);

}
void Visu3D::makeLandMark(QMatrix4x4 mTransformation){

    // repere
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(mTransformation.constData());
    glLineWidth(2.5);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(1, 0, 0);
    glEnd();

    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0, 1, 0);
    glEnd();

    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0, 0, 1);
    glEnd();
}


/// Fonction pour la rotation
void Visu3D::mousePressEvent(QMouseEvent *event)
{
    lastMousePosition = event->pos(); // On sauvegarde les coords pour
                                      // calculer le delta de deplacement
    event->accept(); // L'evenement ne pourra pas declencher d'autre evenement
}

/// Fonction pour la rotation
void Visu3D::mouseMoveEvent(QMouseEvent *event)
{
    int deltaX = event->x() - lastMousePosition.x();
    int deltaY = event->y() - lastMousePosition.y();

    if (event->buttons() & Qt::LeftButton) {
        alpha -= deltaX;
        while (alpha < 0) { // on garde l'angle alpha entre 0 et 360
            alpha += 360;

        }
        while (alpha >= 360) {
            alpha -= 360;
        }

        beta -= deltaY;
        if (beta < -90) { // On bloque la vue afin qu'il n'y est
            beta = -90;   // pas d'inversion des commandes (ex: la rotation horizontale serait inverse)
        }                 // et permet aussi de pas inverser le "top" de la camera
        if (beta > 90) {
            beta = 90;
        }
    }

    lastMousePosition = event->pos();
    event->accept();
    updateGL();  // mise a jour de la scene

}
/// Fonction pour le zoom/dezoom
void Visu3D::wheelEvent(QWheelEvent *event)
{
    int delta = event->delta();

    if (event->orientation() == Qt::Vertical) {
        if (delta < 0) {
            distance *= 1.1;
        } else if (delta > 0) {
            distance *= 0.9;
        }
    }

    event->accept();
    updateGL(); // mise a jour de la scene

}


