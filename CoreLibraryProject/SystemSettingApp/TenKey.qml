import QtQuick 2.0
import QtQuick.Controls 2.5

Popup {
    id: tenkey
    property string valueStr: ""

    onOpened: {
        valueStr = ""
    }

    Rectangle {
        id: valueField
        width: 226
        height: 44
        anchors.top: parent.top
        anchors.left: parent.left
        border.color: "black"

        Text {
            id: inputValue
            anchors.fill: parent
            text: valueStr
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 16
        }
    }

    Button {
        id: tenkey_seven
        width: 49
        height: 64
        text: qsTr("7")
        font.pointSize: 16
        anchors.top: valueField.bottom
        anchors.topMargin: 10
        anchors.left: valueField.left
        onClicked: {
            if(tenkey.valueStr.length < 15)
                tenkey.valueStr += "7"
        }
    }

    Button {
        id: tenkey_eight
        width: 49
        height: 64
        text: qsTr("8")
        font.pointSize: 16
        anchors.left: tenkey_seven.right
        anchors.leftMargin: 10
        anchors.top: tenkey_seven.top
        onClicked: {
            if(tenkey.valueStr.length < 15)
                tenkey.valueStr += "8"
        }
    }

    Button {
        id: tenkey_nine
        width: 49
        height: 64
        text: qsTr("9")
        font.pointSize: 16
        anchors.left: tenkey_eight.right
        anchors.leftMargin: 10
        anchors.top: tenkey_eight.top
        onClicked: {
            if(tenkey.valueStr.length < 15)
                tenkey.valueStr += "9"
        }
    }

    Button {
        id: tenkey_four
        width: 49
        height: 64
        text: qsTr("4")
        font.pointSize: 16
        anchors.top: tenkey_seven.bottom
        anchors.topMargin: 10
        anchors.left: tenkey_seven.left
        onClicked: {
            if(tenkey.valueStr.length < 15)
                tenkey.valueStr += "4"
        }
    }

    Button {
        id: tenkey_five
        width: 49
        height: 64
        text: qsTr("5")
        font.pointSize: 16
        anchors.left: tenkey_four.right
        anchors.leftMargin: 10
        anchors.top: tenkey_four.top
        onClicked: {
            if(tenkey.valueStr.length < 15)
                tenkey.valueStr += "5"
        }
    }

    Button {
        id: tenkey_six
        width: 49
        height: 64
        text: qsTr("6")
        font.pointSize: 16
        anchors.left: tenkey_five.right
        anchors.leftMargin: 10
        anchors.top: tenkey_five.top
        onClicked: {
            if(tenkey.valueStr.length < 15)
                tenkey.valueStr += "6"
        }
    }

    Button {
        id: tenkey_one
        width: 49
        height: 64
        text: qsTr("1")
        font.pointSize: 16
        anchors.top: tenkey_four.bottom
        anchors.topMargin: 10
        anchors.left: tenkey_four.left
        onClicked: {
            if(tenkey.valueStr.length < 15)
                tenkey.valueStr += "1"
        }
    }

    Button {
        id: tenkey_two
        width: 49
        height: 64
        text: qsTr("2")
        font.pointSize: 16
        anchors.left: tenkey_one.right
        anchors.leftMargin: 10
        anchors.top: tenkey_one.top
        onClicked: {
            if(tenkey.valueStr.length < 15)
                tenkey.valueStr += "2"
        }
    }

    Button {
        id: tenkey_three
        width: 49
        height: 64
        text: qsTr("3")
        font.pointSize: 16
        anchors.left: tenkey_two.right
        anchors.leftMargin: 10
        anchors.top: tenkey_two.top
        onClicked: {
            if(tenkey.valueStr.length < 15)
                tenkey.valueStr += "3"
        }
    }

    Button {
        id: tenkey_zero
        width: 49
        height: 64
        text: qsTr("0")
        font.pointSize: 16
        anchors.top: tenkey_one.bottom
        anchors.topMargin: 10
        anchors.left: tenkey_one.left
        onClicked: {
            if((tenkey.valueStr.length != 0) && (tenkey.valueStr.length < 15))
                tenkey.valueStr += "0"
        }
    }

    Button {
        id: tenkey_dot
        width: 49
        height: 64
        text: qsTr(".")
        font.pointSize: 16
        anchors.left: tenkey_zero.right
        anchors.leftMargin: 10
        anchors.top: tenkey_zero.top
        onClicked: {
            if((tenkey.valueStr.length != 0) && (tenkey.valueStr.length < 15))
                tenkey.valueStr += "."
        }
    }

    Button {
        id: tenkey_del
        width: 49
        height: 64
        text: qsTr("DEL")
        font.pointSize: 13
        anchors.left: tenkey_dot.right
        anchors.leftMargin: 10
        anchors.top: tenkey_dot.top
        onClicked: {
            if(tenkey.valueStr.length > 0)
                tenkey.valueStr = tenkey.valueStr.substring(0, tenkey.valueStr.length - 1)
        }
    }

    Button {
        id: tenkey_equal
        width: 49
        height: 64
        text: qsTr("=")
        font.pointSize: 16
        anchors.left: tenkey_del.right
        anchors.leftMargin: 10
        anchors.top: tenkey_del.top
        onClicked: {
            close()
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
