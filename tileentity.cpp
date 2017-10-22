#include "tileentity.h"

#include <QPhongMaterial>
#include "utils/imagemanager.h"

TileEntity::TileEntity(QNode *parent)
    : Qt3DCore::QEntity(parent)
    , m_transform(new Qt3DCore::QTransform())
    , m_mesh(new TileMesh())
    , m_material(new TileMaterial())
{
//    Qt3DRender::QTechnique m_GL3Technique;
//    Qt3DRender::QTechnique m_GL2Technique;
//    Qt3DRender::QTechnique m_ES2Technique;
//    Qt3DRender::QShaderProgram m_GL3Shader;
//    Qt3DRender::QShaderProgram m_GL2ES2Shader;
//    Qt3DRender::QFilterKey m_filterKey;
//    Qt3DRender::QMaterialPrivate

//    m_GL3Shader->setVertexShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl(QStringLiteral("qrc:/shader/gl3/maptile.vert"))));
//    m_GL3Shader->setFragmentShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl(QStringLiteral("qrc:/shader/gl3/maptile.frag"))));

//    m_GL2ES2Shader->setVertexShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl(QStringLiteral("qrc:/shader/es2/maptile.vert"))));
//    m_GL2ES2Shader->setFragmentShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl(QStringLiteral("qrc:/shader/es2/maptile.frag"))));

//    m_GL3Technique->graphicsApiFilter()->setApi(Qt3DRender::QGraphicsApiFilter::OpenGL);
//    m_GL3Technique->graphicsApiFilter()->setMajorVersion(3);
//    m_GL3Technique->graphicsApiFilter()->setMinorVersion(1);
//    m_GL3Technique->graphicsApiFilter()->setProfile(Qt3DRender::QGraphicsApiFilter::CoreProfile);

//    m_GL2Technique->graphicsApiFilter()->setApi(Qt3DRender::QGraphicsApiFilter::OpenGL);
//    m_GL2Technique->graphicsApiFilter()->setMajorVersion(2);
//    m_GL2Technique->graphicsApiFilter()->setMinorVersion(0);
//    m_GL2Technique->graphicsApiFilter()->setProfile(Qt3DRender::QGraphicsApiFilter::NoProfile);

//    m_ES2Technique->graphicsApiFilter()->setApi(Qt3DRender::QGraphicsApiFilter::OpenGLES);
//    m_ES2Technique->graphicsApiFilter()->setMajorVersion(2);
//    m_ES2Technique->graphicsApiFilter()->setMinorVersion(0);
//    m_ES2Technique->graphicsApiFilter()->setProfile(Qt3DRender::QGraphicsApiFilter::CoreProfile);

//    m_filterKey->setParent(this);
//    m_filterKey->setName(QStringLiteral("renderingStyle"));
//    m_filterKey->setValue(QStringLiteral("forward"));

//    m_GL3Technique->addFilterKey(m_filterKey);
//    m_GL2Technique->addFilterKey(m_filterKey);
//    m_ES2Technique->addFilterKey(m_filterKey);

//    m_GL3RenderPass->setShaderProgram(m_GL3Shader);
//    m_GL2RenderPass->setShaderProgram(m_GL2ES2Shader);
//    m_ES2RenderPass->setShaderProgram(m_GL2ES2Shader);

//    m_GL3Technique->addRenderPass(m_GL3RenderPass);
//    m_GL2Technique->addRenderPass(m_GL2RenderPass);
//    m_ES2Technique->addRenderPass(m_ES2RenderPass);

//    m_effect->addTechnique(m_GL3Technique);
//    m_effect->addTechnique(m_GL2Technique);
//    m_effect->addTechnique(m_ES2Technique);

//    m_effect->addParameter(m_textureParameter);
//    m_effect->addParameter(m_elevationParameter);

//    m_effect->setParent(this);

//    m_material->setEffect(m_effect);

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
