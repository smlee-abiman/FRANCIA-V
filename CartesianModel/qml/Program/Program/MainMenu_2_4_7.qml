import QtQuick 2.12
import QtQuick.Controls 2.5
import "../../Components"
/*************************************************************************************
* Title: UI/UX of AEK
* File: MainMenu_2_4_7.qml
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
Rectangle{
    property bool pageActive: false
    color: "transparent"

    QtObject {
        id: local
        readonly property var programViewModel: teachingViewModel.pageModel[6]
        property var menuModel: programViewModel.progMenu2.subMenuList[3].subMenuList[6]
        property bool checked: menuModel.checked
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
    Rectangle {
        x: 30
        y: 20
        width: 250
        height: 50
        color: "#eaeaea"
        border.color: "#999999"

        CheckBox {
            x: 20
            anchors.verticalCenter: parent.verticalCenter
            width: 100
            height: 36
            checked: local.checked

            indicator: Image {
                scale: 1.3
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                source: parent.checked ? resourceManager.imagePath + "/16_program/a160_73.png" : resourceManager.imagePath + "/16_program/a160_72.png"
            }

            contentItem: Text {
                text: resourceManager.label(41,532, language.value)
                color: resourceManager.rgb(41,532)
                font.pixelSize: 17
                font.bold: true
                leftPadding: 30
                topPadding: 4
            }

            onClicked: {
                local.menuModel.checked = checked
                checked = Qt.binding(function() { return local.checked })
            }
        }
    }
}
