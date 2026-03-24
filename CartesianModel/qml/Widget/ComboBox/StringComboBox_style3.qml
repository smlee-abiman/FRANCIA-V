import QtQuick 2.12
import QtQml 2.3
import QtQuick.Controls 2.12

/**********适用于有很多model项目的组合框[机种选择画面]***********/

ComboBox {
    id: root
    width: parent.width
    height: parent.height
    font.pixelSize: root.fontSize
    model: comboBoxMode

    property color bgRectColor: "#ffffff"
    property color bgIndicatorColor: "#ffffff"
    property color bgRectBorderColor: "#999999"
    property color bgPopupColor: "#ffffff"
    property int bgRectBorderWidth: 1
    property int bgPopupBorderWidth: 1
    property int bgRectRadius: 0
    property int fontSize: 20
    property var comboBoxMode: null
    property int popupDelegateNum: 8
    // PopupのcontentItemのheightをpopupのheightで参照しようとするとバインディングループになる
    property int popupHeight: root.height * popupDelegateNum + root.bgRectBorderWidth

    QtObject{
        id:local
        property int currentListIndex: 0
        function prevPage() {
            currentListIndex = Math.max(currentListIndex - 1, 0);
            dropDownList.positionViewAtIndex(currentListIndex, ListView.Beginning)
        }
        function nextPage() {
            currentListIndex = Math.min(currentListIndex + 1, root.delegateModel.count - popupDelegateNum);
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
    }//组合框背景

    indicator: Rectangle{
        id: indicator
        anchors.right: parent.right
        width: root.width / 5
        height: parent.height
        border.color: root.bgRectBorderColor
        border.width: root.bgRectBorderWidth
        color: root.bgIndicatorColor
        Column{
            anchors.centerIn: parent
            spacing: 5
            Triangle{
                width: 22
                height: 15
                rotation: 180
                triangleColor: root.popup.opened ? "#cc3333" : "white"
            }
            Triangle{
                width: 22
                height: 15
                triangleColor: root.popup.opened ? "#cc3333" : "white"
            }
        }
    }//组合框指示图标

    contentItem: Text {
        id: rootText
        width: root.width - indicator.width
        height: parent.height
        verticalAlignment: Text.AlignVCenter
        anchors.left: parent.left
        anchors.leftMargin: 8
        font.pixelSize: root.fontSize
        elide: Text.ElideRight
        color: (comboBoxMode[root.currentIndex] == comboBoxMode.modelName ? "black" : resourceManager.dataColor(18, 7))
        text: (root.currentIndex != -1 ? (comboBoxMode[root.currentIndex] ? comboBoxMode[root.currentIndex] : "") : "")
    }//组合框当前显示文字

    popup: Popup{
        width: parent.width
        implicitHeight: popupHeight
        y: parent.height - root.bgRectBorderWidth
        padding: 1
        background: Rectangle{
            width: parent.width
            height: parent.height
            color: root.bgPopupColor
            border.color: root.bgRectBorderColor
            border.width: root.bgRectBorderWidth
        }
        contentItem: ListView{
            id:dropDownList
            width: parent.width - indicator.width
            height: popupHeight
            clip: true
            model: root.popup.visible ? root.delegateModel : null
            currentIndex: root.currentIndex
            boundsBehavior: Flickable.StopAtBounds
            interactive: false
        }
        Rectangle{
            width: indicator.width
            height: parent.height
            anchors.right: parent.right
            anchors.rightMargin: -1
            border.color: root.bgRectBorderColor
            border.width: root.bgRectBorderWidth
            color: root.bgRectColor
            Rectangle{
                width: indicator.width
                height: indicator.width
                color: upmouse.pressed ? "#ffffff" : "#cccccc"
                border.color: root.bgRectBorderColor
                border.width: root.bgRectBorderWidth
                Image {
                    anchors.centerIn: parent
                    source: upmouse.pressed ? resourceManager.imagePath + "/00_COMMON/d000_70.png" : resourceManager.imagePath + "/00_COMMON/d000_69.png"
                }
                MouseArea{
                    id:upmouse
                    anchors.fill: parent
                    onPressed: {
                        local.prevPage()
                    }
                    onClicked: phyKeyManager.ctrlBuzzer()
                }
            }//上键
            Rectangle{
                width: indicator.width
                height: indicator.width
                color: downmouse.pressed ? "#ffffff" : "#cccccc"
                border.color: root.bgRectBorderColor
                border.width: root.bgRectBorderWidth
                anchors.bottom: parent.bottom
                Image {
                    anchors.centerIn: parent
                    source: downmouse.pressed ? resourceManager.imagePath + "/00_COMMON/d000_72.png" : resourceManager.imagePath + "/00_COMMON/d000_71.png"
                }
                MouseArea{
                    id:downmouse
                    anchors.fill: parent
                    onPressed: {
                        local.nextPage()
                    }
                    //onClicked: phyKeyManager.ctrlBuzzer()
                }
            }//下键
        }//换页按钮
    }//组合框下拉菜单

    delegate: Rectangle {
        width: root.width - indicator.width
        height: root.height
        color: "#E6F6FC"
        border.width: root.bgPopupBorderWidth
        border.color: root.bgRectBorderColor
        Text{
            width: parent.width - 8
            height: parent.height
            verticalAlignment: Text.AlignVCenter
            anchors.left: parent.left
            anchors.leftMargin: 8
            font.pixelSize: root.fontSize
            elide: Text.ElideRight
            text: modelData
        }
        MouseArea{
            anchors.fill: parent
            onPressed: parent.color = "#cc3333"
            onReleased: parent.color = "#ffffff"
            onClicked: {
                currentIndex = index
                popup.close()
                phyKeyManager.ctrlBuzzer()
            }
        }
    }
}
