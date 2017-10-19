#ifndef TILEEFFECT_H
#define TILEEFFECT_H

#include <QEffect>
#include <QTechnique>

class TileEffect : public Qt3DRender::QEffect
{
public:
    explicit TileEffect(Qt3DCore::QNode *parent = 0);

    inline Qt3DRender::QTechnique *gl3Technique() const { return m_gl3Technique; }
    inline Qt3DRender::QTechnique *gl2Technique() const { return m_gl2Technique; }

private :
    Qt3DRender::QTechnique *m_gl3Technique;
    Qt3DRender::QTechnique *m_gl2Technique;
    Qt3DRender::QRenderPass *m_gl2Pass;
    Qt3DRender::QRenderPass *m_gl3Pass;
};

#endif // TILEEFFECT_H
