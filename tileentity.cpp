#include "tileentity.h"

#include <QPhongMaterial>
#include "utils/imagemanager.h"

TileEntity::TileEntity(QNode *parent)
    : Qt3DCore::QEntity(parent)
    , m_transform(new Qt3DCore::QTransform())
    , m_mesh(new TileMesh())
    , m_material(new TileMaterial())
{
    addComponent(m_transform);
    addComponent(m_mesh);
    addComponent(m_material);

    m_mesh->setWidth(10.0f);
    m_mesh->setHeight(10.0f);
    m_mesh->setMeshResolution(QSize(2, 2));
}

TileEntity::~TileEntity()
{
}

const QUrl& TileEntity::baseUrl() const
{
    // Return the url.
    return QUrl("https://api.mapbox.com/v4/mapbox.satellite/%25zoom/%25x/%25y.png?access_token=%access_token");
}

QUrl TileEntity::tileQuery(int x, int y, int zoom)
{
    return QUrl(baseUrl().toString()
        .replace("%25x", QString::number(x))
        .replace("%25y", QString::number(y))
        .replace("%25zoom", QString::number(zoom))
        .replace("%access_token", "pk.eyJ1IjoibnV0c3RpY2siLCJhIjoiY2o4aTh1anUxMTB2bTJ3bDlqYmo5ODJvaSJ9.YN8ymbV5tq9XsSHGflhblw"));
}

void TileEntity::setTileCoordinate(int x, int y, int z)
{
    m_x = x;
    m_y = y;
    m_z = z;

    ImageManager::get().image(tileQuery(x, y, z), QSize(256, 256));
}

void TileEntity::setPosition(float x, float y)
{
    m_transform->setTranslation(QVector3D(x, 0, y));
}
