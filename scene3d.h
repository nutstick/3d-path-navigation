#ifndef SCENE3D_H
#define SCENE3D_H

#include <Qt3DWindow>
#include "cameracontroller.h"
#include "entities/map.h"

class Scene3D : public Qt3DExtras::Qt3DWindow
{
    Q_OBJECT
    Q_PROPERTY(CameraController *cameraController READ cameraController WRITE setCameraController NOTIFY cameraControllerChanged)

public:
    explicit Scene3D(QScreen *screen = nullptr);
    ~Scene3D();

    CameraController *cameraController() const;

    void setCameraController(CameraController *cameraController);

signals:
    void cameraControllerChanged();

private:
    Entity::Map *m_map;
    CameraController *m_cameraController;
};

#endif // SCENE3D_H
