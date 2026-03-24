import QtQuick 2.0
/*************************************************************************************
* Title: UI/UX of AEK
* File: Area_Input.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
 - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.16
* Worker: SH.HONG
**************************************************************************************/
Item {
    id:area

    signal clicked(string data,string data1)

    property bool edit: false
    property bool disable: false
    property int edindex: 0
    property alias valueColor: value.color
    property alias valueText: value.text
    property string m_color: "white"

    Rectangle{
        width: parent.width
        height: parent.height
        anchors.fill: parent
        radius: 8
        color: disable ? "#999999" : edit ? "#cc3333" : "white"
        border.color: "#999999"
        border.width: 2

        MouseArea{
            anchors.fill: parent
            onClicked: {
                let valueText = value.text
                let str = ""
                let str1 = valueText.substr(0, valueText.length )
                area.clicked(str,str1)
            }
        }
    }

    Text {
        id:value
        anchors.centerIn: parent
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignRight
        font.pointSize: 18
    }
}
