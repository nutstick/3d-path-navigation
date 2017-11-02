#include "map.h"

Entity::Map::Map(Qt3DCore::QNode *parent)
    : Qt3DCore::QEntity(parent)
    , m_terrain(new Terrain(this))
{

}

Entity::Map::~Map()
{
}
