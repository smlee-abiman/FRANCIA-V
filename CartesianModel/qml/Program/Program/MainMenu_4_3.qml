import QtQml 2.12
import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import "../../Widget"
import "../../Components"
/*************************************************************************************
* Title: UI/UX of AEK
* File: MainMenu_4_3.qml
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
    id:root
    width: parent.width
    height: parent.height
    color: "transparent"
    border.width: 2
    border.color: "#999999"
    readonly property var programViewModel: teachingViewModel.pageModel[6]
    property bool pageActive: false
    function setMenuIndex(index) {
        local.menuModel.menuIndex = index
    }

    QtObject {
        id: local
        readonly property var menu4Model: programViewModel.progMenu4
        readonly property var menuModel: menu4Model.subMenuList[2]
        readonly property int menuIndex: menuModel.menuIndex
        readonly property bool isSelectedMenu431: (menuIndex === 0)
        readonly property bool isSelectedMenu432: (menuIndex === 1)
        readonly property bool isSelectedMenu433: (menuIndex === 2)
        readonly property bool isSelectedMenu434: (menuIndex === 3)
    }

    onPageActiveChanged: {
        if (pageActive) {
            menu_4_3_1.active = true
            menu_4_3_2.active = true
            menu_4_3_3.active = true
            menu_4_3_4.active = true
            local.menuModel.activate()
        } else {
            setMenuIndex(0)
            local.menuModel.deactivate()
        }
    }

    Component.onDestruction: local.menuModel.deactivate()

    Language { id: language }

    Rectangle{
        y:8
        width: 400
        height: 40
        anchors.left: parent.left
        anchors.leftMargin: 4
        Row{
            anchors.centerIn: parent
            spacing: 2
            Repeater{
                model: 4
                Rectangle{
                    width: 94
                    height: 40
                    border.width: 2
                    border.color: "#999999"
                    color: local.menuIndex === index ? "#cc3333" : "#cccccc"
                    Text{
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 18
                        font.bold: true
                        text: resourceManager.label(41,625+index, language.value)
                        color: local.menuIndex === index ? "white" : "#666666"
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            root.setMenuIndex(index)
                        }
                    }
                }
            }
        }
    }
    StackLayout{
        width: parent.width
        height: 401
        anchors.horizontalCenter: parent.horizontalCenter
        y:50
        currentIndex: local.menuIndex
        Loader {
            id: menu_4_3_1
            property bool isActive: root.pageActive && local.isSelectedMenu431
            active: false
            asynchronous: !isActive
            visible: (status == Loader.Ready) && isActive
            sourceComponent: MainMenu_4_3_1 {
                pageActive: menu_4_3_1.isActive
            }
        }//计时器+1

        Loader {
            id: menu_4_3_2
            property bool isActive: root.pageActive && local.isSelectedMenu432
            active: false
            asynchronous: !isActive
            visible: (status == Loader.Ready) && isActive
            sourceComponent: MainMenu_4_3_2 {
                pageActive: menu_4_3_2.isActive
            }
        }//计数器复位

        Loader {
            id: menu_4_3_3
            property bool isActive: root.pageActive && local.isSelectedMenu433
            active: false
            asynchronous: !isActive
            visible: (status == Loader.Ready) && isActive
            sourceComponent: MainMenu_4_3_3 {
                pageActive: menu_4_3_3.isActive
            }
        }//寄存器+1

        Loader {
            id: menu_4_3_4
            property bool isActive: root.pageActive && local.isSelectedMenu434
            active: false
            asynchronous: !isActive
            visible: (status == Loader.Ready) && isActive
            sourceComponent: MainMenu_4_3_4 {
                pageActive: menu_4_3_4.isActive
            }
        }//寄存器-1
    }
}
