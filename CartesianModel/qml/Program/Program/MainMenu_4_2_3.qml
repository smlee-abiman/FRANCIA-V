import QtQml 2.12
import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import "../../Widget"
import "../../Components"
/*************************************************************************************
* Title: UI/UX of AEK
* File: MainMenu_4_2_3.qml
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
//并行
Rectangle{
    width: parent.width
    height: parent.height
    color: "transparent"

    readonly property var programViewModel: teachingViewModel.pageModel[6]
    property bool pageActive: false

    QtObject {
        id: local
        readonly property var menu4Model: programViewModel.progMenu4    // ProgramMenu4
        readonly property var menu42Model: menu4Model.subMenuList[1]    // ProgramMenu4_2
        readonly property var menuModel: menu42Model.subMenuList[2]     // ProgramMenu4_2_3
        readonly property var programNoList: menuModel.programNoList
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

    Component {
        id: listDelegate4_2_3
        Rectangle{
            x: 10
            width: 382
            height: 80
            readonly property int programNo: local.programNoList[index]
            readonly property bool isSelected: (index === local.menuModel.selectedIndex)
            border.color: "#999999"
            color: "#eaeaea"
            Row{
                x:8
                anchors.verticalCenter: parent.verticalCenter
                spacing: 6
                Text{
                    width: 120
                    height: 30
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    font.pixelSize: 18
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                    color:resourceManager.rgb(41, 623 + index)
                    text: resourceManager.label(41,623 + index, language.value)
                }
                Rectangle{
                    width: 220
                    height: 40
                    anchors.verticalCenter: parent.verticalCenter
                    border.color: "#999999"
                    color: isSelected ? "#cc3333" : "white"
                    Text {
                        readonly property int resourceIndex: 37 + (programNo - 1) // program no ... 1base.
                        x: 15
                        visible: (1 <= programNo)
                        font.pixelSize: 18
                        font.bold: true
                        anchors.verticalCenter: parent.verticalCenter
                        color: "black"
                        text: resourceManager.label(41, resourceIndex, language.value)
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            local.menuModel.selectedIndex = (isSelected && (1 <= programNo)) ? -1 : index
                            if (isSelected) {
                                local.menuModel.openProgramSelectionDialog = true
                            }
                        }
                    }
                }
            }
        }
    }

    Column {
        anchors.fill: parent
        spacing: -1
        Repeater {
            clip: true
            model: local.programNoList.length
            delegate: listDelegate4_2_3
        }
    }
}
