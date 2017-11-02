#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "scene3d.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow();

signals:

public slots:

private:
    Scene3D *m_scene3d;
};

#endif // MAINWINDOW_H
