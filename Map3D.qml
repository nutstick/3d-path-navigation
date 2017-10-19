import QtQuick 2.0 as QQ2
import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Logic 2.0
import Qt3D.Extras 2.0

import qt.nutstick.map 1.0
import qt.nutstick.tile.entity 1.0

Entity {
    id: sceneRoot

    Camera {
        id: camera
        projectionType: CameraLens.PerspectiveProjection
        fieldOfView: 45
        aspectRatio: width / height
        nearPlane : 0.1
        farPlane : 1000.0
        position: Qt.vector3d( 0.0, 40.0, 40.0 )
        upVector: Qt.vector3d( 0.0, 1.0, 0.0 )
        viewCenter: Qt.vector3d( 0.0, 0.0, 0.0 )
    }

    OrbitCameraController {
        camera: camera
    }

    components: [
        RenderSettings {
            activeFrameGraph: ForwardRenderer {
                clearColor: Qt.rgba(0, 0.5, 1, 1)
                camera: camera
            }
        },
        // Event Source will be set by the Qt3DQuickWindow
        InputSettings { }
    ]

    /* PhongMaterial {
        id: material
    }

    PlaneMesh {
        id: planeMesh
        width: 10.0
        height: 10.0
    }

    Transform {
        id: torusTransform
        matrix: {
            var m = Qt.matrix4x4();
            m.translate(Qt.vector3d(10, 0, 0));
            return m;
        }
    }

    Entity {
        id: torusEntity
        components: [ planeMesh, material, torusTransform ]
    } */

//    SphereMesh {
//        id: sphereMesh
//        radius: 3
//    }

//    Transform {
//        id: sphereTransform
//        property real userAngle: 0.0
//        matrix: {
//            var m = Qt.matrix4x4();
//            m.rotate(userAngle, Qt.vector3d(0, 1, 0));
//            m.translate(Qt.vector3d(20, 0, 0));
//            return m;
//        }
//    }

//    QQ2.NumberAnimation {
//        target: sphereTransform
//        property: "userAngle"
//        duration: 10000
//        from: 0
//        to: 360

//        loops: QQ2.Animation.Infinite
//        running: true
//    }

//    Entity {
//        id: sphereEntity
//        components: [ sphereMesh, material, sphereTransform ]
//    }

    Map {
        id: mm
    }

    TileEntity {
        id: t
    }
}
