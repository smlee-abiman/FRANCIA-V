import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import "../ComboboxEx"


Item {
    id: element1
    height: 450
    property alias scrollView: scrollView
    clip: true
    property alias comboBoxCategory: comboBoxCategory
    property alias gridBytes: gridBytes
    property alias repeaterNumberEdit: repeaterNumberEdit
    width: 630

    Item {
        id: rowHeader
        x: 0
        y: 0
        width: 608
        height: 27
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.top: parent.top
        anchors.topMargin: 4

        Text {
            id: element
            text: qsTr("カテゴリ： ")
            font.pointSize: 12
            verticalAlignment: Text.AlignVCenter
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 2
        }

        ComboboxEx {
            id: comboBoxCategory
            anchors.left: element.right
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            anchors.leftMargin: 61
            clip: false
            hoverEnabled: true
            model: (dbgManager !== null) ? dbgManager.byteTypeCategoryGroupCapt : null;
        }
    }

    ScrollView {
        id: scrollView
        focusPolicy: Qt.NoFocus
        enabled: true
        contentHeight: -1      // set event handler
        contentWidth: -1
        anchors.rightMargin: 5
        anchors.leftMargin: 5
        anchors.bottomMargin: 4
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.topMargin: 6
        anchors.top: rowHeader.bottom
        anchors.bottom: parent.bottom
        clip: true
        wheelEnabled: true

        GridLayout {
            id: gridBytes
            clip: true
            anchors.top: parent.top
            anchors.topMargin: 2
            anchors.rightMargin: 5
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.leftMargin: 2
            Layout.fillHeight: true
            columns: 9
            rowSpacing: 5
            columnSpacing: 5
            Repeater {
                id: repeaterNumberEdit
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                //Layout.margins: 1
                model: (dbgManager !== null) ? dbgManager.byteTypeListDisp : null
                delegate: delegateNumberCell
            }
        }
    }
}

/*##^##
Designer {
    D{i:1;anchors_width:613}D{i:4;anchors_height:200;anchors_width:200;anchors_x:20;anchors_y:62}
}
##^##*/
