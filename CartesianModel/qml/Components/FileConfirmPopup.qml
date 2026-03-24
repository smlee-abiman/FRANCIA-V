import QtQuick 2.12
import QtQml 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import Enums 1.0
import "../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: FileconfirmPopup.qml
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
    id:file_confirm_dialog
    width: 642
    height: 310
    modal : Qt. WindowModal
    closePolicy: Popup.CloseOnEscape
    anchors.centerIn: parent
    property string text: ""
    property var viewModel:null
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
    signal dialogClose()
    signal processingDialogOpen(int btnIndex)
    signal enactSuccessDialogOpen(int btnIndex, string fileNumber, string fileName)
    signal enactFailDialogOpen(int btnIndex, string fileNumber, string fileName, string errCode)
    signal callAttentionDialogOpen(int btnIndex)
    signal reOrderSetupDataListFinishNotify(bool isReOrderSetupDataListFinish)
    signal compareDiscordFailDialogOpen(int factors)

    function executeProcess() {
        let ret = 0
        switch (file_confirm_dialog.tabIndex){
        case 0:
            if (file_confirm_dialog.listIndex >= 0){
                switch(file_confirm_dialog.buttonIndex){
                case 0:
                    ret = viewModel.readSetupFile(file_confirm_dialog.listIndex);
                    break
                case 2:
                    ret = viewModel.writeSetupFile(file_confirm_dialog.listIndex);
                    break
                case 3:
                    ret = viewModel.compareSetupFile(file_confirm_dialog.listIndex);
                    break
                case 4:
                    ret = viewModel.eraseSetupFile(file_confirm_dialog.listIndex);
                    break
                default:
                    break
                }
            }
            break
        case 1:
            if (file_confirm_dialog.buttonIndex === 0){
                ret = viewModel.createBackupFolder()
                if (ret >= 0){
                    ret = viewModel.writeToUsb(file_confirm_dialog.listIndex, file_confirm_dialog.subListIndex);
                }
            } else {
                ret = viewModel.createBackupFolder()
                if (ret >= 0){
                    ret = viewModel.writeInBulkToUsb();
                }
            }
            break
        case 2:
            ret = viewModel.writeFromUsb(file_confirm_dialog.listIndex, file_confirm_dialog.subListIndex);
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
            if(file_confirm_dialog.isRequestPowerRestart === true){
                file_confirm_dialog.dialogClose()
                file_confirm_dialog.close()
                file_confirm_dialog.isRequestPowerRestart = false
                file_confirm_dialog.callAttentionDialogOpen(file_confirm_dialog.buttonIndex)
            }
        }
    }

    background: Rectangle{
        width: 642
        height: 310
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
                spacing: 10
                Image {
                    anchors.verticalCenter: parent.verticalCenter
                    source: resourceManager.imagePath + "/08_MENU/d082_11.png"
                }
                Text{
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 26
                    font.weight: Font.Bold
                    font.bold: true
                    color:resourceManager.rgb(51,15)            // TODO "急停警报"
                    text:resourceManager.label(51,15,resourceManager.language)
                }
            }
            ImageButton{
                id: file_confirm_dialog_close
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 15
                scale: 1.5
                visible:file_confirm_dialog.isCloseBtnVisible
                upImageSource: resourceManager.imagePath + "/28_USB/a280_02.png"
                downImageSource: resourceManager.imagePath + "/28_USB/a280_02.png"
                onClicked: {
                    setupChangeViewModel.notifyingProcessResult = false
                    file_confirm_dialog.errCode = ""
                    file_confirm_dialog.dialogClose()
                    file_confirm_dialog.close()
                }
            }//关闭弹窗
        }//标题栏
    }

    Text{
        id:confirm_dialog_msg
        anchors.centerIn: parent
        font.pixelSize: 30
        text: file_confirm_dialog.text
    }

    Rectangle{
        width: 108
        height: 50
        x:500
        y:235
        visible:file_confirm_dialog.isBtnVisible
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
                file_confirm_dialog.processResult = file_confirm_dialog.executeProcess()
                console.debug("onClicked processResult:"+file_confirm_dialog.processResult)
                if (file_confirm_dialog.processResult >= 0){
                    file_confirm_dialog.close()
                    file_confirm_dialog.processingDialogOpen(file_confirm_dialog.buttonIndex)
                } else {
                    file_confirm_dialog.close()
                    if (file_confirm_dialog.processResult > -2){
                        file_confirm_dialog.enactFailDialogOpen(file_confirm_dialog.buttonIndex, file_confirm_dialog.saveFileNumber, file_confirm_dialog.saveFileName, "")
                    }
                }
            }
        }
    }//按钮

    Connections {
        target:file_confirm_dialog.viewModel
        onProcessingResultChanged:{
            console.debug("FileConfirmPopup onProcessingResultChanged processingResult:"+processingResult, "processResult:"+file_confirm_dialog.processResult,
                "isFtpFileAcquired:"+setupChangeViewModel.isFtpFileAcquired, "isDeterNoopCmd:"+ftpManager.isDeterNoopCmd, "isSetupProcActive:"+setupChangeViewModel.isSetupProcActive)
            if (ftpManager.isDeterNoopCmd === false){
                ftpManager.execFtpCommand(Enums.FtpCommandNOOP, null, "");
            }
            if (file_confirm_dialog.processResult >= 0 && (setupChangeViewModel.isFtpFileAcquired === true || setupChangeViewModel.isSetupProcActive === true)){
                file_confirm_dialog.dialogClose()
                file_confirm_dialog.close()
                if (processingResult === true){
                    file_confirm_dialog.errCode = ""
                    file_confirm_dialog.enactSuccessDialogOpen(file_confirm_dialog.buttonIndex, file_confirm_dialog.saveFileNumber, file_confirm_dialog.saveFileName)
                } else {
                    file_confirm_dialog.enactFailDialogOpen(file_confirm_dialog.buttonIndex, file_confirm_dialog.saveFileNumber, file_confirm_dialog.saveFileName, file_confirm_dialog.errCode)
                }
            }
        }
        onProcessingTimeoutChanged:{
            console.debug("FileConfirmPopup onProcessingTimeoutChanged processingTimeout:"+processingTimeout, "processResult:"+file_confirm_dialog.processResult)
            if (file_confirm_dialog.processResult >= 0){
                file_confirm_dialog.errCode = ""
                file_confirm_dialog.dialogClose()
                file_confirm_dialog.close()
                file_confirm_dialog.enactFailDialogOpen(file_confirm_dialog.buttonIndex, file_confirm_dialog.saveFileNumber, file_confirm_dialog.saveFileName, "")
            }
        }
        onErrCodeChanged:{
            console.debug("FileConfirmPopup onErrCodeChanged errCode:"+errCode)
            file_confirm_dialog.errCode = errCode
        }
        onRequestPowerRestartChanged:{
            console.debug("FileConfirmPopup onRequestPowerRestartChanged requestPowerRestart:"+requestPowerRestart, "processResult:"+file_confirm_dialog.processResult)
            if (file_confirm_dialog.processResult >= 0){
                if (requestPowerRestart === true){
                    file_confirm_dialog.isRequestPowerRestart = requestPowerRestart
                    popupTimer.running = true;
                    popupTimer.repeat = true;
                }
            }
        }
        onIsReOrderSetupDataListFinishChanged:{
            console.debug("FileConfirmPopup onIsReOrderSetupDataListFinishChanged isReOrderSetupDataListFinish:"+isReOrderSetupDataListFinish)
            file_confirm_dialog.reOrderSetupDataListFinishNotify(isReOrderSetupDataListFinish)
        }
        onIsReOrderSetupDataFinishChanged:{
            console.debug("FileConfirmPopup onIsReOrderSetupDataFinishChanged isReOrderSetupDataFinish:"+isReOrderSetupDataFinish)
            file_confirm_dialog.dialogClose()
            file_confirm_dialog.close()
            if (isReOrderSetupDataFinish === true){
                externalMemoryViewModel.isNeedUpdateSetupData = false
                setupChangeViewModel.isNeedUpdateSetupData = false
            }
        }
        onIsFtpFileAcquiredChanged:{
            console.debug("FileConfirmPopup onIsFtpFileAcquiredChanged isFtpFileAcquired:"+isFtpFileAcquired, "notifyingProcessResult:"+setupChangeViewModel.notifyingProcessResult)
            if (isFtpFileAcquired === true){
                if (setupChangeViewModel.notifyingProcessResult === false){
                    file_confirm_dialog.dialogClose()
                    file_confirm_dialog.close()
                }
                externalMemoryViewModel.isNeedUpdateSetupData = false
                setupChangeViewModel.isNeedUpdateSetupData = false
            }
        }
        onCompareDiscordFactorChanged:{
            console.debug("FileConfirmPopup onCompareDiscordFactorChanged compareDiscordFactor:"+compareDiscordFactor)
            if (compareDiscordFactor >= 0){
                file_confirm_dialog.dialogClose()
                file_confirm_dialog.close()
                file_confirm_dialog.compareDiscordFailDialogOpen(compareDiscordFactor)
            }
        }
    }
}

