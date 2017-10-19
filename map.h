#ifndef MAP_H
#define MAP_H

#include <QEntity>

class TileEntity;

class Map : public Qt3DCore::QEntity
{
    Q_OBJECT

public:
    explicit Map(Qt3DCore::QNode *parent = 0);
    ~Map();

private:
    QVector<TileEntity *> m_tiles;
};

#endif // MAP_H
