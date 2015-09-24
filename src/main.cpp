#include <QApplication>
#include "Image3D.h"
#include "Visu3D.h"

int main(int argc, char **argv)
{
    // TODO: S'occuper du nombre de pointeur pointant vers le tableau de niveau de gris
    QApplication a(argc, argv);

    char str1 = 'a';
    char str2 = 'd';
    char str3 = str1 | str2;


    const char* in = "test.pgm3d";
    Image3D image3D(in);


    // Recuperation des parametres pour ne pas avoir à relir le fichier avant l'affichage opengl
    size_t size[DIM];
    image3D.getSize(size);

    int nbLvlGrey;
    nbLvlGrey = image3D.getNbGrey();

    QVector<QVector3D> verticeData = image3D.verticesData;
    QVector<int> greyData = image3D.greyData;
    Visu3D *visualisation = new Visu3D(verticeData, greyData);

    visualisation->show();

    return a.exec();
}
