import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import TeachingModePageModel 1.0
import TeachingModeData 1.0
import "../../Widget"
import "../../Components"
/*************************************************************************************
* Title: UI/UX of AEK
* File: MainMenu_2_4.qml
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
Rectangle{
    id:menu2_4
    color: "transparent"
    border.width: 2
    border.color: "#B5B5B6"

    property bool pageActive: false
    readonly property var programViewModel: teachingViewModel.pageModel[6]
    property var pageModel: teachingViewModel.pageModel[1]
    property var stacktype: programViewModel.progMenu2.stackType
    property var pagemodel: programViewModel.progMenu2.pointlist

    property var progMenu2: programViewModel.progMenu2
    property var menuModel: programViewModel.progMenu2.subMenuList[3]
    property int menuIndex: menuModel.menuIndex

    onPageActiveChanged: {
        if (pageActive) {
            menu_2_4_1_loader.active = true
            menu_2_4_2_loader.active = true
            menu_2_4_3_loader.active = true
            menu_2_4_4_loader.active = true
            menu_2_4_5_loader.active = true
            menu_2_4_6_loader.active = true
            menu_2_4_7_loader.active = true
        } else {
            menuModel.menuIndex = 0
        }
    }

    Language { id: language }

    Rectangle{
        width: 120
        height: parent.height
        border.color: "#999999"
        color: "#eaeaea"
        Column{
            y: 10
            spacing: 4
            anchors.horizontalCenter: parent.horizontalCenter
            Repeater{
                model: 7
                Rectangle{
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: 100
                    height: 40
                    border.color: "#999999"
                    color:menu2_4.menuIndex == index ? "#cc3333" : "#cccccc"
                    Text{
                        anchors.fill: parent
                        font.pixelSize: 17
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.WordWrap
                        color: menu2_4.menuIndex == index ? "white" : "#666666"
                        text: resourceManager.label(41,514+index, language.value)
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked:{
                            menuModel.menuIndex = index
                        }
                    }
                }
            }
        }
    }
    StackLayout{
        id: menu2_4_stack
        width: 270
        height: parent.height
        x:100
        currentIndex: menu2_4.menuIndex

        Loader {
            id: menu_2_4_1_loader
            property bool isActive: menu2_4.pageActive && (menu2_4_stack.currentIndex === 0)
            active: false
            asynchronous: !isActive
            visible: (status == Loader.Ready) && isActive
            sourceComponent: MainMenu_2_4_1 {
                width: parent.width
                height: parent.height
                pageActive: menu_2_4_1_loader.isActive
            }
        }//直线插补

        Loader {
            id: menu_2_4_2_loader
            property bool isActive: menu2_4.pageActive && (menu2_4_stack.currentIndex === 1)
            active: false
            asynchronous: !isActive
            visible: (status == Loader.Ready) && isActive
            sourceComponent: MainMenu_2_4_2 {
                width: parent.width
                height: parent.height
                pageActive: menu_2_4_2_loader.isActive
            }
        }//圆弧插补

        Loader {
            id: menu_2_4_3_loader
            property bool isActive: menu2_4.pageActive && (menu2_4_stack.currentIndex === 2)
            active: false
            asynchronous: !isActive
            visible: (status == Loader.Ready) && isActive
            sourceComponent: MainMenu_2_4_3 {
                width: parent.width
                height: parent.height
                pageActive: menu_2_4_3_loader.isActive
            }
        }//轴PASS

        Loader {
            id: menu_2_4_4_loader
            property bool isActive: menu2_4.pageActive && (menu2_4_stack.currentIndex === 3)
            active: false
            asynchronous: !isActive
            visible: (status == Loader.Ready) && isActive
            sourceComponent: MainMenu_2_4_4 {
                width: parent.width
                height: parent.height
                pageActive: menu_2_4_4_loader.isActive
            }
        }//轴复归

        Loader {
            id: menu_2_4_5_loader
            property bool isActive: menu2_4.pageActive && (menu2_4_stack.currentIndex === 4)
            active: false
            asynchronous: !isActive
            visible: (status == Loader.Ready) && isActive
            sourceComponent: MainMenu_2_4_5 {
                width: parent.width
                height: parent.height
                pageActive: menu_2_4_5_loader.isActive
            }
        }//轴停止

        Loader {
            id: menu_2_4_6_loader
            property bool isActive: menu2_4.pageActive && (menu2_4_stack.currentIndex === 5)
            active: false
            asynchronous: !isActive
            visible: (status == Loader.Ready) && isActive
            sourceComponent: MainMenu_2_4_6 {
                width: parent.width
                height: parent.height
                pageActive: menu_2_4_6_loader.isActive
            }
        }//加减速

        Loader {
            id: menu_2_4_7_loader
            property bool isActive: menu2_4.pageActive && (menu2_4_stack.currentIndex === 6)
            active: false
            asynchronous: !isActive
            visible: (status == Loader.Ready) && isActive
            sourceComponent: MainMenu_2_4_7 {
                id: menu2_4_7
                width: parent.width
                height: parent.height
                pageActive: menu_2_4_7_loader.isActive
            }
        }//全轴完了
    }
}
