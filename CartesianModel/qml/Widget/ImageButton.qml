import QtQuick 2.12

Item {
    id: root
    width: imgUp.width
    height: imgUp.height
    property string downImageSource: ""
    property string upImageSource: ""
    property string textImageSource: ""
    property string textColor: "white"
    property string text: ""
    property int fontPixelSize: 12
    property bool enableBtnTextColorChg:false
    property string btnPressColor: "black"
    signal clicked()
    signal pressed()
    signal released()

    Image{
        id: imgUp
        source: root.upImageSource
    }

    Image{
        id: imgDown
        visible: false
        source: root.downImageSource
    }

    Image{
        id: imgText
        x: (imgUp.width - imgText.width) / 2
        y: (imgUp.height - imgText.height) / 2

        visible: (root.textImageSource.length > 0)
        source: root.textImageSource
    }

    Text {
        id:btnText
        anchors.fill: imgUp
        color: root.textColor
        font.pixelSize: root.fontPixelSize
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        lineHeight: 0.8
        text: root.text
        visible: (root.text.length > 0)
    }

    MouseArea{
        anchors.fill: imgUp
        onPressed: {
            root.state = "down";
            if (root.enableBtnTextColorChg === true){
                btnText.color = root.btnPressColor
            }
            root.pressed()
        }
        onReleased: {
            root.state = "up";
            if (root.enableBtnTextColorChg === true){
                btnText.color = root.textColor
            }
            root.released()
        }
        onClicked: {
            root.clicked();
            phyKeyManager.ctrlBuzzer()
        }
    }

    states: [
        State {
            name: "down"
            PropertyChanges { target: imgDown; visible: true }
            PropertyChanges { target: imgUp; visible: false }
        },
        State {
            name: "up"
            PropertyChanges { target: imgDown; visible: false }
            PropertyChanges { target: imgUp; visible: true }
        }
    ]

}
