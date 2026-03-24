import QtQuick 2.0
import QtQuick.Window 2.12

Window {
    width: 1024
    height: 768
    visible: true
    title: qsTr("ITZ3000 Demo")

    MasterView {
        anchors.fill: parent
    }
}
