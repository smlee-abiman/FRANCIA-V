import QtQuick 2.12
import "../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: SpeedControl.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
    - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.06.23
* Worker: IJ.YI
**************************************************************************************/
Item{
    width: 394
    height: 90
    property int value: 0
    property int maxValue: 6
    property string valueText: ""
    property color valueTextColor
    signal plusButtonPressed(int value)
    signal minusButtonPressed(int value)

    Row{
        spacing: 14
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        ImageButton{
            anchors.verticalCenter: parent.verticalCenter
            upImageSource: resourceManager.imagePath + "/08_MENU/a080_17.png"
            downImageSource: resourceManager.imagePath + "/08_MENU/a080_18.png"
            onClicked: {
                var tempValue = value
                if (tempValue > 0) {
                    tempValue--
                    minusButtonPressed(tempValue)
                }
            }
        }
        Rectangle{
            width: 190
            height: 60
            border.width: 2
            radius: 10
            color: "#CCCCCC"
            border.color: "#999999"
            Text{
                id: free_speed
                anchors.centerIn: parent
                font.pixelSize: 25
                font.bold: true
                text: valueText
                color: "#666666"
            }
        }
        ImageButton{
            anchors.verticalCenter: parent.verticalCenter
            upImageSource: resourceManager.imagePath + "/08_MENU/a080_15.png"
            downImageSource: resourceManager.imagePath + "/08_MENU/a080_16.png"
            onClicked: {
                var tempValue = value
                if (tempValue < maxValue) {
                    tempValue++
                    plusButtonPressed(tempValue)
                }
            }
        }
    }
}
