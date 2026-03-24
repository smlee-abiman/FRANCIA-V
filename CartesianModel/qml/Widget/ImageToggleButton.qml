import QtQuick 2.12
import QtQuick.Layouts 1.3


Item{
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
    property bool isRadioButton: false
    property int textHAlignment: Text.AlignHCenter

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

    RowLayout{
        anchors.centerIn : imgOff
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
        anchors.fill: imgOff
        onClicked: {
            if(isRadioButton){
                if(!root.on){
                    root.on = true;
                }
            }else{
                root.on = (!root.on);
            }
        }
    }
}
