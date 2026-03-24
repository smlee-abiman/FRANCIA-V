import QtQuick 2.12
import QtQml 2.12
import QtQuick.Controls 2.12
import Enums 1.0
import "../../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: ModeRemind_Dialog.qml
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
Dialog{
    id:mode_remind_dialog
    width: 642
    height: 310
    x:60
    y:120
    modal: false
    closePolicy: Popup.CloseOnEscape
    background: Rectangle{
        width: 642
        height: 310
        border.width: 2
        border.color: "#999999"
        color: "#ffffff"
        Rectangle{
            width: 642
            height: 55
            border.color: "#999999"
            color: "#999999"
            Row{
                anchors.centerIn: parent
                spacing: 10
                Image {
                    x:200
                    y:2
                    source: resourceManager.imagePath + "/26_machine_p/d260_11.png"
                }
                Text{
                    font.pixelSize: 26
                    font.weight: Font.Bold
                    font.bold: true
                    color: resourceManager.rgb(43,2)
                    text: resourceManager.label(43,2,resourceManager.language)
                }
            }
            ImageButton{
                id:date_dialog_close
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 15
                scale: 1.5
                upImageSource: resourceManager.imagePath + "/28_USB/a280_02.png"
                downImageSource: resourceManager.imagePath + "/28_USB/a280_02.png"
                onClicked: {
                    mode_remind_dialog.close()
                }
            }
        }//标题栏
    }

    Text{
        anchors.centerIn: parent
        font.pixelSize: 26
        text: resourceManager.label(35,67,resourceManager.language)
    }

}
