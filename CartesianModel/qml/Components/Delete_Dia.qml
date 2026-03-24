import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import "../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: Delete_Dia.qml
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
    id:root
    width: 500
    height: 240
    focus: true
    closePolicy: Popup.NoAutoClose

    signal selectDetermine()

    property int deleteLabelNo: -1
    property int page: 0

    background: Rectangle{
        width: 500
        height: 240
        border.width: 2
        border.color: "#999999"
        color: "#ffffff"
        Rectangle{
            width: 500
            height: 40
            border.width: 2
            border.color: "#999999"
            color: "#999999"
            Text{
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 22
                color: resourceManager.rgb(41, 474)
                text: resourceManager.label(41, 474, resourceManager.language)
            }
        }
        ImageButton{
            x:445
            scale: 1.5
            upImageSource: resourceManager.imagePath + "/28_USB/a280_02.png"
            downImageSource: resourceManager.imagePath + "/28_USB/a280_02.png"
            onClicked: {
                root.close()
            }
        }
    }

    Text{
        anchors.centerIn: parent
        font.pixelSize: 30
        color: resourceManager.rgb(41, 475)
        text: {
            let str1 = resourceManager.label(41,  32, resourceManager.language)
            let str2 = "L" + ('000' + deleteLabelNo).slice(-3)
            let str3 = resourceManager.label(41, 475, resourceManager.language)
            return `${str1}[${str2}]${str3}`
        }
    }

    Rectangle{
        width: 85
        height: 40
        x:392
        y:176
        border.width: 2
        border.color: "#999999"
        color: save_mouse.pressed ? "#cc3333" : "#cccccc"
        Text {
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: save_mouse.pressed ? 25 : 24
            color: resourceManager.rgb(41,292)
            text: resourceManager.label(41,292,resourceManager.language)
        }
        MouseArea{
            id:save_mouse
            anchors.fill: parent
            onClicked: {
                root.selectDetermine()
                root.close()
            }
        }
    }//确定按钮
}
