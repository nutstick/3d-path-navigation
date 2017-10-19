#include <QGraphicsApiFilter>
#include <QUrl>

#include "tileeffect.h"

TileEffect::TileEffect(Qt3DCore::QNode *parent)
    : Qt3DRender::QEffect(parent)
    , m_gl3Technique(new Qt3DRender::QTechnique())
    , m_gl2Technique(new Qt3DRender::QTechnique())
    , m_gl2Pass(new Qt3DRender::QRenderPass())
    , m_gl3Pass(new Qt3DRender::QRenderPass())
{
    m_gl3Technique->graphicsApiFilter()->setApi(Qt3DRender::QGraphicsApiFilter::OpenGL);
    m_gl3Technique->graphicsApiFilter()->setMajorVersion(3);
    m_gl3Technique->graphicsApiFilter()->setMinorVersion(2);

    m_gl3Technique->graphicsApiFilter()->setProfile(Qt3DRender::QGraphicsApiFilter::CoreProfile);

    m_gl2Technique->graphicsApiFilter()->setApi(Qt3DRender::QGraphicsApiFilter::OpenGL);
    m_gl2Technique->graphicsApiFilter()->setMajorVersion(2);
    m_gl2Technique->graphicsApiFilter()->setMinorVersion(0);
    m_gl2Technique->graphicsApiFilter()->setProfile(Qt3DRender::QGraphicsApiFilter::NoProfile);

    Qt3DRender::QShaderProgram *gl3Shader = new Qt3DRender::QShaderProgram();
    gl3Shader->setVertexShaderCode(gl3Shader->loadSource(QUrl(QStringLiteral("qrc:/shader/gl3/maptile.gl3.vert"))));
    gl3Shader->setFragmentShaderCode(gl3Shader->loadSource(QUrl(QStringLiteral("qrc:/shader/gl3/maptile.gl3.frag"))));

    m_gl3Pass->setShaderProgram(gl3Shader);
    m_gl3Technique->addRenderPass(m_gl3Pass);

    Qt3DRender::QShaderProgram *gl2Shader = new Qt3DRender::QShaderProgram();
    gl2Shader->setVertexShaderCode(gl2Shader->loadSource(QUrl(QStringLiteral("qrc:/shader/gl2/maptile.gl2.vert"))));
    gl2Shader->setFragmentShaderCode(gl2Shader->loadSource(QUrl(QStringLiteral("qrc:/shader/gl2/maptile.gl2.frag"))));

    m_gl2Pass->setShaderProgram(gl2Shader);
    m_gl2Technique->addRenderPass(m_gl2Pass);

    addTechnique(m_gl3Technique);
    addTechnique(m_gl2Technique);
}
