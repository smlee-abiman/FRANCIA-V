import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

Rectangle {
    id: stringCell
    property alias stringCell: stringCell
    property alias stringEdit: stringEdit
    property alias text: stringEdit.text
    property alias readOnly: stringEdit.readOnly
    Layout.minimumWidth: (stringEdit.font.pixelSize / 2 + stringEdit.font.wordSpacing) * stringEdit.maximumLength
                                    // Different from expectations. set from upper element.
    width: Layout.minimumWidth
    //height: 28
    height: (stringEdit.font.pixelSize + 3 * 2)
    border.width: 1
    color: "transparent"
    clip: true

    MouseArea {
        id: ma
        anchors.fill: parent
        hoverEnabled: true      // necessary for ToolTip
        TextInput {
            id: stringEdit
            text: modelData.value
            //focus: true
            selectByMouse: true
            anchors.right: parent.right
            anchors.rightMargin: 3
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin: 3
            anchors.top: parent.top     // Set from upper element
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 14
            cursorVisible: activeFocus
            maximumLength: 6

            ToolTip.visible: ma.containsMouse
            ToolTip.delay: 200
            ToolTip.timeout: 3000
            ToolTip.text: modelData.label

            onTextChanged: {
                if (text !== modelData.value) {
                    modelData.value = text;
                }
            }
        }
    }

}

