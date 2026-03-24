import QtQuick 2.12
import QtQml 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import Enums 1.0
import "../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: DummyFileDialogTitle.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
 - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.23
* Worker: SH.HONG
**************************************************************************************/
Rectangle{
    width: 1018
    height: 65
    radius: 6
    color: "#999999"
    border.color: "#999999"
    border.width: 2

    Row{
        anchors.centerIn: parent
        spacing: 10
        Image {
            x:200
            y:2
            source: resourceManager.imagePath + "/28_USB/d280_01.png"
        }
        Text{
            font.pixelSize: 26
            color: resourceManager.rgb(51,0)
            text: resourceManager.label(51,0,resourceManager.language)
        }
    }
    ImageButton{
        x:945
        y:14
        scale: 1.5
        upImageSource: resourceManager.imagePath + "/28_USB/a280_02.png"
        downImageSource: resourceManager.imagePath + "/28_USB/a280_02.png"
        onClicked: {
        }
    }

}
