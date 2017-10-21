#include "tilematerial.h"

#include <QGraphicsApiFilter>
#include <QTextureWrapMode>

TileMaterial::TileMaterial(Qt3DCore::QNode *parent)
    : Qt3DRender::QMaterial(parent)
    , m_effect(new Qt3DRender::QEffect())
    , m_texture(new Qt3DRender::QTexture2D())
    , m_elevation(new Qt3DRender::QTexture2D())
    , m_textureParameter(new Qt3DRender::QParameter(QStringLiteral("texture"), m_texture))
    , m_elevationParameter(new Qt3DRender::QParameter(QStringLiteral("elevation"), m_elevation))
    , m_GL3Technique(new Qt3DRender::QTechnique())
    , m_GL2Technique(new Qt3DRender::QTechnique())
    , m_ES2Technique(new Qt3DRender::QTechnique())
    , m_GL3RenderPass(new Qt3DRender::QRenderPass())
    , m_GL2RenderPass(new Qt3DRender::QRenderPass())
    , m_ES2RenderPass(new Qt3DRender::QRenderPass())
    , m_GL3Shader(new Qt3DRender::QShaderProgram())
    , m_GL2ES2Shader(new Qt3DRender::QShaderProgram())
    , m_filterKey(new Qt3DRender::QFilterKey)
{

    connect(m_textureParameter, &Qt3DRender::QParameter::valueChanged,
            this, &TileMaterial::handleTextureChanged);
    connect(m_elevationParameter, &Qt3DRender::QParameter::valueChanged,
            this, &TileMaterial::handleElevationMapChanged);

    m_texture->setMagnificationFilter(Qt3DRender::QAbstractTexture::Linear);
    m_texture->setMinificationFilter(Qt3DRender::QAbstractTexture::LinearMipMapLinear);
    m_texture->setWrapMode(Qt3DRender::QTextureWrapMode(Qt3DRender::QTextureWrapMode::Repeat));
    m_texture->setGenerateMipMaps(true);

    m_GL3Shader->setVertexShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl(QStringLiteral("qrc:/shader/gl3/maptile.vert"))));
    m_GL3Shader->setFragmentShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl(QStringLiteral("qrc:/shader/gl3/maptile.frag"))));

    m_GL2ES2Shader->setVertexShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl(QStringLiteral("qrc:/shader/es2/maptile.vert"))));
    m_GL2ES2Shader->setFragmentShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl(QStringLiteral("qrc:/shader/es2/maptile.frag"))));

    m_GL3Technique->graphicsApiFilter()->setApi(Qt3DRender::QGraphicsApiFilter::OpenGL);
    m_GL3Technique->graphicsApiFilter()->setMajorVersion(3);
    m_GL3Technique->graphicsApiFilter()->setMinorVersion(1);
    m_GL3Technique->graphicsApiFilter()->setProfile(Qt3DRender::QGraphicsApiFilter::CoreProfile);

    m_GL2Technique->graphicsApiFilter()->setApi(Qt3DRender::QGraphicsApiFilter::OpenGL);
    m_GL2Technique->graphicsApiFilter()->setMajorVersion(2);
    m_GL2Technique->graphicsApiFilter()->setMinorVersion(0);
    m_GL2Technique->graphicsApiFilter()->setProfile(Qt3DRender::QGraphicsApiFilter::NoProfile);

    m_ES2Technique->graphicsApiFilter()->setApi(Qt3DRender::QGraphicsApiFilter::OpenGLES);
    m_ES2Technique->graphicsApiFilter()->setMajorVersion(2);
    m_ES2Technique->graphicsApiFilter()->setMinorVersion(0);
    m_ES2Technique->graphicsApiFilter()->setProfile(Qt3DRender::QGraphicsApiFilter::CoreProfile);

    m_filterKey->setParent(this);
    m_filterKey->setName(QStringLiteral("renderingStyle"));
    m_filterKey->setValue(QStringLiteral("forward"));

    m_GL3Technique->addFilterKey(m_filterKey);
    m_GL2Technique->addFilterKey(m_filterKey);
    m_ES2Technique->addFilterKey(m_filterKey);

    m_GL3RenderPass->setShaderProgram(m_GL3Shader);
    m_GL2RenderPass->setShaderProgram(m_GL2ES2Shader);
    m_ES2RenderPass->setShaderProgram(m_GL2ES2Shader);

    m_GL3Technique->addRenderPass(m_GL3RenderPass);
    m_GL2Technique->addRenderPass(m_GL2RenderPass);
    m_ES2Technique->addRenderPass(m_ES2RenderPass);

    m_effect->addTechnique(m_GL3Technique);
    m_effect->addTechnique(m_GL2Technique);
    m_effect->addTechnique(m_ES2Technique);

    m_effect->addParameter(m_textureParameter);
    m_effect->addParameter(m_elevationParameter);

    m_effect->setParent(this);

    this->setEffect(m_effect);
}

TileMaterial::~TileMaterial()
{
}

Qt3DRender::QTexture2D *TileMaterial::texture() const
{
    return m_textureParameter->value().value<Qt3DRender::QTexture2D *>();
}

Qt3DRender::QTexture2D *TileMaterial::elevationMap() const
{
    return m_elevationParameter->value().value<Qt3DRender::QTexture2D *>();
}

void TileMaterial::setTexture(Qt3DRender::QTexture2D *texture)
{
    m_textureParameter->setValue(QVariant::fromValue(texture));
}

void TileMaterial::setElevationMap(Qt3DRender::QTexture2D *elevationMap)
{
    m_elevationParameter->setValue(QVariant::fromValue(elevationMap));
}

void TileMaterial::handleTextureChanged(const QVariant &var)
{
    emit textureChanged(var.value<Qt3DRender::QTexture2D *>());
}

void TileMaterial::handleElevationMapChanged(const QVariant &var)
{
    emit elevationMapChanged(var.value<Qt3DRender::QTexture2D *>());
}
