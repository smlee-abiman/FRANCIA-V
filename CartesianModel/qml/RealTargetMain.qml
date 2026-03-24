import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    width: 1024
    height: 768
    visible: true
    color: "black"
    title: qsTr("3G Pendent Demo")

    MasterView {
    }

    Component.onCompleted: {
        platformHelpers.notifyGuiReady();
    }
}
