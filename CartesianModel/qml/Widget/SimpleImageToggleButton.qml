import QtQuick 2.12
import QtQuick.Layouts 1.3

Item {
    id: root
    width: imgOff.width
    height: imgOff.height

    property bool on: false
    property string text: ""
    property string offImage: ""
    property string onImage: ""
    property string icon: ""
    property int fontSize: 20
    property string fontColor: "white"
    property int textHAlignment: Text.AlignHCenter
    signal pressed()
    signal released()
    signal clicked()

    Image {
        id: imgOff
        visible: (!root.on)
        source: root.offImage
    }

    Image {
        id: imgOn
        visible: root.on
        source: root.onImage
    }

    RowLayout {
        anchors.centerIn : parent

        Image{
            visible: (root.icon.length > 0)
            source: root.icon
        }

        Text {
            color: root.fontColor
            font.pixelSize: root.fontSize
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: textHAlignment
            text: root.text
        }
    }

    MouseArea{
        anchors.fill: parent
        onPressed: {
            root.pressed()
        }
        onReleased: {
            root.released()
        }
        onClicked: {
            root.clicked()
            phyKeyManager.ctrlBuzzer()
        }
    }
}
