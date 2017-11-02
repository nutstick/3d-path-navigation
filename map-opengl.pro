TEMPLATE = app

QT += qml quick \
	concurrent \
	3dcore 3drender 3dinput \
	3dquick 3dquickrender 3dquickinput 3dquickextras \
	network \
	widgets

CONFIG += resources_big c++11

SOURCES += main.cpp \
	maptiles/maptexturegenerator.cpp \
	maptiles/maptextureimage.cpp \
    utils/imagemanager.cpp \
	utils/networkmanager.cpp \
    cameracontroller.cpp \
	utils/sphericalmercator.cpp \
	mainwindow.cpp \
    entities/map.cpp \
    entities/terrain.cpp \
    meshes/tilemesh.cpp \
    entities/tile.cpp \
    scene3d.cpp

OTHER_FILES += \
	*.qml \
	glcode.js \
	sphericalmercator.js \
	shaders/gl3/*.* \
	shaders/es2/*.* \

RESOURCES += qml.qrc \
    utils.qrc \
    shaders.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
	maptiles/maptexturegenerator.h \
	maptiles/maptextureimage.h \
	utils/imagemanager.h \
	utils/networkmanager.h \
    cameracontroller.h \
    utils/sphericalmercator.h \
	mainwindow.h \
    entities/map.h \
    entities/terrain.h \
    meshes/tilemesh.h \
    entities/tile.h \
    scene3d.h
