import QtQuick 2.12
import QtQml 2.3
import QtQuick.Controls 2.12
/*************************************************************************************
* Title: UI/UX of AEK
* File: StringComboBox_style.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
    - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.10
* Worker: IJ.YI
**************************************************************************************/

/**********适用于有固定少量model项目的组合框[堆叠画面]***********/
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
    property int popupHeight: root.height * popupDelegateNum - root.bgRectBorderWidth * (popupDelegateNum - 2)

    // MODIFIED : design variables to the indicator. By IJ.YI 25.07.10
    property int bgIndicatorRadius : 0
    property int bgIndicatorBorderWidth : 0
    property color bgIndicatorRectBorderColor: "#FFFFFF"
    property color bgindiTriangleOnColor: "#cc3333"
    property color bgindiTriangleOffColor: "#eaeaea"
    property color bgComboListColor : "#FFFFFF"
    property color bgComboListBorderColor : "#999999"

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
        y: 2
        anchors.right: parent.right
        anchors.rightMargin:2
        width: (root.width / 4) - 4
        height: parent.height - 4
        border.color: root.bgIndicatorRectBorderColor
        border.width: root.bgIndicatorBorderWidth
        radius: root.bgIndicatorRadius
        color: root.bgIndicatorColor
        Column{
            anchors.centerIn: parent
            // spacing: 5
            // Triangle{
            //     triangleColor: popup.opened ? root.bgindiTriangleOnColor : root.bgindiTriangleOffColor
            //     width: 22
            //     height: 15
            //     rotation: 180
            // }
            Triangle{
                triangleColor: popup.opened ? root.bgindiTriangleOnColor : root.bgindiTriangleOffColor
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
        anchors.leftMargin: 5
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
    }//组合框下拉菜单

    ListModel{
        id:listModelData
        Component.onCompleted: {
            for(var i = 0; i < root.popupDelegateNum; i++){
                listModelData.append({
                    modelData: resourceManager.label(root.resourceGroupId, root.resourceStartIndex + i, resourceManager.language),
                    listColor: resourceManager.rgb(root.resourceGroupId, root.resourceStartIndex + i)
                });
            }
        }
    }//model

    delegate: Rectangle {
        width: root.width
        height: root.height
        anchors.horizontalCenter: parent.horizontalCenter
//        color: "#E6F6FC"
        color: root.bgComboListColor
        border.width: root.bgPopupBorderWidth
        border.color: root.bgComboListBorderColor
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
            onPressed: parent.color = "#CC3333"
            onReleased: parent.color = "#FFFFFF"
            onClicked: {
                currentIndex = index
                popup.close()
                //phyKeyManager.ctrlBuzzer()
            }
        }
    }

    Connections {
        target:resourceManager
        onLanguageChanged:{
            for(var i = 0; i < listModelData.count; i++){
                listModelData.set(i, {modelData: resourceManager.label(root.resourceGroupId, root.resourceStartIndex + i, resourceManager.language),
                                      listColor: resourceManager.rgb(root.resourceGroupId, root.resourceStartIndex + i)
                });
            }
        }
    }
}
