import QtQuick 2.12
import QtQml 2.3
import QtQuick.Controls 2.12
/*************************************************************************************
* Title: UI/UX of AEK
* File: ResouceStringComboBox.qml
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
    property string indicatorImage: ""
    property int resourceGroupId: 0
    property int resourceStartIndex: 0
    property int listItemCount: 0
    property int rectRadius:8
    property string borderColor:"#999999"
    property int borderWidth:2
    property string rectColor: "#ffffff"
    property var horizontalAlign:Text.AlignHCenter
    property int textLeftMargin: 0
    property bool isUseMessageValue:false
    property int adjResIndexValue:1
    property bool isAutoGenerateIndex:false

    contentItem:Text {
        id: rootText
        anchors.fill: parent
        anchors.leftMargin: root.textLeftMargin
        text: root.displayText
        font.pixelSize: root.fontSize
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment:root.horizontalAlign
    }

    background: Rectangle {
        border.color: root.borderColor
        border.width: root.borderWidth
        radius: root.rectRadius
        color: root.rectColor
    }

    indicator: Image {
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 6
        source: root.indicatorImage
    }

    font.pixelSize: root.fontSize
    model:listModelData

    ListModel{
        id:listModelData
        Component.onCompleted: {
            for(var i = 0; i < root.listItemCount; i++){
                listModelData.append({
                                    modelData: (root.isUseMessageValue === true) ?
                                        resourceManager.getMessageStringValue(String(root.resourceGroupId), root.resourceStartIndex + i * root.adjResIndexValue, resourceManager.language):
                                        ((root.isAutoGenerateIndex === true) ? resourceManager.label(root.resourceGroupId, root.resourceStartIndex, resourceManager.language) +
                                            (i + 1 < 10 ? "0" + String(i + 1) : String(i + 1)):
                                        resourceManager.label(root.resourceGroupId, root.resourceStartIndex + i * root.adjResIndexValue, resourceManager.language)),
                                    listColor: (root.isAutoGenerateIndex === true) ? resourceManager.rgb(root.resourceGroupId, root.resourceStartIndex):
                                        resourceManager.rgb(root.resourceGroupId, root.resourceStartIndex + i * root.adjResIndexValue)
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
            color: "#cccccc"
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
            anchors.fill: parent
            anchors.leftMargin: root.textLeftMargin
            text: modelData
            color: listColor
            font: root.font
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: root.horizontalAlign
        }
    }

    onCurrentIndexChanged: {
        setSelectTextColor();
    }

    function setSelectTextColor(){
        if((currentIndex !== -1) && (listModelData.count > 0)){
            let item = listModelData.get(currentIndex);
            rootText.color = item.listColor;
        }
    }

    Connections {
        target:resourceManager
        onLanguageChanged:{
            for(var i = 0; i < listModelData.count; i++){
                listModelData.set(i, {
                                    modelData: (root.isUseMessageValue === true) ?
                                        resourceManager.getMessageStringValue(String(root.resourceGroupId), root.resourceStartIndex + i * root.adjResIndexValue, resourceManager.language):
                                        ((root.isAutoGenerateIndex === true) ? resourceManager.label(root.resourceGroupId, root.resourceStartIndex, resourceManager.language) +
                                            (i + 1 < 10 ? "0" + String(i + 1) : String(i + 1)):
                                        resourceManager.label(root.resourceGroupId, root.resourceStartIndex + i * root.adjResIndexValue, resourceManager.language)),
                                    listColor: (root.isAutoGenerateIndex === true) ? resourceManager.rgb(root.resourceGroupId, root.resourceStartIndex):
                                        resourceManager.rgb(root.resourceGroupId, root.resourceStartIndex + i * root.adjResIndexValue)
                                });
            }
        }
    }
}



