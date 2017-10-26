#include "cameracontroller.h"

#include <Qt3DRender/QObjectPicker>
#include <Qt3DRender/QPickEvent>

CameraController::CameraController(Qt3DCore::QNode *parent)
  : Qt3DCore::QEntity(parent)
  , m_camera(nullptr)
  , m_leftMouseButtonAction(new Qt3DInput::QAction())
  , m_rightMouseButtonAction(new Qt3DInput::QAction())
  , m_altButtonAction(new Qt3DInput::QAction())
  , m_shiftButtonAction(new Qt3DInput::QAction())
  , m_rxAxis(new Qt3DInput::QAxis())
  , m_ryAxis(new Qt3DInput::QAxis())
  , m_txAxis(new Qt3DInput::QAxis())
  , m_tyAxis(new Qt3DInput::QAxis())
  , m_tzAxis(new Qt3DInput::QAxis())
  , m_leftMouseButtonInput(new Qt3DInput::QActionInput())
  , m_rightMouseButtonInput(new Qt3DInput::QActionInput())
  , m_altButtonInput(new Qt3DInput::QActionInput())
  , m_shiftButtonInput(new Qt3DInput::QActionInput())
  , m_mouseRxInput(new Qt3DInput::QAnalogAxisInput())
  , m_mouseRyInput(new Qt3DInput::QAnalogAxisInput())
  , m_mouseTzXInput(new Qt3DInput::QAnalogAxisInput())
  , m_mouseTzYInput(new Qt3DInput::QAnalogAxisInput())
  , m_keyboardTxPosInput(new Qt3DInput::QButtonAxisInput())
  , m_keyboardTyPosInput(new Qt3DInput::QButtonAxisInput())
  , m_keyboardTzPosInput(new Qt3DInput::QButtonAxisInput())
  , m_keyboardTxNegInput(new Qt3DInput::QButtonAxisInput())
  , m_keyboardTyNegInput(new Qt3DInput::QButtonAxisInput())
  , m_keyboardTzNegInput(new Qt3DInput::QButtonAxisInput())
  , m_keyboardDevice(new Qt3DInput::QKeyboardDevice())
  , m_mouseDevice(new Qt3DInput::QMouseDevice())
  , m_logicalDevice(new Qt3DInput::QLogicalDevice())
  , m_frameAction(new Qt3DLogic::QFrameAction())
{
//    object picker for terrain for correct map panning. it will be associated as a component of terrain entity
//    m_terrainPicker = new Qt3DRender::QObjectPicker;
//    connect(m_terrainPicker, &Qt3DRender::QObjectPicker::pressed, this, &CameraController::onPickerMousePressed);

    // Left Mouse Button Action
    m_leftMouseButtonInput->setButtons(QVector<int>() << Qt::LeftButton);
    m_leftMouseButtonInput->setSourceDevice(m_mouseDevice);
    m_leftMouseButtonAction->addInput(m_leftMouseButtonInput);

    // Right Mouse Button Action
    m_rightMouseButtonInput->setButtons(QVector<int>() << Qt::RightButton);
    m_rightMouseButtonInput->setSourceDevice(m_mouseDevice);
    m_rightMouseButtonAction->addInput(m_rightMouseButtonInput);

    // Alt Keys
    m_altButtonInput->setButtons(QVector<int>() << Qt::Key_Alt);
    m_altButtonInput->setSourceDevice(m_keyboardDevice);
    m_altButtonAction->addInput(m_altButtonInput);

    // Shift Keys
    m_shiftButtonInput->setButtons(QVector<int>() << Qt::Key_Shift);
    m_shiftButtonInput->setSourceDevice(m_keyboardDevice);
    m_shiftButtonAction->addInput(m_shiftButtonInput);

    // Mouse X
    m_mouseRxInput->setAxis(Qt3DInput::QMouseDevice::X);
    m_mouseRxInput->setSourceDevice(m_mouseDevice);
    m_rxAxis->addInput(m_mouseRxInput);

    // Mouse Y
    m_mouseRyInput->setAxis(Qt3DInput::QMouseDevice::Y);
    m_mouseRyInput->setSourceDevice(m_mouseDevice);
    m_ryAxis->addInput(m_mouseRyInput);

    // Mouse Wheel X
    m_mouseTzXInput->setAxis(Qt3DInput::QMouseDevice::WheelX);
    m_mouseTzXInput->setSourceDevice(m_mouseDevice);
    m_tzAxis->addInput(m_mouseTzXInput);

    // Mouse Wheel Y
    m_mouseTzYInput->setAxis(Qt3DInput::QMouseDevice::WheelY);
    m_mouseTzYInput->setSourceDevice(m_mouseDevice);
    m_tzAxis->addInput(m_mouseTzYInput);

    // Keyboard Pos Tx (Key Right)
    m_keyboardTxPosInput->setButtons(QVector<int>() << Qt::Key_Right);
    m_keyboardTxPosInput->setScale(1.0f);
    m_keyboardTxPosInput->setSourceDevice(m_keyboardDevice);
    m_txAxis->addInput(m_keyboardTxPosInput);

    // Keyboard Pos Tz (Key Page Up)
    m_keyboardTzPosInput->setButtons(QVector<int>() << Qt::Key_PageUp);
    m_keyboardTzPosInput->setScale(1.0f);
    m_keyboardTzPosInput->setSourceDevice(m_keyboardDevice);
    m_tzAxis->addInput(m_keyboardTzPosInput);

    // Keyboard Pos Ty (Key Up)
    m_keyboardTyPosInput->setButtons(QVector<int>() << Qt::Key_Up);
    m_keyboardTyPosInput->setScale(1.0f);
    m_keyboardTyPosInput->setSourceDevice(m_keyboardDevice);
    m_tyAxis->addInput(m_keyboardTyPosInput);

    // Keyboard Pos Tx (Key Left)
    m_keyboardTxNegInput->setButtons(QVector<int>() << Qt::Key_Left);
    m_keyboardTxNegInput->setScale(-1.0f);
    m_keyboardTxNegInput->setSourceDevice(m_keyboardDevice);
    m_txAxis->addInput(m_keyboardTxNegInput);

    // Keyboard Pos Tz (Key Page Down)
    m_keyboardTzNegInput->setButtons(QVector<int>() << Qt::Key_PageDown);
    m_keyboardTzNegInput->setScale(-1.0f);
    m_keyboardTzNegInput->setSourceDevice(m_keyboardDevice);
    m_tzAxis->addInput(m_keyboardTzNegInput);

    // Keyboard Pos Ty (Key Down)
    m_keyboardTyNegInput->setButtons(QVector<int>() << Qt::Key_Down);
    m_keyboardTyNegInput->setScale(-1.0f);
    m_keyboardTyNegInput->setSourceDevice(m_keyboardDevice);
    m_tyAxis->addInput(m_keyboardTyNegInput);

    // Logical Device
    m_logicalDevice->addAction(m_leftMouseButtonAction);
    m_logicalDevice->addAction(m_rightMouseButtonAction);
    m_logicalDevice->addAction(m_altButtonAction);
    m_logicalDevice->addAction(m_shiftButtonAction);
    m_logicalDevice->addAxis(m_rxAxis);
    m_logicalDevice->addAxis(m_ryAxis);
    m_logicalDevice->addAxis(m_txAxis);
    m_logicalDevice->addAxis(m_tyAxis);
    m_logicalDevice->addAxis(m_tzAxis);

    // FrameAction
    connect(m_frameAction, &Qt3DLogic::QFrameAction::triggered, this, &CameraController::onTriggered);

    // Disable the logical device when the entity is disabled
    connect(this, &Qt3DCore::QEntity::enabledChanged, m_logicalDevice, &Qt3DInput::QLogicalDevice::setEnabled);

    this->addComponent(m_frameAction);
    this->addComponent(m_logicalDevice);
}

