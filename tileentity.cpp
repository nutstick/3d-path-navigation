#include "tileentity.h"

#include <QTextureMaterial>

TileEntity::TileEntity(QNode *parent)
    : Qt3DCore::QEntity(parent)
    , m_transform(new Qt3DCore::QTransform())
    , m_mesh(new TileMesh())
    , m_material(new Qt3DExtras::QTextureMaterial())
    , m_effect(new Qt3DRender::QEffect)
    , m_texture(new Qt3DRender::QTexture2D)
    , m_elevation(new Qt3DRender::QTexture2D)
    , m_textureParameter(new Qt3DRender::QParameter(QStringLiteral("textureMap"), m_texture))
    , m_elevationParameter(new Qt3DRender::QParameter(QStringLiteral("elevationMap"), m_elevation))
    , m_textureImage(new MapTextureImage(QImage()))
    , m_elevationImage(new MapTextureImage(QImage()))
{
    m_material->effect()->techniques()[0]->renderPasses()[0]->shaderProgram()->setFragmentShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl(QStringLiteral("qrc:/shader/gl3/maptile.vert"))));
    m_material->effect()->techniques()[1]->renderPasses()[0]->shaderProgram()->setFragmentShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl(QStringLiteral("qrc:/shader/es2/maptile.vert"))));
    m_material->effect()->techniques()[2]->renderPasses()[0]->shaderProgram()->setFragmentShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl(QStringLiteral("qrc:/shader/es2/maptile.vert"))));

    m_material->effect()->techniques()[0]->renderPasses()[0]->shaderProgram()->setFragmentShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl(QStringLiteral("qrc:/shader/gl3/maptile.frag"))));
    m_material->effect()->techniques()[1]->renderPasses()[0]->shaderProgram()->setFragmentShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl(QStringLiteral("qrc:/shader/es2/maptile.frag"))));
    m_material->effect()->techniques()[2]->renderPasses()[0]->shaderProgram()->setFragmentShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl(QStringLiteral("qrc:/shader/es2/maptile.frag"))));

    m_material->addParameter(m_textureParameter);
    m_material->addParameter(m_elevationParameter);

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
    return QUrl(QString("https://api.mapbox.com/v4/mapbox.satellite/%25zoom/%25x/%25y.png?access_token=%access_token")
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

    MapTextureImage* image = new MapTextureImage(tileQuery(x, y, z));
    m_texture->addTextureImage(image);
    m_texture->setMinificationFilter(Qt3DRender::QTexture2D::Linear);
    m_texture->setMagnificationFilter(Qt3DRender::QTexture2D::Linear);
}

void TileEntity::setPosition(float x, float y)
{
    m_transform->setTranslation(QVector3D(x, 0, y));
}
