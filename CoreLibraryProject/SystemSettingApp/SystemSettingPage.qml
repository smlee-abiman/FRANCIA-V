import QtQml 2.12
import QtQuick 2.12
import QtQuick.Shapes 1.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

Item {
    anchors.fill: parent

    signal componentDestructed(string source)
    signal folderSelected(int index)
    signal folderOpen(string folderPath, int depth)
    signal fileSelected(int index)
    signal childFolderOpened(int index)
    signal showColorPattern()                                                // dummy. not use
    signal closeColorPattern()                                               // dummy. not use
    signal inputTestExecute()                                                // dummy. not use
    signal outputTestExecute()                                               // dummy. not use
    signal retClicked(string ipAddr, string subnetMask)                      // dummy. not use
    signal calibrationClicked()
    signal testAndSetClicked()
    signal executeClicked(string currentPath)
    signal directionClicked()
    signal createFolderClicked()
    signal eraceFolderClicked()
    signal displaySelectClicked()
    signal touchClicked()
    signal exitTouch()                                                       // dummy. not use
    signal backLightCountDown()                                              // dummy. not use
    signal backLightCountUp()                                                // dummy. not use

    Component.onDestruction: {
        componentDestructed("SystemSettingPage.qml")
    }

    Text {
        id: folderTitle
        width: 256
        height: 22
        text: qsTr("folder")
        horizontalAlignment: Text.AlignHCenter
    }

    ListView {
        id: folderList
        width: 256
        height: 550
        anchors.top: folderTitle.bottom
        anchors.left: folderTitle.left
        interactive: false
        clip: true
        model: folderListModel
        delegate: Rectangle {
            id: folderNameRow
            readonly property variant myData: model
            readonly property string textColor: ListView.isCurrentItem ? "white" : "black"
            width: ListView.view.width
            height: 22
            color: ListView.isCurrentItem ? "black" : "white"
            Row {
                Item {
                    width: 22; height: width
                    anchors.verticalCenter: parent.verticalCenter
                    Text {
                        anchors.centerIn: parent
                        text: qsTr("*")
                        color: textColor
                        visible: selected
                    }
                }
                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    text: dirName
                    color: textColor
                    elide: Text.ElideRight
                    leftPadding: depth * 10
                }
            }
        }
    }

    Row {
        anchors.top: folderList.bottom
        anchors.topMargin: 19
        anchors.left: folderList.left
        anchors.leftMargin: 59
        spacing: 50

        ControlPanel {
            id: folderControllPanel
            title: "folder"
            onPrevClicked: {
                if(folderList.currentIndex > 0) {
                    folderList.decrementCurrentIndex()
                }
            }
            onNextClicked: {
                if(folderList.currentIndex < (folderList.count - 1)) {
                    folderList.incrementCurrentIndex()
                }
            }
            onEnterClicked: {
                if(folderList.currentItem.myData.selected) {
                    folderOpen(folderList.currentItem.myData.dirPath, folderList.currentItem.myData.depth)
                } else {
                    folderSelected(folderList.currentIndex)
                }
            }
        }
        ControlPanel {
            id: fildeControllPanel
            title: "file"
            onPrevClicked: {
                if(fileNameList.currentIndex > 0) {
                    fileNameList.decrementCurrentIndex()
                    usbFileList.decrementCurrentIndex()
                    controllerFileList.decrementCurrentIndex()
                }
            }
            onNextClicked: {
                if(fileNameList.currentIndex < (fileNameList.count - 1)) {
                    fileNameList.incrementCurrentIndex()
                    usbFileList.incrementCurrentIndex()
                    controllerFileList.incrementCurrentIndex()
                }
            }
            onEnterClicked: {
                if(usbFileList.currentItem.myData.isDirectory) {
                    childFolderOpened(usbFileList.currentIndex)
                } else {
                    fileSelected(usbFileList.currentIndex)
                }
            }
        }
    }

    Text {
        id: usbTitle
        width: 256
        height: 22
        anchors.top: parent.top
        anchors.left: folderTitle.right
        text: qsTr("USB")
        horizontalAlignment: Text.AlignHCenter
    }

    ListView {
        id: usbFileList
        width: parent.width / 4
        height: 574
        anchors.top: usbTitle.bottom
        anchors.left: usbTitle.left
        interactive: false
        model: sortFilterFileListModel
        clip: true
        delegate: Component {
            Rectangle {
                property variant myData: model
                id: usbFileRow
                width: 256
                height: 22
                color: (nowCopying || ListView.isCurrentItem) ? "black" : "white"
                Text {
                    text: {
                        if(!isDirectory) {
                            (selected == true) && (systemSettingManager.isCopyToUSB == false)
                              ? Qt.formatDateTime(usbLastModified, "yyyy/MM/dd hh:mm") + " *"
                              : Qt.formatDateTime(usbLastModified, "yyyy/MM/dd hh:mm")
                        } else {
                            name
                        }
                    }
                    color: usbFileRow.ListView.isCurrentItem ? "white" : "black"
                    elide: Text.ElideRight
                    horizontalAlignment: Text.AlignLeft
                }
            }
        }
    }

    Text {
        id: copyDirection
        width: 256
        height: 22
        anchors.top: parent.top
        anchors.left: usbTitle.right
        text: systemSettingManager.copyDirection
        horizontalAlignment: Text.AlignHCenter
    }

    ListView {
        id: fileNameList
        width: parent.width / 4
        height: 574
        anchors.top: copyDirection.bottom
        anchors.left: copyDirection.left
        interactive: false
        model: sortFilterFileListModel
        clip: true
        delegate: Component {
            Rectangle {
                id: fileNameRow
                width: 256
                height: 22
                color: (nowCopying || ListView.isCurrentItem) ? "black" : "white"
                Text {
                    anchors.fill: parent
                    text: (!isDirectory) ? name : ""
                    color: (nowCopying || fileNameRow.ListView.isCurrentItem) ? "white" : "black"
                    elide: Text.ElideRight
                    horizontalAlignment: Text.AlignLeft
                }
            }
        }
    }

    Text {
        id: controllerTitle
        width: 256
        height: 22
        anchors.top: parent.top
        anchors.left: copyDirection.right
        text: qsTr("controller")
        horizontalAlignment: Text.AlignHCenter
    }

    ListView {
        id: controllerFileList
        width: parent.width / 4
        height: 574
        anchors.top: controllerTitle.bottom
        anchors.left: controllerTitle.left
        interactive: false
        model: sortFilterFileListModel
        clip: true
        delegate: Component {
            Rectangle {
                id: controllerFileRow
                width: 256
                height: 22
                color: (nowCopying || ListView.isCurrentItem) ? "black" : "white"
                Text {
                    text: {
                        if(!isDirectory) {
                            (selected == true) && (systemSettingManager.isCopyToUSB == true)
                              ? Qt.formatDateTime(controllerLastModified, "yyyy/MM/dd hh:mm") + " *"
                              : Qt.formatDateTime(controllerLastModified, "yyyy/MM/dd hh:mm")
                        } else {
                            ""
                        }
                    }
                    color: controllerFileRow.ListView.isCurrentItem ? "white" : "black"
                    elide: Text.ElideRight
                    horizontalAlignment: Text.AlignLeft
                }
            }
        }
    }

    Button {
        id: calibrationButton
        width: 179
        height: 71
        anchors.top: usbFileList.bottom
        anchors.topMargin: 10
        anchors.left: usbFileList.left
        anchors.leftMargin: 10
        text: qsTr("calibration")
        onClicked: { calibrationClicked() }
    }

    Button {
        id: testAndSetButton
        width: 179
        height: 71
        anchors.top: calibrationButton.top
        anchors.left: calibrationButton.right
        anchors.leftMargin: 10
        text: qsTr("test&set")
        onClicked: { testAndSetClicked() }
    }

    Button {
        id: executeButton
        width: 179
        height: 71
        anchors.top: testAndSetButton.top
        anchors.left: testAndSetButton.right
        anchors.leftMargin: 10
        text: qsTr("execute")
        onClicked: { executeClicked(folderList.currentItem.myData.dirPath) }
    }

    Button {
        id: directionButton
        width: 179
        height: 71
        anchors.top: executeButton.top
        anchors.left: executeButton.right
        anchors.leftMargin: 10
        text: qsTr("direction")
        onClicked: { directionClicked() }
    }

    Button {
        id: createFolderButton
        width: 179
        height: 71
        anchors.top: calibrationButton.bottom
        anchors.topMargin: 10
        anchors.left: calibrationButton.left
        text: qsTr("create_folder")
        onClicked: { createFolderClicked() }

    }

    Rectangle {
        id: eraseFolderButton
        width: 179
        height: 71
        anchors.top: createFolderButton.top
        anchors.left: createFolderButton.right
        anchors.leftMargin: 10
        color: systemSettingManager.selectdTopDirName.length === 0 ? "gray" : "#e0e0e0"
        enabled: systemSettingManager.selectdTopDirName.length === 0 ? false : true

        Text {
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("erase_folder")
        }

        TapHandler {
            onTapped: { folderEraceConfirmMessage.open() }
        }
    }

    Button {
        id: displaySelectButton
        width: 179
        height: 71
        anchors.top: eraseFolderButton.top
        anchors.left: eraseFolderButton.right
        anchors.leftMargin: 10
        text: qsTr("display_select")
        onClicked: { displaySelectClicked() }
    }

    Button {
        id: touchButton
        width: 179
        height: 71
        anchors.top: displaySelectButton.top
        anchors.left: displaySelectButton.right
        anchors.leftMargin: 10
        text: qsTr("touch")
        onClicked: { touchClicked() }
    }

    Popup {
        id: folderCreateErrorMessage
        width: 384
        height: 128
        anchors.centerIn: parent
        closePolicy: "NoAutoClose"
        modal: true

        Text {
            id: errorMessage
            height: (parent.height / 2) - 20
            x: (parent.width - contentWidth) / 2
            y: 20
            text: qsTr("It cannot be executed.\nPlease execute again after deleting the folder.")
        }

        Button {
            width: 80
            height: 22
            anchors.top: errorMessage.bottom
            anchors.topMargin: 20
            anchors.right: errorMessage.right
            anchors.rightMargin: 50
            text: qsTr("OK")
            onClicked: { folderCreateErrorMessage.close() }
        }
    }

    Popup {
        id: usbNotConnectedMessagePopup
        width: 384
        height: 128
        anchors.centerIn: parent
        closePolicy: "NoAutoClose"
        modal: true

        Text {
            id: usbNotConnectedMessage
            height: (parent.height / 2) - 20
            x: (parent.width - contentWidth) / 2
            y: 20
            text: qsTr("USB drive not found.")
        }

        Button {
            width: 80
            height: 22
            anchors.top: usbNotConnectedMessage.bottom
            anchors.topMargin: 20
            anchors.right: usbNotConnectedMessage.right
            anchors.rightMargin: 50
            text: qsTr("OK")
            onClicked: { usbNotConnectedMessagePopup.close() }
        }
    }

    Connections {
        target: systemSettingManager
        onFolderCreateFailed: { folderCreateErrorMessage.open() }
        onIsUSBConnectedChanged: {
            if(systemSettingManager.isUSBConnected == false) {
                usbNotConnectedMessagePopup.open()
            } else {
                if(usbNotConnectedMessagePopup.opened === true) {
                    usbNotConnectedMessagePopup.close()
                }
            }
        }
    }

    Popup {
        id: folderEraceConfirmMessage
        anchors.centerIn: parent
        closePolicy: Popup.NoAutoClose
        modal: true
        leftPadding: 30
        rightPadding: 30

        contentItem: GridLayout {
            columns: 3
            rows: 2
            columnSpacing: 10
            rowSpacing: 10

            Text {
                Layout.row: 0; Layout.column: 0; Layout.columnSpan: 2
                Layout.fillHeight: true
                Layout.fillWidth: true
                text: qsTr("[") + systemSettingManager.selectdTopDirName + qsTr("]\nDelete the folder on USB.\nYES: execute deletion        NO: cancel            ")
            }

            Button {
                Layout.row: 1; Layout.column: 0
                Layout.fillHeight: true
                Layout.fillWidth: true
                text: qsTr("YES")
                onClicked: { eraceFolderClicked(); folderEraceConfirmMessage.close() }
            }

            Button {
                Layout.row: 1; Layout.column: 1
                Layout.fillHeight: true
                Layout.fillWidth: true
                text: qsTr("NO")
                onClicked: { folderEraceConfirmMessage.close() }
            }
        }
    }
}
