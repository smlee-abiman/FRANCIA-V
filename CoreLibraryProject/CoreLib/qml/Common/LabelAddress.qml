import QtQuick 2.5
import QtQuick.Layouts 1.3

Rectangle {
    width: 58
    height: 30
    border.width: 0
    property alias text: labelAddress.text
    property alias font: labelAddress.font
    TextEdit {
        id: labelAddress
        text: "0000:"
        anchors.rightMargin: 4
        anchors.leftMargin: 4
        anchors.bottomMargin: 2
        anchors.topMargin: 2
        anchors.fill: parent
        readOnly: true
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 14
        color: "blue"

        //Component.onCompleted: {
        //}
    }
}
