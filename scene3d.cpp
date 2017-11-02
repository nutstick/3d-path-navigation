#include "scene3d.h"

Scene3D::Scene3D(QScreen *screen)
    : Qt3DExtras::Qt3DWindow(screen)
    , m_map(new Entity::Map)
    , m_cameraController(new CameraController)
{
    camera()->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    camera()->setPosition(QVector3D(-40.0f, 40.0f, 0));
    camera()->setViewCenter(QVector3D(0, 0, 0));

    m_cameraController->setCamera(camera());
    m_cameraController->setLinearSpeed(10.0f);
    m_cameraController->setLookSpeed(50.0f);
    m_cameraController->setZoomInLimit(1.0f);
    m_cameraController->setMaxTiltAngle(90);
    m_cameraController->setMinTiltAngle(45);

    this->setRootEntity(m_map);
}

Scene3D::~Scene3D()
{
}

CameraController* Scene3D::cameraController() const
{
    return m_cameraController;
}

void Scene3D::setCameraController(CameraController *cameraController)
{
    m_cameraController = cameraController;
}
