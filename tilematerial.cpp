#include "tilematerial.h"
#include "tileeffect.h"

TileMaterial::TileMaterial(Qt3DCore::QNode *parent)
    : Qt3DRender::QMaterial(parent)
{
    TileEffect *tileEffect = new TileEffect;
    setEffect(tileEffect);
}
