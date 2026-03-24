import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import TeachingModePageModel 1.0
import TeachingModeData 1.0
import "../../Widget"
import "../../Components"
/*************************************************************************************
* Title: UI/UX of AEK
* File: MainMenu_2.qml
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
    id:menu2
    width: 500
    height: 511

    readonly property var programViewModel: teachingViewModel.pageModel[6]
    property var progMenu2: programViewModel.progMenu2
    property int menuIndex: progMenu2.menuIndex
    property var stacktype: programViewModel.progMenu2.stackType
    property bool pageActive: false
    property var stackPageModel: teachingViewModel.pageModel[2]

    onPageActiveChanged: {
        if (pageActive) {
            // menu2_1.active = true
            menu2_2.active = true
            menu2_3.active = true
            menu2_4.active = true
        }else{
            stackPageModel.editFromProgEditScreenIng = false
        }
    }

    Language { id: language }

    // program page no.2(servo) page
    Rectangle{
        width: 430
        height: 85
        color: "#999999"
        Row{
            anchors.verticalCenter: parent.verticalCenter
            x:30
            y:10
            spacing: 2
            // Hide unused functions
            // fixed position
             Column{
                 visible: false
                 ImageButton{
                     anchors.horizontalCenter: parent.horizontalCenter
                     upImageSource:(menuIndex == 0) ? resourceManager.imagePath + "/16_program/d161_43.png" : resourceManager.imagePath + "/16_program/d161_39.png"
                     downImageSource: resourceManager.imagePath + "/16_program/d161_43.png"
                     onClicked:{
                         progMenu2.menuIndex = 0
                     }
                 }
                 Text{
                     width: 90
                     height: 26
                     verticalAlignment: Text.AlignVCenter
                     horizontalAlignment: Text.AlignHCenter
                     font.pixelSize: lineCount != 1 ? 16 : 18
                     color: "white"
                     text: resourceManager.label(41,17, language.value)
                 }
             }
             // position point
            Column{
                ImageButton{
                    anchors.horizontalCenter: parent.horizontalCenter
                    upImageSource:(menuIndex == 1) ? resourceManager.imagePath + "/16_program/a160_51.png" : resourceManager.imagePath + "/16_program/a160_50.png"
                    downImageSource: resourceManager.imagePath + "/16_program/a160_51.png"
                    onClicked:{
                        progMenu2.menuIndex = 1
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
                    text: resourceManager.label(41,18, language.value)
                }
            }
            // packing
            Column{
                ImageButton{
                    anchors.horizontalCenter: parent.horizontalCenter
                    upImageSource:(menuIndex == 2) ? resourceManager.imagePath + "/16_program/a160_53.png" : resourceManager.imagePath + "/16_program/a160_52.png"
                    downImageSource: resourceManager.imagePath + "/16_program/a160_53.png"
                    onClicked:{
                        progMenu2.menuIndex = 2
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
                    text: resourceManager.label(41,19, language.value)
                }
            }
            // others
            Column{
                ImageButton{
                    anchors.horizontalCenter: parent.horizontalCenter
                    upImageSource:(menuIndex == 3) ? resourceManager.imagePath + "/16_program/a160_55.png" : resourceManager.imagePath + "/16_program/a160_54.png"
                    downImageSource: resourceManager.imagePath + "/16_program/a160_55.png"
                    onClicked:{
                        progMenu2.menuIndex = 3
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
                    text: resourceManager.label(41,20, language.value)
                }
            }
        }
    }

    StackLayout{
        id: menu2_stack
        width: parent.width
        height: 453
        y:85
        currentIndex: menuIndex
        // fixed position
        Loader {
            id: menu2_1
            property bool isActive: menu2.pageActive && (menu2_stack.currentIndex === 0)
            active: false
            asynchronous: !isActive
            visible: (status == Loader.Ready) && isActive
            sourceComponent: MainMenu_2_1 {
                width: parent.width
                height: parent.height
                pageActive: menu2_1.isActive
            }
        }
        // position point
        Loader {
            id: menu2_2
            property bool isActive: menu2.pageActive && (menu2_stack.currentIndex === 1)
            active: false
            asynchronous: !isActive
            visible: (status == Loader.Ready) && isActive
            sourceComponent: MainMenu_2_2 {
                width: parent.width
                height: parent.height
                pageActive: menu2_2.isActive
            }
        }
         // packing
        Loader {
            id: menu2_3
            property bool isActive: menu2.pageActive && (menu2_stack.currentIndex === 2)
            active: false
            asynchronous: !isActive
            visible: (status == Loader.Ready) && isActive
            sourceComponent: MainMenu_2_3 {
                width: parent.width
                height: parent.height
                pageActive: menu2_3.isActive
            }
        }
        // others
        Loader {
            id: menu2_4
            property bool isActive: menu2.pageActive && (menu2_stack.currentIndex === 3)
            active: false
            asynchronous: !isActive
            visible: (status == Loader.Ready) && isActive
            sourceComponent: MainMenu_2_4 {
                width: parent.width
                height: parent.height
                pageActive: menu2_4.isActive
            }
        }//其他
    }
}
