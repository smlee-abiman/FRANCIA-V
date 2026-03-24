import QtQuick 2.12

Image {
    id:root
    property bool count: false
    property string use_te1: ""
    property string use_te2: ""
    property string use_te1_color: ""
    property string use_te2_color: ""

    signal toggleButtonClick(bool isOnOff)

    source: count ? resourceManager.imagePath + "/10_MODE/d100_12.png" : resourceManager.imagePath + "/10_MODE/d100_11.png"
    Image {
        id:indicator
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin:12
        anchors.verticalCenterOffset: 2
        source: resourceManager.imagePath + "/10_MODE/d100_13.png"
        state: count ? "right" : "left"
        states:[
            State{
                name:"left";
                //when:mousearea.pressed&count==0
                when:(count === false)
                changes: [
                    PropertyChanges{
                        target:indicator
                        anchors.leftMargin: 12
                        source: resourceManager.imagePath + "/10_MODE/d100_13.png"
                        restoreEntryValues: false
                    }
                ]
            },
            State{
                name:"right";
                //when:mousearea.pressed&count==1
                when:(count === true)
                changes:[
                    PropertyChanges{
                        target:indicator
                        anchors.leftMargin: 80
                        source: resourceManager.imagePath + "/10_MODE/d100_14.png"
                        restoreEntryValues: false;
                    }
                ]
            }
        ]
    }
    Text {
        id: te1
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 14
        font.pixelSize: 18
        color: use_te1_color
        text: use_te1
        visible: count
    }
    Text {
        id: te2
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: 14
        font.pixelSize: 18
        color: use_te2_color
        text: use_te2
        visible: !count
    }
    MouseArea{
        id:mousearea
        anchors.fill:parent;
        onClicked: {
            root.toggleButtonClick(!count)
            phyKeyManager.ctrlBuzzer()
        }
    }
}
