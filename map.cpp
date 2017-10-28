#include "map.h"
#include "tileentity.h"

Map::Map(Qt3DCore::QNode *parent)
    : Qt3DCore::QEntity(parent)
{
    int x = 0, y = 0, z= 14;
    for (int i = -3; i < 3; i++) {
        for (int j = -3; j < 3; j++) {
            // Load texture
            TileEntity *tile = new TileEntity(this);
            tile->setTileCoordinate(x + i, y + j, z);
            tile->setPosition(i * 10, j * 10);
            m_tiles.append(tile);
        }
    }
}

Map::~Map()
{
    qDeleteAll(m_tiles);
}
