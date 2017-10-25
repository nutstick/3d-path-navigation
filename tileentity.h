#ifndef TILEENTITY_H
#define TILEENTITY_H

#include <QObject>
#include <QEntity>
#include <QUrl>
#include <QTransform>
#include <QMaterial>
#include <QEffect>
#include <QTechnique>
#include <QGraphicsApiFilter>
#include <QRenderPass>
#include <QTexture>
#include <QParameter>
#include <QPaintedTextureImage>

#include "tilemesh.h"
#include "maptiles/maptextureimage.h"

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
    Qt3DRender::QMaterial *m_material;
    Qt3DRender::QEffect *m_effect;
    Qt3DRender::QTexture2D *m_texture;
    Qt3DRender::QTexture2D *m_elevation;
    Qt3DRender::QParameter *m_textureParameter;
    Qt3DRender::QParameter *m_elevationParameter;
    MapTextureImage *m_textureImage;
    MapTextureImage *m_elevationImage;

    int m_x;
    int m_y;
    int m_z;
};

#endif // TILEENTITY_H
