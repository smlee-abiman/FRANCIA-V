import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import "../../Widget"
import "../../Components"
import "../.."
/*************************************************************************************
* Title: UI/UX of AEK
* File: MainMenu_1.qml
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
    id:menu1
    width: 370
    height: 538
    readonly property var programViewModel: teachingViewModel.pageModel[6]
    property var progMenu1: programViewModel.progMenu1
    property bool pageActive: false
    property int menuIndex: progMenu1.menuIndex

    onPageActiveChanged: {
        if (pageActive) {
            componentLoader.loadNextComponent()
        }
    }

    ComponentLoader {
        id: componentLoader
        loadComponentList: [
            menu1_1,
            menu1_2,
            menu1_3,
            menu1_4
        ]
    }

    Language { id: language }

    // program page no.1 io signal page
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
             Column{
                 visible: false
                 ImageButton{
                     anchors.horizontalCenter: parent.horizontalCenter
                     upImageSource: (menuIndex == 0) ? resourceManager.imagePath + "/16_program/d161_35.png" : resourceManager.imagePath + "/16_program/d161_31.png"
                     downImageSource: resourceManager.imagePath + "/16_program/d161_35.png"
                     onClicked: {
                         progMenu1.menuIndex = 0
                         clear_select()
                     }
                 }
                 Text{
                     width: 90
                     height: 26
                     verticalAlignment: Text.AlignVCenter
                     horizontalAlignment: Text.AlignHCenter
                     font.pixelSize: lineCount != 1 ? 16 : 18

                     color:resourceManager.rgb(41, 13)
                     text: resourceManager.label(41,13, language.value)
                 }
             }
             // Hide unused functions
             Column{
                 visible: false
                 ImageButton{
                     anchors.horizontalCenter: parent.horizontalCenter
                     upImageSource:(menuIndex == 1) ? resourceManager.imagePath + "/16_program/d161_36.png" : resourceManager.imagePath + "/16_program/d161_32.png"
                     downImageSource: resourceManager.imagePath + "/16_program/d161_36.png"
                     onClicked:{
                         progMenu1.menuIndex = 1
                         clear_select()
                     }
                 }
                 Text{
                     width: 90
                     height: 26
                     verticalAlignment: Text.AlignVCenter
                     horizontalAlignment: Text.AlignHCenter
                     font.pixelSize: lineCount != 1 ? 16 : 18

                     color:resourceManager.rgb(41, 14)
                     text: resourceManager.label(41,14, language.value)
                 }
             }
             // Hide unused functions
             Column{
                 visible: false
                 ImageButton{
                     anchors.horizontalCenter: parent.horizontalCenter
                     upImageSource:(menuIndex == 2) ? resourceManager.imagePath + "/16_program/d161_37.png" : resourceManager.imagePath + "/16_program/d161_33.png"
                     downImageSource: resourceManager.imagePath + "/16_program/d161_37.png"
                     onClicked:{
                         progMenu1.menuIndex = 2
                         clear_select()
                     }
                 }
                 Text{
                     width: 90
                     height: 26
                     verticalAlignment: Text.AlignVCenter
                     horizontalAlignment: Text.AlignHCenter
                     font.pixelSize: lineCount != 1 ? 16 : 18
                     color:resourceManager.rgb(41, 15)
                     text: resourceManager.label(41,15, language.value)
                 }
             }
            // out put signal
            Column{
                ImageButton{
                    anchors.horizontalCenter: parent.horizontalCenter
                    upImageSource:(menuIndex == 3) ? resourceManager.imagePath + "/16_program/a160_49.png" : resourceManager.imagePath + "/16_program/a160_49.png"
                    downImageSource: resourceManager.imagePath + "/16_program/a160_49.png"
                    onClicked:{
                        progMenu1.menuIndex = 3
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
                    color: "black"
                    text: resourceManager.label(41,16, language.value)
                }
            }
        }
    }//菜单栏

    StackLayout{
        id: menu1_stack
        width: parent.width
        height: 453
        y:85
        // NOTE : Due to button deactivation, menuIndex is fixed at 3. By.SH.HONG 25.7.19
//         currentIndex: menu1.menuIndex
        currentIndex: 3
        Loader {
            id: menu1_1
            property bool isActive: menu1.pageActive && (menu1_stack.currentIndex === 0)
            asynchronous: !isActive
            visible: (status == Loader.Ready) && isActive
            sourceComponent: MainMenu_1_1 {
                width: parent.width
                height: parent.height
                pageActive: menu1_1.isActive
            }
            onLoaded: componentLoader.loadNextComponent()
        }//注塑机

        Loader {
            id: menu1_2
            property bool isActive: menu1.pageActive && (menu1_stack.currentIndex === 1)
            active: false
            asynchronous: !isActive
            visible: (status == Loader.Ready) && isActive
            source: "qrc:/qml/Program/Program/MainMenu_1_2.qml"
            onLoaded: {
                item.pageActive = isActive
                componentLoader.loadNextComponent()
            }
            onIsActiveChanged: {
                if (item != null) {
                    item.pageActive = isActive;
                }
            }
        }//机械手

        Loader {
            id: menu1_3
            property bool isActive: menu1.pageActive && (menu1_stack.currentIndex === 2)
            active: false
            asynchronous: !isActive
            visible: (status == Loader.Ready) && isActive
            source: "qrc:/qml/Program/Program/MainMenu_1_3.qml"
            onLoaded: {
                item.pageActive = isActive
                componentLoader.loadNextComponent()
            }
            onIsActiveChanged: {
                if (item != null) {
                    item.pageActive = isActive;
                }
            }
        }//治具

        Loader {
            id: menu1_4
            property bool isActive: menu1.pageActive && (menu1_stack.currentIndex === 3)
            active: false
            asynchronous: !isActive
            visible: (status == Loader.Ready) && isActive
            source: "qrc:/qml/Program/Program/MainMenu_1_4.qml"
            onLoaded: {
                item.pageActive = isActive
                componentLoader.loadNextComponent()
            }
            onIsActiveChanged: {
                if (item != null) {
                    item.pageActive = isActive;
                }
            }
        }//其他
    }

    function clear_select(){
        let menu1_4Model = progMenu1.subMenuList[3]
        menu1_4Model.clearSelect()
    }
}
