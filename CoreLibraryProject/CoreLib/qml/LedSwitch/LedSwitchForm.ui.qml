import QtQuick 2.12
import QtQuick.Controls 2.5

Item {
    id: elemSwitch
    property alias elemSwitch: elemSwitch
    property alias switchPopupTrig: switchPopupTrig
    property alias switchBody: switchBody
    property bool _checkable
    property bool usePopupTips: false
    width: 21
    height: 21
    scale: 1.0

    MouseArea {
        id: switchPopupTrig
        anchors.fill: parent
        hoverEnabled: usePopupTips
        Button {
            id: switchBody
            anchors.fill: parent
            display: AbstractButton.IconOnly
            text: ""
            clip: true
            checkable: _checkable
            background: LedSwitchLook {
                id: rectLed
                //lightOn: ("0" !== modelData.value) ? true : false
            }
            hoverEnabled: !usePopupTips
            ToolTip.visible: hovered
            ToolTip.delay: 200
            ToolTip.timeout: 3000
            ToolTip.text: modelData.label
            /*
            ToolTip {
                // invalid
                leftMargin: 0
                leftInset: 0
                font.bold: true
                background: Rectangle {
                    anchors.fill: parent
                    color: "#FFFF33"
                    opacity: 0.75
                }
            }
            */
        }
    }
}
