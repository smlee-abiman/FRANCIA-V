import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import "../../Widget"
import "../../Components"
/*************************************************************************************
* Title: UI/UX of AEK
* File: MainMenu_5.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
    - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.21
* Worker: SH.HONG
**************************************************************************************/
Item {
    id:menu5
    width: 370
    height: 511

    readonly property var programViewModel: teachingViewModel.pageModel[6]
    property var subMenu2Model: programViewModel.progMenu5.subMenuList[1]
    property var progMenu5: programViewModel.progMenu5
    property int menuIndex: progMenu5.menuIndex
    property bool pageActive: false

    onPageActiveChanged: {
        if (pageActive) {
            menu5_1.active = true
            menu5_2.active = true
        }
    }

    Language { id: language }

    Rectangle{
        width: 430
        height: 85
        color: "#999999"
        Row{
            anchors.verticalCenter: parent.verticalCenter
            x:31
            spacing: 2
            Column{
                ImageButton{
                    anchors.horizontalCenter: parent.horizontalCenter
                    upImageSource:(menuIndex == 0) ? resourceManager.imagePath + "/16_program/a160_67.png" : resourceManager.imagePath + "/16_program/a160_66.png"
                    downImageSource: resourceManager.imagePath + "/16_program/a160_67.png"
                    onClicked: {
                        clear_select()
                        programViewModel.progMenu5.Menu5_Index = 0
                        progMenu5.menuIndex = 0
                    }
                }
                Text{
                    width: 90
                    height: 26
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: lineCount != 1 ? 16 : 18
                    font.bold: true

                    color:"black"
                    text: resourceManager.label(41,26, language.value)
                }
            }
            Column{
                ImageButton{
                    anchors.horizontalCenter: parent.horizontalCenter
                    upImageSource:(menuIndex == 1) ? resourceManager.imagePath + "/16_program/a160_69.png" : resourceManager.imagePath + "/16_program/a160_68.png"
                    downImageSource: resourceManager.imagePath + "/16_program/a160_69.png"
                    onClicked: {
                        programViewModel.progMenu5.Menu5_Index = 1
                        progMenu5.menuIndex = 1
                        clear_select()
                    }
                }
                Text{
                    width: 90
                    height: 26
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: lineCount != 1 ? 16 : 18
                    font.bold: true

                    color:"black"
                    text: resourceManager.label(41,27, language.value)
                }
            }
        }
    }

    StackLayout{
        id: menu5_stack
        width: parent.width
        height: 453
        y:85
        currentIndex: menuIndex

        Loader {
            id: menu5_1
            property bool isActive: menu5.pageActive && (menu5_stack.currentIndex === 0)
            active: false
            asynchronous: !isActive
            visible: (status == Loader.Ready) && isActive
            sourceComponent: MainMenu_5_1 {
            width: parent.width
            height: parent.height
                pageActive: menu5_1.isActive
            }
        }

        Loader {
            id: menu5_2
            property bool isActive: menu5.pageActive && (menu5_stack.currentIndex === 1)
            active: false
            asynchronous: !isActive
            visible: (status == Loader.Ready) && isActive
            sourceComponent: MainMenu_5_2 {
                width: parent.width
                height: parent.height
                pageActive: menu5_2.isActive
            }
        }
    }

    function clear_select()
    {
        let menu5_1Model = progMenu5.subMenuList[1]
        menu5_1Model.clearSelect()
    }
}
