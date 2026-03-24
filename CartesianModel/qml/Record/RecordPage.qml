import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import Enums 1.0
/*************************************************************************************
* Title: UI/UX of AEK
* File: RecordPage.qml
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
    id: record_page
    width: parent.width
    height: parent.height
    property bool pageActive: false
    //property bool jogDialSwOpe:true

    QtObject {
        id: penSet
        readonly property int groupNo: 25
        readonly property string selectedTabColor: resourceManager.dataColor(groupNo, 1)
    }

    QtObject {
        id: local
        readonly property var userSettingView: functionViewModel.controllerSettingsView.userSettingView
        readonly property int userLevel: commonViewModel.userLevel
        readonly property int dp1: Enums.MainScreenIdHistory
        property int language: 0
    }

    onPageActiveChanged: {
        let tabIndex = -1
        if (pageActive) {
            rec_io.visible = local.userSettingView.getScreenSettings(local.dp1,2).visibles[local.userLevel]
            if (rec_io.visible) {
                tabIndex = rec_io.TabBar.index
            }
            rec_operate.visible = local.userSettingView.getScreenSettings(local.dp1,1).visibles[local.userLevel]
            if (rec_operate.visible) {
                tabIndex = rec_operate.TabBar.index
            }
            rec_alm.visible = local.userSettingView.getScreenSettings(local.dp1,0).visibles[local.userLevel]
            if (rec_alm.visible) {
                tabIndex = rec_alm.TabBar.index
            }

        } else {
            // hide tab.
            rec_alm.visible = false
            rec_operate.visible = false
            rec_io.visible = false
        }

        rec_tab.setCurrentIndex(tabIndex)
    }

    Binding {
        target: local
        property: "language"
        value: resourceManager.language
        when: visible
    }

//todo:전체크기조정, 배경 색깔
    Rectangle{
        width: 1024
        height: 768
        anchors.fill: parent.Center
        //radius: 6
        color: "#EAEAEA"
        //센터 내용표시 배경
        Rectangle{
            width:1014
            height:600
            x: 5
            y: 89
            anchors.fill: parent.Center
            color: "#FFFFFF"
            border.color: "#cccccc"
        }
    }//배경


    StackLayout{
        id:stack_monitor
        width: 1012
        height: 520
        x:10
        y:140
        currentIndex: rec_tab.currentIndex

        Record_Alm{
            pageActive: record_page.pageActive && (rec_tab.currentIndex === 0)
        }
        Loader {
            source: "qrc:/qml/Record/Record_Operate.qml";
            active: false
            property bool isActive: (rec_tab.currentIndex == 1)
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
            source: "qrc:/qml/Record/Record_Io.qml";
            active: false
            property bool isActive: (rec_tab.currentIndex == 2)
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

    //画面切换按钮
    TabBar{
        id:rec_tab
        width:491
        height: 78
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 38
        anchors.leftMargin: 5
        z:1
        spacing: 16
        background: Rectangle{
            color: "transparent"
        }
        TabButton{
            id: rec_alm
            width: visible ? implicitWidth : -rec_tab.spacing
            background: Image {
                source: (rec_tab.currentIndex == 0 || rec_alm.pressed) ? resourceManager.imagePath + "/33_ALM_history/a330_05.png" : resourceManager.imagePath + "/33_ALM_history/a330_04.png"
            }
            Text {
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.bold: true
                font.pixelSize: rec_alm.pressed ? 23 : 24
                text: resourceManager.label(50, 1,local.language)
                color: (rec_tab.currentIndex == 0) ? "white" : "#666666"
            }
            onClicked: {
                rec_tab.currentIndex = 0
                phyKeyManager.ctrlBuzzer()
            }
        }
        TabButton{
            id: rec_operate
            width: visible ? implicitWidth : -rec_tab.spacing
            background: Image {
                source: (rec_tab.currentIndex == 1 || rec_operate.pressed) ? resourceManager.imagePath + "/33_ALM_history/a330_05.png" : resourceManager.imagePath + "/33_ALM_history/a330_04.png"
            }
            Text {
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.bold: true
                font.pixelSize: rec_operate.pressed ? 23 : 24
                text: resourceManager.label(50, 2,local.language)
                color: (rec_tab.currentIndex == 1) ? "white" : "#666666"
            }
            onClicked: {
                rec_tab.currentIndex = 1
                phyKeyManager.ctrlBuzzer()
            }
        }
        TabButton{
            id: rec_io
            width: visible ? implicitWidth : -rec_tab.spacing
            background: Image {
                source: (rec_tab.currentIndex == 2 || rec_io.pressed) ? resourceManager.imagePath + "/33_ALM_history/a330_05.png" : resourceManager.imagePath + "/33_ALM_history/a330_04.png"
            }
            Text {
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.bold: true
                font.pixelSize: rec_io.pressed ? 23 : 24
                text: resourceManager.label(50, 3,local.language)
                color: (rec_tab.currentIndex == 2) ? "white" : "#666666"
            }
            onClicked: {
                rec_tab.currentIndex = 2
                phyKeyManager.ctrlBuzzer()
            }
        }
    }
/***********************************************************************/
//    MouseArea {
//        anchors.fill: parent
//        propagateComposedEvents: true
//        onPressed: mouse.accepted = false
//        onReleased: mouse.accepted = false
//        onClicked: mouse.accepted = false
//        onWheel: {
//            if (!REAL_TARGET) {
//                if(wheel.angleDelta.y > 0) {
//                    phyKeyManager.decrementEmulatedJogCount()
//                } else if(wheel.angleDelta.y < 0) {
//                    phyKeyManager.incrementEmulatedJogCount()
//                }
//            }
//        }
//    }
//    Connections{
//        target:phyKeyManager
//        enabled: record_page.pageActive
//        onJogDialSwitchInvalidChanged:{
//            if (record_page.jogDialSwOpe === true){
//                let nextIndex
//                switch (rec_tab.currentIndex) {
//                case 0:
//                    nextIndex = rec_operate.visible ? 1 :
//                                rec_io.visible      ? 2 :
//                                                      0
//                    break;
//                case 1:
//                    nextIndex = rec_io.visible      ? 2 :
//                                rec_alm.visible     ? 0 :
//                                                      1
//                    break;
//                case 2:
//                    nextIndex = rec_alm.visible     ? 0 :
//                                rec_operate.visible ? 1 :
//                                                      2
//                    break;
//                }

//                rec_tab.currentIndex = nextIndex
//            }
//        }
//    }
//    Connections{
//        target:top_Tab.item
//        onDialogOpen: {
//            record_page.jogDialSwOpe = false
//        }
//        onDialogClose: {
//            record_page.jogDialSwOpe = true
//        }
//    }
}
