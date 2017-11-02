#ifndef TILE_H
#define TILE_H

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

#include "meshes/tilemesh.h"
#include "maptiles/maptextureimage.h"

namespace Entity {

class Tile : public Qt3DCore::QEntity
{
    Q_OBJECT

public:
    struct TileCoords {
        int x;
        int y;
        int z;
        QString key;

        TileCoords(int x, int y, int z)
            : x(x), y(y), z(z)
        {
            key = QString("%25x/%25y/%25z")
                    .replace("%25x", QString::number(x))
                    .replace("%25y", QString::number(y))
                    .replace("%25z", QString::number(z));
        }
    };

    explicit Tile(Qt3DCore::QNode *parent = nullptr);
    Tile(TileCoords coords, Qt3DCore::QNode *parent = nullptr);
    ~Tile();

    void setTileCoords(int x, int y, int z);
    void setTileCoords(TileCoords coords);

    const QUrl baseUrl() const;

private:
    QUrl tileQuery(TileCoords coords);
    void setPosition(float x, float y);

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

    TileCoords m_coords;
};

}

#endif // TILE_H
