import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
//import QtQuick.Controls.Material 2.3
import QtQuick.Controls.Universal 2.3
import "qml/PageBitType"
import "qml/PageByteType"
import "qml/PageWordType"
import "qml/PageLongType"
import "qml/PageOtherType"
import "qml/PagePointType"

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("ITZ-3000  Alternative to pendant")
    //Material.theme: Material.Light
    //Material.accent: Material.Blue
    Universal.theme: Universal.Light
    Universal.accent: Universal.Blue

    SwipeView {
        id: mainSwipeView
        Layout.minimumWidth: 640    // No effect
        Layout.minimumHeight: 480   // No effect
        anchors.fill: parent
        currentIndex: tabBarDataType.currentIndex
        BitTypePage {
        }

        ByteTypePage {
        }

        WordTypePage {
        }

        LongTypePage {
        }

        OtherTypePage {
        }

        PointTypePage {

        }
    }

    footer: Row {
        spacing: 2
        TabBar {
            id: tabBarDataType
            width: parent.width - indicatorDataType.width
            height: 27
            contentHeight: 25
            currentIndex: mainSwipeView.currentIndex
            font.pixelSize: 14
            Repeater {
                model: [qsTr("BIT Type"), qsTr("BYTE Type"), qsTr("WORD Type"), qsTr("LONG Type"), qsTr("OTHER Type"), qsTr("POINT Type")]
                TabButton {
                    text: modelData
                    font.bold: true
                    background: Rectangle {
                        //color: "paleturquoise"
                        //color: "lightblue"
                        //color: "bisque"
                        color: "khaki"
                        border.width: 1
                        border.color: "saddlebrown"
                    }
                }
            }
        }

        PageIndicator {
            id: indicatorDataType
            width: 70
            scale: 1.0
            //topPadding: 20
            topPadding: 10
            rightPadding: 20
            count: mainSwipeView.count
            currentIndex: mainSwipeView.currentIndex
            //delegate: Rectangle {
            //    color: "paleturquoise"
            //}
        }
    }

    Popup {
        id: switchPopup
        property alias switchPopup: switchPopup
        property alias popupMsg: popupMsg
        //x: 400
        x: 2
        y: 2
        width: 100
        height: 28
        //width: popupMsg.width
        //height: popupMsg.height
        focus: true
        clip: false
        opacity: 0.75
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
        background: Rectangle {
            anchors.fill: parent
            anchors.topMargin: 0
            anchors.bottomMargin: 0
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            //color: "lightsteelblue"
            color: "#FFFF33"
            border.width: 1
            border.color: "goldenrod"
        }
        Text {
            id: popupMsg
            anchors.fill: parent
            anchors.topMargin: 0
            anchors.bottomMargin: 0
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            font.pixelSize: 14
            verticalAlignment: Text.AlignVCenter
            text: qsTr("項目名を表示")
        }
    }


    Item {
        id: commonFunc
        property alias commonFunc: commonFunc
        function setCaptWordAdr(objRepeater, lngBaseAdr) {
            for(var i = 0; i < objRepeater.count; i++){
                var txtAdr = objRepeater.itemAt(i).textAdr
                var iNum = i * 2 + lngBaseAdr;
                txtAdr.text = qsTr(("0000" + iNum.toString(16).toUpperCase()).slice(-4) + ":");
            }
            return 0;
        }
    }
}
