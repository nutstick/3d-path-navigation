#ifndef TILEENTITY_H
#define TILEENTITY_H

#include <QObject>
#include <QEntity>
#include <QTransform>
#include <QPhongMaterial>

#include "tilemesh.h"
#include "tilematerial.h"

class TileEntity : public Qt3DCore::QEntity
{
    Q_OBJECT

public:
    TileEntity(Qt3DCore::QNode *parent = 0);
    ~TileEntity();

    void setTileCoordinate(int x, int y, int z);
    void setPosition(float x, float y);

    const QUrl& baseUrl() const;

private:
    QUrl tileQuery(int x, int y, int z);

private:
    Qt3DCore::QTransform *m_transform;
    TileMesh *m_mesh;
    TileMaterial *m_material;

    int m_x;
    int m_y;
    int m_z;
};

#endif // TILEENTITY_H
