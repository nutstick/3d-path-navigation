#include <QApplication>
#include <QQmlApplicationEngine>
#include <QBoxLayout>
#include <QWidget>

#include <Qt3DCore/QEntity>
#include <Qt3DCore/QAspectEngine>
#include <Qt3DRender/QCamera>

#include <Qt3DExtras/qt3dwindow.h>
#include <QFirstPersonCameraController>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow *mainWindow = new MainWindow();

    mainWindow->show();

    return app.exec();
}
