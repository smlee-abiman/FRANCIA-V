import QtQuick 2.12
import QtQuick.Controls 2.5
import "../ComboboxEx"
//import QtQuick.Window 2.3
//import QtQuick.Layouts 1.3
//import QtQuick.Controls.Styles 1.4

Item {
    id: _root
    height: 453
    property alias comboBoxCategory: comboBoxCategory
    property alias txtCategoryCapt: txtCategoryCapt
    property alias iOSwitchForm: iOSwitchForm
    property alias rectReadOnly: rectReadOnly

    Item {
        id: rowHeader
        x: 0
        y: 0
        width: 310
        height: 27
        anchors.left: parent.left
        anchors.leftMargin: 3
        anchors.top: parent.top
        anchors.topMargin: 2

        Text {
            id: txtCategoryCapt
            text: qsTr("カテゴリ： ")        // Should be set from upper element
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
            anchors.rightMargin: 61
            anchors.left: txtCategoryCapt.right
            anchors.leftMargin: 61
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            model: (dbgManager !== null) ? dbgManager.ioCategoryCapt : null;
        }

        Rectangle {
            id: rectReadOnly
            x: 0
            width: 50
            color: "lightcyan"
            border.color: "skyblue"
            border.width: 2
            anchors.left: comboBoxCategory.right
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            Text {
                id: txtReadOnly
                text: qsTr("R/O")
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors.fill: parent
                font.pointSize: 12
            }
        }
    }

    IOSwitch {
        id: iOSwitchForm
        anchors.right: parent.right
        anchors.rightMargin: 3
        anchors.left: parent.left
        anchors.leftMargin: 3
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: rowHeader.bottom
        anchors.topMargin: 6
        repeaterAdr.model: (dbgManager !== null) ? (dbgManager.ioListDisp.length / 16) : null
        repeaterIoSwitch.model: (dbgManager !== null) ? dbgManager.ioListDisp : null
        scrollView.contentHeight: (dbgManager !== null) ? (height * dbgManager.ioListDisp.length / 256) : 0
        //usePopupTips: true
    }
}
