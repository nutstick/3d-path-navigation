#ifndef TILEMATERIAL_H
#define TILEMATERIAL_H

#include <QMaterial>
#include <QVariant>
#include <QEffect>
#include <QTechnique>
#include <QTexture>
#include <QParameter>

class TileMaterial : public Qt3DRender::QMaterial
{
    Q_OBJECT
    Q_PROPERTY(Qt3DRender::QTexture2D *texture READ texture WRITE setTexture NOTIFY textureChanged)
    Q_PROPERTY(Qt3DRender::QTexture2D *elevationMap READ elevationMap WRITE setElevationMap NOTIFY elevationMapChanged)
public:
    explicit TileMaterial(Qt3DCore::QNode *parent = nullptr);
    ~TileMaterial();

    Qt3DRender::QTexture2D *texture() const;
    Qt3DRender::QTexture2D *elevationMap() const;

signals:
    void textureChanged(Qt3DRender::QTexture2D *texture);
    void elevationMapChanged(Qt3DRender::QTexture2D *elevationMap);

public slots:
    void setTexture(Qt3DRender::QTexture2D *texture);
    void setElevationMap(Qt3DRender::QTexture2D *elevationMap);

private:
    void handleTextureChanged(const QVariant &var);
    void handleElevationMapChanged(const QVariant &var);

    Qt3DRender::QEffect *m_effect;

    Qt3DRender::QTexture2D *m_texture;
    Qt3DRender::QTexture2D *m_elevation;
    Qt3DRender::QParameter *m_textureParameter;
    Qt3DRender::QParameter *m_elevationParameter;

    Qt3DRender::QTechnique *m_GL3Technique;
    Qt3DRender::QTechnique *m_GL2Technique;
    Qt3DRender::QTechnique *m_ES2Technique;

    Qt3DRender::QRenderPass *m_GL3RenderPass;
    Qt3DRender::QRenderPass *m_GL2RenderPass;
    Qt3DRender::QRenderPass *m_ES2RenderPass;

    Qt3DRender::QShaderProgram *m_GL3Shader;
    Qt3DRender::QShaderProgram *m_GL2ES2Shader;

    Qt3DRender::QFilterKey *m_filterKey;
};

#endif // TILEMATERIAL_H
