#include "Image3D.h"

Image3D::Image3D(const char *in_file): in_name_file(in_file)
{
    readFile(in_file);
    build();
}
QVector<QVector3D> Image3D::getVerticesData() const
{
    return verticesData;
}

void Image3D::setVerticesData(const QVector<QVector3D> &value)
{
    verticesData = value;
}



void Image3D::makeQuad(coord3D posA, coord3D posB, coord3D posC, coord3D posD)
{
    verticesData << posA << posD << posB; // Premier triangle, sens anti-horaire
    verticesData << posA << posC << posD;

}

void Image3D::makeCube(coord3D position)
{
    //     E-----F
    //    /|    /|
    //   A-----B |
    //   | G --|-H
    //   |/    |/
    //   C-----D

    QVector3D a =  QVector3D(position.x-SIZEVOXEL_X/2, position.y+SIZEVOXEL_Y/2, position.z-SIZEVOXEL_Z/2);
    QVector3D b =  QVector3D(position.x+SIZEVOXEL_X/2, position.y+SIZEVOXEL_Y/2, position.z-SIZEVOXEL_Z/2);
    QVector3D c =  QVector3D(position.x-SIZEVOXEL_X/2, position.y-SIZEVOXEL_Y/2, position.z-SIZEVOXEL_Z/2);
    QVector3D d =  QVector3D(position.x+SIZEVOXEL_X/2, position.y-SIZEVOXEL_Y/2, position.z-SIZEVOXEL_Z/2);
    QVector3D e =  QVector3D(position.x-SIZEVOXEL_X/2, position.y+SIZEVOXEL_Y/2, position.z+SIZEVOXEL_Z/2);
    QVector3D f =  QVector3D(position.x+SIZEVOXEL_X/2, position.y+SIZEVOXEL_Y/2, position.z+SIZEVOXEL_Z/2);
    QVector3D g =  QVector3D(position.x-SIZEVOXEL_X/2, position.y-SIZEVOXEL_Y/2, position.z+SIZEVOXEL_Z/2);
    QVector3D h =  QVector3D(position.x+SIZEVOXEL_X/2, position.y-SIZEVOXEL_Y/2, position.z+SIZEVOXEL_Z/2);

    makeQuad(a,b,c,d); // Front
    makeQuad(e,a,g,c); // Left
    makeQuad(b,f,d,h);  // Right
    makeQuad(f,e,h,g);  // Back
    makeQuad(e,f,a,b);  // Top
    makeQuad(c,d,g,h);  // BOTTOM

}




void Image3D::build()
{
    coord3D halfSizeGrid;
    halfSizeGrid.x = (int)sizeGrid.x/2;
    halfSizeGrid.y = (int)sizeGrid.y/2;
    halfSizeGrid.z = (int)sizeGrid.z/2;

    coord3D centreCube;

    for (int i = 0; i < sizeGrid.z; ++i){
        for (int j = 0; j < sizeGrid.y; ++j){
            for (int k = 0; k < sizeGrid.x; ++k){
                if (greyData->at(i*sizeGrid.y*sizeGrid.x + j*sizeGrid.x + k) != 0){
                    centreCube.x = (k*SIZEVOXEL_X) - halfSizeGrid.x;
                    centreCube.y = (j*SIZEVOXEL_Y) - halfSizeGrid.y,
                    centreCube.z = (i*SIZEVOXEL_Z) - halfSizeGrid.z,

                    makeCube(centreCube);
                }
            }
        }
    }
}



void Image3D::readFile(const char *in)
{
    inStream.open(in_file);

    string str;
    inStream >> str;
    if (str != "PGM3D"){
        cerr << "Mauvais format de fichier.\n";
        exit(EXIT_FAILURE);
    }

    // Passer les commentaires
    getline(inStream, str);
    while (str.size() > 0 && str.at(0) == '#'){ // Simple skip des commentaires situe apres le magic number
        getline(inStream, str);
    }
    str.clear();

    // Recuperation des tailles
    for (unsigned int i = 0; i < DIM; ++i){
        inStream >> str;
        sizeGrid[i] = atoi(str.c_str());
    }
    str.clear();

    // Nb lvl of grey
    inStream >> str;
    nbGrey = atoi(str.c_str());
    str.clear();

    unsigned int nbElements =  sizeGrid[0] * sizeGrid[1] * sizeGrid[2];

    int i = 0;
    greyData.resize(nbElements);
    while (nbElements-i > 0 ){
        greyData[i++] = atoi(str.c_str());
    }

    inStream.close();
}




void Image3D::getSize(size_t _size[DIM])
{
    _size[0] = sizeGrid[0];
    _size[1] = sizeGrid[1];
    _size[2] = sizeGrid[2];

}

unsigned int Image3D::getNbGrey() const
{
    return nbGrey;
}


