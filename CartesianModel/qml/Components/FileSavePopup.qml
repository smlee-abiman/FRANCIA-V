import QtQuick 2.12
import QtQml 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import Enums 1.0
import "../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: FileSavePopup.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
 - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.23
* Worker: SH.HONG
**************************************************************************************/
Dialog {
    id:file_save_dialog
    modal : Qt. WindowModal
    closePolicy: Popup.CloseOnEscape
    anchors.centerIn: parent
    property string text: ""
    property var viewModel:null
    property string freeNumber: ""
    property int buttonIndex:-1
    property int processResult:0
    property int curAttrIndex:-1
    property bool checkResult:false
    property string commentText: ""

    // background: Image {
    //     source: resourceManager.imagePath + "/08_MENU/d084_01" + ".png"
    // }
    signal dialogClose()
    signal processingSaveDialogOpen(int btnIndex, string fileNumber, string fileName)
    signal enactFailDialogOpen(int btnIndex, string fileNumber, string fileName, string errCode)
    signal confirmFailDialogOpen(bool btnVisible, string fileNumber)
    signal cautionFileNameIllegalDialogOpen(int btnIndex, string fileName)
    signal openVirtualKeyboard()

    Text{
        x:284
        y:12
        color:resourceManager.rgb(51,18)
        font.pixelSize: 24
        text:resourceManager.label(51,18,resourceManager.language)
    }
    Image{
        x:560
        source: resourceManager.imagePath + "/08_MENU/d080_02" + ".png"
        MouseArea{
            anchors.fill: parent
            onPressed:{
                parent.source = resourceManager.imagePath + "/08_MENU/d080_03" + ".png"
            }
            onReleased: {
                parent.source = resourceManager.imagePath + "/08_MENU/d080_02" + ".png"
            }
            onClicked: {
                file_save_dialog.dialogClose()
                file_save_dialog.close()
            }
        }
    }
    Column{
        x:15
        y:70
        spacing: 40
        Repeater {
            model:3
            Text{
                font.pixelSize: 18
                text: resourceManager.label(51,19+index,resourceManager.language)
                color: resourceManager.rgb(51,19+index)
            }
        }
    }
    Column{
        x:135
        y:65
        spacing: 28
        LimitNumericEditBox{
            id:listItemNumberArea
            width: 300
            height: 40
            max: "999"
            min: "1"
            unit: ""
            value: file_save_dialog.freeNumber
            color: "transparent"
            textColor: "black"
            borderColor:"#999999"
            fontPixelSize: 18
            keyLayoutType:3
            textrightMargin:245
            horizontalAlignment:Text.AlignRight
            onEnter: {
                let numValue = parseInt(newValue);
                if (newValue === ""){
                    // NOP
                } else {
                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                    file_save_dialog.checkResult = viewModel.checkFileNumber(numValue);
                    if (file_save_dialog.checkResult === true){
                        if (newValue.length < 3){
                            if (newValue.length < 2){
                                if (numValue < 10){
                                    value = "00" + newValue;
                                } else if (numValue < 100){
                                    value = "0" + newValue;
                                } else {
                                    value = newValue;
                                }
                            } else {
                                if (numValue < 100){
                                    value = "0" + newValue;
                                } else {
                                    value = newValue;
                                }
                            }
                        } else {
                            value = newValue;
                        }
                    } else {
                        let retValue
                        if (newValue.length < 3){
                            if (newValue.length < 2){
                                if (numValue < 10){
                                    retValue = "00" + newValue;
                                } else if (numValue < 100){
                                    retValue = "0" + newValue;
                                } else {
                                    retValue = newValue;
                                }
                            } else {
                                if (numValue < 100){
                                    retValue = "0" + newValue;
                                } else {
                                    retValue = newValue;
                                }
                            }
                        } else {
                            retValue = newValue;
                        }
                        file_save_dialog.confirmFailDialogOpen(false, retValue)
                    }
                }
            }
        }
        Rectangle {
            id: listItemCommentArea
            width: 300
            height: 40
            color: "transparent"
            border.color:"#999999"
            border.width:2
            Text {
                id: editText
                anchors.fill: parent
                anchors.leftMargin: 25
                anchors.rightMargin: 0
                text:file_save_dialog.commentText
                font.pixelSize: 20
                horizontalAlignment:Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                color: "black"
            }
            MouseArea {
                id: mouseArea
                anchors.fill: parent
                onClicked: {
                    file_save_dialog.dialogClose()
                    file_save_dialog.close()
                    file_save_dialog.openVirtualKeyboard()
                }
            }
        }
        ResourceStringComboBox{
            id:listItemAttrArea
            width: 300
            height: 40
            indicatorImage: ""
            fontSize: 18
            resourceGroupId: 51
            resourceStartIndex: 8
            listItemCount: 2
            currentIndex:0
            borderColor:"#999999"
            rectColor:"transparent"
            textLeftMargin:25
            horizontalAlign:Text.AlignLeft
            onCurrentIndexChanged:{
                //console.log("listItemAttrArea resourceGroupId:"+listItemAttrArea.resourceGroupId, "resourceStartIndex:"+listItemAttrArea.resourceStartIndex,
                //    "currentIndex:"+listItemAttrArea.currentIndex)
            }
        }
    }

    // ボタン
    Row {
        x:360
        y:290
        spacing: 15
        Rectangle{
            width: 108
            height: 50
            color:(cancelBtnMouseArea.pressed === true) ? "#cc3333" : "#cccccc"
            border.width: 2
            border.color: "grey"
            Image {
                x:14
                anchors.verticalCenter: parent.verticalCenter
                source: resourceManager.imagePath + "/05_ORG/d050_13.png"
            }
            Text {
                x:50
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 23
                text: resourceManager.label(51,27,resourceManager.language)
            }
            MouseArea{
                id:cancelBtnMouseArea
                anchors.fill: parent
                onPressed: {
                    //parent.color = "#dae000"
                }
                onReleased: {
                    //parent.color = "#e6f6fc"
                }
                onClicked: {
                    file_save_dialog.dialogClose()
                    file_save_dialog.close()
                }
            }
        }
        Rectangle{
            width: 108
            height: 50
            color:(decisionBtnMouseArea.pressed === true) ? "#cc3333" : "#cccccc"
            border.width: 2
            border.color: "#999999"
            Image {
                x:10
                anchors.verticalCenter: parent.verticalCenter
                source: resourceManager.imagePath + "/05_ORG/d050_12.png"
            }
            Text {
                x:50
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 23
                text: resourceManager.label(51,26,resourceManager.language)
            }
            MouseArea{
                id:decisionBtnMouseArea
                anchors.fill: parent
                onPressed: {
                    //parent.color = "#dae000"
                }
                onReleased: {
                    //parent.color = "#e6f6fc"
                }
                onClicked: {
                    file_save_dialog.curAttrIndex = listItemAttrArea.currentIndex
                    //console.log("onClicked file_save_dialog curAttrIndex:"+file_save_dialog.curAttrIndex)
                    file_save_dialog.processResult = viewModel.saveSetupFile(listItemNumberArea.value, listItemCommentArea.text, file_save_dialog.curAttrIndex);
                    file_save_dialog.close()
                    //console.log("onClicked file_save_dialog processResult:"+file_save_dialog.processResult)
                    if (file_save_dialog.processResult >= 0){
                        file_save_dialog.processingSaveDialogOpen(file_save_dialog.buttonIndex, listItemNumberArea.value, listItemCommentArea.text)
                    } else {
                        if (file_save_dialog.processResult < -1){
                            file_save_dialog.cautionFileNameIllegalDialogOpen(file_save_dialog.buttonIndex, listItemCommentArea.text)
                        } else {
                            file_save_dialog.enactFailDialogOpen(file_save_dialog.buttonIndex, listItemNumberArea.value, listItemCommentArea.text, "")
                        }
                    }
                }
            }
        }
    }
}
