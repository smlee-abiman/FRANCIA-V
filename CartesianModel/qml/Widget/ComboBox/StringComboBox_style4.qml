import QtQuick 2.12
import QtQml 2.3
import QtQuick.Controls 2.12
/*************************************************************************************
* Title: UI/UX of AEK
* File: StringComboBox_style4.qml
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.16
* Worker: SH.HONG
**************************************************************************************/

ComboBox {
    id: root
    width: parent.width
    height: parent.height
    font.pixelSize: root.fontSize
    model: comboBoxMode

    property color bgRectColor: "#ffffff"
    property color bgIndicatorColor: "#cccccc"
    property color bgRectBorderColor: "#999999"
    property color bgPopupColor: "#ffffff"
    property int bgRectBorderWidth: 1
    property int bgPopupBorderWidth: 1
    property int bgRectRadius: 0
    property int fontSize: 20
    property var comboBoxMode: null
    property int popupDelegateNum: 8
    property int popupHeight: root.height * popupDelegateNum + root.bgRectBorderWidth
    property int resourceGroupId: 0

    QtObject {
        id: local
        property int currentListIndex: 0

        // 📌 페이지 단위로 이전 항목 보기
        function prevPage() {
            // currentListIndex를 한 페이지 크기만큼 감소시키며 0 이하로 내려가지 않도록 제한
            currentListIndex = Math.max(currentListIndex - popupDelegateNum, 0)
            dropDownList.positionViewAtIndex(currentListIndex, ListView.Beginning)
        }

        // 📌 페이지 단위로 다음 항목 보기
        function nextPage() {
            // currentListIndex를 한 페이지 크기만큼 증가시키며 최대 값 제한
            currentListIndex = Math.min(currentListIndex + popupDelegateNum,
                                        root.delegateModel.count - popupDelegateNum)
            dropDownList.positionViewAtIndex(currentListIndex, ListView.Beginning)
        }
    }

    background: Rectangle {
        width: root.width
        height: root.height
        border.color: root.bgRectBorderColor
        border.width: root.bgRectBorderWidth
        radius: root.bgRectRadius
        color: root.bgRectColor
    }

    indicator: Rectangle {
        y: 1
        id: indicator
        anchors.right: parent.right
        anchors.rightMargin: 1
        height: parent.height - 2
        width: root.width / 5
        border.color: root.bgRectBorderColor
        border.width: root.bgRectBorderWidth
        color: root.bgIndicatorColor

        Column {
            anchors.centerIn: parent
            Triangle {
                width: 22
                height: 15
                triangleColor: root.popup.opened ? "#cc3333" : "white"
            }
        }
    }

    contentItem: Text {
        id: rootText
        width: root.width - indicator.width
        height: parent.height
        verticalAlignment: Text.AlignVCenter
        anchors.left: parent.left
        anchors.leftMargin: 8
        font.pixelSize: root.fontSize
        elide: Text.ElideRight
        color: resourceManager.dataColor("16", 174)
        text: resourceManager.label(resourceGroupId, comboBoxMode[currentIndex].labelIndex, resourceManager.language)
    }

    popup: Popup {
        width: parent.width
        implicitHeight: popupHeight
        y: parent.height - root.bgRectBorderWidth
        padding: 1

        background: Rectangle {
            width: parent.width
            height: parent.height
            color: root.bgPopupColor
            border.color: root.bgRectBorderColor
            border.width: root.bgRectBorderWidth
        }

        contentItem: ListView {
            id: dropDownList
            width: parent.width - indicator.width
            height: popupHeight
            clip: true
            model: root.popup.visible ? root.delegateModel : null
            currentIndex: root.currentIndex
            boundsBehavior: Flickable.StopAtBounds
            interactive: false
        }

        Rectangle {
            width: indicator.width
            height: parent.height
            anchors.right: parent.right
            anchors.rightMargin: -1
            border.color: root.bgRectBorderColor
            border.width: root.bgRectBorderWidth
            color: root.bgRectColor

            // ↑ 위로 스크롤
            Rectangle {
                width: indicator.width
                height: indicator.width
                color: upmouse.pressed ? "#cc3333" : "#cccccc"
                border.color: root.bgRectBorderColor
                border.width: root.bgRectBorderWidth

                Image {
                    anchors.centerIn: parent
                    source: upmouse.pressed ? resourceManager.imagePath + "/00_COMMON/d000_70.png"
                                             : resourceManager.imagePath + "/00_COMMON/d000_69.png"
                }

                MouseArea {
                    id: upmouse
                    anchors.fill: parent
                    onPressed: {
                        local.prevPage()  // 📌 페이지 단위로 이전 보기
                    }
                    onClicked: phyKeyManager.ctrlBuzzer()
                }
            }

            // ↓ 아래로 스크롤
            Rectangle {
                width: indicator.width
                height: indicator.width
                color: downmouse.pressed ? "#cc3333" : "#cccccc"
                border.color: root.bgRectBorderColor
                border.width: root.bgRectBorderWidth
                anchors.bottom: parent.bottom

                Image {
                    anchors.centerIn: parent
                    source: downmouse.pressed ? resourceManager.imagePath + "/00_COMMON/d000_72.png"
                                               : resourceManager.imagePath + "/00_COMMON/d000_71.png"
                }

                MouseArea {
                    id: downmouse
                    anchors.fill: parent
                    onPressed: {
                        local.nextPage()  // 📌 페이지 단위로 다음 보기
                    }
                }
            }
        }
    }

    delegate: Rectangle {
        width: root.width - indicator.width
        height: root.height
        color: "#ffffff"
        border.width: root.bgPopupBorderWidth
        border.color: root.bgRectBorderColor

        Text {
            width: parent.width - 8
            height: parent.height
            verticalAlignment: Text.AlignVCenter
            anchors.left: parent.left
            anchors.leftMargin: 8
            font.pixelSize: root.fontSize
            elide: Text.ElideRight
            text: resourceManager.label(resourceGroupId, modelData.labelIndex, resourceManager.language)
        }

        MouseArea {
            anchors.fill: parent
            onPressed: parent.color = "#cc3333"
            onReleased: parent.color = "#ffffff"
            onClicked: {
                currentIndex = index
                popup.close()
            }
        }
    }
}
