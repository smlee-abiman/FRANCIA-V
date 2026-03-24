import QtQuick 2.12
import QtQuick.Controls 2.12
import "../../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: Remind_Dialog.qml
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
    id:remind_dialog
    width:620
    height: 320
    closePolicy: Popup.CloseOnEscape
    anchors.centerIn: parent
    background: Rectangle{
        width:620
        height: 320
        border.width: 2
        border.color: "#999999"
        color: "#ffffff"
        Rectangle{
            width: 620
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
                    color: "white"
                    text: resourceManager.label(43,2,resourceManager.language)
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
                    remind_dialog.close()
                }
            }//关闭弹窗
        }//标题栏
    }

    property string message: "message"

    signal clickOKButton()
    signal pressed()
    signal released()


    Text{
        anchors.centerIn: parent
        font.pixelSize: 30
        text: message
    }

    Rectangle{
        width: 120
        height: 55
        x:475
        y:240
        color: "#ffffff"
        border.color: "#999999"
        Image {
            x:10
            anchors.verticalCenter: parent.verticalCenter
            source: resourceManager.imagePath + "/00_COMMON/d000_75.png"
        }
        Text {
            x:50
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 23
            text: resourceManager.label(43,15,resourceManager.language)
        }
        MouseArea{
            anchors.fill: parent
            onPressed: {
                parent.color = "#cc3333"
                remind_dialog.pressed()
            }
            onReleased: {
                parent.color = "#cccccc"
                remind_dialog.released()
            }
            onClicked: {
                remind_dialog.clickOKButton()
                remind_dialog.close()
            }
        }
    }
}
