#ifndef MAP_H
#define MAP_H

#include <QEntity>
#include "terrain.h"

class MapTextureGenerator;

//namespace Entity {
//    class Terrain;
//}

namespace Entity {

class Map : public Qt3DCore::QEntity
{
    Q_OBJECT

public:
    explicit Map(Qt3DCore::QNode *parent = 0);
    ~Map();

private:
    Terrain *m_terrain;
};

}

#endif // MAP_H
