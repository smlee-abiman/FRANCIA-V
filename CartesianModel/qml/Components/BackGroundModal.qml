import QtQuick 2.12
import QtQml 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import Enums 1.0
import "../Widget"

Rectangle{
    width: 1024
    height: 768
    radius: 2
    anchors.top:parent.top
    anchors.topMargin:-217
    anchors.left:parent.left
    anchors.leftMargin:-9
    color: "gray"
    border.color: "gray"
    border.width: 1
    opacity: 0.3

    MouseArea{
        anchors.fill: parent
        onPressed: {
        }
        onReleased: {
        }
        onClicked: {
        }
    }
}
