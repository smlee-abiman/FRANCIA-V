import QtQuick 2.12
import QtQml 2.3
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import TeachingViewModel 1.0
import Enums 1.0
import "../../Widget"
import "../.."
/*************************************************************************************
* Title: UI/UX of AEK
* File: StackPage.qml
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
    id: stackPage
    width: parent.width
    height: parent.height
    property bool pageActive: false
    property var pageModel: teachingViewModel.pageModel[2]
    property var axisOrder: pageModel.axisOrder
    property int axisCount: 5
    property int curStandardStackNum: pageModel.curStandardPalletizeNum //current standard stack number
    property int curFreeStackNum: pageModel.curFreePalletizeNum//current free stack number

    signal editFromProgEditScreenFinished()

    QtObject {
        id: local
        readonly property var userSettingView: functionViewModel.controllerSettingsView.userSettingView
        readonly property int userLevel: commonViewModel.userLevel
        readonly property int dp1: Enums.MainScreenIdTeachingPalletize
        property int language: 0
    }
    Binding {
        target: local
        property: "language"
        value: resourceManager.language
        when: visible
    }

    ComponentLoader {
        id: componentLoader
        guard: !stackPage.pageActive
        loadComponentList: [
            free_stack_page
        ]
    }

    onPageActiveChanged: {
        let tabIndex = -1
        //pageModel.curTabIndex = 0
        if (pageActive) {
            tabIndex = pageModel.curTabIndex
            pageModel.activate()
            let tab2Visible = local.userSettingView.getScreenSettings(local.dp1,1).visibles[local.userLevel]
            if (!tab2Visible && (tabIndex === 1)) {
                tabIndex = 0
            }
            stack_tab2.visible = false

            let tab1Visible = local.userSettingView.getScreenSettings(local.dp1,0).visibles[local.userLevel]
            if (!tab1Visible && (tabIndex === 0)) {
                tabIndex = tab2Visible ? 1 : -1
            }
            stack_tab1.visible = false
            componentLoader.loadNextComponent()
        } else {
            // hide tab.
            stack_tab1.visible = false
            stack_tab2.visible = false

            pageModel.deactivate()
        }

        changePage(tabIndex);
        stack_tab.setCurrentIndex(tabIndex)
    }


    function changePage(page) {
        switch(page) {
        case 0:
            pageModel.curTabIndex = page
            pageModel.startPageChanging()
            commonViewModel.pageChange(Enums.TeachingStandardPalletize, curStandardStackNum)
            for(let i = 0; i < stackPage.axisCount; i++){
                pageModel.modbusGetPointValue(curStandardStackNum, i);
            }
            for(let j = 0; j < 4; j++){
                pageModel.modbusGetPointData(209, j)
                pageModel.modbusGetPointData(210, j)
            }
            operationRecord.recordScreenSwitching(12, 0)
            break
        case 1:
            pageModel.curTabIndex = page
            pageModel.startPageChanging()
            commonViewModel.pageChange(Enums.TeachingFreePalletize, (curFreeStackNum + 8))
            pageModel.modbusGetPointData(-1, 0)
            pageModel.modbusGetPointValue(curFreeStackNum, 0);
            operationRecord.recordScreenSwitching(12, 1)
            break
        default:
            // NOP
            break
        }
    }

//    Image {
//        x:-0.5
//        y:-3.5
//        Rectangle{
//            width: 557
//            height:4
//            anchors.bottom: parent.bottom
//            anchors.right: parent.right
//            anchors.rightMargin: 2
//            color: "#E5F4F9"
//        }
//        source: resourceManager.imagePath + "/14_N_palletize/d140_06.png"
//    }//background

    TabBar{
        id:stack_tab
        width:240
        height: 47
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 6.6
        z:1
        spacing: 14
        background: Rectangle{
            color: "transparent"
        }
        TabButton{
            id:stack_tab1
            anchors.top: parent.top
            anchors.topMargin: 5
            z: (stack_tab.currentIndex == 1)
            width: visible ? implicitWidth : -stack_tab.spacing
            background: Image {
                source: (stack_tab.currentIndex == 0 || mouse1.pressed) ? resourceManager.imagePath + "/14_N_palletize/d140_07.png" : resourceManager.imagePath + "/14_N_palletize/d140_08.png"
            }
            Text {
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                text: resourceManager.label(37,3,local.language)
                font.pixelSize: mouse1.pressed ? 23 : 22
                color: (stack_tab.currentIndex === 0 )? "black" : resourceManager.rgb(37,3)
            }
            MouseArea{
                id:mouse1
                anchors.fill: parent
                onClicked: {
                    if(stack_tab.currentIndex === 0){
                        return
                    }
                    pageModel.deactivate()
                    stack_tab.currentIndex = 0
                    stackPage.changePage(0)
                    pageModel.activate()
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }

        TabButton{
            id:stack_tab2
            anchors.top: parent.top
            anchors.topMargin: 5
            z: (stack_tab.currentIndex == 1)
            width: visible ? implicitWidth : -stack_tab.spacing
            background: Image {
                source: (stack_tab.currentIndex == 1 || mouse2.pressed) ? resourceManager.imagePath + "/14_N_palletize/d140_07.png" : resourceManager.imagePath + "/14_N_palletize/d140_08.png"
            }
            Text {
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                text: resourceManager.label(37,4,local.language)
                font.pixelSize: mouse2.pressed ? 23 : 22
                color: (stack_tab.currentIndex === 1)? "black" : resourceManager.rgb(37,4)
            }
            MouseArea{
                id:mouse2
                anchors.fill: parent
                onClicked: {
                    if(stack_tab.currentIndex === 1){
                        return
                    }
                    pageModel.deactivate()
                    stack_tab.currentIndex = 1
                    stackPage.changePage(1)
                    pageModel.activate()
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }
    }

    StackLayout{
        id:stack_pos
        width: parent.width
        height: 520
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 78
        anchors.leftMargin: 21
        currentIndex: pageModel.curTabIndex
        //currentIndex: stack_tab.currentIndex

        Standard_Stack{
            pageModel:stackPage.pageModel
            pageActive: (stackPage.pageActive && stack_tab.currentIndex === 0)
            onCloseButtonClicked: {
                pageModel.editFromProgEditScreen = false
                editFromProgEditScreenFinished()
            }
        }

        Loader {
            id: free_stack_page
            property bool isActive: stackPage.pageActive && (stack_tab.currentIndex === 1)
            sourceComponent: free_stack_component
            active: false
            asynchronous: !isActive
            onIsActiveChanged: {
                if (isActive) {
                    active = true
                }
            }
            Component {
                id: free_stack_component
                Free_Stack{
                    pageModel:stackPage.pageModel
                    pageActive: free_stack_page.isActive
                    onCloseButtonClicked: {
                        pageModel.editFromProgEditScreen = false
                        editFromProgEditScreenFinished()
                    }
                }
            }
        }
    }
}

