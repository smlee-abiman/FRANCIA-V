import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import Enums 1.0
/*************************************************************************************
* Title: UI/UX of AEK
* File: MaintainPage.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
 - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.18
* Worker: SH.HONG
**************************************************************************************/
Item {
    id: maintainPage
    width: parent.width
    height: parent.height
    property var viewModel: maintainViewModel
    property bool pageActive: false
//    property bool jogDialSwOpe:true

    QtObject {
        id: penSet
        readonly property string groupNo: "20"
        property string selectedTabColor: ""
        Component.onCompleted: {
            selectedTabColor = resourceManager.dataColor(groupNo, 1)
        }
    }

    QtObject {
        id: local
        readonly property var userSettingView: functionViewModel.controllerSettingsView.userSettingView
        readonly property int userLevel: commonViewModel.userLevel
        readonly property int dp1: Enums.MainScreenIdFunctionMaintain
        property int language: 0
    }

    Binding {
        target: local
        property: "language"
        value: resourceManager.language
        when: visible
    }

    onPageActiveChanged: {
        let tabIndex = -1
        if (pageActive) {
            viewModel.activate()

            maintain.visible = local.userSettingView.getScreenSettings(local.dp1,1).visibles[local.userLevel]
            if (maintain.visible) {
                tabIndex = maintain.TabBar.index
            }
            maintain_check.visible = local.userSettingView.getScreenSettings(local.dp1,0).visibles[local.userLevel]
            if (maintain_check.visible) {
                tabIndex = maintain_check.TabBar.index
            }

        } else {
            // hide tab.
            maintain_check.visible = false
            maintain.visible = false

            viewModel.deactivate()
        }

        maintain_tab.setCurrentIndex(tabIndex)
    }

    Rectangle{
        x: 2
        y: 20
        width: 1018
        height: 650
        anchors.fill: parent.Center
        border.color: "#999999"
        border.width: 2
        color: "#ffffff"
    }//背景
    StackLayout{
        id:stack_maintain
        width: 1012
        height: 520
        x:4
        y:100
        currentIndex: maintain_tab.currentIndex

        Maintain_1{
            pageActive: maintainPage.pageActive && (parent.currentIndex === 0)
        }
        Loader {
            id: maintain2_loader
            property bool isActive: maintainPage.pageActive && (parent.currentIndex === 1)
            sourceComponent: maintain2_component
            active: false
            onIsActiveChanged: {
                if (isActive) {
                    active = true
                }
            }
            Component {
                id: maintain2_component
                Maintain_2{
                    pageActive: maintain2_loader.isActive
                }
            }
        }
    }

    TabBar{
        id:maintain_tab
        width:336
        height: 78
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 55
        anchors.leftMargin: 10
        spacing:16
        background: Rectangle{
            color: "transparent"
        }
        TabButton{
            id: maintain_check
            width: 140
            height: 50
            background: Rectangle {
                anchors.fill: parent
                color: (maintain_tab.currentIndex === 0) ? "#cc3333" : "#cccccc"
                border.color: "#999999"
            }
            Text {
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: maintain_check.pressed ? 25 : 24
                text: resourceManager.label(45,1,local.language)
                color: (maintain_tab.currentIndex === 0) ? "#ffffff" : "black"
            }
            onClicked: {
                maintain_tab.currentIndex = 0
                phyKeyManager.ctrlBuzzer()
            }
        }

        TabButton{
            id: maintain
            width: 140
            height: 50
            background: Rectangle {
                anchors.fill: parent
                color: (maintain_tab.currentIndex === 1) ? "#cc3333" : "#cccccc"
                border.color: "#999999"
            }
            Text {
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: maintain.pressed ? 25 : 24
                text: resourceManager.label(45,2,local.language)
                color: (maintain_tab.currentIndex === 1) ? "#ffffff" : "black"
            }
            onClicked: {
                maintain_tab.currentIndex = 1
                phyKeyManager.ctrlBuzzer()
            }
        }
    }//画面切换按钮

/***********************************************************************/
//    Connections{
//        target:phyKeyManager
//        enabled: maintainPage.pageActive
//        onJogDialSwitchInvalidChanged:{
//            if (maintainPage.jogDialSwOpe === true){
//                let nextIndex
//                switch (maintain_tab.currentIndex) {
//                case 0:
//                    nextIndex = maintain.visible ? 1 : 0
//                    break;
//                case 1:
//                    nextIndex = maintain_check.visible ? 0 : 1
//                    break;
//                }

//                maintain_tab.currentIndex = nextIndex
//            }
//        }
//    }
//    Connections{
//        target:top_Tab.item
//        onDialogOpen: {
//            maintainPage.jogDialSwOpe = false
//        }
//        onDialogClose: {
//            maintainPage.jogDialSwOpe = true
//        }
//    }
}
