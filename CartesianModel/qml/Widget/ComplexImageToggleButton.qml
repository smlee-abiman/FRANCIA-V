import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQml 2.12

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
    property bool isEnablePressingSignal:false
    property bool isEnablePressing:false
    signal pressed()
    signal released()
    signal clicked()
    signal pressing()

    Timer {
        id: pressTimer
        interval: 25
        running: false
        repeat: false
        onTriggered: {
            if (root.isEnablePressing === true){
                root.pressing()
            }
        }
    }

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
            pressTimer.repeat = true
            pressTimer.running = true
            if (root.isEnablePressingSignal === true){
                root.isEnablePressing = true
            }
        }
        onReleased: {
            root.isEnablePressing = false
            pressTimer.repeat = false
            pressTimer.repeat = false
            root.released()
        }
        onClicked: {
            root.clicked()
        }
    }
}
