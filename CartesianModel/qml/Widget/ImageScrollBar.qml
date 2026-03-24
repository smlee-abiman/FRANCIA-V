import QtQuick 2.12
import QtQml 2.12

Item {
    id: root
    width: btnPageUp.width
    property string topButtonPressImage: ""
    property string topButtonReleaseImage: ""
    property string botomButtonPressImage: ""
    property string botomButtonReleaseImage: ""

    property string pageLabelColor: "black"
    property string pageLabal: ""
    property bool jogScrollOn: false

    signal scrollUpClicked()
    signal scrollDownClicked()

    //pageラベル
    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        color: root.pageLabelColor
        font.pixelSize: 18
        font.bold:true
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        text: root.pageLabal
    }

    ImageButton{
        id: btnPageUp
        anchors.left: parent.left
        downImageSource: root.topButtonPressImage
        upImageSource: root.topButtonReleaseImage
        onClicked: {
            root.scrollUpClicked();
        }
    }
    ImageButton{
        id: btnPageDown
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        downImageSource: root.botomButtonPressImage
        upImageSource: root.botomButtonReleaseImage
        onClicked: {
            root.scrollDownClicked();
        }
    }
//    Connections{
//        target:phyKeyManager
//        onJogScrollDownChanged:{
//            if(root.jogScrollOn){
//                //console.log("received jogScrollDown:" + jogScrollDown)
//                root.scrollDownClicked();
//            }
//        }
//        onJogScrollUpChanged:{
//            if(root.jogScrollOn){
//                //console.log("received jogScrollUp:" + jogScrollUp)
//                root.scrollUpClicked();
//            }
//        }
//    }//Jog switch page unuse
}
