import QtQuick 2.12
import QtQuick.Controls 2.12
import "./Widget"
Rectangle {
    id:root
    width: 1024
    height: 678
    color: "#DAE3F3"

    signal onBottomClick()


    Operation {
        id: mainview
        width: 1018
        height: 590 + 85
        z:1
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 3
        anchors.leftMargin: 3
   }//中部主画面部分

}

