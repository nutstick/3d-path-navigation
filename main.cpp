#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <Qt3DCore/QEntity>
#include <Qt3DCore/QAspectEngine>
#include <Qt3DRender/QCamera>

#include <Qt3DExtras/qt3dwindow.h>
#include <QFirstPersonCameraController>

#include "map.h"
#include "cameracontroller.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Qt3DExtras::Qt3DWindow view;

    // Scene Root
    Map *scene = new Map();

    // Camera
    Qt3DRender::QCamera *camera = view.camera();
    camera->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    camera->setPosition(QVector3D(-40.0f, 40.0f, 0));
    camera->setViewCenter(QVector3D(0, 0, 0));

    // For camera controls
    CameraController *camController = new CameraController(scene);
    camController->setCamera(camera);
    camController->setLinearSpeed(20.0f);
    camController->setLookSpeed(5.0f);
    camController->setZoomInLimit(1.0f);

    view.setRootEntity(scene);
    view.show();

    return app.exec();
}
