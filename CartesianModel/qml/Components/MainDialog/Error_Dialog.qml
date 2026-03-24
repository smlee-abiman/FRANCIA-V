import QtQuick 2.12
import QtQml 2.12
import QtQuick.Controls 2.12
import "../../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: Error_Dialog.qml
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
Dialog  {
    id:error_dialog
    width: 642
    height: 310
    modal: false
    closePolicy:Popup.NoAutoClose

    signal dialogClose()
    property int errStatus: commonViewModel.errorStatus
    property var errorStatusText: errStatus <= 0 ? "" : resourceManager.getMessageStringValue("13", errStatus, resourceManager.language)
    property var errorStatusColor: resourceManager.rgb(13,0)
    property var errorTitle:resourceManager.label(53,0,resourceManager.language)
    property var errorTitleColor:resourceManager.rgb(53,0)

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
                    x:200
                    y:2
                    source: resourceManager.imagePath + "/08_MENU/d084_02.png"
                }
                Text{
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 26
                    font.weight: Font.Bold
                    font.bold: true
                    color: errorTitleColor
                    text: errorTitle
                }
            }
            ImageButton{
                id:error_dialog_close
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 15
                scale: 1.5
                upImageSource: resourceManager.imagePath + "/28_USB/a280_02.png"
                downImageSource: resourceManager.imagePath + "/28_USB/a280_02.png"
                onClicked: {
                    error_dialog.dialogClose()
                    error_dialog.close()
                }
            }//关闭弹窗
        }//标题栏
    }

    Text{
        x:10
        y:60
        font.pixelSize: 22
        color: error_dialog.errorStatusColor
        text: error_dialog.errorStatusText
    }//提示信息
}
