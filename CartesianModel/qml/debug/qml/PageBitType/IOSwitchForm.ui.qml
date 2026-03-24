import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import "../LedSwitch"
import "../Common"

Item {
    id: ioSwitchForm
    //property alias ioSwitchForm: ioSwitchForm
    property alias scrollView: scrollView
    property alias gridLayoutAdr: gridLayoutAdr
    property alias repeaterAdr: repeaterAdr
    property alias gridLayoutIo: gridLayoutIo
    property alias repeaterIoSwitch: repeaterIoSwitch
    property alias rowBackTile: rowBackTile
    transformOrigin: Item.Center
    property bool usePopupTips: false
    scale: 1
    ScrollView {
        id: scrollView
        //contentHeight: -1
        anchors.fill: parent
        clip: true
        wheelEnabled: true

        GridLayout {
            id: gridLayoutAdr
            columnSpacing: 1
            rowSpacing: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 4
            anchors.top: parent.top
            anchors.topMargin: -5
            Layout.fillWidth: true
            Layout.fillHeight: true
            transformOrigin: Item.Center
            flow: GridLayout.LeftToRight
            columns: 1
            Repeater {
                id: repeaterAdr
                model: 16
                delegate: Column {
                    id: rowAdr
                    property alias textAdr: textAdr
                    Rectangle {
                        id: rectAdr
                        width: 40
                        height: 21
                        border {
                            color: "blue"
                            width: 0
                        }
                        LabelAddress {
                            id: textAdr
                            text: "0000:"
                            //font.pixelSize: 14
                            font.pointSize: 11
                        }
                    }
                }
            }
        }

        Row {
            id: rowBackTile
            anchors.right: parent.right
            anchors.rightMargin: 4
            anchors.left: gridLayoutAdr.right
            anchors.leftMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0

            Repeater {
                model: 2
                Repeater {
                    model: ["beige", "lavender"]
                    Rectangle {
                        height: parent.height
                        width: parent.width / 4
                        color: modelData
                    }
                }
            }
        }

        // NOTE: Do not use GridLayout because drawing is slow.
        Grid　{
            id: gridLayoutIo
            visible: false
            width: 495
            scale: 1
            property bool checkable: true
            y: 2   // set upper component
            anchors.right: parent.right
            anchors.rightMargin: 4
            anchors.left: gridLayoutAdr.right
            anchors.leftMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            //Layout.fillWidth: true
            Layout.fillHeight: true
            transformOrigin: Item.Center
            flow: Grid.LeftToRight
            columns: 16
            Repeater {
                id: repeaterIoSwitch
                Layout.margins: 1
                //model: dbgManager.fooList    // Set from upper element
                delegate: LedSwitch {
                    _checkable: gridLayoutIo.checkable
                    usePopupTips: ioSwitchForm.usePopupTips
                }
            }
        }
    }
}


/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:4;anchors_height:20}D{i:6;anchors_height:426}
}
##^##*/
