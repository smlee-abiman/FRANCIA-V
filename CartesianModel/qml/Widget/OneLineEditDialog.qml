import QtQml 2.12
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.VirtualKeyboard 2.3
/*************************************************************************************
* Title: UI/UX of AEK
* File: OneLineEditDialog.qml
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
    id: root
    parent: Overlay.overlay
    padding: 0
    x: 5
    y: 160
    closePolicy: Popup.CloseOnEscape
    modal: true
    width: 1017
    height: 520
    background: Rectangle{
        anchors.fill: parent
        color: "transparent"
    }

    property string defaultText: ""
    property string lineText: ""
    property int textMaxLength: 100
    property int keyLayoutType: 0
    property bool isEnterkeyPressd: false
    property bool passwordMode: false

    signal enter(string newValue)

    function getHintKeyTypeValue(keytype){
        switch(keytype){
        case 0:
        default:
            return 0x10000000;
        case 1:
            return 0x20000000;
        case 2:
            return 0x30000000;
        case 3:
            return 0x40000000;
        case 4:
            return 0x50000000;
        case 5:
            return 0x60000000;
        case 6:
            return 0x70000000;
        case 7:
            return 0x80000000;
        }
    }

    onVisibleChanged: {
        if (visible) {
            inputField.forceActiveFocus()
        }
    }
    onOpened: {
        isEnterkeyPressd = false
        inputField.text = defaultText
    }
    onClosed: {
       defaultText = ""
    }

    Rectangle{
        id: baseImage
        anchors.fill: parent
        color: "#ffffff"
        radius: 16
        border.color: "#999999"
        border.width: 2

        //Xボタン
        ImageButton{
            y:10
            x:950
            downImageSource: resourceManager.imagePath + "/00_COMMON/d000_04.png"
            upImageSource: resourceManager.imagePath + "/00_COMMON/d000_03.png"
            onClicked: {
                root.isEnterkeyPressd = false;
                root.close();
            }
        }

        Rectangle {
            id: textArea
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.leftMargin: 20
            width:675
            height:51
            radius: 24
            border.color: "#999999"
            border.width: 2
            color: "#ffffff"

            TextField {
                id: inputField
                anchors.fill: parent
                //anchors.left: parent.left
                anchors.leftMargin: 10
                font.pointSize: 20
                focus: true
                color: "black"
                inputMethodHints: Qt.ImhPreferUppercase | Qt.ImhNoAutoUppercase
                background: Rectangle {
                    color: "transparent"
                }
                maximumLength: root.textMaxLength
                passwordCharacter: "*"
                echoMode: ( passwordMode ) ? TextInput.Password: TextInput.Normal
                passwordMaskDelay: 300
                placeholderText: root.lineText
                Keys.onPressed: {
                    if (REAL_TARGET) {
                        if (event.key === Qt.Key_Home) {
                            cursorPosition = 0
                        } else if (event.key === Qt.Key_Left) {
                            cursorPosition = Math.max(cursorPosition - 1, 0)
                        } else if (event.key === Qt.Key_Right) {
                            cursorPosition = Math.min(cursorPosition + 1, text.length)
                        } else if (event.key === Qt.Key_Delete) {
                            remove(cursorPosition, cursorPosition + 1)
                        }
                    }
                }
                onAccepted: {
                    root.lineText = inputField.text
                    root.isEnterkeyPressd = true
                    root.enter(inputField.text) // Issue signal..
                    root.close()
                }
            }
        }

        // VirtualKeyboard
        InputPanel {
            id: inputPanel
            width: parent.width - 20
            anchors.top: textArea.bottom
            anchors.topMargin: 20
            anchors.left: baseImage.left
            anchors.leftMargin: 10

            // フォーカスが当たると表示
            //visible: active

            // 言語切り替え(true : OFF, false : ON )
            externalLanguageSwitchEnabled: true
        }
    }
}
