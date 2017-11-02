#include "mainwindow.h"

MainWindow::MainWindow()
{
    m_scene3d = new Scene3D();
    QWidget* container = QWidget::createWindowContainer(m_scene3d, this);

    qDebug() << container;
    setCentralWidget(container);
    // TOOD: Add Button, Compass and etc.
}
