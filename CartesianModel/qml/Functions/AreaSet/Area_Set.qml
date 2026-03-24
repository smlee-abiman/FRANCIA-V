import QtQml 2.12
import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import Enums 1.0
/*************************************************************************************
* Title: UI/UX of AEK
* File: Area_Set.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
 - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.16
* Worker: SH.HONG
**************************************************************************************/
Item {
    id: area_set
    width: parent.width
    height: parent.height
    property bool pageActive: false
    property var viewModel: areaSetViewModel

    QtObject {
        id: local
        readonly property bool jogDialSwOpe: (commonViewModel.topPopupDialog === Enums.PopupDialogStatusNone)
        readonly property var userSettingView: functionViewModel.controllerSettingsView.userSettingView
        readonly property int userLevel: commonViewModel.userLevel
        readonly property int dp1: Enums.MainScreenIdFunctionAreaSet
    }

    onPageActiveChanged: {
        if (pageActive) {
            // update tab visiblity.
//            tab_areaSet1.visible = false
            tab_areaSet2.visible = local.userSettingView.getScreenSettings(local.dp1, 1).visibles[local.userLevel]
            // update current tab index.
            area_tab.currentIndex = 1
            viewModel.activate()
        } else {
//            tab_areaSet1.visible = false
            tab_areaSet2.visible = false
            area_tab.currentIndex = -1
            viewModel.deactivate()
        }
    }

    QtObject {
        id: penSet
        readonly property string groupNo: "19"
        property string selectedTabColor: ""
        Component.onCompleted: {
            selectedTabColor = resourceManager.dataColor(groupNo, 1)
        }
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
        id:stack_area
        width: 1012
        height: 520
        x:4
        y:100
        currentIndex: 1

        Area_Set_1{
            pageActive: area_set.pageActive && (parent.currentIndex === 0)
        }

        Loader {
            id: area_fix_loader
            property bool isActive: area_set.pageActive && (parent.currentIndex === 1)
            sourceComponent: area_set2_component
            active: isActive
            onIsActiveChanged: {
                if (isActive) {
                    active = true
                }
            }
            Component {
                id: area_set2_component
                Area_Set_2{
                    pageActive: area_fix_loader.isActive
                }
            }
        }
    }

    TabBar{
        id:area_tab
        width:336
        height: 78
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 69
        anchors.leftMargin: 14
        spacing:10
        background: Rectangle{
            color: "transparent"
        }
        // TabButton{
        //     id: tab_areaSet1
        //     height: 50
        //     width: 140
        //     background: Rectangle {
        //         anchors.fill: parent
        //         color: (area_tab.currentIndex == 0 || tab_areaSet1.pressed) ? "#cc3333" : "#cccccc"
        //         border.color: "#999999"
        //     }
        //     Text {
        //         anchors.fill: parent
        //         horizontalAlignment: Text.AlignHCenter
        //         verticalAlignment: Text.AlignVCenter
        //         lineHeight: 0.7
        //         font.pixelSize: tab_areaSet1.pressed ? 24 : 23
        //         text: resourceManager.label(44, 1,resourceManager.language)
        //         color: (area_tab.currentIndex === 0) ? "#ffffff" : "black"
        //     }
        //     onClicked: {
        //         area_tab.currentIndex = 0
        //     }
        // }
        TabButton{
            id: tab_areaSet2
            width: 140
            height: 50
            background: Rectangle {
                anchors.fill: parent
                color: "#999999"
                border.color: "#999999"
            }
            Text {
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.bold: true
                font.pixelSize: tab_areaSet2.pressed ? 23 : 24
                text: resourceManager.label(44, 2,resourceManager.language)
                color: "#ffffff"
            }
            onClicked: {
                area_tab.currentIndex = 1
            }
        }
    }//画面切换按钮
/***********************************************************************/
//    Connections{
//        target:phyKeyManager
//        enabled: area_set.pageActive && local.jogDialSwOpe
//        onJogDialSwitchInvalidChanged:{
//            let nextTabIndex = area_tab.currentIndex
//            switch (area_tab.currentIndex) {
//            case tab_areaSet1.TabBar.index:
//                nextTabIndex = tab_areaSet2.visible ? 1 : nextTabIndex
//                break;
//            case tab_areaSet2.TabBar.index:
//                nextTabIndex = tab_areaSet1.visible ? 0 : nextTabIndex
//                break;
//            }
//            area_tab.currentIndex = nextTabIndex
//        }
//    }
}
