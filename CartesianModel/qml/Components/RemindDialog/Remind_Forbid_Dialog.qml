import QtQuick 2.12
import QtQuick.Controls 2.12
import "../../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: Remind_Forbid_Dialog.qml
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
    id:root
    width:620
    height: 320
    modal: true
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
                    root.close()
                }
            }//关闭弹窗
        }//标题栏
    }

    property string message: "message"

    signal clickOKButton()
    signal pressed()
    signal released()

    onOpened: timer.running = true
    onClosed: timer.running = false
    Timer{
        id:timer
        interval: 2000
        repeat: false
        running: false
        onTriggered: root.close()
    }

    Text{
        width: 600
        height: 200
        anchors.centerIn: parent
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        wrapMode: Text.Wrap
        font.pixelSize: 30
        text: message
    }
}
