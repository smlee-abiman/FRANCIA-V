import QtQuick 2.12
import QtQml 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import Enums 1.0
import "../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: FileSave.qml
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
Item {
    id:file_save
    anchors.centerIn: parent
    property string text: ""
    property var viewModel:""
    property string freeNumber: ""
    property int buttonIndex:-1
    property int processResult:0
    property int curAttrIndex:-1
    property bool checkResult:false
    property string commentText:""
    property string holdCommentText:""
    property bool isVtkStatus:false

    signal itemClose()
    signal processingSaveItemOpen(int btnIndex, string fileNumber, string fileName)
    signal enactFailItemOpen(int btnIndex, string fileNumber, string fileName, string errCode)
    signal confirmFailItemOpen(bool btnVisible, string fileNumber)
    signal cautionFileNameIllegalItemOpen(int btnIndex, string fileName)
    signal openVirtualKeyboard()

    Rectangle{
        id:bgImg
        width: 642
        height: 340
        anchors.centerIn: parent
        border.width: 2
        border.color: "#999999"
        color: "#ffffff"
        Rectangle{
            width: 642
            height: 55
            border.width: 2
            border.color: "#999999"
            color: "#999999"
            Row{
                anchors.centerIn: parent
                spacing: 5
                Image {
                    anchors.verticalCenter: parent.verticalCenter
                    source: resourceManager.imagePath + "/08_MENU/d082_11.png"
                }
                Text{
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 22
                    color: "white"
                    text: resourceManager.label(51,18,resourceManager.language)
                }
            }
            ImageButton{
                id: compa2re_result_close
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 15
                scale: 1.5
                upImageSource: resourceManager.imagePath + "/28_USB/a280_02.png"
                downImageSource: resourceManager.imagePath + "/28_USB/a280_02.png"
                onClicked: {
                    file_save.isVtkStatus = false
                    file_save.itemClose()
                }
            }
        }//标题栏
    }

    Column{
        anchors.top:bgImg.top
        anchors.topMargin:85
        anchors.left:bgImg.left
        anchors.leftMargin:25
        spacing: 50
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
        anchors.top:bgImg.top
        anchors.topMargin:78
        anchors.left:bgImg.left
        anchors.leftMargin:145
        spacing: 28
        LimitNumericEditBox{
            id:listItemNumberArea
            width: 300
            height: 40
            max: "999"
            min: "1"
            unit: ""
            value: file_save.freeNumber
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
                    file_save.checkResult = viewModel.checkFileNumber(numValue);
                    if (file_save.checkResult === true){
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
                        file_save.confirmFailItemOpen(false, retValue)
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
                text:file_save.commentText
                font.pixelSize: 20
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                color: "black"
            }
            MouseArea {
                id: mouseArea
                anchors.fill: parent
                onClicked: {
                    file_save.openVirtualKeyboard()
                    file_save.isVtkStatus = true
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

    Rectangle{
        width: 108
        height: 50
        x:700
        y:335
        color: "#cc3333"
        Text {
            anchors.centerIn: parent
            font.pixelSize: 23
            text: resourceManager.label(51,26,resourceManager.language)
            color: "white"
        }
        MouseArea{
            id:decisionBtnMouseArea
            anchors.fill: parent
            onClicked: {
                if (file_save.isVtkStatus === true){
                    file_save.holdCommentText = file_save.commentText
                } else {
                    file_save.commentText = ""
                    file_save.holdCommentText = file_save.commentText
                }
                file_save.curAttrIndex = listItemAttrArea.currentIndex
                file_save.isVtkStatus = false
                file_save.itemClose()
                console.debug("onClicked file_save curAttrIndex:"+file_save.curAttrIndex)
                file_save.processResult = viewModel.saveSetupFile(listItemNumberArea.value, file_save.holdCommentText, file_save.curAttrIndex);
                console.debug("onClicked file_save processResult:"+file_save.processResult)
                if (file_save.processResult >= 0){
                    file_save.processingSaveItemOpen(file_save.buttonIndex, listItemNumberArea.value, file_save.holdCommentText)
                } else {
                    if (file_save.processResult < -1){
                        file_save.cautionFileNameIllegalItemOpen(file_save.buttonIndex, file_save.holdCommentText)
                    } else {
                        file_save.enactFailItemOpen(file_save.buttonIndex, listItemNumberArea.value, file_save.holdCommentText, "")
                    }
                }
            }
        }
    }
}
