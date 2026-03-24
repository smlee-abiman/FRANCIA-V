import QtQuick 2.12
import QtQml 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import Enums 1.0
/*************************************************************************************
* Title: UI/UX of AEK
* File: ConfirmPopup.qml
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
Dialog {
    id:confirm_dialog
    width: 640
    height: 320
    modal : Qt. WindowModal
    closePolicy: Popup.CloseOnEscape
    anchors.centerIn: parent
    property string text: ""
    property string color: ""
    property var viewModel: null
    property bool isBtnVisible:true
    property bool isDetailBtnVisible:false
    property int processResult:0
    property string errCode:""
    property int detailBtnResourceGroup:0
    property int detailBtnResourceIndex:0

    signal dialogClose()
    signal processingDialogOpen()
    signal enactSuccessDialogOpen()
    signal enactFailDialogOpen(string errCode)
    signal detailBtnClick()

    background: Rectangle{
        width: 640
        height: 320
        border.width: 2
        border.color: "#999999"
        color: "#ffffff"
        Rectangle{
            width: 640
            height: 50
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
                    font.pixelSize: 24
                    color: resourceManager.rgb(41,474)
                    text: resourceManager.label(41,474,resourceManager.language)
                }
            }
            ImageButton{
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 15
                scale:1.2
                upImageSource: resourceManager.imagePath + "/16_program/a160_03.png"
                downImageSource: resourceManager.imagePath + "/16_program/a160_04.png"
                onClicked: {
                    confirm_dialog.errCode = ""
                    confirm_dialog.isDetailBtnVisible = false
                    confirm_dialog.dialogClose()
                    confirm_dialog.close()
                }
            } //close button.
        }
    }

    //message
    Text{
        id:confirm_dialog_msg
        anchors.centerIn: parent
        font.pixelSize: 30
        color: confirm_dialog.color
        text: confirm_dialog.text
    }

    Row {
        x:390
        y:240
        spacing: 15
        //cancel button
        Rectangle{
            visible:confirm_dialog.isBtnVisible && !confirm_dialog.isDetailBtnVisible
            width: 108
            height: 50
            radius: 8
            color:(cancelBtnMouseArea.pressed === true) ? "#cc3333" : "#cccccc"
            border.width: 2
            border.color: "#999999"
            Image {
                x:14
                anchors.verticalCenter: parent.verticalCenter
                source : resourceManager.imagePath + "/00_COMMON/d000_75.png"
            }
            Text {
                x:50
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 23
                color: resourceManager.rgb(41,293)
                text: resourceManager.label(41,293,resourceManager.language)
            }
            MouseArea{
                id:cancelBtnMouseArea
                anchors.fill: parent
                onClicked: {
                    confirm_dialog.errCode = ""
                    confirm_dialog.close()
                }
            }
        }
        //decision button
        Rectangle{
            visible:confirm_dialog.isBtnVisible && !confirm_dialog.isDetailBtnVisible
            width: 108
            height: 50
            radius: 8
            color:(decisionBtnMouseArea.pressed === true) ? "#cc3333" : "#cccccc"
            border.width: 2
            border.color: "grey"
            Image {
                x:10
                anchors.verticalCenter: parent.verticalCenter
                source: resourceManager.imagePath + "/00_COMMON/d000_75.png"
            }
            Text {
                x:50
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 23
                color: resourceManager.rgb(41,292)
                text: resourceManager.label(41,292,resourceManager.language)
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
                    //close and emit accepted signal.
                    confirm_dialog.accept()
                    //console.log("onReleased processResult:"+confirm_dialog.processResult)
                    if (confirm_dialog.processResult >= 0){
                        confirm_dialog.processingDialogOpen()
                    } else {
                        confirm_dialog.enactFailDialogOpen("")
                    }
                }
            }
        }
        Rectangle{
            id:detailBtnRect
            visible:!confirm_dialog.isBtnVisible && confirm_dialog.isDetailBtnVisible
            width: 108
            height: 50
            radius: 8
            color:(detailBtnMouseArea.pressed === true) ? "#cc3333" : "#cccccc"
            border.width: 2
            border.color: "grey"
            Text {
                anchors.verticalCenter: detailBtnRect.verticalCenter
                anchors.horizontalCenter: detailBtnRect.horizontalCenter
                font.pixelSize: 23
                color: resourceManager.rgb(confirm_dialog.detailBtnResourceGroup,confirm_dialog.detailBtnResourceIndex)
                text: resourceManager.label(confirm_dialog.detailBtnResourceGroup,confirm_dialog.detailBtnResourceIndex,resourceManager.language)
            }
            MouseArea{
                id:detailBtnMouseArea
                anchors.fill: parent
                onPressed: {
                    console.log("detailBtnMouseArea onPressed:")
                    confirm_dialog.detailBtnClick()
                }
            }
        }
    }

    Connections {
        target:confirm_dialog.viewModel
        onProcessingResultChanged:{
            //console.log("ConfirmPopup onProcessingResultChanged processingResult:"+processingResult)
        }
        onProcessingTimeoutChanged:{
            console.log("ConfirmPopup onProcessingTimeoutChanged processingTimeout:"+processingTimeout)
            confirm_dialog.dialogClose()
            confirm_dialog.close()
            confirm_dialog.enactFailDialogOpen("")
        }
        onErrCodeChanged:{
            console.log("onErrCodeChanged errCode:"+errCode)
            confirm_dialog.errCode = errCode
        }
    }
}
