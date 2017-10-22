#ifndef TILEMATERIAL_H
#define TILEMATERIAL_H

#include <QMaterial>
#include <QVariant>
#include <QEffect>
#include <QTechnique>
#include <QTexture>
#include <QParameter>

#include "tilematerial_p.h"

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

public slots:
    void setTexture(Qt3DRender::QTexture2D *texture);
    void setElevationMap(Qt3DRender::QTexture2D *elevationMap);

signals:
    void textureChanged(Qt3DRender::QTexture2D *texture);
    void elevationMapChanged(Qt3DRender::QTexture2D *elevationMap);

private:
    Q_DECLARE_PRIVATE(TileMaterial)
};

#endif // TILEMATERIAL_H
