import QtQuick 2.12
import QtQml 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import Enums 1.0
import "../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: FileConfirm.qml
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
Item {
    id:file_confirm
    anchors.centerIn: parent
    property string text: ""
    property var viewModel:""
    property int listIndex:-1
    property int subListIndex:-1
    property int buttonIndex:-1
    property int tabIndex:-1
    property bool isBtnVisible:true
    property int processResult:0
    property string saveFileNumber: ""
    property string saveFileName: ""
    property string errCode:""
    property bool isRequestPowerRestart:false
    property bool isCloseBtnVisible:true

    signal itemClose()
    signal processingItemOpen(int btnIndex)
    signal enactSuccessItemOpen(int btnIndex, string fileNumber, string fileName)
    signal enactFailItemOpen(int btnIndex, string fileNumber, string fileName, string errCode)
    signal callAttentionItemOpen(int btnIndex)
    signal reOrderSetupDataListFinishNotify(bool isReOrderSetupDataListFinish)

    function executeProcess() {
        let ret = 0
        switch (file_confirm.tabIndex){
        case 0:
            if (file_confirm.listIndex >= 0){
                switch(file_confirm.buttonIndex){
                case 0:
                    ret = viewModel.readSetupFile(file_confirm.listIndex);
                    break
                case 2:
                    ret = viewModel.writeSetupFile(file_confirm.listIndex);
                    break
                case 3:
                    ret = viewModel.compareSetupFile(file_confirm.listIndex);
                    break
                case 4:
                    ret = viewModel.eraseSetupFile(file_confirm.listIndex);
                    break
                default:
                    break
                }
            }
            break
        case 1:
            if (file_confirm.buttonIndex === 0){
                ret = viewModel.createBackupFolder()
                if (ret >= 0){
                    ret = viewModel.writeToUsb(file_confirm.listIndex, file_confirm.subListIndex);
                }
            } else {
                ret = viewModel.createBackupFolder()
                if (ret >= 0){
                    ret = viewModel.writeInBulkToUsb();
                }
            }
            break
        case 2:
            ret = viewModel.writeFromUsb(file_confirm.listIndex, file_confirm.subListIndex);
            break
        default:
            break
        }
        return ret;
    }

    Timer {
        id:popupTimer
        interval: 1000
        running: false
        repeat: false
        onTriggered:{
            popupTimer.running = false;
            popupTimer.repeat = false;
            if(file_confirm.isRequestPowerRestart === true){
                file_confirm.itemClose()
                file_confirm.isRequestPowerRestart = false
                file_confirm.callAttentionItemOpen(file_confirm.buttonIndex)
            }
        }
    }

    Image {
        id:bgImg
        anchors.centerIn: parent
        source: resourceManager.imagePath + "/08_MENU/d084_01" + ".png"
    }

    Image {
        id:popIcon
        anchors.top:bgImg.top
        anchors.topMargin:25
        anchors.left:bgImg.left
        anchors.leftMargin:255
        source: resourceManager.imagePath + "/08_MENU/d082_11" + ".png"
    }
    Text{
        anchors.top:bgImg.top
        anchors.topMargin:25
        anchors.left:popIcon.right
        anchors.leftMargin:10
        color:resourceManager.rgb(51,15)
        font.pixelSize: 24
        text:resourceManager.label(51,15,resourceManager.language)
    }
    Image{
        anchors.top:bgImg.top
        anchors.topMargin:10
        anchors.right:bgImg.right
        anchors.rightMargin:10
        visible:file_confirm.isCloseBtnVisible
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
                file_confirm.errCode = ""
                file_confirm.itemClose()
            }
        }
    }
    Text{
        id:confirm_msg
        anchors.centerIn: parent
        font.pixelSize: 30
        text: file_confirm.text
    }
    // ボタン
    Row {
        anchors.top:bgImg.top
        anchors.topMargin:300
        anchors.left:bgImg.left
        anchors.leftMargin:370
        spacing: 15
        Rectangle{
            visible:file_confirm.isBtnVisible
            width: 108
            height: 50
            radius: 8
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
                    file_confirm.errCode = ""
                }
            }
        }
        Rectangle{
            visible:file_confirm.isBtnVisible
            width: 108
            height: 50
            radius: 8
            color:(decisionBtnMouseArea.pressed === true) ? "#cc3333" : "#cccccc"
            border.width: 2
            border.color: "grey"
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
                    file_confirm.processResult = file_confirm.executeProcess()
                    //console.log("onClicked processResult:"+file_confirm.processResult)
                    if (file_confirm.processResult >= 0){
                        file_confirm.processingItemOpen(file_confirm.buttonIndex)
                    } else {
                        if (file_confirm.processResult > -2){
                            file_confirm.enactFailItemOpen(file_confirm.buttonIndex, file_confirm.saveFileNumber, file_confirm.saveFileName, "")
                        }
                    }
                }
            }
        }
    }
}