CameraController::~CameraController()
{
}

Qt3DRender::QCamera *CameraController::camera() const
{
    return m_camera;
}

float CameraController::linearSpeed() const
{
    return m_linearSpeed;
}

float CameraController::lookSpeed() const
{
    return m_lookSpeed;
}

float CameraController::zoomInLimit() const
{
    return m_zoomInLimit;
}

void CameraController::setCamera(Qt3DRender::QCamera *camera)
{
    m_camera = camera;
}

void CameraController::setLinearSpeed(float linearSpeed)
{
    m_linearSpeed = linearSpeed;
}

void CameraController::setLookSpeed(float lookSpeed)
{
    m_lookSpeed = lookSpeed;
}

void CameraController::setZoomInLimit(float zoomInLimit)
{
    m_zoomInLimit = zoomInLimit;
}

float clampInputs(float input1, float input2)
{
    float axisValue = input1 + input2;
    return (axisValue < -1) ? -1 : (axisValue > 1) ? 1 : axisValue;
}

void CameraController::onTriggered(float dt)
{
    if (m_camera == nullptr) {
        return;
    }

    if (m_leftMouseButtonAction->isActive()) {
        if (m_rightMouseButtonAction->isActive()) {
            // TODO:
        } else {
            // Yaw Pitch
            qDebug() << "yP";
            m_camera->translate(QVector3D(clampInputs(m_rxAxis->value(), m_txAxis->value()) * m_linearSpeed, clampInputs(m_ryAxis->value(), m_tyAxis->value()) * m_linearSpeed, 0) * dt);
        }
    } else if (m_rightMouseButtonAction->isActive()) {
        // Panning
        qDebug() << "P";
        m_cameraData.bearing = m_cameraData.bearing + (m_rxAxis->value() * m_lookSpeed) * dt;
        m_cameraData.tilt = m_cameraData.tilt + (m_ryAxis->value() * m_lookSpeed) * dt;
    }

    if (m_altButtonAction->isActive()) {
        m_cameraData.bearing = m_cameraData.bearing + (m_rxAxis->value() * m_lookSpeed) * dt;
        m_cameraData.tilt = m_cameraData.tilt + (m_ryAxis->value() * m_lookSpeed) * dt;
    } else if (m_shiftButtonAction->isActive()) {
        // TODO:
    } else {
        m_camera->translate(QVector3D(clampInputs(m_leftMouseButtonAction->isActive() ? m_rxAxis->value() : 0, m_txAxis->value()) * m_linearSpeed,
                                      clampInputs(m_leftMouseButtonAction->isActive() ? m_ryAxis->value() : 0, m_tyAxis->value()) * m_linearSpeed,
                                      m_tzAxis->value() * m_linearSpeed) * dt);
    }
}
