import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQml 2.12
import Enums 1.0
import "../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: Drive_Monitor.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
    - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.01
* Worker: IJ.YI
**************************************************************************************/

Item {
    id : drive_monitor_page
    width: 1012
    height: 648
    property bool pageActive: false
    property bool pageCallerDrvParam:false
    property var pageModel:monitorViewModel.drvMonitorView
    property var axisDataList: pageModel.axisInfo.axisDataList
    property int axisCount: pageModel.getAxisCount()
    property int page: 0
//    property bool realMonJogScrollOn:false
//    property bool peakJogScrollOn:false
//    property bool curveJogScrollOn:false
    QtObject {
        id: local
        property int language: 0
    }

    Binding {
        target: local
        property: "language"
        value: resourceManager.language
        when: visible
    }

    Component.onDestruction: pageModel.deactivate()

    onPageActiveChanged: {
            if (pageActive) {
                //console.debug("Drive_Monitor onPageActiveChanged pageActive:"+pageActive)
                if (drive_monitor_page.pageCallerDrvParam === true){
                    commonViewModel.pageChange(Enums.FunctionDriverMonitor)
                }
                pageModel.activate()
                /*tietech.aek-master_250519*/
    //            if(page == 2){
    //                if (drvMonitorGraph.graphMode === false){
    //                    pageModel.setGraphDrawMode(0)
    //                } else {
    //                    pageModel.setGraphDrawMode(1)
    //                }
    //            }
    //            //pageModel.setTimerValue(0, 0);
    //            if (drvMonitorGraph.graphMode === false){
    //                pageModel.setTimerValue(0, drvMonitorGraph.oneAxisTimerSettingToggle);
    //            } else {
    //                pageModel.setTimerValue(1, drvMonitorGraph.threeAxis1TimerSettingToggle);
    //                pageModel.setTimerValue(2, drvMonitorGraph.threeAxis2TimerSettingToggle);
    //                pageModel.setTimerValue(3, drvMonitorGraph.threeAxis3TimerSettingToggle);
    //            }
                maxvalue.active = true
                drvMonitorGraph.active = true
            } else {
                //console.debug("Drive_Monitor onPageActiveChanged pageActive:"+pageActive)
                pageModel.deactivate()
                maxvalue.active = false
                drvMonitorGraph.active = false
            }
        }
    // Top Select page
    Rectangle{
        width: 350
        height: 60
        anchors.right: parent.right
        color: "transparent"
        Image {
            anchors.top: parent.top
            source: resourceManager.imagePath + "/08_Monitor/a080_07.png"
        }
        // RealTime, Max, curve
        Row {
            spacing: 10
            leftPadding: 15
            // Real-time monitoring
            Rectangle {
                width: 100
                height: 40
                anchors.top: parent.top
                anchors.topMargin: 11
                color: page == 0 || but_mouse1.pressed ? "#CC3333" : "#CCCCCC"
                border.width: 1
                border.color: "#999999"
                Text {
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 25
                    font.bold: true
                    color: page == 0 || but_mouse1.pressed ? "#FFFFFF" : "#666666"
                    text: resourceManager.label(33, 16, local.language)
                }
                MouseArea {
                    id: but_mouse1
                    anchors.fill: parent
                    onClicked: {
                        let preIndex =page
                        page = 0
                        pageModel.setSubViewIndex(page)
                        if(DRV_MONITOR_GRAPH_10MS_CYCLE){
                            if (preIndex === 1 || preIndex === 2){
                                pageModel.startPageChanging()
                                commonViewModel.pageChange(Enums.MonitorDriver)
                            }
                        }
                        pageModel.setGraphDrawMode(-1);
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }
            // Maximum
            Rectangle {
                width: 100
                height: 40
                anchors.top: parent.top
                anchors.topMargin: 11
                color: page == 1 || but_mouse2.pressed ? "#CC3333" : "#CCCCCC"
                border.width: 1
                border.color: "#999999"
                Text {
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 25
                    font.bold: true
                    color: page == 1 || but_mouse2.pressed ? "#FFFFFF" : "#666666"
                    text: resourceManager.label(33, 17, local.language)
                }
                MouseArea {
                    id : but_mouse2
                    anchors.fill: parent
                    onClicked: {
                        let preIndex = page
                        page = 1
                        pageModel.setSubViewIndex(page)
                        if(DRV_MONITOR_GRAPH_10MS_CYCLE){
                            if (preIndex === 2){
                                pageModel.startPageChanging()
                                commonViewModel.pageChange(Enums.MonitorDriver)
                            }
                        }
                        pageModel.setGraphDrawMode(-1);
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }
            // Curve graph
            Rectangle{
                width: 100
                height: 40
                anchors.top: parent.top
                color: page == 2 || but_mouse3.pressed ? "#CC3333" : "#CCCCCC"
                anchors.topMargin: 11
                border.width: 1
                border.color: "#999999"
                Text {
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 25
                    font.bold: true
                    color: page == 2 || but_mouse3.pressed ? "#FFFFFF" : "#666666"
                    text: resourceManager.label(33, 18, local.language)
                }
                MouseArea {
                    id : but_mouse3
                    anchors.fill: parent
                    onClicked: {
                        page = 2
                        pageModel.setSubViewIndex(page)
                        if(DRV_MONITOR_GRAPH_10MS_CYCLE){
                            pageModel.startPageChanging()
                            commonViewModel.pageChange(Enums.MonitorDriverWaveform)
                        }
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }
        }
    }
    // list area
    StackLayout {
        id : stack_drv_monitor
        width: parent.width
        height: parent.height
        currentIndex : page

        // Real-time monitoring // Driver_RealTimeValue.qml
        Driver_RealTimeValue{
            id:reltimevalue
            property bool isActive: (drive_monitor_page.pageActive && (page == 0))
        }
        // Maximum // Driver_MaxValue.qml
        Loader {
            id:maxvalue
            property bool isActive: (drive_monitor_page.pageActive && (page == 1))
            active: false
            asynchronous: !isActive
            sourceComponent: Driver_MaxValue {
            }
        }
        // Curve graph
        Loader {
            id:drvMonitorGraph
            property bool isActive: (drive_monitor_page.pageActive && (page == 2))
            active: false
            asynchronous: !isActive
            sourceComponent: DriveMonitorGraph {
                pageActive: drvMonitorGraph.isActive
            }
        }
    }
} // main item
