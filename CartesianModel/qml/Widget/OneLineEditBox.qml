import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
/*************************************************************************************
* Title: UI/UX of AEK
* File: OneLineEditBox.qml
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
Item {
    id: root
    property string text: "0"
    property string source: ""
    property int horizontalAlignment: Text.AlignHCenter
    property int fontPixelSize: 22
    property int textMaxLength: 12
    property int keyLayoutType: 0
    property bool isAutoOpenDialog: true
    property string textColor: "black"
    property string color: "white"
    property int textleftMargin: 0
    property int textrightMargin: 0
    property int rectRadius:8
    property string borderColor:"#999999"
    property int borderWidth:2

    signal enter( string newValue);
    signal ready();
    signal closeVtk();

    Rectangle {
        width: parent.width
        height: parent.height
        radius: root.rectRadius
        color: root.color
        border.color: (root.source.length > 0) ? "transparent": root.borderColor
        border.width: (root.source.length > 0) ? 0: root.borderWidth

        Image {
            anchors.fill: parent
            visible: (root.source.length > 0)
            source: root.source
        }

        Text {
            id: editText
            anchors.fill: parent
            anchors.leftMargin: textleftMargin
            anchors.rightMargin: textrightMargin
            text: root.text
            font.pixelSize: root.fontPixelSize
            horizontalAlignment: root.horizontalAlignment
            verticalAlignment: Text.AlignVCenter
            color: textColor
        }
        MouseArea {
            id: mouseArea
            anchors.fill: parent
            onClicked: {
                if(isAutoOpenDialog){
                    openDialog();
                }
                root.ready();
            }
        }
    }

    // OneLineEditDialog.
    function openDialog() {
        if (oneLineEditDialogLoader.item != null) {
            oneLineEditDialogLoader.item.lineText = root.text;
            oneLineEditDialogLoader.item.open()
        } else {
            oneLineEditDialogLoader.activeByRequest = true
        }
    }
    Component {
        id: oneLineEditDialogComponent
        OneLineEditDialog {
            textMaxLength: root.textMaxLength
            keyLayoutType: root.keyLayoutType
        }
    }
    Loader {
        id: oneLineEditDialogLoader
        property bool activeByRequest: false
        sourceComponent: oneLineEditDialogComponent
        active: true
        asynchronous: true
        onLoaded: {
            if (activeByRequest) {
                item.lineText = root.text;
                item.open()
            }
        }
    }
    Connections {
        target: oneLineEditDialogLoader.item
        onClosed: {
            if(target.isEnterkeyPressd){
                root.enter(target.lineText);
            } else {
                root.closeVtk();
            }
        }
    }
}
