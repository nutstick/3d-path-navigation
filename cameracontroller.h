#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <QEntity>
#include <QCamera>
#include <QAction>
#include <QAxis>
#include <QActionInput>
#include <QAnalogAxisInput>
#include <QButtonAxisInput>
#include <QKeyboardDevice>
#include <QMouseDevice>
#include <QLogicalDevice>
#include <QObjectPicker>
#include <QFrameAction>
#include <QtMath>

#include "entities/tile.h"

class CameraController : public Qt3DCore::QEntity
{
    Q_OBJECT
    Q_PROPERTY(Qt3DRender::QCamera *camera READ camera WRITE setCamera NOTIFY cameraChanged)
    Q_PROPERTY(float linearSpeed READ linearSpeed WRITE setLinearSpeed NOTIFY linearSpeedChanged)
    Q_PROPERTY(float lookSpeed READ lookSpeed WRITE setLookSpeed NOTIFY lookSpeedChanged)
    Q_PROPERTY(float zoomInLimit READ zoomInLimit WRITE setZoomInLimit NOTIFY zoomInLimitChanged)
    Q_PROPERTY(float maxTiltAngle READ maxTiltAngle WRITE setMaxTiltAngle NOTIFY maxTiltAngleChanged)
    Q_PROPERTY(float minTiltAngle READ minTiltAngle WRITE setMinTiltAngle NOTIFY minTiltAngleChanged)

public:
    explicit CameraController(Qt3DCore::QNode *parent = nullptr);
    ~CameraController();

    Entity::Tile::TileCoords getTileCoords();

    Qt3DRender::QCamera *camera() const;
    float linearSpeed() const;
    float lookSpeed() const;
    float zoomInLimit() const;
    float maxTiltAngle() const;
    float minTiltAngle() const;

    void setCamera(Qt3DRender::QCamera *camera);
    void setLinearSpeed(float linearSpeed);
    void setLookSpeed(float lookSpeed);
    void setZoomInLimit(float zoomInLimit);
    void setMaxTiltAngle(float maxTiltAngle);
    void setMinTiltAngle(float minTiltAngle);

signals:
    void cameraChanged();
    void linearSpeedChanged();
    void lookSpeedChanged();
    void zoomInLimitChanged();
    void maxTiltAngleChanged();
    void minTiltAngleChanged();

private:
    void onTriggered(float dt);

private:
    Qt3DRender::QCamera *m_camera;

    Qt3DInput::QAction *m_leftMouseButtonAction;
    Qt3DInput::QAction *m_rightMouseButtonAction;
    Qt3DInput::QAction *m_altButtonAction;
    Qt3DInput::QAction *m_shiftButtonAction;

    Qt3DInput::QAxis *m_rxAxis;
    Qt3DInput::QAxis *m_ryAxis;
    Qt3DInput::QAxis *m_txAxis;
    Qt3DInput::QAxis *m_tyAxis;
    Qt3DInput::QAxis *m_tzAxis;

    Qt3DInput::QActionInput *m_leftMouseButtonInput;
    Qt3DInput::QActionInput *m_rightMouseButtonInput;
    Qt3DInput::QActionInput *m_altButtonInput;
    Qt3DInput::QActionInput *m_shiftButtonInput;

    Qt3DInput::QAnalogAxisInput *m_mouseRxInput;
    Qt3DInput::QAnalogAxisInput *m_mouseRyInput;
    Qt3DInput::QAnalogAxisInput *m_mouseTzXInput;
    Qt3DInput::QAnalogAxisInput *m_mouseTzYInput;
    Qt3DInput::QButtonAxisInput *m_keyboardTxPosInput;
    Qt3DInput::QButtonAxisInput *m_keyboardTyPosInput;
    Qt3DInput::QButtonAxisInput *m_keyboardTzPosInput;
    Qt3DInput::QButtonAxisInput *m_keyboardTxNegInput;
    Qt3DInput::QButtonAxisInput *m_keyboardTyNegInput;
    Qt3DInput::QButtonAxisInput *m_keyboardTzNegInput;

    Qt3DInput::QKeyboardDevice *m_keyboardDevice;
    Qt3DInput::QMouseDevice *m_mouseDevice;

    Qt3DInput::QLogicalDevice *m_logicalDevice;

    Qt3DLogic::QFrameAction *m_frameAction;

    float m_linearSpeed;
    float m_lookSpeed;
    float m_zoomInLimit;
    float m_maxTiltAngle;
    float m_minTiltAngle;

    struct CameraData {
        float x = 0, y = 0;  // ground point towards which the camera is looking
        float radius = 40;  // distance of camera from the point it is looking at
        float tilt = 90; // aircraft nose up/down (0 = looking straight down to the plane)
        float bearing = 0;   // aircraft nose left/right

        float width, height;
        const QVector3D cameraUp = QVector3D(0, 0, -1);

        bool operator==(const CameraData& other) const
        {
            return x == other.x && y == other.y && radius == other.radius && tilt == other.tilt && bearing == other.bearing;
        }
        bool operator!=(const CameraData& other) const
        {
            return !operator==(other);
        }

        void setCamera(Qt3DRender::QCamera* camera)
        {
            float x_t = radius * qCos(M_PI * tilt / 180.0f) * qSin(M_PI * bearing / 180.0f) + x;
            float y_t = radius * qCos(M_PI * tilt / 180.0f) * qCos(M_PI * bearing / 180.0f) + y;
            float z_t = radius * qSin(M_PI * tilt / 180.0f);
//            qDebug() << x << y;
//            qDebug() << x_t << y_t << z_t;
            camera->setUpVector(cameraUp);
            camera->setPosition(QVector3D(x_t, z_t, y_t));
//            camera->setPosition(QVector3D(0, 40, 0));
            camera->setViewCenter(QVector3D(x, 0, y));
//            camera->rotateAboutViewCenter(QQuaternion::fromEulerAngles(pitch, yaw, 0));
        }
    };

    CameraData m_cameraData;
};

#endif // CAMERACONTROLLER_H
