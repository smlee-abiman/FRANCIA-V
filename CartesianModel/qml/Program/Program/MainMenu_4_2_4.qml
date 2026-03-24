import QtQml 2.12
import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import "../../Widget"
import "../../Components"
/*************************************************************************************
* Title: UI/UX of AEK
* File: MainMenu_4_2_4.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
    - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.10
* Worker: SH.HONG
**************************************************************************************/
//返回
Rectangle{
    width: parent.width
    height: parent.height
    color: "transparent"

    readonly property var programViewModel: teachingViewModel.pageModel[6]
    property bool pageActive: false

    QtObject {
        id: local
        readonly property var menu4Model: programViewModel.progMenu4
        readonly property var menu42Model : menu4Model.subMenuList[1]
        readonly property var menuModel : menu42Model.subMenuList[3]    // ProgramMenu4_2_4
    }

    onPageActiveChanged: {
        if (pageActive) {
            local.menuModel.activate()
        } else {
            local.menuModel.deactivate()
        }
    }

    Component.onDestruction: local.menuModel.deactivate()

    Language { id: language }

    Rectangle{
        x: 10
        width: 382
        height: 40
        border.color: "#999999"
        color: "#eaeaea"

        CheckBox{
            width: 100
            height: 36
            x:15
            anchors.verticalCenter: parent.verticalCenter
            indicator:Image{
                scale: 1.3
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                source: parent.checked ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
            }
            contentItem: Text{
                text: resourceManager.label(41,544, language.value)
                color: resourceManager.rgb(41,544)
                font.pixelSize: 18
                font.bold: true
                leftPadding: 30
                topPadding:2
            }
            checked: local.menuModel.use
            onClicked: local.menuModel.use = checked
        }
    }
}
