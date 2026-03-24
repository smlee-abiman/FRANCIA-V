import QtQuick 2.0

Rectangle{
    id:point

    property bool edit: false
    property bool disable: false
    property int edindex: 0
    property alias valueColor: value.color
    property alias valueText: value.text
    property string m_color: "white"
    signal clicked(string data,string data1)

    width: parent.width
    height: parent.height
    radius: 8
    color: disable ? "grey" : edit ? "#daff00" : "white"
    border.color: "grey"
    border.width: 2

    Text {
        id:value
        anchors.centerIn: parent
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignRight
        font.pointSize: 18
    }

    MouseArea {
        anchors.fill: parent

        onClicked: {
            let valueText = value.text
            let str = ""
            let str1 = valueText.substr(0, valueText.length )
            point.clicked(str,str1)
        }
    }
}
