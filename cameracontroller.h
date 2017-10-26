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

class CameraController : public Qt3DCore::QEntity
{
    Q_OBJECT
    Q_PROPERTY(Qt3DRender::QCamera *camera READ camera WRITE setCamera NOTIFY cameraChanged)
    Q_PROPERTY(float linearSpeed READ linearSpeed WRITE setLinearSpeed NOTIFY linearSpeedChanged)
    Q_PROPERTY(float lookSpeed READ lookSpeed WRITE setLookSpeed NOTIFY lookSpeedChanged)
    Q_PROPERTY(float zoomInLimit READ zoomInLimit WRITE setZoomInLimit NOTIFY zoomInLimitChanged)

public:
    explicit CameraController(Qt3DCore::QNode *parent = nullptr);
    ~CameraController();

    Qt3DRender::QCamera *camera() const;
    float linearSpeed() const;
    float lookSpeed() const;
    float zoomInLimit() const;

    void setCamera(Qt3DRender::QCamera *camera);
    void setLinearSpeed(float linearSpeed);
    void setLookSpeed(float lookSpeed);
    void setZoomInLimit(float zoomInLimit);

signals:
    void cameraChanged();
    void linearSpeedChanged();
    void lookSpeedChanged();
    void zoomInLimitChanged();

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
    QVector3D m_cameraUp;

    struct CameraData {
        float x = 0, y = 0;  // ground point towards which the camera is looking
        float radius = 40;  // distance of camera from the point it is looking at
        float tilt = 0; // aircraft nose up/down (0 = looking straight down to the plane)
        float bearing = 0;   // aircraft nose left/right

        bool operator==(const CameraData& other) const
        {
            return x == other.x && y == other.y && dist == other.dist && pitch == other.pitch && yaw == other.yaw;
        }
        bool operator!=(const CameraData& other) const
        {
            return !operator==(other);
        }

        void setCamera(Qt3DRender::QCamera* camera)
        {
            // basic scene setup:
            // - x grows to the right
            // - z grows to the bottom
            // - y grows towards camera
            // so a point on the plane (x',y') is transformed to (x,-z) in our 3D world

            camera->panAboutViewCenter(bearing, m_cameraUp);
            camera->tiltAboutViewCenter(tilt);

            camera->setUpVector(QVector3D(0,0,-1));
            camera->setPosition(QVector3D(x,dist, y));
            camera->setViewCenter(QVector3D(x,0,y));
            camera->rotateAboutViewCenter(QQuaternion::fromEulerAngles(pitch, yaw, 0));
        }
    };

    CameraData m_cameraData;
};

#endif // CAMERACONTROLLER_H
