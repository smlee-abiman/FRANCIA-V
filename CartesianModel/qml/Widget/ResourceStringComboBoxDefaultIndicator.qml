import QtQuick 2.0
import QtQml 2.3
import QtQuick.Controls 2.12
/*************************************************************************************
* Title: UI/UX of AEK
* File: ResourceStringComboBoxDefaultIndicator.qml
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
ComboBox {
    id:root
    width: 126
    height: 50

    property int fontSize: 16
    property int resourceGroupId: 0
    property int resourceStartIndex: 0
    property int listItemCount: 0
    property color bgRectBorderColor:"#999999"
    property color bgRectColor:"#ffffff"
    property int bgRectBorderWidth:2
    property int bgRectRadius:12
    property int textLeftMargin:10
    property color selectedColor:"black"

    contentItem:Text {
        id: rootText
        text: root.displayText
        font.pixelSize: root.fontSize
        verticalAlignment: Text.AlignVCenter
        anchors.left:parent.left
        anchors.leftMargin:root.textLeftMargin
    }

    background: Rectangle {
        border.color: root.bgRectBorderColor
        border.width: root.bgRectBorderWidth
        radius: root.bgRectRadius
        color: root.bgRectColor
    }

    font.pixelSize: root.fontSize
    model:listModelData

    ListModel{
        id:listModelData
        Component.onCompleted: {
            for(var i = 0; i < root.listItemCount; i++){
                listModelData.append({
                                   modelData: resourceManager.label(root.resourceGroupId, root.resourceStartIndex + i, resourceManager.language),
                                   listColor: resourceManager.rgb(root.resourceGroupId, root.resourceStartIndex + i)
                               });
            }

            setSelectTextColor();
        }
    }

    popup: Popup{
        width: parent.width
        implicitHeight: dropDownList.contentHeight
        y:parent.height-1
        padding: 1
        background: Rectangle{
            color: "#ffffff"
            radius: 8
            border.color: "#999999"
            border.width: 2
        }
        contentItem: ListView{
            id:dropDownList
            clip: true
            model: root.popup.visible ? root.delegateModel : null
            currentIndex: root.highlightedIndex
            boundsBehavior: Flickable.StopAtBounds
        }
    }

    delegate: ItemDelegate{
        width: parent.width
        contentItem: Text {
            text: modelData
            color: listColor
            font: root.font
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }
    }

    onCurrentIndexChanged: {
        setSelectTextColor();
    }

    function setSelectTextColor(){
        if((currentIndex !== -1) && (listModelData.count > 0)){
            let item = listModelData.get(currentIndex);
            rootText.color = root.selectedColor;
        }
    }

    Connections {
        target:resourceManager
        onLanguageChanged:{
            for(var i = 0; i < listModelData.count; i++){
                listModelData.set(i, {
                                      modelData: resourceManager.label(root.resourceGroupId, root.resourceStartIndex + i, resourceManager.language),
                                      listColor: resourceManager.rgb(root.resourceGroupId, root.resourceStartIndex + i)
                                  });
            }
        }
    }
}
