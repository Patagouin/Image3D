#ifndef IMAGE3D_H
#define IMAGE3D_H

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <QVector>
#include <QVector3D>

static const short DIM = 3;
static const int SCREEN_SIZE_X = 800;
static const int SCREEN_SIZE_Y = 600;

static const float SIZEVOXEL_X = 1;
static const float SIZEVOXEL_Y = 1;
static const float SIZEVOXEL_Z = 1;

using namespace std;

struct coord3D{
    int x;
    int y;
    int z;
};

class Image3D
{
public:
    Image3D(const char* in);

    coord3D sizeGrid; // Size of the voxel grid

    QVector<int> greyData; // grey level array
    unsigned int nbGrey;

    QVector<QVector3D> verticesData;

    void makeQuad(coord3D posA, coord3D posB, coord3D posC, coord3D posD); // (floats)
    void makeCube(coord3D position); // centre du cube (des ints)

    unsigned int getNbGrey() const;
    void getSize(size_t _size[DIM]);

    void readFile(const char* in);

    void build();


    void setVerticesData(const QVector<QVector3D> &value);

protected:
    ifstream inStream;

    string in_name_file;
};

#endif // IMAGE3D_H
