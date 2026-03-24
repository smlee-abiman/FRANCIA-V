import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import "../../Widget"
import "../../Components"
/*************************************************************************************
* Title: UI/UX of AEK
* File: MainMenu_3.qml
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
    id:menu3
    width: 370
    height: 511

    readonly property var programViewModel: teachingViewModel.pageModel[6]
    //property int menuIndex: programViewModel.progMenu3.Menu3_Index
    property var progMenu3: programViewModel.progMenu3
    property int menuIndex: progMenu3.menuIndex
    property bool pageActive: false

    onPageActiveChanged: {
        if (pageActive) {
            menu3_1.active = true
            menu3_2.active = true
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
                    upImageSource:(menuIndex == 0) ? resourceManager.imagePath + "/16_program/a160_57.png" : resourceManager.imagePath + "/16_program/a160_56.png"
                    downImageSource: resourceManager.imagePath + "/16_program/a160_57.png"
                    onClicked: {
                        programViewModel.progMenu3.Menu3_Index = 0
                        progMenu3.menuIndex = 0
                    }
                }
                Text{
                    width: 90
                    height: 26
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: lineCount != 1 ? 16 : 18
                    font.bold: true

                    color: "black"
                    text: resourceManager.label(41,21, language.value)
                }
            }
            Column{
                ImageButton{
                    anchors.horizontalCenter: parent.horizontalCenter
                    upImageSource:(menuIndex == 1) ? resourceManager.imagePath + "/16_program/a160_59.png" : resourceManager.imagePath + "/16_program/a160_58.png"
                    downImageSource: resourceManager.imagePath + "/16_program/a160_59.png"
                    onClicked: {
                        programViewModel.progMenu3.Menu3_Index = 1
                        progMenu3.menuIndex = 1
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
                    text: resourceManager.label(41,22, language.value)
                }
            }
        }
    }//菜单栏

    StackLayout{
        id: menu3_stack
        width: parent.width
        height: 453
        y:85
        currentIndex: menu3.menuIndex
        Loader {
            id: menu3_1
            property bool isActive: menu3.pageActive && (menu3_stack.currentIndex === 0)
            active: false
            asynchronous: !isActive
            visible: (status == Loader.Ready) && isActive
            sourceComponent: MainMenu_3_1 {
            width: parent.width
            height: parent.height
                pageActive: menu3_1.isActive
            }
        }

        Loader {
            id: menu3_2
            property bool isActive: (menu3.pageActive && menu3_stack.currentIndex == 1)
            active: false
            asynchronous: !isActive
            visible: (status == Loader.Ready) && isActive
            sourceComponent: MainMenu_3_2 {
                width: parent.width
                height: parent.height
                pageActive: menu3_2.isActive
            }
        }
    }
}
