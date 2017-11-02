#ifndef TERRAIN_H
#define TERRAIN_H

#include <QEntity>
#include <QList>

#include "../cameracontroller.h"
#include "tile.h"

namespace Entity {

class Terrain : public Qt3DCore::QEntity
{
    Q_OBJECT

    Q_PROPERTY(CameraController *cameraController READ cameraController WRITE setCameraController NOTIFY cameraControllerChanged)

public:
    explicit Terrain(Qt3DCore::QNode *parent = nullptr);
    Terrain(CameraController *cameraController, Qt3DCore::QNode *parent = nullptr);
    ~Terrain();

    CameraController *cameraController() const;

    void setCameraController(CameraController *cameraController);

signals:
    void cameraControllerChanged();

private:
    void init();

private:
    QList<Tile *> m_tiles;
    CameraController *m_cameraController;
    // TODO: TilePyramid
    // TilePyramid m_tilePyramid;
};

}

#endif // TERRAIN_H
