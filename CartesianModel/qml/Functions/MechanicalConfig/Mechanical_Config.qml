import QtQml 2.12
import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import Enums 1.0
import "DriveParameters"
/*************************************************************************************
* Title: UI/UX of AEK
* File: Mechanical_Config.qml
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
    id: mechanicalPage
    property bool pageActive:false
    width: parent.width
    height: parent.height

    QtObject {
        id: local
        readonly property var userSettingView: functionViewModel.controllerSettingsView.userSettingView
        readonly property int userLevel: commonViewModel.userLevel
        readonly property int dp1: Enums.MainScreenIdFunctionMachineParam
        //readonly property bool jogDialSwOpe: (commonViewModel.topPopupDialog === Enums.PopupDialogStatusNone)
        property int language: 0
    }
    Binding {
        target: local
        property: "language"
        value: resourceManager.language
        when: visible
    }

    function changePage(page) {
        switch(page) {
        case 0:
            commonViewModel.pageChange(Enums.FunctionMechanicalParamModelSelect)
            break
        case 1:
            commonViewModel.pageChange(Enums.FunctionMechanicalParam)
            break
        case 2:
            //commonViewModel.pageChange(Enums.FunctionMechanicalParamDrvParam) // On each page pageChange(Driver_1/Driver_2)
            break
        default:
            // NOP
            break
        }
    }

    onPageActiveChanged: {
        if (pageActive) {
            // update tab visiblity.
            tab_typeSelect.visible = local.userSettingView.getScreenSettings(local.dp1, 0).visibles[local.userLevel]
            tab_mechParam.visible  = local.userSettingView.getScreenSettings(local.dp1, 1).visibles[local.userLevel]
            tab_driveParam.visible = local.userSettingView.getScreenSettings(local.dp1, 2).visibles[local.userLevel]
            // update current tab index.
            let tabIndex = tab_typeSelect.visible ? 0 :
                           tab_mechParam.visible  ? 1 :
                           tab_driveParam.visible ? 2 : 0
            mec_tab.currentIndex = tabIndex
            mechanicalPage.changePage(mec_tab.currentIndex)
        } else {
            // hide tab.
            tab_typeSelect.visible = false
            tab_mechParam.visible  = false
            tab_driveParam.visible = false
            mec_tab.currentIndex = -1
        }
    }

    Rectangle{
        x: 20
        y: 20
        width: 980
        height: 650
        anchors.fill: parent.Center
        border.color: "#999999"
        border.width: 2
        color: "#ffffff"
    }//背景
    StackLayout{
        id:stack_monitor
        width: 1012
        height: 520
        x:20
        y:80
        currentIndex: mec_tab.currentIndex

        Type_select{
            pageActive: mechanicalPage.pageActive && (mec_tab.currentIndex == 0)
        }

        Loader {
            source: "qrc:/qml/Functions/MechanicalConfig/Mechanical_parameters.qml";
            active: false
            property bool isActive: (mechanicalPage.pageActive && mec_tab.currentIndex === 1)
            onLoaded: {
                item.pageActive = isActive;
            }
            onIsActiveChanged: {
                if (isActive) {
                    active = true
                }
                if(status == Loader.Ready){
                    item.pageActive = isActive;
                }
            }
        }

        Loader {
            source: "qrc:/qml/Functions/MechanicalConfig/DriveParameters/Drive_parameters.qml";
            active: false
            property bool isActive: (mechanicalPage.pageActive && mec_tab.currentIndex === 2)
            onLoaded: {
                item.pageActive = isActive;
            }
            onIsActiveChanged: {
                if (isActive) {
                    active = true
                }
                if(status == Loader.Ready){
                    item.pageActive = isActive;
                }
            }
        }
    }

    TabBar{
        id:mec_tab
        width:491
        height: 0
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 30
        anchors.topMargin: 55
        spacing:16
        background: Rectangle{
            color: "transparent"
        }
        TabButton{
            id: tab_typeSelect
            width: 150
            height: 60
            background: Rectangle {
                anchors.fill: parent
                color: (mec_tab.currentIndex === 0) ? "#cc3333" : "#cccccc"
                border.color: "#999999"
            }
            Text {
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.bold: true
                font.pixelSize: tab_typeSelect.pressed ? 24 : 23
                text: resourceManager.label(43,22,local.language)
                color: (mec_tab.currentIndex == 0) ? "#ffffff" : "black"
            }
            onClicked: {
                mec_tab.currentIndex = 0
                mechanicalPage.changePage(0)
                phyKeyManager.ctrlBuzzer()
            }
        }
        TabButton{
            id: tab_mechParam
            width: 150
            height: 60
            background: Rectangle {
                anchors.fill: parent
                color: (mec_tab.currentIndex === 1) ? "#cc3333" : "#cccccc"
                border.color: "#999999"
            }
            Text {
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.bold: true
                font.pixelSize: tab_mechParam.pressed ? 24 : 23
                text: resourceManager.label(43,23,local.language)
                color: (mec_tab.currentIndex == 1) ? "#ffffff" : "black"
            }
            onClicked: {
                mec_tab.currentIndex = 1
                mechanicalPage.changePage(1)
                phyKeyManager.ctrlBuzzer()
            }
        }
        TabButton{
            id: tab_driveParam
            width: 150
            height: 60
            background: Rectangle {
                anchors.fill: parent
                color: (mec_tab.currentIndex === 2) ? "#cc3333" : "#cccccc"
                border.color: "#999999"
            }
            Text {
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.bold: true
                font.pixelSize: tab_driveParam.pressed ? 24 : 23
                text: resourceManager.label(43,24,local.language)
                color: (mec_tab.currentIndex == 2) ? "#ffffff" : "black"
            }
            onClicked: {
                mec_tab.currentIndex = 2
                mechanicalPage.changePage(2)
                phyKeyManager.ctrlBuzzer()
            }
        }
    }//画面切换按钮

/***********************************************************************/
//    Connections{
//        target:phyKeyManager
//        enabled: mechanicalPage.pageActive && local.jogDialSwOpe
//        onJogDialSwitchInvalidChanged: {
//            let nextTabIndex = mec_tab.currentIndex
//            switch (mec_tab.currentIndex) {
//            case tab_typeSelect.TabBar.index:
//                nextTabIndex = tab_mechParam.visible  ? 1 :
//                               tab_driveParam.visible ? 2 : nextTabIndex
//                break;
//            case tab_mechParam.TabBar.index:
//                nextTabIndex = tab_driveParam.visible ? 2 :
//                               tab_typeSelect.visible ? 0 : nextTabIndex
//                break;
//            case tab_driveParam.TabBar.index:
//                nextTabIndex = tab_typeSelect.visible ? 0 :
//                               tab_mechParam.visible  ? 1 : nextTabIndex
//                break;
//            }
//            mec_tab.currentIndex = nextTabIndex
//            mechanicalPage.changePage(mec_tab.currentIndex)
//        }
//    }
}
