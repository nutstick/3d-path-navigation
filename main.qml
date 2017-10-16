import QtQuick 2.7
import QtQuick.Window 2.3
import QtQuick.Scene3D 2.0

Window {
    title: "test"
    width: 1280
    height: 768
    visible: true

    Scene3D {
        anchors.fill: parent
        aspects: ["render", "logic", "input"]

        Map3D { id: map }
    }

}
