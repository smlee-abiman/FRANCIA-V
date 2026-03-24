import QtQml 2.12
import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.Shapes 1.12
import Enums 1.0
/*************************************************************************************
* Title: UI/UX of AEK
* File: MonitorPage.qml
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
    id: monitorPage
    width: 1024
    height: 768

    property bool pageActive: false
    property bool jogDialSwOpe:true
    property var pageModel:monitorViewModel.drvMonitorView
    property var internalMonitorPageModel:monitorViewModel.internalMonitorView
    property int internalCurTabIndex:-1

    QtObject {
        id: local
        readonly property var userSettingView: functionViewModel.controllerSettingsView.userSettingView
        readonly property int userLevel: commonViewModel.userLevel
        readonly property int dp1: Enums.MainScreenIdMonitor
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
            commonViewModel.pageChange(Enums.MonitorIOExpress)
            monitorPage.internalCurTabIndex = page;
            break
        case 1:
            io_force.active = true
            commonViewModel.pageChange(Enums.MonitorIOForced)
            monitorPage.internalCurTabIndex = page;
            break
        case 2:
            drive_monitor.active = true
            if(DRV_MONITOR_GRAPH_10MS_CYCLE){
                pageModel.startPageChanging()
            }
            if (DRV_MONITOR_GRAPH_10MS_CYCLE){
                if (drive_monitor.item.preSubTabIndex === 2){
                    commonViewModel.pageChange(Enums.MonitorDriverWaveform)
                } else {
                    commonViewModel.pageChange(Enums.MonitorDriver)
                }
            } else {
                commonViewModel.pageChange(Enums.MonitorDriver)
            }
            monitorPage.internalCurTabIndex = page;
            break
        case 3:
            internal_monitor.active = true
            internalMonitorPageModel.startPageChanging()
            let bankOffset = internalMonitorPageModel.getBankOffsetNum()
            if (bankOffset >= 0){
                commonViewModel.pageChange(Enums.MonitorInternal, bankOffset)
            } else {
                commonViewModel.pageChange(Enums.MonitorInternal, 0)
            }
            monitorPage.internalCurTabIndex = page;
            break
        default:
            // NOP
            break
        }
    }

    onPageActiveChanged: {
        let tabIndex = -1
        if (pageActive) {
            let visibleInsideMon = local.userSettingView.getScreenSettings(local.dp1,3).visibles[local.userLevel]
            if (visibleInsideMon) {
                inside_mon.visible = true
                tabIndex = inside_mon.TabBar.index
            }
            let visibleDriveMon = local.userSettingView.getScreenSettings(local.dp1,2).visibles[local.userLevel]
            if (visibleDriveMon) {
                drive_mon.visible = true
                tabIndex = drive_mon.TabBar.index
            }
            let visibleIoExp = local.userSettingView.getScreenSettings(local.dp1,0).visibles[local.userLevel]
            if (visibleIoExp) {
                io_exp.visible = true
                tabIndex = io_exp.TabBar.index
            }

            if (tabIndex > -1) {
                monitor_tab.visible = true
            } else {
                monitor_tab.visible = false
            }

        } else {
            io_exp.visible = false
            drive_mon.visible = false
            inside_mon.visible = false
        }

        changePage(tabIndex);
        monitor_tab.setCurrentIndex(tabIndex)
    }
    // The content area excluding the TabBar
    StackLayout{
        id:stack_monitor
        width: 964
        height: 648
        anchors.top: parent.top
        anchors.topMargin: 25
        anchors.left: parent.left
        anchors.leftMargin: 30
        currentIndex: monitor_tab.currentIndex

        // I/O monitor page
        IO_Express{
            pageActive: monitorPage.pageActive && (monitor_tab.currentIndex == 0)
        }

        // I/O force page
        Loader {
            id: io_force
            source: "qrc:/qml/Monitor/IO_Force.qml";
            active: false
            property bool isActive: (monitorPage.pageActive && monitor_tab.currentIndex == 1)
            onLoaded: {
                item.pageActive = isActive;
            }
            onIsActiveChanged: {
                if(status == Loader.Ready){
                    item.pageActive = isActive;
                }
            }
        }

        // Drive monitor
        Loader {
            id: drive_monitor
            source: "qrc:/qml/Monitor/Drive_Monitor.qml";
            active: false
            property bool isActive: (monitorPage.pageActive && monitor_tab.currentIndex === 2)
            onLoaded: {
                item.pageActive = isActive;
                //item.realMonJogScrollOn = isActive;
            }
            onIsActiveChanged: {
                if(status == Loader.Ready){
                    item.pageActive = isActive;
                    //item.realMonJogScrollOn = isActive;
                }
            }
        }

        // Internal monitor
        Loader {
            id: internal_monitor
            source: "qrc:/qml/Monitor/Internal_Monitor.qml";
            active: false
            property bool isActive: (monitorPage.pageActive && monitor_tab.currentIndex === 3)
            onLoaded: {
                item.pageActive = isActive;
            }
            onIsActiveChanged: {
                if(status == Loader.Ready){
                    item.pageActive = isActive;
                }
            }
        }

    }

    TabBar{
        id: monitor_tab
        width: 480
        height: 60
        anchors.top: stack_monitor.top
        anchors.left: stack_monitor.left
        z: 1
        spacing: 15

        background: Rectangle { color: "transparent" }

        TabButton{
            id: io_exp
            visible: false
            width: visible ? implicitWidth : 0   // ✅ 숨기면 자리도 0
            // anchors.top: parent.top  // ❌ 제거 (레이아웃이 배치하게)
            background: Image {
                source: (monitor_tab.currentIndex == 0 || io_exp.pressed)
                        ? resourceManager.imagePath + "/08_Monitor/a080_02.png"
                        : resourceManager.imagePath + "/08_Monitor/a080_01.png"
            }
            Text {
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: io_exp.pressed ? 24 : 23
                text: resourceManager.label(33,1,local.language)
                color: monitor_tab.currentIndex == 0 ? "#FFFFFF" : "#666666"
            }
            onClicked: {
                monitor_tab.currentIndex = 0
                monitorPage.changePage(0)
                phyKeyManager.ctrlBuzzer()
            }
        }

        TabButton{
            id: drive_mon
            visible: false
            width: visible ? implicitWidth : 0   // ✅
            // anchors.top: parent.top  // ❌ 제거
            background: Image {
                source: (monitor_tab.currentIndex == 2 || drive_mon.pressed)
                        ? resourceManager.imagePath + "/08_Monitor/a080_02.png"
                        : resourceManager.imagePath + "/08_Monitor/a080_01.png"
            }
            Text {
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: resourceManager.label(33,3,local.language)
                font.pixelSize: drive_mon.pressed ? 24 : 23
                font.bold: true
                color: monitor_tab.currentIndex == 2 ? "#FFFFFF" : "#666666"
            }
            onClicked: {
                monitor_tab.currentIndex = 2
                monitorPage.changePage(2)
                phyKeyManager.ctrlBuzzer()
            }
        }

        TabButton{
            id: inside_mon
            visible: false
            width: visible ? implicitWidth : 0   // ✅
            // anchors.top: parent.top  // ❌ 제거
            background: Image {
                source: (monitor_tab.currentIndex == 3 || inside_mon.pressed)
                        ? resourceManager.imagePath + "/08_Monitor/a080_02.png"
                        : resourceManager.imagePath + "/08_Monitor/a080_01.png"
            }
            Text {
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: resourceManager.label(33,4,local.language)
                font.pixelSize: inside_mon.pressed ? 24 : 23
                font.bold: true
                color: monitor_tab.currentIndex == 3 ? "#FFFFFF" : "#666666"
            }
            onClicked: {
                if (monitorPage.internalCurTabIndex !== 3){
                    monitor_tab.currentIndex = 3
                    monitorPage.changePage(3)
                }
                phyKeyManager.ctrlBuzzer()
            }
        }
    }

}
