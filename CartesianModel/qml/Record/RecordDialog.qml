import QtQuick 2.12
import QtQuick.Controls 2.12
import "../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: RecordDialog.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
 - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.18
* Worker: SH.HONG
**************************************************************************************/
Dialog {
    id: export_dia

    property var msg: ""
    property var msgColor: ""
    property int language: 0
    property bool cancelButtonVisible: true
    property bool okButtonVisible: true

    signal cancelClicked()
    signal okClicked()

    width: 480
    height: 320
    x:240
    y:70
    modal : Qt. WindowModal
    closePolicy: Popup.CloseOnEscape
    background: Rectangle{
        width: 480
        height: 320
        border.color: "#999999"
        color: "#ffffff"
        radius: 8
        Rectangle{
            width: 480
            height: 54
            border.width: 2
            border.color: "#999999"
            color: "#999999"
            Image {
                id:popupTitleIcon
                x:150
                y:10
                source: resourceManager.imagePath + "/26_machine_p/d260_11.png"
            }
            Text{
                x:190
                font.pixelSize: 25
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
                color: resourceManager.rgb(43,2)
                text: resourceManager.label(43,2,export_dia.language)
            }
        }
        ImageButton{
            x:430
            y:4
            scale: 1.5
            upImageSource: resourceManager.imagePath + "/16_program/a160_03.png"
            downImageSource: resourceManager.imagePath + "/16_program/a160_04.png"
            visible: cancelButtonVisible
            onClicked: export_dia.cancelClicked()
        }
    }
    Text{
        anchors.centerIn: parent
        font.pixelSize: 24
        color: export_dia.msgColor
        text: export_dia.msg
    }
    Rectangle{
        width: 100
        height: 40
        x:355
        y:252
        color: "#cccccc"
        border.color: "#999999"
        visible: okButtonVisible
        Image {
            x:10
            anchors.verticalCenter: parent.verticalCenter
            source: resourceManager.imagePath + "/08_MENU/d080_10.png"
        }
        Text {
            x:44
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 20
            color: resourceManager.rgb(43,15)
            text: resourceManager.label(43,15,export_dia.language)
        }
        MouseArea{
            anchors.fill: parent
            onPressed: parent.color = "#cc3333"
            onReleased: parent.color = "#cccccc"
            onClicked: {
                export_dia.okClicked()
            }
        }
    }
}
