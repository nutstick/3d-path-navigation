#ifndef TILEMATERIAL_P_H
#define TILEMATERIAL_P_H

#include "tilematerial.h"

#include <QMaterial>
#include <QParameter>
#include <QTechnique>
#include <QRenderPass>
#include <QShaderProgram>

class TileMaterialPrivate : public Qt3DRender::QMaterialPrivate
{
public:
    TileMaterialPrivate();

    void init();

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

    Q_DECLARE_PUBLIC(TileMaterial)
}

#endif // TILEMATERIAL_P_H
