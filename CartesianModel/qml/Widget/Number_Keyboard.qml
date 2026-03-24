import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.3
/*************************************************************************************
* Title: UI/UX of AEK
* File: Number_Keyboard.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
    - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.10
* Worker: SH.HONG
**************************************************************************************/
Dialog {
    id: virtual_tenkey
    width: 325
    height: isnumberkpad2 ? 160 :  408
    parent: Overlay.overlay
    modal: true
    closePolicy: isnumberkpad2 ? Popup.CloseOnPressOutside : Popup.CloseOnEscape
    property string digits: ""
    property int textMaxLength: 16
    property int keyLayoutType: 0
    property bool isEnterkeyPressd: false
    property var max: 0
    property var min: 0
    property bool noLimit: false
    property string valueColor: "black"
    property bool enablePressAndHoldClearKey: false
    property bool isEmptyPermit: false
    property bool isnumberkpad2: false
    property int numberkpad2LayoutType: 0
    property int pointno: 0
    property int axisno: 0

    signal numberkpad2enter();

    onVisibleChanged: {
        numberkpad1.pointno = pointno
        numberkpad1.axisno = axisno
    }

    background: Rectangle{
        width: 325
        height: isnumberkpad2 ? 160 :  408
        color: "#eaeaea"
        border.width: 2
        border.color: "#999999"
        Rectangle{
            visible: !isnumberkpad2
            width: 300
            height: 48
            anchors.horizontalCenter: parent.horizontalCenter
            y:70
            color: "white"
            border.color: "black"
        }
        ImageButton{
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.rightMargin: 10
            anchors.topMargin: 10
            visible: !isnumberkpad2
            upImageSource: resourceManager.imagePath + "/00_COMMON/d000_03.png"
            downImageSource: resourceManager.imagePath + "/00_COMMON/d000_04.png"
            onClicked: {
                digits = ""
                virtual_tenkey.close()
            }
        }
        Row{
            x:12
            y:2
            visible: !isnumberkpad2
            spacing: 10
            Column{
                spacing: 1
                Text{
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: 18
                    color: resourceManager.rgb(20, 30)
                    text: resourceManager.label(20,30,resourceManager.language)
                }
                Rectangle{
                    width: 120
                    height: 34
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: "#cccccc"
                    border.color: "#999999"
                    Text{
                        anchors.centerIn: parent
                        font.pixelSize: 20
                        font.bold: true
                        text: noLimit ? "" : min
                    }
                }
            }
            Column{
                spacing: 1
                Text{
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: 18
                    font.bold: true
                    color: resourceManager.rgb(20, 29)
                    text: resourceManager.label(20,29,resourceManager.language)
                }
                Rectangle{
                    width: 120
                    height: 34
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: "#cccccc"
                    border.color: "#999999"
                    Text{
                        anchors.centerIn: parent
                        font.pixelSize: 20
                        text: noLimit ? "" : max
                    }
                }
            }
        }//最大值 最小值
    }

    NumericKeyPad{
        id:numberkpad1
        y:70
        anchors.centerIn: parent
        visible: !isnumberkpad2
        inputText: digits
        keyLayoutType: virtual_tenkey.keyLayoutType
        textMaxLength: virtual_tenkey.textMaxLength
        maxValue: max
        minValue: min
        textColor: valueColor
        noLimit: virtual_tenkey.noLimit
        enablePressAndHoldClearKey: virtual_tenkey.enablePressAndHoldClearKey
        isEmptyPermit: virtual_tenkey.isEmptyPermit
        onEnter: {
            virtual_tenkey.isEnterkeyPressd = true;
            virtual_tenkey.digits = newValue; //tkmodel.Num;
            virtual_tenkey.close()
        }
        onClose: {
           virtual_tenkey.close()
        }
    }

    NumericKeyPad_adjust{
        id:numberkpad2
        anchors.centerIn: parent
        visible: isnumberkpad2
        inputText: digits
        textColor: valueColor
        maxValue: max
        minValue: min
        noLimit: virtual_tenkey.noLimit
        keyLayoutType: numberkpad2LayoutType
        onEnter: {
            virtual_tenkey.digits = newValue;
            numberkpad2enter()
        }
    }

    Connections{
        target: phyKeyManager
        enabled: virtual_tenkey.opened
        onUpdateSelectSwInfo: {
            virtual_tenkey.close()
        }
    }
}
