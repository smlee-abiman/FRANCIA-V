import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQml 2.12

Popup {
    id: abnormalPopupRoot
    modal: true
    closePolicy: Popup.CloseOnEscape
    background: Rectangle {
        color: "transparent"
    }
    property string baseImageSource: ""
    property string downButtonImageSource: ""
    property string upButtonImageSource: ""
    property string buttonOffImageSource: ""
    property string buttonOnImageSource: ""
    property string abnormalText : ""
    property string abnormalTextColor : ""
    property string buttonText : ""
    property string buttonTextColor : ""
    property int fontPixelSize: 20
    property int abnormalInfo: 0
    property int topMarginSize : 10
    property int leftMarginSize : 80

    signal closeClick()
    signal buttonClick()

    Image{
        id: imgBase
        anchors.top: parent.top
        anchors.topMargin: abnormalPopupRoot.topMarginSize
        anchors.left: parent.left
        anchors.leftMargin: abnormalPopupRoot.leftMarginSize
        source: abnormalPopupRoot.baseImageSource
        Text {
            id:abnormalText
            anchors.top: imgBase.top
            anchors.topMargin: 20
            anchors.left: imgBase.left
            anchors.leftMargin: 35
            text:abnormalPopupRoot.abnormalText
            color: abnormalPopupRoot.abnormalTextColor
            font.pixelSize:20
        }
        ImageButton{
            id: closeButton
            anchors.top:imgBase.top
            anchors.topMargin: 0
            anchors.right:imgBase.right
            anchors.rightMargin: 0
            downImageSource: abnormalPopupRoot.downButtonImageSource
            upImageSource: abnormalPopupRoot.upButtonImageSource
            onClicked: {
                abnormalPopupRoot.closeClick()
                abnormalPopupRoot.close()
            }
        }
        ImageButton{
            id: exeButton
            anchors.bottom:imgBase.bottom
            anchors.bottomMargin:15
            anchors.horizontalCenter: imgBase.horizontalCenter
            downImageSource: abnormalPopupRoot.buttonOnImageSource
            upImageSource: abnormalPopupRoot.buttonOffImageSource
            text:abnormalPopupRoot.buttonText
            textColor:abnormalPopupRoot.buttonTextColor
            fontPixelSize:20
            onClicked: {
                abnormalPopupRoot.buttonClick()
                abnormalPopupRoot.close()
            }
        }
    }
}
