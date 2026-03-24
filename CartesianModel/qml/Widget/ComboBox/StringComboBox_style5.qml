import QtQuick 2.12
import QtQml 2.3
import QtQuick.Controls 2.12
/*************************************************************************************
* Title: UI/UX of AEK
* File: StringComboBox_style5.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
 - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.16
* Worker: SH.HONG
**************************************************************************************/
/**********适用于有固定少量model项目的组合框[教导5-1]***********/
/**********适用于有固定少量model项目的组合框[教导5-1]***********/
ComboBox {
    id:root
    width: parent.width
    height: parent.height
    font.pixelSize: root.fontSize
    model: listModelData

    property color bgRectColor: "#ffffff"
    property color bgIndicatorColor: "#ffffff"
    property color bgRectBorderColor: "#999999"
    property color bgPopupColor: "#ffffff"
    property color displayTextColor: "#000000"
    property int bgRectBorderWidth: 1
    property int bgPopupBorderWidth: 1
    property int bgRectRadius: 0
    property int fontSize: 20
    property int popupDelegateNum: 0
    property int resourceGroupId: 0
    property int resourceStartIndex: 0
    // PopupのcontentItemのheightをpopupのheightで参照しようとするとバインディングループになる
    property int popupHeight: root.height * 5 - root.bgRectBorderWidth * 3

    QtObject{
        id:local
        property int currentListIndex: 0
        function prevPage() {
            currentListIndex = Math.max(currentListIndex - 1, 0);
            dropDownList.positionViewAtIndex(currentListIndex, ListView.Beginning)
        }
        function nextPage() {
            currentListIndex = Math.min(currentListIndex + 1, root.delegateModel.count - 5);
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
        y:1
        anchors.right: parent.right
        anchors.rightMargin:1
        width: root.width / 5
        height: parent.height -2
        border.color: root.bgRectBorderColor
        border.width: root.bgRectBorderWidth
        color: root.bgIndicatorColor
        Column{
            anchors.centerIn: parent
            // spacing: 5
            // Triangle{
            //     triangleColor: popup.opened ? " yellow" : "white"
            //     width: 22
            //     height: 15
            //     rotation: 180
            // }
            Triangle{
                triangleColor: popup.opened ? " #cc3333" : "white"
                width: 22
                height: 15
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
        color: root.displayTextColor
        text: root.displayText
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
            width: parent.width
            height: popupHeight
            clip: true
            spacing: -2
            model: root.popup.visible ? root.delegateModel : null
            currentIndex: root.highlightedIndex
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
                color: upmouse.pressed ? "#cc3333" : "#cccccc"
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
                color: downmouse.pressed ? "#cc3333" : "#cccccc"
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
                    onClicked: phyKeyManager.ctrlBuzzer()
                }
            }//下键
        }//换页按钮
    }//组合框下拉菜单

    ListModel{
        id:listModelData
        Component.onCompleted: {
            for(var i = 0; i < root.popupDelegateNum; i++){
                listModelData.append({
                    modelData: resourceManager.label(root.resourceGroupId, root.resourceStartIndex, resourceManager.language) + (i + 1 < 10 ? "0" + String(i + 1) : String(i + 1)),
                    listColor: resourceManager.rgb(root.resourceGroupId, root.resourceStartIndex)
                });
            }
        }
    }//model

    delegate: Rectangle {
        width: root.width
        height: root.height
        anchors.horizontalCenter: parent.horizontalCenter
        color: "#ffffff"
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
            color: listColor
        }
        MouseArea{
            anchors.fill: parent
            onPressed: parent.color = "#cc3333"
            onReleased: parent.color = "#ffffff"
            onClicked: {
                currentIndex = index
                popup.close()
                //phyKeyManager.ctrlBuzzer()
            }
        }
    }

    Connections {
        target:resourceManager
        enabled: visible
        onLanguageChanged:{
            if(resourceStartIndex == 668){
                for(var i = 0; i < listModelData.count; i++){
                    listModelData.set(i, {modelData: resourceManager.label(root.resourceGroupId, root.resourceStartIndex, resourceManager.language)+ (i + 1 < 10 ? "0" + String(i + 1) : String(i + 1)),
                                          listColor: resourceManager.rgb(root.resourceGroupId, root.resourceStartIndex)
                                      });
                }
            }else{
                for(var j = 0; j < listModelData.count; j++){
                    listModelData.set(j, {modelData: resourceManager.label(root.resourceGroupId, root.resourceStartIndex + j, resourceManager.language),
                                          listColor: resourceManager.rgb(root.resourceGroupId, root.resourceStartIndex + j)
                                      });
                }
            }
        }
    }
}
