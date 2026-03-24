import QtQuick 2.12
import QtQml 2.3
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.2
import OriginViewModel 1.0
import Enums 1.0
import QtQuick.Layouts 1.3
import "./Widget"
import "./Manual"
import "./Program/PosSet/PosSetUtils.js" as Utils
/*************************************************************************************
* Title: UI/UX of AEK
* File: OperateModePage.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
    - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.06.17
* Worker: IJ.YI
**************************************************************************************/

/******************原点&手动&自动画面****************************/

Item {
    id:root
    width: 1018
    height: 698
    property bool pageActive: false
    property var pageViewModel: manualViewModel.panelModel[1]
    property var axisOrder: commonViewModel.axisOrder//axis No.
    property int numAxis: axisOrder.length//axis number
    property var axisDataList: commonViewModel.axisInfo.axisDataList //axis data
    property var prodView: autoViewModel.prodView //生产管理
    property bool freeOperate: commonViewModel.freeOperate//true:free operate; false:manual operate
    property bool freeOperateIndex: false//true:free operate; false:manual operate
    property bool isAlreadyOriginPopup:false
    property int originstatus: originViewModel.originStatus
    property bool auto_start_operable: (autoViewModel.oneCycle || autoViewModel.cycle) && !autoViewModel.step && (!autoViewModel.autoRunning || autoViewModel.isRun)
//    property bool auto_start_opacity: (autoViewModel.oneCycle || autoViewModel.cycle) && !autoViewModel.step
    property bool auto_pause_operable: (autoViewModel.oneCycle || autoViewModel.cycle) && autoViewModel.autoRunning && !autoViewModel.step && !autoViewModel.isRun
//    property bool auto_pause_opacity: (autoViewModel.oneCycle || autoViewModel.cycle) && autoViewModel.autoRunning && !autoViewModel.step
    property bool auto_stop_operable: autoViewModel.oneCycle || autoViewModel.cycle
    property bool step_operable: (autoViewModel.oneCycle || autoViewModel.cycle) && !autoViewModel.autoRunning
    property bool step_operable1: (autoViewModel.oneCycle || autoViewModel.cycle) && autoViewModel.step

    onPageActiveChanged: {
        if (pageActive) {

        } else {

        }
    }
    signal screenChange(int id)

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

    Component.onCompleted: {
        if (phyKeyManager.selectSwMode == 1) {
            stack_mode.currentIndex = 2
        }else if(phyKeyManager.selectSwMode == 2){
            stack_mode.currentIndex = 1
        }else if(phyKeyManager.selectSwMode == 3){
            stack_mode.currentIndex = 0
        }
    }
    function getProduceData(index){
        switch(index) {
        case 0:
            return resourceManager.formatData(prodView.fetchTime, resourceManager.getPenSetIntValue("7", 29, 0), 2)
        case 1:
            return resourceManager.formatData(prodView.cycleTime, resourceManager.getPenSetIntValue("7", 30, 0), 2)
        case 2:
            return resourceManager.formatData(prodView.moldingTime, resourceManager.getPenSetIntValue("7", 31, 0))
        case 3:
            return resourceManager.formatData(prodView.oneFetchNum, resourceManager.getPenSetIntValue("7", 32, 0)) + "/" +  resourceManager.formatData(prodView.fetchNum, resourceManager.getPenSetIntValue("7", 32, 0))
        case 4:
            return resourceManager.formatData(prodView.prodNum, resourceManager.getPenSetIntValue("7", 33, 0)) + "/" +  resourceManager.formatData(prodView.progNum, resourceManager.getPenSetIntValue("7", 33, 0))
        case 5:
            return resourceManager.formatData(prodView.badRate, resourceManager.getPenSetIntValue("7", 34, 0)) + "/" +  resourceManager.formatData(prodView.achievementRate, resourceManager.getPenSetIntValue("7", 34, 0))
        case 6:
            return resourceManager.formatData(prodView.notificationTime, resourceManager.getPenSetIntValue("7", 35, 0))
        case 7:
            return resourceManager.formatData(prodView.badCount, resourceManager.getPenSetIntValue("7", 36, 0))
        case 8:
            return prodView.prodEnd
        }
    }
///----------------------------------------------------------------------
    // 새롭게 전체 구성 진행

    Row{
        anchors.left: parent.left
        anchors.leftMargin: 30
        anchors.top: parent.top
        anchors.topMargin: 25
        spacing: 30

        // left aera all
        Rectangle {
            width: 500
            height: 648
            color: "transparent"
            Column{
                width: parent.width
                anchors.top: parent.top
                spacing: 25
                // left area top
                Rectangle{
                    width: parent.width
                    height: 340
                    radius: 10
                    color: "#FFFFFF"
                    border.width: 2
                    border.color: "#999999"
                    // title background
                    Rectangle {
                        anchors.top: parent.top
                        anchors.topMargin: 20
                        anchors.left: parent.left
                        anchors.leftMargin: 20
                        width: 460
                        height: 55
                        color: "#999999"
                        border.color: "#FFFFFF"
                        Row {
                            // title = 1
                            Rectangle {
                                width: 105
                                height: 55
                                color: "transparent"
                                Text {
                                    anchors.fill: parent
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignHCenter
                                    font.pixelSize: 22
                                    font.bold: true
                                    text: resourceManager.label(31,37,local.language)
                                }
                            }
                            // title = 2
                            Rectangle {
                                width: 100
                                height: 55
                                color: "transparent"
                                Text {
                                    anchors.fill: parent
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignHCenter
                                    font.pixelSize: 22
                                    font.bold: true
                                    text: resourceManager.label(36,120,local.language)
                                }
                            }
                            // title = 3
                            Rectangle {
                                width: 145
                                height: 55
                                color: "transparent"
                                Text {
                                    anchors.fill: parent
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignHCenter
                                    font.pixelSize: 22
                                    font.bold: true
                                    text: resourceManager.label(30,16,local.language)
                                }
                            }
                            // title = 4
                            Rectangle {
                                width: 110
                                height: 55
                                color: "transparent"
                                Text {
                                    anchors.fill: parent
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignHCenter
                                    font.pixelSize: 22
                                    font.bold: true
                                    text: resourceManager.label(31,9,local.language)
                                }
                            }
                        }
                        // axis data format
                        Column {
                            y : 57
                            spacing: numAxis > 5 ? -1 : 1
                            Repeater {
                                model : numAxis
                                Rectangle {
                                    id : axisDataBase
                                    width: 460
                                    height: 48
                                    color: "#CCCCCC"
                                    property int axisorder: axisOrder[index]
                                    Row {
                                        // home sensor condition
                                        Rectangle {
                                            width: 105
                                            height: 48
                                            color: "transparent"
                                            border.color: "#FFFFFF"
                                            // home sensor condition icon
                                            Rectangle{
                                                anchors.centerIn: parent
                                                width: 20
                                                height: 20
                                                radius: 10
                                                border.width: 2
                                                border.color: "#FFFFFF"
                                                color: axisDataList[axisorder].home ? "#00bf1b" : "#999999"
                                            }
                                        }
                                        // axis name data
                                        Rectangle {
                                            width: 100
                                            height: 48
                                            color: "transparent"
                                            border.color: "#FFFFFF"
                                            Text {
                                                anchors.fill: parent
                                                verticalAlignment: Text.AlignVCenter
                                                horizontalAlignment: Text.AlignHCenter
                                                font.pixelSize: 22
                                                font.bold: true
                                                color: {
                                                    if (resourceManager.axis_select === 5) {
                                                        return (axisDataBase.axisorder === 0 ||
                                                                axisDataBase.axisorder === 1 ||
                                                                axisDataBase.axisorder === 3)
                                                                ? "#00bf1b" : "#000000"
                                                    } else {
                                                        return (axisDataBase.axisorder === 0 ||
                                                                axisDataBase.axisorder === 2 ||
                                                                axisDataBase.axisorder === 4)
                                                                ? "#00bf1b" : "#000000"
                                                    }
                                                }
                                                text: resourceManager.label(22, axisDataBase.axisorder, local.language)
                                            }
                                        }
                                        // axis realTarget position
                                        Rectangle {
                                            width: 145
                                            height: 48
                                            color: "transparent"
                                            border.color: "#FFFFFF"
                                            Text {
                                                anchors.fill: parent
                                                verticalAlignment: Text.AlignVCenter
                                                horizontalAlignment: Text.AlignHCenter
                                                font.pixelSize: 22
                                                font.bold: true
                                                color: {
                                                    if (resourceManager.axis_select === 5) {
                                                        return (axisDataBase.axisorder === 0 ||
                                                                axisDataBase.axisorder === 1 ||
                                                                axisDataBase.axisorder === 3)
                                                                ? "#00bf1b" : "#000000"
                                                    } else {
                                                        return (axisDataBase.axisorder === 0 ||
                                                                axisDataBase.axisorder === 2 ||
                                                                axisDataBase.axisorder === 4)
                                                                ? "#00bf1b" : "#000000"
                                                    }
                                                }
                                                text: axisDataList[axisDataBase.axisorder].position
                                                      //+ "\n" + axisDataList[axisDataBase.axisorder].encoder
                                            }
                                        }
                                        // servo free on/off
                                        Rectangle {
                                            width: 110
                                            height: 48
                                            color: "transparent"
                                            border.color: "#FFFFFF"
                                            Image {
                                                anchors.centerIn: parent
                                                source: (axisDataList[axisDataBase.axisorder].servo) ? resourceManager.imagePath + "/13_POINT_set/a130_01.png"
                                                                                                     : resourceManager.imagePath + "/13_POINT_set/a130_00.png"
                                                MouseArea{
                                                    anchors.centerIn: parent
                                                    width: parent.width + 15
                                                    height: parent.height + 15
                                                    onPressed: pageViewModel.modBusServoControl(axisDataBase.axisorder, false)
                                                    onReleased: pageViewModel.modBusServoControl(axisDataBase.axisorder, true)
                                                    onClicked: {
                                                        let currentValue = axisDataList[aixsDataBase.axisorder].servo
                                                        operationRecord.recordOpeManualServo(aixsDataBase.axisorder, currentValue, !currentValue)
                                                        phyKetManager.ctrlBuzzer()
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                //  ---------------------------
                // left area bottom
                Rectangle{
                    width: parent.width
                    height: 283
                    radius: 10
                    color: "#FFFFFF"
                    border.width: 2
                    border.color: "#999999"
                    // IMM monitor title
                    Rectangle {
                        id : monitor_title
                        width: 460
                        height: 30
                        anchors.top: parent.top
                        anchors.topMargin: 20
                        anchors.left: parent.left
                        anchors.leftMargin: 20
                        Text {
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                            font.pixelSize: 25
                            font.bold: true
                            color: "#000000"
                            text: resourceManager.label(33,7,local.language) +" "+ resourceManager.label(33,0,local.language)
                        }
                    }
                    // monitor cycle time data format
                    Column{
                        anchors.top: monitor_title.bottom
                        anchors.topMargin: 5
                        leftPadding: 20
                        spacing: 2
                        Repeater {
                            model : 4
                            Rectangle {
                                width: 460
                                height: 48
                                color: "#CCCCCC"
                                border.color: "#FFFFFF"
                                Text {
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.left: parent.left
                                    anchors.leftMargin: 8
                                    font.pixelSize: 22
                                    font.bold: true
                                    color: "#000000"
                                    text: resourceManager.label(32, 25+index, local.language)
                                }
                                Text {
                                    visible: prodView.dataLoaded
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.right: parent.right
                                    anchors.rightMargin: 8
                                    font.pixelSize: 22
                                    color: "#000000"
                                    text: getProduceData(index)
                                }
                            }
                        }
                    }
                }
            }
        }
// ----------------------------------------------------------------------------------------------------------------------------------------
        // 우측 전체
        Rectangle {
            width: 434
            height: 648
            color: "transparent"
            Column {
                width : parent.width
                anchors.top : parent.top
                spacing : 25
                // right area top
                Rectangle {
                    width : parent.width
                    height : 310
                    radius : 10
                    color : "#FFFFFF"
                    border.width: 2
                    border.color: "#999999"
                    Grid {
                        anchors.centerIn: parent
                        spacing: 15
                        columns: 2
                        // 1. monitor tab
                        Rectangle {
                            width: 189
                            height: 127
                            radius: 10
                            color: mouse_monitorBtn.pressed ? "#CC3333" : "#CCCCCC"
                            border.width: 2
                            border.color: "#999999"
                            Text {
                                id : monitorBtn
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.bottom: parent.bottom
                                anchors.bottomMargin: 20
                                font.pixelSize: 25
                                font.bold: true
                                color: mouse_monitorBtn.pressed ? "#FFFFFF" : "#666666"
                                text: resourceManager.label(21,3,resourceManager.language)
                            }
                            Image {
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.top: parent.top
                                anchors.topMargin: 25
                                smooth: true
                                source: resourceManager.imagePath + (mouse_monitorBtn.pressed ? "/08_MENU/a080_01_01.png" : "/08_MENU/a080_01.png")
                            }
                            MouseArea {
                                id : mouse_monitorBtn
                                anchors.fill: parent
                                onClicked: {
                                    screenChange(1)
                                }
                            }
                        }
                        // 2. edit tab
                        Rectangle {
                            width: 189
                            height: 127
                            radius: 10
                            color: mouse_editBtn.pressed ? "#CC3333" : "#CCCCCC"
                            border.width: 2
                            border.color: "#999999"
                            Text {
                                id: editBtn
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.bottom: parent.bottom
                                anchors.bottomMargin: 20
                                font.pixelSize: 25
                                font.bold: true
                                color: mouse_editBtn.pressed ? "#FFFFFF" : "#666666"
                                text: resourceManager.label(21,4,resourceManager.language)
                            }
                            Image {
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.top: parent.top
                                anchors.topMargin: 25
                                smooth: true
                                source: resourceManager.imagePath + (mouse_editBtn.pressed ? "/08_MENU/a080_02_01.png" : "/08_MENU/a080_02.png")
                            }
                            MouseArea {
                                id : mouse_editBtn
                                anchors.fill: parent
                                onClicked: {
                                    screenChange(2)
                                }
                            }
                        }
                        // 3. function
                        Rectangle {
                            width: 189
                            height: 127
                            radius: 10
                            color: mouse_functionBtn.pressed ? "#CC3333" : "#CCCCCC"
                            border.width: 2
                            border.color: "#999999"
                            Text {
                                id : functionBtn
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.bottom: parent.bottom
                                anchors.bottomMargin: 20
                                font.pixelSize: 25
                                font.bold: true
                                color: mouse_functionBtn.pressed ? "#FFFFFF" : "#666666"
                                text: resourceManager.label(21,5,resourceManager.language)
                            }
                            Image {
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.top: parent.top
                                anchors.topMargin: 25
                                smooth: true
                                source: resourceManager.imagePath + (mouse_functionBtn.pressed ? "/08_MENU/a080_03_01.png" : "/08_MENU/a080_03.png")
                            }
                            MouseArea {
                                id : mouse_functionBtn
                                anchors.fill: parent
                                onClicked: {
                                    func_dialog.open()
                                }
                            }
                        }
                        // 4. record
                        Rectangle {
                            width: 189
                            height: 127
                            radius: 10
                            color: mouse_recordBtn.pressed ? "#CC3333" : "#CCCCCC"
                            border.width: 2
                            border.color: "#999999"
                            Text {
                                id : recordBtn
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.bottom: parent.bottom
                                anchors.bottomMargin: 20
                                font.pixelSize: 25
                                font.bold: true
                                color: mouse_recordBtn.pressed ? "#FFFFFF" : "#666666"
                                text: resourceManager.label(21,6,resourceManager.language)
                            }
                            Image {
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.top: parent.top
                                anchors.topMargin: 25
                                smooth: true
                                source: resourceManager.imagePath + (mouse_recordBtn.pressed ? "/08_MENU/a080_04_01.png" : "/08_MENU/a080_04.png")
                            }
                            MouseArea {
                                id : mouse_recordBtn
                                anchors.fill: parent
                                onClicked: {
                                    screenChange(4)
                                }
                            }
                        }
                    }
                }
                // right area bottom
                Rectangle {
                    width: parent.width
                    height: 313
                    radius: 10
                    color: "#FFFFFF"
                    border.width: 2
                    border.color: "#999999"
                    StackLayout {
                        id : stack_mode
                        anchors.fill: parent
                        currentIndex : 0
                        // section ORG
                        Item {
                            id : org
                            width: parent.width
                            height: parent.height
                            property bool orgActive: pageActive && (stack_mode.currentIndex == 0)
                            onOrgActiveChanged: {
                                if(orgActive){
                                    commonViewModel.pageChange(Enums.Origin)
                                    originViewModel.activate()
                                }else{
                                    originViewModel.stopOriginCarriageMonitor()
                                    originViewModel.deactivate()
                                }
                            }
                            // condition messages
                            Rectangle {
                                x:20
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.top: parent.top
                                anchors.topMargin: 30
                                radius: 20
                                width: 394
                                height: 50
                                color: "#CCCCCC"
                                border.color: "#FFFFFF"
                                Text {
                                    anchors.fill: parent
                                    verticalAlignment: Text.AlignVCenter
                                    leftPadding: 40
                                    font.pixelSize: 25
                                    font.bold: true
                                    color: "#000000"
                                    text:(originstatus == 0) ? resourceManager.label(30, 27, resourceManager.language)
                                                                            :(originstatus == 1 || originstatus == 3) ? resourceManager.label(30, 22, resourceManager.language)
                                                                                      : originstatus == 2 ? resourceManager.label(30, 23, resourceManager.language)
                                                                                                            : resourceManager.label(30, 24, resourceManager.language)
                                }
                                Image {
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.left: parent.left
                                    anchors.leftMargin: 10
                                    smooth: true
                                    source: resourceManager.imagePath + "/08_MENU/a080_05.png"
                                }
                            }
                            // ORG section btn
                            Row {
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.bottom: parent.bottom
                                anchors.bottomMargin: 30
                                spacing: 14
                                // org start
                                Rectangle {
                                    id : org_start
                                    enabled: !isAlreadyOriginPopup
                                    width: 120
                                    height: 120
                                    radius: 10
                                    border.width: 2
                                    border.color: "#999999"
                                    color: mouse_org.pressed ? "#CC3333" : "#CCCCCC"
                                    Text {
                                        anchors.horizontalCenter: parent.horizontalCenter
                                        anchors.bottom: parent.bottom
                                        anchors.bottomMargin: 25
                                        font.pixelSize: mouse_org.pressed ? 25 : 24
                                        font.bold: true
                                        color: mouse_org.pressed ? "#FFFFFF" : "#666666"
                                        text: resourceManager.label(33, 33, local.language)
                                    }
                                    Image {
                                        anchors.horizontalCenter: parent.horizontalCenter
                                        anchors.top: parent.top
                                        anchors.topMargin: 30
                                        smooth: true
                                        source: resourceManager.imagePath + (mouse_org.pressed ? "/08_MENU/a080_06_01.png" : "/08_MENU/a080_06.png")
                                    }
                                    MouseArea {
                                        id : mouse_org
                                        anchors.fill: parent
                                        onClicked: {
                                            if (org.orgActive === true) {
                                                commonViewModel.modbusMemblemSwControl(true, Enums.OriginStartKeyNo)
                                                originViewModel.originStatus = 0;
                                                commonViewModel.modbusMemblemSwControl(false, Enums.OriginStartKeyNo)
                                                originViewModel.startOriginCarriageMonitor()
                                            }
                                            phyKeyManager.ctrlBuzzer()
                                        }
                                    }
                                }
                                // org stop
                                Rectangle {
                                    id : org_stop
                                    enabled: isAlreadyOriginPopup
                                    width: 120
                                    height: 120
                                    radius: 10
                                    border.width: 2
                                    border.color: "#999999"
                                    color: mouse_stop.pressed ? "#CC3333" : "#CCCCCC"
                                    Text {
                                        anchors.horizontalCenter: parent.horizontalCenter
                                        anchors.bottom: parent.bottom
                                        anchors.bottomMargin: 25
                                        font.pixelSize: mouse_stop.pressed ? 25 : 24
                                        font.bold: true
                                        color: mouse_stop.pressed ? "#FFFFFF" : "#666666"
                                        text: resourceManager.label(32, 10, local.language)
                                    }
                                    Image {
                                        anchors.horizontalCenter: parent.horizontalCenter
                                        anchors.top: parent.top
                                        anchors.topMargin: 30
                                        smooth: true
                                        source: resourceManager.imagePath + (mouse_stop.pressed ? "/08_MENU/a080_07_01.png" : "/08_MENU/a080_07.png")
                                    }
                                    MouseArea {
                                        id: mouse_stop
                                        anchors.fill: parent
                                        onClicked: {
                                            if (org.orgActive === true){
                                                originViewModel.stopOriginCarriageMonitor()
                                                commonViewModel.modbusMemblemSwControl(true, Enums.OriginCancelKeyNo)
                                                commonViewModel.modbusMemblemSwControl(false, Enums.OriginCancelKeyNo)
                                                isAlreadyOriginPopup = false
                                            }
                                            phyKeyManager.ctrlBuzzer()
                                        }
                                    }
                                }
                                // org pause
                                Rectangle {
                                    id : org_pause
                                    visible: !(originViewModel.originStatus === 4)
                                    width: 120
                                    height: 120
                                    radius: 10
                                    border.width: 2
                                    border.color: "#999999"
                                    color: mouse_pause.pressed ? "#CC3333" : "#CCCCCC"
                                    Text {
                                        anchors.horizontalCenter: parent.horizontalCenter
                                        anchors.bottom: parent.bottom
                                        anchors.bottomMargin: 25
                                        font.pixelSize: mouse_pause.pressed ? 25 : 24
                                        font.bold: true
                                        color: mouse_pause.pressed ? "#FFFFFF" : "#666666"
                                        text: resourceManager.label(32, 9, local.language)
                                    }
                                    Image {
                                        anchors.horizontalCenter: parent.horizontalCenter
                                        anchors.top: parent.top
                                        anchors.topMargin: 30
                                        smooth: true
                                        source: resourceManager.imagePath + (mouse_pause.pressed ? "/08_MENU/a080_08_01.png" : "/08_MENU/a080_08.png")
                                    }
                                    MouseArea{
                                        id:mouse_pause
                                        anchors.fill: parent
                                        onClicked: {
                                            commonViewModel.modbusMemblemSwControl(true, Enums.OriginPauseKeyNo)
                                            originViewModel.pauseOriginCarriageMonitor()
                                            commonViewModel.modbusMemblemSwControl(false, Enums.OriginPauseKeyNo)
                                            phyKeyManager.ctrlBuzzer()
                                        }
                                    }
                                }
                            }
                        } // 아이템
                        //section manual
                        Item {
                            id : manual
                            width: parent.width
                            height: parent.height
                            property bool manualActive: pageActive && (stack_mode.currentIndex == 1)
                            onManualActiveChanged: {
                                if(manualActive){
                                    freeOperateIndex = false // MODIFIED : Manual mode -> Active Manual mode on
                                    commonViewModel.pageChange(Enums.ManualAxisOpe)
                                    commonViewModel.modbusFreeControl(freeOperateIndex)
                                    manualViewModel.panelModel[1].activate()
                                }else{
                                    manualViewModel.panelModel[1].deactivate()
                                }//else
                            }
                            Column{
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.top: parent.top
                                anchors.topMargin: 20
                                spacing: 1
                                // select operate
                                Rectangle{
                                    width: 394
                                    height: 90
                                    Row {
                                        anchors.verticalCenter: parent.verticalCenter
                                        spacing: 16
                                        // free btn
                                        Rectangle{
                                            id : free_operate
                                            width: 189
                                            height: 60
                                            radius: 10
                                            border.width: 2
                                            color: freeOperate ?  "#CC3333" : "#CCCCCC"
                                            border.color: "#999999"
                                            Image {
                                                anchors.verticalCenter: parent.verticalCenter
                                                anchors.left: parent.left
                                                anchors.leftMargin: 30
                                                source: resourceManager.imagePath + (freeOperateIndex ? "/08_MENU/a080_09_01.png" : "/08_MENU/a080_09.png")
                                            }
                                            Text {
                                                anchors.verticalCenter: free_operate.verticalCenter
                                                anchors.horizontalCenter: free_operate.horizontalCenter
                                                font.pixelSize: 25
                                                font.bold: true
                                                color: !freeOperate ?  "#666666" : "#FFFFFF"
                                                text: resourceManager.label(31, 26, local.language)
                                            }
                                            MouseArea {
                                                anchors.fill: parent
                                                onClicked: {
                                                    if (!freeOperate) {
                                                        operationRecord.recordOpeFreeManual(freeOperate, true)
                                                        commonViewModel.modbusFreeControl(true)
                                                        freeOperateIndex = true
                                                    }
                                                    phyKeyManager.ctrlBuzzer()
                                                }
                                            }
                                        }
                                        // manual btn
                                        Rectangle {
                                            id : manual_operate
                                            width: 189
                                            height: 60
                                            radius: 10
                                            border.width: 2
                                            color: !freeOperate ?  "#CC3333" : "#CCCCCC"
                                            border.color: "#999999"
                                            Image {
                                                anchors.verticalCenter: parent.verticalCenter
                                                anchors.left: parent.left
                                                anchors.leftMargin: 30
                                                source: resourceManager.imagePath + (!freeOperateIndex ? "/08_MENU/a080_10_01.png" : "/08_MENU/a080_10.png")
                                            }
                                            Text {
                                                anchors.verticalCenter: manual_operate.verticalCenter
                                                anchors.horizontalCenter: manual_operate.horizontalCenter
                                                font.pixelSize: 25
                                                font.bold: true
                                                color: freeOperate ?  "#666666" : "#FFFFFF"
                                                text: resourceManager.label(31, 25, local.language)
                                            }
                                            MouseArea {
                                                anchors.fill: parent
                                                onClicked: {
                                                    if (freeOperate) {
                                                        operationRecord.recordOpeFreeManual(freeOperate, false)
                                                        commonViewModel.modbusFreeControl(false)
                                                        freeOperateIndex = false
                                                    }
                                                    phyKeyManager.ctrlBuzzer()
                                                }
                                            }
                                        }
                                    }
                                }
                                // speed set
                                // speed set
                                Rectangle{
                                    width: 394
                                    height: 90
                                    color: "transparent"

                                    // 1) ✅ 프리 조작일 때: 기존 SpeedControl 표시
                                    SpeedControl {
                                        id: manualSpeedControl
                                        anchors.fill: parent
                                        visible: freeOperate   // ✅ 프리일 때만 보이기

                                        maxValue: commonViewModel.maxManualSpeed()
                                        value: Utils.getManualSpeedValue(commonViewModel.manualSpeedStatus,
                                                                         commonViewModel.manualSpeedSetting)
                                        valueText: resourceManager.label(31,45 + value, local.language)
                                        valueTextColor: resourceManager.rgb(31,45 + value)

                                        onPlusButtonPressed: {
                                            let currentSpeed = Utils.getManualSpeedValue(commonViewModel.manualSpeedStatus,
                                                                                         commonViewModel.manualSpeedSetting)
                                            let speed = Utils.getManualSpeedValue(value, commonViewModel.manualSpeedSetting)
                                            commonViewModel.modbusSpeedControl(speed)
                                            operationRecord.recordSetManualSpeedByManual(currentSpeed, speed)
                                        }
                                        onMinusButtonPressed: {
                                            let currentSpeed = Utils.getManualSpeedValue(commonViewModel.manualSpeedStatus,
                                                                                         commonViewModel.manualSpeedSetting)
                                            let speed = Utils.getDecrementedManualSpeedValue(value,
                                                                                             commonViewModel.manualSpeedSetting,
                                                                                             maxValue)
                                            commonViewModel.modbusSpeedControl(speed)
                                            operationRecord.recordSetManualSpeedByManual(currentSpeed, speed)
                                        }
                                    }

                                    // 2) ✅ 수동 조작일 때: Speed_Dialog의 auto speed UI 표시
                                    Item {
                                        id: autoSpeedBox
                                        anchors.fill: parent
                                        visible: !freeOperate   // 수동일 때만 표시

                                        Row{
                                            anchors.centerIn: parent
                                            spacing: 14

                                            // ▼ 감소
                                            ImageButton{
                                                anchors.verticalCenter: parent.verticalCenter
                                                upImageSource: resourceManager.imagePath + "/08_MENU/a080_17.png"
                                                downImageSource: resourceManager.imagePath + "/08_MENU/a080_18.png"
                                                onClicked: {
                                                    let cur = commonViewModel.autoSpeedStatus
                                                    if (cur > 0) {
                                                        commonViewModel.modbusAutoSpeedControl(cur - 1)
                                                        phyKeyManager.ctrlBuzzer()
                                                    }
                                                }
                                            }

                                            // 값 표시
                                            Rectangle{
                                                width: 190
                                                height: 60
                                                border.width: 2
                                                color: "#cccccc"
                                                border.color: "#999999"
                                                radius: 10
                                                Text{
                                                    anchors.centerIn: parent
                                                    text: {
                                                        let v = commonViewModel.autoSpeedStatus
                                                        return (v >= 0 && v <= 9) ? ((v + 1) * 10) + "%" : ""
                                                    }
                                                    font.pixelSize: 25
                                                    color: "#666666"
                                                }
                                            }

                                            // ▲ 증가
                                            ImageButton{
                                                anchors.verticalCenter: parent.verticalCenter
                                                upImageSource: resourceManager.imagePath + "/08_MENU/a080_15.png"
                                                downImageSource: resourceManager.imagePath + "/08_MENU/a080_16.png"
                                                onClicked: {
                                                    let cur = commonViewModel.autoSpeedStatus
                                                    if (cur < 9) {
                                                        commonViewModel.modbusAutoSpeedControl(cur + 1)
                                                        phyKeyManager.ctrlBuzzer()
                                                    }
                                                }
                                            }
                                        }
                                    }


                                }

                                // select arms
                                Rectangle{
                                    width: 394
                                    height: 90
                                    Row {
                                        anchors.verticalCenter: parent.verticalCenter
                                        spacing: 16
                                        // P arm
                                        Rectangle {
                                            id : pArm
                                            width: 189
                                            height: 60
                                            radius: 10
                                            border.width: 2
                                            color: resourceManager.axis_select === 5 ? "#CC3333" : "#CCCCCC"
                                            border.color: "#999999"
                                            Image {
                                                anchors.verticalCenter: parent.verticalCenter
                                                anchors.left: parent.left
                                                anchors.leftMargin: 30
                                                source: (resourceManager.axis_select === 5) ? resourceManager.imagePath + "/08_MENU/a080_12.png"
                                                                                          : resourceManager.imagePath + "/08_MENU/a080_11.png"
                                            }
                                            Text {
                                                anchors.verticalCenter: pArm.verticalCenter
                                                anchors.horizontalCenter: pArm.horizontalCenter
                                                font.pixelSize: 25
                                                font.bold: true
                                                color: resourceManager.axis_select === 5 ?  "#FFFFFF" : "#666666"
                                                text: resourceManager.label(20, 19, local.language)
                                            }
                                            MouseArea{
                                                anchors.fill: parent
                                                onClicked: {
                                                    if(resourceManager.axis_select === 5){
                                                        return
                                                    }
                                                    commonViewModel.selectPSaxis()
                                                    phyKeyManager.ctrlBuzzer()
                                                }
                                            }
                                        }
                                        // S arm
                                        Rectangle {
                                            id : sArm
                                            width: 189
                                            height: 60
                                            radius: 10
                                            border.width: 2
                                            color:  resourceManager.axis_select === 6 ? "#CC3333" : "#CCCCCC"
                                            border.color: "#999999"
                                            Image {
                                                anchors.verticalCenter: parent.verticalCenter
                                                anchors.left: parent.left
                                                anchors.leftMargin: 30
                                                source: (resourceManager.axis_select === 5) ? resourceManager.imagePath + "/08_MENU/a080_14.png"
                                                                                                    : resourceManager.imagePath + "/08_MENU/a080_13.png"
                                            }
                                            Text {
                                                anchors.verticalCenter: sArm.verticalCenter
                                                anchors.horizontalCenter: sArm.horizontalCenter
                                                font.pixelSize: 25
                                                font.bold: true
                                                color:  resourceManager.axis_select === 6 ? "#FFFFFF" : "#666666"
                                                text: resourceManager.label(20, 20, local.language)
                                            }
                                            MouseArea {
                                                anchors.fill: parent
                                                onClicked: {
                                                    if(resourceManager.axis_select === 6){
                                                        return
                                                    }
                                                    commonViewModel.selectPSaxis()
                                                    phyKeyManager.ctrlBuzzer()
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        // section auto
                        Item {
                            id :auto
                            width: parent.width
                            height: parent.height
                            property bool autoActive: pageActive && (stack_mode.currentIndex == 2)
                            onAutoActiveChanged: {
                                if(autoActive){
                                    commonViewModel.pageChange(Enums.AutoProductMngTimer);
                                    autoViewModel.activate()
                                    prodView.startMonitoring()
                                }else{
                                    autoViewModel.deactivate()
                                    prodView.stopMonitoring()
                                }
                            }
                            // cycle + 1cycle
                            Column{
                                anchors.top: parent.top
                                anchors.topMargin: 20
                                leftPadding: 20
                                spacing: 20
                                Row{
                                    spacing: 15
                                    // 1cycle
                                    Rectangle{
                                        id : auto_cycle1
                                        width: 87
                                        height: 126
                                        color: "transparent"
                                        Image {
                                            id : auto_cycle1_img
                                            anchors.horizontalCenter: parent.horizontalCenter
                                            anchors.top: parent.top
                                            source: (autoViewModel.oneCycle) ? resourceManager.imagePath + "/07_AUTO/a070_02.png" : resourceManager.imagePath + "/07_AUTO/a070_01.png"
                                            MouseArea{
                                                id:auto_cycle1_mouse
                                                anchors.fill: parent
                                                onPressed: {
                                                    commonViewModel.modbusMemblemSwControl(true, 241);
                                                }
                                                onReleased: {
                                                    commonViewModel.modbusMemblemSwControl(false, 241);
                                                }
                                                onClicked: phyKeyManager.ctrlBuzzer()
                                            }
                                        }
                                        Text {
                                            id : auto_cycle1_te
                                            anchors.horizontalCenter: parent.horizontalCenter
                                            anchors.bottom: parent.bottom
                                            anchors.bottomMargin: 5
                                            font.pixelSize: 25
                                            font.bold: true
                                            color: "#666666"
                                            text: resourceManager.label(32,6,local.language)
                                        }
                                    }
                                    // cycle
                                    Rectangle{
                                        id : auto_cycle
                                        width: 87
                                        height: 126
                                        color: "transparent"
                                        Image {
                                            id: auto_cycle_img
                                            anchors.horizontalCenter: parent.horizontalCenter
                                            anchors.top: parent.top
                                            source: (autoViewModel.cycle) ? resourceManager.imagePath + "/07_AUTO/a070_04.png" : resourceManager.imagePath + "/07_AUTO/a070_03.png"
                                        }
                                        MouseArea{
                                            id:auto_cycle2_mouse
                                            anchors.fill: parent
                                            onPressed: {
                                                commonViewModel.modbusMemblemSwControl(true, 240);
                                            }
                                            onReleased: {
                                                commonViewModel.modbusMemblemSwControl(false, 240);
                                            }
                                            onClicked: phyKeyManager.ctrlBuzzer()
                                        }
                                        Text{
                                            id: auto_cycle_te
                                            anchors.horizontalCenter: parent.horizontalCenter
                                            anchors.bottom: parent.bottom
                                            anchors.bottomMargin: 5
                                            font.pixelSize: 25
                                            font.bold: true
                                            color: "#666666"
                                            text: resourceManager.label(32,7,local.language)
                                        }
                                    }
                                }
                                Row{
                                    spacing: 15
                                    // start
                                    Rectangle{
                                        id : auto_start
                                        width: 87
                                        height: 126
                                        color: "transparent"
                                        Image {
                                            id : auto_start_img
                                            anchors.horizontalCenter: parent.horizontalCenter
                                            anchors.top: parent.top
                                            enabled: auto_start_operable || auto_start_mouse.pressed
                                            source: (autoViewModel.autoRunning && !autoViewModel.isRun) ? resourceManager.imagePath + "/07_AUTO/a070_06.png" : resourceManager.imagePath + "/07_AUTO/a070_05.png"
                                            MouseArea{
                                                id:auto_start_mouse
                                                anchors.fill: parent
                                                onPressed: commonViewModel.modbusMemblemSwControl(true, 242)
                                                onReleased: commonViewModel.modbusMemblemSwControl(false, 242)
                                                onClicked: phyKeyManager.ctrlBuzzer()
                                            }
                                        }
                                        Text{
                                            id : auto_start_te
                                            anchors.horizontalCenter: parent.horizontalCenter
                                            anchors.bottom: parent.bottom
                                            anchors.bottomMargin: 5
                                            font.pixelSize: 25
                                            font.bold: true
                                            color: "#666666"
                                            text: resourceManager.label(32,8,local.language)
                                        }
                                    }
                                    // pause
                                    Rectangle{
                                        id : auto_pause
                                        width: 87
                                        height: 126
                                        color: "transparent"
                                        Image {
                                            id : auto_pause_img
                                            anchors.horizontalCenter: parent.horizontalCenter
                                            anchors.top: parent.top
                                            enabled: auto_pause_operable || auto_pause_mouse.pressed
                                            source: autoViewModel.isRun && (autoViewModel.oneCycle || autoViewModel.cycle) ? resourceManager.imagePath + "/07_AUTO/a070_08.png" : resourceManager.imagePath + "/07_AUTO/a070_07.png"
                                            MouseArea{
                                                id : auto_pause_mouse
                                                anchors.fill: parent
                                                onPressed: commonViewModel.modbusMemblemSwControl(true, 248);
                                                onReleased: commonViewModel.modbusMemblemSwControl(false, 248);
                                                onClicked: phyKeyManager.ctrlBuzzer()
                                                }
                                            }
                                        Text {
                                            id : auto_pause_te
                                            anchors.horizontalCenter: parent.horizontalCenter
                                            anchors.bottom: parent.bottom
                                            anchors.bottomMargin: 5
                                            font.pixelSize: 25
                                            font.bold: true
                                            color: "#666666"
                                            text : resourceManager.label(32,9,local.language)
                                        }
                                    }
                                    // stop
                                    Rectangle{
                                        id : auto_stop
                                        width: 87
                                        height: 126
                                        color: "transparent"
                                        Image {
                                            id : auto_stop_img
                                            anchors.horizontalCenter: parent.horizontalCenter
                                            anchors.top: parent.top
                                            enabled: auto_stop_operable || auto_stop_mouse.pressed
                                            source: auto_stop_mouse.pressed ? resourceManager.imagePath + "/07_AUTO/a070_10.png" : resourceManager.imagePath + "/07_AUTO/a070_09.png"
                                            MouseArea {
                                                id : auto_stop_mouse
                                                anchors.fill: parent
                                                onPressed: commonViewModel.modbusMemblemSwControl(true, 243);
                                                onReleased: commonViewModel.modbusMemblemSwControl(false, 243);
                                                onClicked: phyKeyManager.ctrlBuzzer()
                                            }
                                        }
                                        Text{
                                            id : auto_stop_te
                                            anchors.horizontalCenter: parent.horizontalCenter
                                            anchors.bottom: parent.bottom
                                            anchors.bottomMargin: 5
                                            font.pixelSize: 25
                                            font.bold: true
                                            color: "#666666"
                                            text: resourceManager.label(32,10,local.language)
                                        }
                                    }
                                }
                            }
                            // section step
                            // Rectangle {
                            //     id : section_step
                            //     anchors.right: parent.right
                            //     anchors.top: parent.top
                            //     anchors.topMargin: 20
                            //     width: 209
                            //     height: 126
                            //     color: "transparent"
                            //     Row{
                            //         spacing: 15
                            //         // step
                            //         Rectangle{
                            //             id : auto_step
                            //             width: 87
                            //             height: 126
                            //             color: "transparent"
                            //             Image {
                            //                 id : auto_step_img
                            //                 anchors.horizontalCenter: parent.horizontalCenter
                            //                 enabled: step_operable
                            //                 source: resourceManager.imagePath + (autoViewModel.step ? "/07_AUTO/a070_12.png" : "/07_AUTO/a070_11.png")
                            //                 MouseArea {
                            //                     id:auto_step_img_mouse
                            //                     anchors.fill: parent
                            //                     onPressed: commonViewModel.modbusMemblemSwControl(true, 244);
                            //                     onReleased: commonViewModel.modbusMemblemSwControl(false, 244);
                            //                     onClicked: phyKeyManager.ctrlBuzzer()
                            //                 }
                            //             }
                            //             Text {
                            //                 id : auto_step_te
                            //                 anchors.horizontalCenter: parent.horizontalCenter
                            //                 anchors.bottom: parent.bottom
                            //                 anchors.bottomMargin: 5
                            //                 font.pixelSize: 25
                            //                 font.bold: true
                            //                 color: "#666666"
                            //                 text: resourceManager.label(32,11,local.language)
                            //             }
                            //         }
                            //         // next
                            //         Rectangle{
                            //             width: 87
                            //             height: 126
                            //             color: "transparent"
                            //             Image {
                            //                 id : auto_step_2_img
                            //                 enabled: step_operable1
                            //                 source: resourceManager.imagePath + (autoViewModel.isFront ? "/07_AUTO/a070_14.png" : "/07_AUTO/a070_13.png")
                            //                 MouseArea {
                            //                     anchors.fill: parent
                            //                     onPressed: {
                            //                         commonViewModel.modbusMemblemSwControl(true, 245);
                            //                     }
                            //                     onReleased: {
                            //                         commonViewModel.modbusMemblemSwControl(false, 245);
                            //                     }
                            //                     onClicked: phyKeyManager.ctrlBuzzer()
                            //                 }
                            //             }
                            //             Text {
                            //                 id : auto_step_2_te
                            //                 anchors.horizontalCenter: parent.horizontalCenter
                            //                 anchors.bottom: parent.bottom
                            //                 anchors.bottomMargin: 5
                            //                 font.pixelSize: 25
                            //                 font.bold: true
                            //                 color: "#666666"
                            //                 text: resourceManager.label(32,13,local.language)
                            //             }
                            //         }
                            //     }
                            // }
                            // section spare
                            Rectangle{
                                width: 87
                                height: 126
                                anchors.right: parent.right
                                anchors.rightMargin: 20
                                anchors.bottom: parent.bottom
                                anchors.bottomMargin: 15
                                color: "transparent"
                            }
                        }
                    }
                }
            }
        }
    }

    Loader {
        id:func_dialog
        function open(){
            openDialog()
        }
        function close(){
            if (item != null) {
                item.close()
            }
        }
        function openDialog(){
            item.x = 213
            item.y = 150
            item.open()
        } // 펑션 기능 팝업 위치
        active: true
        source: "qrc:/qml/Components/FunctionMenuDialog.qml"
        Connections {
            target: func_dialog.item
            onMenuOperate:{
                screenChange(id)
            }
        }
    }//操作错误弹出窗口



    Connections {
        target:phyKeyManager
        onSelectSwModeChanged: {
            if (selectSwMode == 1) {
                stack_mode.currentIndex = 2
            }else if(selectSwMode == 2){
                stack_mode.currentIndex = 1
            }else if(selectSwMode == 3){
                stack_mode.currentIndex = 0
            }
        }
    }

    Connections{
        target:originViewModel
        enabled: org.orgActive
        onOriginStatusChanged :{
            switch(originViewModel.originStatus){
            case 1:
//                    org_dialog.open()
                    isAlreadyOriginPopup = true
                break
            case 2:
                break
            case 3:
                break
            case 4:
//                orgLampswitch = false
                break
            default:
                break
            }
        }
    }
}
