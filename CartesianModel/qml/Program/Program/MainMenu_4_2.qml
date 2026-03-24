import QtQml 2.12
import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import "../../Widget"
import "../../Components"
/*************************************************************************************
* Title: UI/UX of AEK
* File: MainMenu_4_2.qml
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
    function clearSelect() {
        transfer.clearSelect()
        jump.clearSelect()
    }

    QtObject {
        id: local
        readonly property var menu4Model: programViewModel.progMenu4    // ProgramMenu4
        readonly property var menuModel: menu4Model.subMenuList[1]      // ProgramMenu4_2
        readonly property int menuIndex: menuModel.menuIndex
        readonly property bool isSelectedMenu421: (menuIndex === 0)
        readonly property bool isSelectedMenu422: (menuIndex === 1)
        readonly property bool isSelectedMenu423: (menuIndex === 2)
        readonly property bool isSelectedMenu424: (menuIndex === 3)
    }

    onPageActiveChanged: {
        if (pageActive) {
            transfer.active = true
            jump.active = true
            parallel.active = true
            ret.active = true
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
        width: 370
        height: 40
        anchors.left: parent.left
        anchors.leftMargin: 16
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
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        font.bold: true
                        text: resourceManager.label(41,536+index, language.value)
                        color: local.menuIndex === index ? "white" : "#666666"
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked:{
                            root.setMenuIndex(index)
                            clear_select()
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
            id: transfer
            property bool isActive: root.pageActive && local.isSelectedMenu421

            function clearSelect(){
                if (item)
                    item.clearSelect()
            }

            active: false
            asynchronous: !isActive
            visible: (status == Loader.Ready) && isActive
            sourceComponent:  MainMenu_4_2_1 {
                pageActive: transfer.isActive
            }
        }//调用
        Loader {
            id: jump
            property bool isActive: root.pageActive && local.isSelectedMenu422

            function clearSelect(){
                if (item)
                    item.clearSelect()
            }

            active: false
            asynchronous: !isActive
            visible: (status == Loader.Ready) && isActive
            sourceComponent:  MainMenu_4_2_2 {
                pageActive: jump.isActive
            }
        }//跳转
        Loader {
            id: parallel
            property bool isActive: root.pageActive && local.isSelectedMenu423
            active: false
            asynchronous: !isActive
            visible: (status == Loader.Ready) && isActive
            sourceComponent: MainMenu_4_2_3 {
                pageActive: parallel.isActive
            }
        }//并行
        Loader {
            id: ret
            property bool isActive: root.pageActive && local.isSelectedMenu424
            active: false
            asynchronous: !isActive
            visible: (status == Loader.Ready) && isActive
            sourceComponent: MainMenu_4_2_4 {
                pageActive: ret.isActive
            }
        }//返回
    }
}
