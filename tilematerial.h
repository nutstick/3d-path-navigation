#ifndef TILEMATERIAL_H
#define TILEMATERIAL_H

#include <QMaterial>
#include <QEffect>
#include <QTechnique>

class TileMaterial : public Qt3DRender::QMaterial
{
    Q_OBJECT
public:
    explicit TileMaterial(Qt3DCore::QNode *parent = nullptr);

signals:

public slots:
};

#endif // TILEMATERIAL_H
