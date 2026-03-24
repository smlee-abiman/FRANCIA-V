import QtQuick 2.12
import QtQml 2.12
import QtQuick.Controls 2.12
import "../../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: EmergencyStop_Dialog.qml
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
    id:emergency_dialog
    modal: false
    closePolicy:Popup.NoAutoClose

    signal dialogClose()

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
                    color:resourceManager.rgb(52,13)            // TODO "急停警报"
                    text:resourceManager.label(52,13,resourceManager.language)
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
                    emergency_dialog.dialogClose()
                    emergency_dialog.close()
                }
            }//关闭弹窗
        }//标题栏
    }
    Text{
        x:10
        y:90
        font.pixelSize: 24
        color: resourceManager.rgb(52,14)
        text: resourceManager.label(52,14,resourceManager.language)
    }
}
