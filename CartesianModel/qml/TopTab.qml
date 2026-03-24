import QtQml 2.12
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.12
import Enums 1.0
import "./Components"
import "./Components/MainDialog"
import "./Widget"
import "Program/PosSet/PosSetUtils.js" as PosSetUtils

/*************************************************************************************
* Title: UI/UX of AEK
* File: TopTab.qml
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

/********************屏幕上方标签（状态、模式、日期、菜单页按钮等）****************************/
Item {
    id:top_bar
    width: parent.width
    height: parent.height
    x: parent.x
    y: parent.y

    signal dialogOpen()
    signal dialogClose()
    signal backMenu()

    property int usr_select: commonViewModel.userLevel//用户等级
    property int manipulate: commonViewModel.manipulateMode//操作模式
    property int operatingMode: commonViewModel.operatingMode//当前操作模式
    property int curStatus: commonViewModel.currentStatus//当前状态
    property string curDate : commonViewModel.curDate//当前日期
    property string curTime : commonViewModel.curTime//当前时间
    property bool emergencyStatus: commonViewModel.emergencyStatus  //非常停止
    property bool operableSwStatus:false
    property bool isAlreadyAlarmPopupDisp:false
    property bool isAlreadyErrorPopupDisp:false
    property bool isAlreadyEmergencyPopupDisp:false
    property bool isAlreadyOpeCmdPopupDisp:false
    property bool isAlreadyStartupPopupDisp:false
    property bool isAlreadyTimeoutErrorPopupDisp:false
    property bool userLevel5Visible:false
    property bool isAlarmPopupDispInstructions:false
    property bool isErrorPopupDispInstructions:false
    property bool isEmergencyPopupDispInstructions:false
    property bool isOpeCmdPopupDispInstructions:false
    property bool isStartupPopupDispInstructions:false
    property int timerCnt:0
    property bool isRequestFtpStartProcess:false
    property bool isAlreadyReplyAbortedErrorPopupDisp:false

    property int languageCount: commonViewModel.languageCount
    property int languageCurPage: 1
    property int languagePageCount: ((top_bar.languageCount % 8) === 0 ? top_bar.languageCount / 8 : top_bar.languageCount / 8 + 1)
    property int alarmCount: commonViewModel.curAlarmCount
    property var userLevelPass:commonViewModel.userPassword
    property string hiddenWords: "******";
    property int manualSpeedValue:commonViewModel.manualSpeedStatus
    property int autoSpeedValue:commonViewModel.autoSpeedStatus
    property int displayAutoSpeed: commonViewModel.autoSpeedStatus
    property int pendingAutoSpeed: commonViewModel.autoSpeedStatus
    Timer {
        id: autoSpeedDebounceTimer
        interval: 200
        repeat: false
        onTriggered: {
            top_bar.displayAutoSpeed = top_bar.pendingAutoSpeed
        }
    }
    Connections {
        target: commonViewModel
        onAutoSpeedStatusChanged: {
            top_bar.pendingAutoSpeed = commonViewModel.autoSpeedStatus
            autoSpeedDebounceTimer.restart()
        }
    }
    property var autoSpeed:["10", "20", "30", "40", "50", "60", "70", "80", "90", "100"]
    property int manualSpeedLimitSetting:commonViewModel.manualSpeedSetting
    property var manualSpeed:[
        resourceManager.label(56,2,resourceManager.language),
        resourceManager.label(56,3,resourceManager.language),
        resourceManager.label(56,4,resourceManager.language),
        resourceManager.label(56,5,resourceManager.language),
        resourceManager.label(56,6,resourceManager.language),
        resourceManager.label(56,7,resourceManager.language),
        resourceManager.label(56,8,resourceManager.language)
    ]
    property string curSetupFileNum:commonViewModel.curFileNumber
    property string curSetupFileName:commonViewModel.curFileName
    property int ftpStatus:ftpManager.ftpStatus
    property int cmnLanguage:commonViewModel.language
    property bool filechange: commonViewModel.fileChange
    property var axisOrder: commonViewModel.axisOrder
    property int numAxis: axisOrder.length
    property var axisDataList: commonViewModel.axisInfo.axisDataList
    property int currentoperateaxisOrder: commonViewModel.currentOperateAxisOrder//current operate axis

    // 표시용 모드 인덱스: 0=원점, 1=수동, 2=자동, 3=알람(우선)
    property int uiModeIndex: -1

    function recomputeUiMode() {
        // 1) 알람/비상 우선
        const alarmNow =
            top_bar.emergencyStatus
            || (top_bar.alarmCount > 0)
            || ((top_bar.curStatus & 0x0000FF00) !== 0);

        if (alarmNow) { uiModeIndex = 3; return }

        // 2) 물리 셀렉터 기준 (메인 OperateModePage에서의 매핑과 동일)
        // selectSwMode: 1=자동, 2=수동, 3=원점
        switch (phyKeyManager.selectSwMode) {
        case 3: uiModeIndex = 0; break; // 원점
        case 2: uiModeIndex = 1; break; // 수동
        case 1: uiModeIndex = 2; break; // 자동
        default:
            // 폴백: 기존 operatingMode 값 사용(0/1/2 외는 알람)
            switch (top_bar.operatingMode) {
            case 0: uiModeIndex = 0; break;
            case 1: uiModeIndex = 1; break;
            case 2: uiModeIndex = 2; break;
            default: uiModeIndex = 3; break;
            }
        }
    }

    QtObject {
        id: local
        property string userLevelPassInput:""
        property var passChgInputStatus:[false, false, false]
        property string userLevelPassOld:""
        property string userLevelPassNew:""
        property string userLevelPassVerify:""
        property var pageCommonOpeAuth:[]
        property var pageAlertOpeAuth:[]
        property var pageCaveatOpeAuth:[]
        property var pageMotionOpeAuth:[]
        property var pageSetupChgOpeAuth:[]
        property var pageAlarmOpeAuth:[]
        property var pageUserLevelOpeAuth:[]
        property var pageTimerSetOpeAuth:[]
        property var pageSpeedSetOpeAuth:[]
        property var pageLanguageSetOpeAuth:[]
        readonly property bool userAreaDialogJogDialSw: (local.topPopupDialog === Enums.PopupDialogStatusUserSwitch)
        readonly property bool speedDialogJogDialSw: (local.topPopupDialog === Enums.PopupDialogStatusSpeed)
        readonly property var teachingviewModel: teachingViewModel.pageModel[6]
        property bool bootCompleted: commonViewModel.bootCompleted
        property bool curFileNameDispStatus: true

        function updateMagnification(index) {
            let currentValue = phyKeyManager.jogMagnificationSetting
            switch(index) {
            case 0: // initial (no selected)
                commonViewModel.modbusJogMagnificationControl(Enums.JogMagnification1000)
                recordSetJogSpeed(currentValue, Enums.JogMagnification1)
                break
            case 1: // x100
                commonViewModel.modbusJogMagnificationControl(Enums.JogMagnification100)
                recordSetJogSpeed(currentValue, Enums.JogMagnification100)
                break
            case 2: // x10
                commonViewModel.modbusJogMagnificationControl(Enums.JogMagnification10)
                recordSetJogSpeed(currentValue, Enums.JogMagnification10)
                break
            case 3: // x1
                commonViewModel.modbusJogMagnificationControl(Enums.JogMagnification1)
                recordSetJogSpeed(currentValue, Enums.JogMagnification1)
                break
            default:
                // Illigal value
                break
            }
        }

        function recordSetManualSpeed(currentValue, newValue){
            operationRecord.recordSetManualSpeedBySpeedSetting(currentValue, newValue)
        }

        function recordSetJogSpeed(currentValue, newValue){
            operationRecord.recordSetJogSpeed(currentValue, newValue)
        }

        function recordSetAutoSpeed(currentValue, newValue){
            operationRecord.recordSetAutoSpeed(currentValue, newValue)
        }

        readonly property int topPopupDialog: commonViewModel.topPopupDialog
        readonly property bool popupDialogStatus: (local.topPopupDialog !== Enums.PopupDialogStatusNone)

        onPopupDialogStatusChanged: {
            // console.log("TopTab.onPopupDialogStatusChanged():" + popupDialogStatus)
            if (popupDialogStatus) {
                dialogOpen()
            } else {
                dialogClose()
            }
        }

        function doOpenFileItem() {
            let curPage = commonViewModel.getCurrentPage()
            let curSubPage = commonViewModel.getCurrentSubPage()
            file_item.open(curPage, curSubPage)
        }
        function openFileDialog() {
            if ((teachingviewModel.isViewActive) && (teachingviewModel.ncpFile.edited)) {
                teachingViewModelSaveDialogConnection.requestOpenSaveProgramDialog()
            } else {
                doOpenFileItem()
            }
        }
        function checkStatusDCFlag(){
            let dcFlag = modbusManager.getReceiveDataDI(2565)
            if(dcFlag === true){
                if (local.curFileNameDispStatus) {
                    local.curFileNameDispStatus = false;
                    return true;
                } else {
                    local.curFileNameDispStatus = true;
                    return false;
                }
            } else {
                return true;
            }
        }
        function controlFileNameBlink(){
            let dcFlag = local.checkStatusDCFlag()
            filename1.visible = dcFlag
            filename.visible = dcFlag
        }

        onBootCompletedChanged: {
            if (bootCompleted) {
                componentLoader.loadNextComponent()
            }
        }
    }

    ComponentLoader {
        id: componentLoader
        guard: !local.bootCompleted
        loadComponentList: [
            speed_dialog,
            usr_switch_dialog,
            language_dialog,
            date_dialog
        ]
    }
    Component.onCompleted:{
        if(!REAL_TARGET) {
            timer.running = true
            timer.repeat = true
        } else {
            timer.running = true
            timer.repeat = true
        }
        commonViewModel.modbusReadJogratioSpeed()
        recomputeUiMode()

        for(let i = 0; i < 5; i++){
            local.pageCommonOpeAuth[i] = commonViewModel.getOperationAuthority(1, i)
            local.pageAlertOpeAuth[i] = commonViewModel.getOperationAuthority(2, i)
            local.pageCaveatOpeAuth[i] = commonViewModel.getOperationAuthority(3, i)
            local.pageMotionOpeAuth[i] = commonViewModel.getOperationAuthority(4, i)
            local.pageSetupChgOpeAuth[i] = commonViewModel.getOperationAuthority(26, i)
            local.pageAlarmOpeAuth[i] = commonViewModel.getOperationAuthority(27, i)
            local.pageUserLevelOpeAuth[i] = commonViewModel.getOperationAuthority(28, i)
            local.pageTimerSetOpeAuth[i] = commonViewModel.getOperationAuthority(29, i)
            local.pageSpeedSetOpeAuth[i] = commonViewModel.getOperationAuthority(30, i)
            local.pageLanguageSetOpeAuth[i] = commonViewModel.getOperationAuthority(31, i)
        }
        local.updateMagnification(0)
        setupChangeViewModel.modbusGetCurFileNumberAndName();
        commonViewModelConnections.updateCurrentStatus(commonViewModel.currentStatus)
        commonViewModelConnections.updateEmergencyStatus(commonViewModel.emergencyStatus)
        commonViewModelConnections.updateAxisAlarmStatus()
        commonViewModelConnections.updateOpeCmdStatus(commonViewModel.opeCmdStatus)
        commonViewModelConnections.updateStartupStatus(commonViewModel.startupStatus)
    }

/**********************************time date update**********************************/
    Timer{
        id: timer
        interval: 100 //间隔(单位毫秒):100毫秒=100ミリ秒
        repeat: true //重复
        running: true
        onTriggered:{
            top_bar.timerCnt++
            if (top_bar.timerCnt >= 10){
                if(!REAL_TARGET) {
                    time1.text = top_bar.getTime();
                    date.text = top_bar.getDate();
                }
                let curPage = commonViewModel.getCurrentPage()
                if(REAL_TARGET && !ENABLE_FTP_COM_ASYNC) {
                    if (top_bar.ftpStatus <= 0){
                        if (curPage !== Enums.SetupChgExportUsb && curPage !== Enums.SetupChgImportUsb){
                            ftpManager.restartFtpCommunication()
                        }
                    } else if (top_bar.ftpStatus >= 4 && top_bar.ftpStatus !== 5){
                        //console.log("timer ftpStatus:"+top_bar.ftpStatus, "curPage:"+curPage, "isStartupFtpStatus:"+ftpManager.isStartupFtpStatus, "isRequestFtpStartProcess:"+top_bar.isRequestFtpStartProcess)
                        if (ftpManager.isStartupFtpStatus === false){
                            if (top_bar.isRequestFtpStartProcess === false){
                                ftpManager.asyncStartupFtpProcess()
                                if (ftpManager.isDeterNoopCmd === false){
                                    ftpManager.execFtpCommand(Enums.FtpCommandNOOP, null, "");
                                }
                                top_bar.isRequestFtpStartProcess = true;
                            } else {
                                if (ftpManager.isDeterNoopCmd === false){
                                    ftpManager.execFtpCommand(Enums.FtpCommandNOOP, null, "");
                                }
                            }
                        } else {
                            if (ftpManager.isDeterNoopCmd === false){
                                ftpManager.execFtpCommand(Enums.FtpCommandNOOP, null, "");
                            }
                        }
                    }
                }
                //console.log("subMainView visible:"+subMainView.visible, "isAlarmPopupDispInstructions:"+top_bar.isAlarmPopupDispInstructions,"isErrorPopupDispInstructions:"+top_bar.isErrorPopupDispInstructions)
                if (subMainView.visible === true && top_bar.isAlarmPopupDispInstructions === true){
                    if(local.pageAlarmOpeAuth[commonViewModel.userLevel] !== 0){
                        top_bar.isAlreadyAlarmPopupDisp = true
                        top_bar.isAlarmPopupDispInstructions = false
                        alarm_dialog.open(top_bar.alarmCount, true);
                    }
                }
                if (subMainView.visible === true && top_bar.isErrorPopupDispInstructions === true){
                    top_bar.isAlreadyErrorPopupDisp = true
                    top_bar.isErrorPopupDispInstructions = false
                    error_dialog.open();
                }
                if (subMainView.visible === true && top_bar.isEmergencyPopupDispInstructions === true){
                    top_bar.isAlreadyEmergencyPopupDisp = true
                    top_bar.isEmergencyPopupDispInstructions = false
                    emergency_dialog.open();
                }
                if (subMainView.visible === true && top_bar.isOpeCmdPopupDispInstructions === true){
                    if(local.pageMotionOpeAuth[commonViewModel.userLevel] !== 0){
                        top_bar.isAlreadyOpeCmdPopupDisp = true
                        top_bar.isOpeCmdPopupDispInstructions = false
                        operateCmd_dialog.open();
                    }
                }
                if (subMainView.visible === true && top_bar.isStartupPopupDispInstructions === true){
                    top_bar.isAlreadyStartupPopupDisp = true
                    top_bar.isStartupPopupDispInstructions = false
                    startup_dialog.open();
                }
                top_bar.timerCnt = 0;
            }
            local.controlFileNameBlink();
        }
    }
    function getDate(){
        if(!REAL_TARGET) {
            return Qt.formatDateTime(new Date(), "yyyy/MM/dd");
        } else {
            return top_bar.curDate
        }
    }
    function getTime(){
        if(!REAL_TARGET) {
            var date = new Date
            var hour = date.getHours()
            var minute = date.getMinutes()
            var sec = date.getSeconds()
            commonViewModel.hour = hour
            commonViewModel.minutes = minute
            commonViewModel.seconds = sec
            return commonViewModel.hour + ":" + commonViewModel.minutes + ":" + commonViewModel.seconds;
        } else {
            return top_bar.curTime
        }
    }

/***************************************top base************************************/
    // 1. Main logo area
    Rectangle{
        id: top_base
        width: parent.width
        height: 70
        color: top_bar.operableSwStatus === true ? "#00bf1b" : "#FFFFFF"
        //모드별 색상 변경
        Rectangle {
            id: mode_color
            width: parent.width
            height: 4
            anchors.bottom: parent.bottom
            z: 9999
            color: uiModeIndex === 0 ? "#3399FF" :   // 원점
                   uiModeIndex === 1 ? "#FF9500" :   // 수동
                   uiModeIndex === 2 ? "#00bf1b" :   // 자동
                                        "#FF0600";   // 알람/그 외
        }

        Rectangle{
            id : main_logo
            width: 90
            height: 70
            color: "transparent"
            Image {
                anchors.centerIn: parent
                smooth: true
                source: resourceManager.imagePath + "/00_COMMON/" +
                        (uiModeIndex === 0 ? "aelogo0.png" :
                         uiModeIndex === 1 ? "aelogo1.png" :
                         uiModeIndex === 2 ? "aelogo2.png" :
                                             "aelogo3.png")
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    backMenu()
                }
            }
        }
        // 2. Mold file title
        Rectangle{
            id : mold_file_title
            anchors.left: main_logo.right
            width: 350
            height: parent.height
            color: "transparent"
            Rectangle{
                width: 350
                height: parent.height - 30
                radius: 20
                anchors.verticalCenter: parent.verticalCenter
                color: "#CCCCCC"
                Image {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 12
                    smooth: true
//                    source: resourceManager.imagePath + "/00_COMMON/a000_01.png"
                    source: resourceManager.imagePath + (top_bar.operableSwStatus === true ? "/00_COMMON/a000_01_01.png" : "/00_COMMON/a000_01.png")
                }
            }
            Text {
                id: filename1
                anchors.fill: parent
                anchors.left: parent.left
                anchors.leftMargin: 12
                verticalAlignment: Text.AlignVCenter
                font.bold: true
                font.pixelSize: 20
                color: !ttt.running ? "#000000" : filename.fileccc ? "yellow" : "#000000"
                text: resourceManager.label(51,5,resourceManager.language) + top_bar.curSetupFileNum
            }
            Text {
                id: filename
                z:1
                property bool fileccc: false
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.bold: true
                font.pixelSize: 20
                color: ttt.running == false ? "white" : fileccc ? "yellow" : "white"
                text: top_bar.curSetupFileName
                Timer{
                    id:ttt
                    running: filechange
                    repeat: true
                    interval: 1000
                    onTriggered: {
                        parent.fileccc == false ? parent.fileccc = true : parent.fileccc = false
                    }
                }

            }
            MouseArea{
                id:mouse_file
                anchors.fill: parent
                z:10
                onClicked: {
                    if(local.pageSetupChgOpeAuth[commonViewModel.userLevel] !== 0){
                        local.openFileDialog()
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }
        }

        Rectangle{
            id: psArm
            anchors.left: mold_file_title.right
            width: 60
            height: parent.height
            color: "transparent"

            Image {
                id: psIcon
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 14
                width: 30
                height: 30
                fillMode: Image.PreserveAspectFit
                smooth: true
                scale: 1.3
                // axis_select === 5 이면 P, 아니면 S
                source: resourceManager.imagePath +
                        (resourceManager.axis_select === 5
                            ? "/00_COMMON/aearmp.png"
                            : "/00_COMMON/aearms.png")
            }
        }
        // 2. 상태표시
        // 2. Status indicator (텍스트 단일, 중앙 정렬)
        Rectangle {
            id: opstatus
            width: 80
            height: 60
            anchors.left: psArm.right
            anchors.leftMargin: -6     // ← 이전에 말한 시각 보정 유지
            anchors.top: parent.top
            anchors.topMargin: 5
            color: "transparent"

            Text {
                anchors.centerIn: parent   // ✅ 완전 중앙
                font.pixelSize: 22
                font.bold: true
                color: "#000000"

                text: (top_bar.manipulate === 0) ? commonViewModel.topTitle
                     : (top_bar.manipulate === 1) ? commonViewModel.topTitle
                     : (top_bar.manipulate === 2)
                         ? ((top_bar.operatingMode === 4)
                                ? resourceManager.label(20,3,resourceManager.language)
                                : commonViewModel.topTitle)
                     : ""
                elide: Text.ElideRight
            }
        }

        // 3. data_time
        Rectangle{
            id : data_time
            width: 120
            height: parent.height
            anchors.right: speed_set.left
            anchors.rightMargin: 20
            color: "transparent"
            Rectangle{
                width: 120
                height: parent.height / 2
                anchors.top: parent.top
                color: "transparent"
                Text {
                    id:time1
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    topPadding: 8
                    font.pixelSize: 20
                    font.bold: true
                    color: "#000000"
                    text: top_bar.getTime()
                }
            }//top
            Rectangle{
                width: 120
                height: 1
                anchors.verticalCenter: parent.verticalCenter
                color: "#999999"
            }//middle
            Rectangle{
                width: 120
                height: parent.height / 2
                anchors.bottom: parent.bottom
                color: "transparent"
                Text {
                    id: date
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    bottomPadding: 8
                    font.pixelSize: 20
                    font.bold: true
                    color: "#000000"
                    text: top_bar.getDate()
                }
            }//bottom
            MouseArea{
                id:mouse_datetime
                anchors.fill: parent
                onClicked: {
                    if(local.pageTimerSetOpeAuth[commonViewModel.userLevel] !== 0){
                        commonViewModel.getModbusSystemTime()
                        date_dialog.open()
                    }
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }
        // 4. speed area
        Rectangle{
            id : speed_set
            width: 70
            height: parent.height
            anchors.right: alarm_bell.left
            anchors.rightMargin: 20
            color: "transparent"
//            Image {
//                anchors.fill: parent
//                visible: mouse_speed.pressed
//                source: resourceManager.imagePath + "/00_COMMON/d000_31.png"
//            }
            Rectangle{
                width: 70
                height: parent.height / 2
                anchors.top: parent.top
                color: "transparent"
                Text {
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    topPadding: 8
                    font.bold: true
                    font.pixelSize: 20
                    color: top_bar.operableSwStatus === true ? "/00_COMMON/a000_01_01.png" : "/00_COMMON/a000_01.png"
                    text: (top_bar.manualSpeedValue >= 0 && top_bar.manualSpeedValue <= 6) ?
                              ((top_bar.manualSpeedLimitSetting === 0) ? (top_bar.manualSpeedValue < 2) ? top_bar.manualSpeed[top_bar.manualSpeedValue+2]: top_bar.manualSpeed[top_bar.manualSpeedValue] :
                                                                                                          ((top_bar.manualSpeedLimitSetting === 1) ? (top_bar.manualSpeedValue === 1) ? top_bar.manualSpeed[top_bar.manualSpeedValue+1]: top_bar.manualSpeed[top_bar.manualSpeedValue] :
                                                                                                                                                                                        ((top_bar.manualSpeedLimitSetting === 2) ? (top_bar.manualSpeedValue < 1) ? top_bar.manualSpeed[top_bar.manualSpeedValue+1]: top_bar.manualSpeed[top_bar.manualSpeedValue] :
                                                                                                                                                                                         top_bar.manualSpeed[top_bar.manualSpeedValue]))) : top_bar.manualSpeed[2]
                }
            }//top
            Rectangle{
                width: 70
                height: 1
                anchors.verticalCenter: parent.verticalCenter
                color: "#999999"
            }//middle
            Rectangle{
                width: 70
                height: parent.height / 2
                anchors.bottom: parent.bottom
                color: "transparent"
                Text {
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    bottomPadding: 8
                    font.pixelSize: 20
                    font.bold: true
                    color: "#000000"
                    text: (top_bar.displayAutoSpeed >= 0 && top_bar.displayAutoSpeed <= 9 )
                          ? top_bar.autoSpeed[top_bar.displayAutoSpeed] + "%" : top_bar.autoSpeed[9] + "%"
                }
            }//bottom
            MouseArea{
                id:mouse_speed
                anchors.fill: parent
                onClicked: {
                    if(local.pageSpeedSetOpeAuth[commonViewModel.userLevel] !== 0){
                        speed_dialog.open()
                        commonViewModel.modbusReadJogratioSpeed()
                    }
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }
        // etc. vertical line
        Rectangle {
            width: 2
            height: 48
            anchors.right: alarm_bell.left
            anchors.rightMargin: 3
            anchors.verticalCenter: parent.verticalCenter
            color: "#CCCCCC"
        }
        // 5. alarm bell area
        Rectangle{
            id : alarm_bell
            width: 65
            height: parent.height
            anchors.right: user_level.left
            anchors.rightMargin: 8
            color: "transparent"
            Image {
                anchors.centerIn: parent
                smooth: true
                source: {
                    let path
                    if (top_bar.emergencyStatus || (top_bar.alarmCount >= 1)) {
//                        path = resourceManager.imagePath + "/00_COMMON/a000_04.png" // 알람
                        path = resourceManager.imagePath + (top_bar.operableSwStatus === true ? "/00_COMMON/a000_04_01.png" : "/00_COMMON/a000_04.png") // 알람
                    } else if ((top_bar.curStatus & 0x0000FF00) !== 0) {
//                        path = resourceManager.imagePath + "/00_COMMON/a000_03.png" // 경고 인포
                        path = resourceManager.imagePath + (top_bar.operableSwStatus === true ? "/00_COMMON/a000_03_01.png" : "/00_COMMON/a000_03.png") // 경고 인포
                    } else {
//                        path = resourceManager.imagePath + "/00_COMMON/a000_02.png" // 일반
                        path = resourceManager.imagePath + (top_bar.operableSwStatus === true ? "/00_COMMON/a000_02_01.png" : "/00_COMMON/a000_02.png") // 일반
                    }
                    return path
                }
                MouseArea{
                    id: mouse_alarm
                    anchors.fill: parent
                    onClicked:{
                        if (top_bar.emergencyStatus){
                            emergency_dialog.open()
                        } else if ((top_bar.alarmCount >= 1) && local.pageAlarmOpeAuth[commonViewModel.userLevel] !== 0) {
                            top_bar.isAlreadyAlarmPopupDisp = true
                            alarm_dialog.open(top_bar.alarmCount, true)
                        } else if ((top_bar.curStatus & 0x0000FF00) !== 0x00000000) {
                            top_bar.isAlreadyErrorPopupDisp = true
                            error_dialog.open()
                        } else {
                            top_bar.isAlreadyAlarmPopupDisp = true
                            alarm_dialog.open(top_bar.alarmCount, true)
                        }
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }
        }
        // etc. vertical line
        Rectangle{
            width: 2
            height: 48
            anchors.right: user_level.left
            anchors.rightMargin: 3
            anchors.verticalCenter: parent.verticalCenter
            color: "#CCCCCC"
        }
        // 6. user level aera
        Rectangle{
            id: user_level
                width: 65
                height: parent.height
                anchors.right: lang_flag.left
                anchors.rightMargin: 8
                color: "transparent"

                Image {
                    anchors.centerIn: parent
                    smooth: true
                    source: {
                        const level = !REAL_TARGET ? top_bar.usr_select : commonViewModel.userLevel
                        const fileNames = ["a000_05", "a000_06", "a000_07", "a000_08", "a000_09"]
                        const name = fileNames[Math.min(level, 4)]
                        const suffix = top_bar.operableSwStatus ? "_01" : ""
                        return resourceManager.imagePath + "/00_COMMON/" + name + suffix + ".png"
                    }

                    MouseArea {
                        id: mouse_user
                        anchors.fill: parent
                        onClicked: {
                            if (local.pageUserLevelOpeAuth[commonViewModel.userLevel] !== 0) {
                                usr_switch_dialog.open()
                            }
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }
        }

        // etc. vertical line
        Rectangle{
            width: 2
            height: 48
            anchors.right: lang_flag.left
            anchors.rightMargin: 3
            anchors.verticalCenter: parent.verticalCenter
            color: "#CCCCCC"
        }
        // 7. language flag area
        Rectangle{
            id : lang_flag
            width: 65
            height: parent.height
            anchors.right: parent.right
            color: "transparent"
            Image {
                id: language
                anchors.centerIn: parent
                smooth: true
                source: {
                    let languageInfo;
                    if (REAL_TARGET){
                        languageInfo = top_bar.cmnLanguage
                    } else {
                        languageInfo = resourceManager.language
                    }
                    //console.log("resourceManager language:"+resourceManager.language, "commonViewModel language:"+commonViewModel.getLanguage(), "languageInfo:"+languageInfo)
                    switch(languageInfo){
                    case 0:     // 0.Japanese.
                        resourceManager.imagePath + "/00_COMMON/d000_38.png"
                        break;
                    case 1:     // 1.Chinaese.
                        resourceManager.imagePath + "/00_COMMON/d000_39.png"
                        break;
                    case 2:     // 2.Chinaese.
                        resourceManager.imagePath + "/00_COMMON/d000_40.png"
                        break;
                    case 3:     // 3.Korean.
//                        resourceManager.imagePath + "/00_COMMON/a000_10.png"
                        resourceManager.imagePath + (top_bar.operableSwStatus === true ? "/00_COMMON/a000_10_01.png" : "/00_COMMON/a000_10.png")
                        break;
                    case 4:     // 4.English.
                        resourceManager.imagePath + "/00_COMMON/d000_42.png"
                        break;
                    case 5:     // 5.Vietnam.
                        resourceManager.imagePath + "/00_COMMON/d000_43.png"
                        break;
                    case 6:     // 6.reserve.
                        resourceManager.imagePath + "/00_COMMON/d000_44.png"
                        break;
                    case 7:     // 7.reserve.
                        resourceManager.imagePath + "/00_COMMON/d000_45.png"
                        break;
                    case 8:     // 8.reserve.
                        resourceManager.imagePath + "/00_COMMON/d000_46.png"
                        break;
                    case 9:     // 9.reserve.
                        resourceManager.imagePath + "/00_COMMON/d000_47.png"
                        break;
                    case 10:    // 10.reserve.
                        resourceManager.imagePath + "/00_COMMON/d000_48.png"
                        break;
                    case 11:    // 11.reserve.
                        resourceManager.imagePath + "/00_COMMON/d000_49.png"
                        break;
                    case 12:    // 12.reserve.
                        resourceManager.imagePath + "/00_COMMON/d000_50.png"
                        break;
                    case 13:    // 13.reserve.
                        resourceManager.imagePath + "/00_COMMON/d000_51.png"
                        break;
                    case 14:    // 14.reserve.
                        resourceManager.imagePath + "/00_COMMON/d000_52.png"
                        break;
                    case 15:    // 15.reserve.
                        resourceManager.imagePath + "/00_COMMON/d000_53.png"
                        break;
                    default:
                        resourceManager.imagePath + "/00_COMMON/d000_39.png"
                        break;
                    }
                }
                MouseArea{
                    id:mouse_language
                    anchors.fill: parent
                    onClicked: {
                        if(local.pageLanguageSetOpeAuth[commonViewModel.userLevel] !== 0){
                            language_dialog.open()
                        }
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }
        }

        // 8. current axis position area
        Rectangle{
            width: 580
            height: 20
            y : 70
            color: "transparent"
            visible: teachingViewModel.currentPageTab === 6
            Row{
                id:axis_position_data
                x:5
                spacing: numAxis > 5 ? 12 : 40
                Repeater{
                    model: numAxis
                    Row{
                        id:axis_position_data_base
                        spacing: 2
                        property int axisorder: axisOrder[index]
                        Text{
                            height: 20
                            verticalAlignment: Text.AlignVCenter
                            font.pixelSize: 14
                            font.bold: true
                            color: currentoperateaxisOrder === axis_position_data_base.axisorder ? "yellow" : "#000000"
                            text: resourceManager.label(22, 18 + axisOrder[index], resourceManager.language) + ":"
                        }
                        Text{
                            width: 50
                            height: 20
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                            font.pixelSize: 14
                            font.bold: true
                            color:currentoperateaxisOrder === axis_position_data_base.axisorder ? "yellow" : "#000000"
                            text: axisDataList[axisOrder[index]].position
                        }
                    }
                }
            }
        }//9:current axis position


    }


/**************************************popup****************************************/
    Loader {
        id:usr_switch_dialog
        property bool activeByRequest: false
        function open(){
            if (!active) {
                activeByRequest = true
                asynchronous = false
                active = true
            } else if (status === Loader.Ready) {
                openDialog()
            }
        }

        function openDialog() {
            item.x = 200
            item.y = 200
            item.open()
        }
        active: false
        asynchronous: true
        source: "qrc:/qml/Components/MainDialog/User_Dialog.qml"
        onLoaded: {
            item.modal = true
            if (activeByRequest) {
                openDialog()
                activeByRequest = false
            }
            componentLoader.loadNextComponent()
        }
        Connections {
            target: usr_switch_dialog.item
            onOpened: {
                commonViewModel.setPopupDialogStatus(Enums.PopupDialogStatusUserSwitch, true)
                operationRecord.recordScreenSwitching(28, usr_switch_dialog.item.currentIndex)
            }
            onClosed: {
                commonViewModel.setPopupDialogStatus(Enums.PopupDialogStatusUserSwitch, false)
            }
        }
    }//用户切换弹出窗口
    Loader {
        id:date_dialog
        property bool activeByRequest: false
        function open(){
            if (!active) {
                activeByRequest = true
                asynchronous = false
                active = true
            } else if (status === Loader.Ready) {
                openDialog()
            }
        }
        function openDialog() {
            item.x = 200
            item.y = 200
            item.open()
        }
        active: false
        asynchronous: true
        source: "qrc:/qml/Components/MainDialog/Datetime_Dialog.qml"
        onLoaded: {
            item.modal = true
            if (activeByRequest) {
                openDialog()
                activeByRequest = false
            }
            componentLoader.loadNextComponent()
        }
        Connections {
            target: date_dialog.item
            onOpened: {
                commonViewModel.setPopupDialogStatus(Enums.PopupDialogStatusDate, true)
                operationRecord.recordScreenSwitching(29, 0)
            }
            onClosed: {
                commonViewModel.setPopupDialogStatus(Enums.PopupDialogStatusDate, false)
            }
        }
    }//日期功能弹出框
    Loader {
        id:speed_dialog
        property bool activeByRequest: false
        function open(){
            if (!active) {
                activeByRequest = true
                asynchronous = false
                active = true
            } else if (status === Loader.Ready) {
                openDialog()
            }
        }
        function openDialog() {
            item.x = 170
            item.y = 340
            item.open()
        }
        active: false
        asynchronous: true
        source: "qrc:/qml/Components/MainDialog/Speed_Dialog.qml"
        onLoaded: {
            item.modal = true
            if (activeByRequest) {
                openDialog()
                activeByRequest = false
            }
            componentLoader.loadNextComponent()
        }
        Connections {
            target: speed_dialog.item
            onOpened: {
                commonViewModel.setPopupDialogStatus(Enums.PopupDialogStatusSpeed, true)
                speed_dialog.item.recordScreenSwitching(0)
            }
            onClosed: {
                commonViewModel.setPopupDialogStatus(Enums.PopupDialogStatusSpeed, false)
            }
        }
    }//速度功能弹出框
    Loader {
        id:language_dialog
        property bool activeByRequest: false
        function open(){
            if (!active) {
                activeByRequest = true
                asynchronous = false
                active = true
            } else if (status === Loader.Ready) {
                openDialog()
            }
        }
        function openDialog() {
            item.x = 200
            item.y = 200
            item.open()
        }
        active: false
        asynchronous: true
        source: "qrc:/qml/Components/MainDialog/Language_Dialog.qml"
        onLoaded: {
            item.modal = true
            if (activeByRequest) {
                openDialog()
                activeByRequest = false
            }
            componentLoader.loadNextComponent()
        }
        Connections {
            target: language_dialog.item
            onOpened: {
                commonViewModel.setPopupDialogStatus(Enums.PopupDialogStatusLanguage, true)
                operationRecord.recordScreenSwitching(31, 0)
            }
            onClosed: {
                commonViewModel.setPopupDialogStatus(Enums.PopupDialogStatusLanguage, false)
            }
        }
    }//语言选择功能弹出框
    Loader {
        id:emergency_dialog
        property bool activeByRequest: false
        function open(){
            if (!active) {
                activeByRequest = true
                asynchronous = false
                active = true
            } else if (status === Loader.Ready) {
                openDialog()
            }
        }
        function close(){
            if (item != null) {
                item.close()
            }
            activeByRequest = false
        }
        function openDialog(){
            item.open()
        }
        active: false
        asynchronous: true
        source: "qrc:/qml/Components/MainDialog/EmergencyStop_Dialog.qml"
        onLoaded: {
            item.x = 200
            item.y = 200
            item.modal = true
            if (activeByRequest) {
                openDialog()
                activeByRequest = false
            }
            componentLoader.loadNextComponent()
        }
        Connections {
            target: emergency_dialog.item
            onOpened: {
                commonViewModel.setPopupDialogStatus(Enums.PopupDialogStatusEmergencyStop, true)
            }
            onClosed: {
                commonViewModel.setPopupDialogStatus(Enums.PopupDialogStatusEmergencyStop, false)
            }
            onDialogClose:{
                top_bar.isAlreadyEmergencyPopupDisp = false
            }
        }
    }//紧急停止弹出窗口
    Loader {
        id:operateCmd_dialog
        property bool activeByRequest: false
        function open(){
            if (!active) {
                activeByRequest = true
                asynchronous = false
                active = true
            } else if (status === Loader.Ready) {
                openDialog()
            }
        }
        function close(){
            if (item != null) {
                item.close()
            }
            activeByRequest = false
        }
        function openDialog(){
            item.x = 200
            item.y = 200
            item.open()
        }
        active: false
        asynchronous: true
        source: "qrc:/qml/Components/MainDialog/OperateCmd_Dialog.qml"
        onLoaded: {
            if (activeByRequest) {
                openDialog()
                activeByRequest = false
            }
            componentLoader.loadNextComponent()
        }
        Connections {
            target: operateCmd_dialog.item
            onOpened: {
                commonViewModel.setPopupDialogStatus(Enums.PopupDialogStatusOpeCmd, true)
            }
            onClosed: {
                commonViewModel.setPopupDialogStatus(Enums.PopupDialogStatusOpeCmd, false)
            }
            onDialogClose:{
                top_bar.isAlreadyOpeCmdPopupDisp = false
            }
        }
    }//动作表示弹出窗口
    Loader {
        id:connectError_dialog
        property bool activeByRequest: false
        property bool isAbortedError: false
        function open(isAborted){
            connectError_dialog.isAbortedError = isAborted;
            if (!active) {
                activeByRequest = true
                asynchronous = false
                active = true
            } else if (status === Loader.Ready) {
                openDialog()
            }
        }
        function close(){
            if (item != null) {
                item.close()
                if(connectError_dialog.isAbortedError === true){
                    top_bar.isAlreadyReplyAbortedErrorPopupDisp = false
                    modbusManager.communicationReplyAbortedError = false
                }
            }
            activeByRequest = false
        }
        function openDialog(){
            item.x = 200
            item.y = 200
            item.modal = true
            item.errorStatusColor = resourceManager.rgb(22,9)
            item.errorStatusText = resourceManager.label(22,9,resourceManager.language)
            item.errorTitle = resourceManager.label(22,10,resourceManager.language)
            item.errorTitleColor = resourceManager.rgb(22,10)
            item.open()
            if(connectError_dialog.isAbortedError === true){
                top_bar.isAlreadyReplyAbortedErrorPopupDisp = true
            }
        }
        active: false
        asynchronous: true
        source: "qrc:/qml/Components/MainDialog/Error_Dialog.qml"
        onLoaded: {
            if (activeByRequest) {
                openDialog()
                activeByRequest = false
            }
            componentLoader.loadNextComponent()
        }
        Connections {
            target: connectError_dialog.item
            onOpened: {
                commonViewModel.setPopupDialogStatus(Enums.PopupDialogStatusConnectError, true)
            }
            onClosed: {
                commonViewModel.setPopupDialogStatus(Enums.PopupDialogStatusConnectError, false)
            }
            onDialogClose:{
                top_bar.isAlreadyTimeoutErrorPopupDisp = false
                if(connectError_dialog.isAbortedError === true){
                    top_bar.isAlreadyReplyAbortedErrorPopupDisp = false
                    modbusManager.communicationReplyAbortedError = false
                }
            }
        }
    }//通讯连接错误弹出窗口
    Loader {
        id:error_dialog
        property bool activeByRequest: false
        function open(){
            if (!active) {
                activeByRequest = true
                asynchronous = false
                active = true
            } else if (status === Loader.Ready) {
                openDialog()
            }
        }
        function close(){
            if (item != null) {
                item.close()
            }
            activeByRequest = false
        }
        function openDialog(){
            item.open()
        }
        active: false
        asynchronous: true
        source: "qrc:/qml/Components/MainDialog/Error_Dialog.qml"
        onLoaded: {
            item.x = 200
            item.y = 200
            item.modal = true
            if (activeByRequest) {
                openDialog()
                activeByRequest = false
            }
            componentLoader.loadNextComponent()
        }
        Connections {
            target: error_dialog.item
            onOpened: {
                commonViewModel.setPopupDialogStatus(Enums.PopupDialogStatusError, true)
            }
            onClosed: {
                commonViewModel.setPopupDialogStatus(Enums.PopupDialogStatusError, false)
            }
            onDialogClose:{
                top_bar.isAlreadyErrorPopupDisp = false
            }
        }
    }//操作错误弹出窗口
    Loader {
        id: startup_dialog
        property bool activeByRequest: false
        function open(){
            if (!active) {
                activeByRequest = true
                asynchronous = false
                active = true
            } else if (status === Loader.Ready) {
                openDialog()
            }
        }
        function close(){
            if (item != null) {
                item.close()
            }
            activeByRequest = false
        }
        function openDialog(){
            item.x = 200
            item.y = 200
            item.modal = true
            item.errorStatusColor = resourceManager.rgb(22,17)
            item.errorStatusText = resourceManager.label(22,17,resourceManager.language)
            item.open()
        }
        active: false
        asynchronous: true
        source: "qrc:/qml/Components/MainDialog/Error_Dialog.qml"
        onLoaded: {
            if (activeByRequest) {
                openDialog()
                activeByRequest = false
            }
            componentLoader.loadNextComponent()
        }
        Connections {
            target: startup_dialog.item
            onOpened: {
                commonViewModel.setPopupDialogStatus(Enums.PopupDialogStartup, true)
            }
            onClosed: {
                commonViewModel.setPopupDialogStatus(Enums.PopupDialogStartup, false)
            }
            onDialogClose:{
                top_bar.isAlreadyStartupPopupDisp = false
            }
        }
    }//通讯连接错误弹出窗口
    Loader {
        id: alarm_dialog
        property bool activeByRequest: false
        property int alarmCount: 0
        property bool jogScrollOn: false
        function open(alarmCount, jogScrollOn){
            alarm_dialog.alarmCount = alarmCount
            alarm_dialog.jogScrollOn = jogScrollOn
            if (!active) {
                activeByRequest = true
                asynchronous = false
                active = true
            } else if (status === Loader.Ready) {
                openDialog()
            }
        }
        function close(){
            if (item != null) {
                item.close()
            }
            activeByRequest = false
        }
        function openDialog(){
            item.alarmCount = alarm_dialog.alarmCount
            item.jogScrollOn = alarm_dialog.jogScrollOn
            item.open()
        }
        active: true
        asynchronous: true
        source: "qrc:/qml/Components/MainDialog/Alarm_Dialog.qml"
        onLoaded: {
            item.x = 200
            item.y = 200
            item.modal = true
            if (activeByRequest) {
                openDialog()
                activeByRequest = false
            }
            componentLoader.loadNextComponent()
        }
        Connections {
            target: alarm_dialog.item
            onOpened: {
                commonViewModel.setPopupDialogStatus(Enums.PopupDialogStatusAlarm, true)
            }
            onClosed: {
                commonViewModel.setPopupDialogStatus(Enums.PopupDialogStatusAlarm, false)
            }
            onDialogClose:{
                top_bar.isAlreadyAlarmPopupDisp = false
            }
        }
    } //警报弹出窗口
    Loader {
        id: saveFileNameArea
        property bool activeByRequest: false
        function open(){
            if (!active) {
                activeByRequest = true
                asynchronous = false
                active = true
            } else if (status === Loader.Ready) {
                openDialog()
            }
        }
        function close(){
            visible = false
            activeByRequest = false
        }
        function openDialog(){
            item.openDialog()
        }

        active: false
        asynchronous: true
        visible: false
        source: "qrc:/qml/Widget/OneLineEditBox.qml"
        onLoaded: {
            item.width = 300
            item.height = 40
            item.fontPixelSize = 20
            item.textMaxLength = 24
            item.color = "transparent"
            item.borderColor = "#257DC7"
            item.text = ""
            item.textleftMargin = 25
            item.horizontalAlignment = Text.AlignLeft
            if (activeByRequest) {
                openDialog()
                activeByRequest = false
            }
            componentLoader.loadNextComponent()
        }
        Connections {
            target: saveFileNameArea.item
            onEnter: {
                saveFileNameArea.item.text = newValue;
                saveFileNameArea.close()
                dummy_file_dia_title.visible = false
                dummy_file_dia_bg.visible = false
                file_item.openSaveDialog(newValue);
            }
            onCloseVtk: {
                saveFileNameArea.close()
                dummy_file_dia_title.visible = false
                dummy_file_dia_bg.visible = false
                file_item.openSaveDialog("");
            }
        }
        Connections {
            target: commonViewModel
            enabled: !saveFileNameArea.active
            function judgeLoadSaveFileNameArea(userLevel, topPopupDialog)
            {
                if (topPopupDialog === Enums.PopupDialogStatusFile) {
                    let saveButtonVisible = (setupChangeViewModel.getCmdButtonDispSetting(1, userLevel) === 1)
                    if (saveButtonVisible) {
                        saveFileNameArea.active = true
                    }
                }
            }
            onTopPopupDialogChanged: judgeLoadSaveFileNameArea(commonViewModel.userLevel, commonViewModel.topPopupDialog)
            onUserLevelChanged: judgeLoadSaveFileNameArea(commonViewModel.userLevel, commonViewModel.topPopupDialog)
        }
    }//文件保存弹出窗口

    DummyFileDialogBackGround{
        id:dummy_file_dia_bg
        visible:false
        x:3
        y:155
    }
    DummyFileDialogTitle{
        id:dummy_file_dia_title
        visible:false
        x:3
        y:93
    }

    Loader {
        id:file_item
        property bool activeByRequest: false
        property int preMainPage:0
        property int preSubPage:0

        function open(curPage, curSubPage){
            preMainPage = curPage
            preSubPage = curSubPage
            if (!active) {
                activeByRequest = true
                asynchronous = false
                active = true
            } else if (status === Loader.Ready) {
                openDialog()
            }
        }
        function openSaveDialog(text){
            if (item != null) {
                item.commentText = text
                item.pageActive = true
                item.openSaveDialog()
                visible = true
            }
        }
        function close(){
            visible = false
            activeByRequest = false
        }
        function openDialog(){
            item.preMainPage = file_item.preMainPage
            item.preSubPage = file_item.preSubPage
            item.changePage(0)
            item.pageActive = true
            visible = true
            opened();
        }
        function opened(){
            commonViewModel.setPopupDialogStatus(Enums.PopupDialogStatusFile, true)
        }
        function closed(){
            commonViewModel.setPopupDialogStatus(Enums.PopupDialogStatusFile, false)
        }

        x: 10
        y: 110
        scale : 0.9
        active: true
        asynchronous: true
        visible:false
        source: "qrc:/qml/Components/File_Item.qml"
        onLoaded: {
            if (activeByRequest) {
                openDialog()
                activeByRequest = false
            }
            componentLoader.loadNextComponent()
        }
        Connections {
            target: file_item.item
            onItemClose:{
                file_item.item.pageActive = false
                file_item.visible = false
                file_item.closed();
            }
            onOpenVtk:{
                file_item.item.pageActive = false
                file_item.visible = false
                dummy_file_dia_title.visible = true
                dummy_file_dia_bg.visible = true
                saveFileNameArea.open()
            }
        }
    }//文件弹出窗口

/*************************************************************************************/
    // 물리 셀렉터가 바뀔 때마다 재계산
    Connections {
        target: phyKeyManager
        onSelectSwModeChanged: recomputeUiMode()
    }

    // 알람/비상/상태/운영모드 변화에도 재계산
    Connections {
        target: commonViewModel
        onEmergencyStatusChanged: recomputeUiMode()
        onCurAlarmCountChanged:  recomputeUiMode()
        onCurrentStatusChanged:  recomputeUiMode()
        onOperatingModeChanged:  recomputeUiMode()
    }
    Connections {
        target: phyKeyManager
        onOperableSwStatusChanged: {
            operationRecord.recordDispOperableKey(top_bar.operableSwStatus, operableSwStatus)
            top_bar.operableSwStatus = operableSwStatus;
        }
    }
    Connections {
        id: commonViewModelConnections
        property int prevStatus: 0
        function isErrorOccured(currentStatus, prevStatus){
            let currentErrorStatus = getErrorStatus(currentStatus)
            let prevErrorStatus = getErrorStatus(prevStatus)
            return (currentErrorStatus !== 0) && (prevErrorStatus !== currentErrorStatus)
        }

        function isSystemAlarmOccured(currentStatus, prevStatus){
            let currentAlarmStatus = getSystemAlarmStatus(currentStatus)
            let prevAlarmStatus = getSystemAlarmStatus(prevStatus)
            return (currentAlarmStatus !== 0) && (prevAlarmStatus !== currentAlarmStatus)
        }

        function isNormalAlarmOccured(currentStatus, prevStatus){
            let currentAlarmStatus = getNormalAlarmStatus(currentStatus)
            let prevAlarmStatus = getNormalAlarmStatus(prevStatus)
            return (currentAlarmStatus !== 0) && (prevAlarmStatus !== currentAlarmStatus)
        }

        function getErrorStatus(status){
            return (status & 0x0000FF00) >> 8
        }

        function getSystemAlarmStatus(status){
            return (status & 0x00FF0000) >> 16
        }

        function getNormalAlarmStatus(status){
            return (status & 0x000000FF)
        }

        function updateCurrentStatus(currentStatus){
            if (currentStatus !== 0){
                if (isSystemAlarmOccured(currentStatus, prevStatus) || isNormalAlarmOccured(currentStatus, prevStatus)) {
                    if (subMainView.visible === true && top_bar.isAlreadyAlarmPopupDisp === false){
                        if(local.pageAlarmOpeAuth[commonViewModel.userLevel] !== 0){
                            top_bar.isAlreadyAlarmPopupDisp = true
                            alarm_dialog.open(top_bar.alarmCount, true)
                        }
                    } else {
                        top_bar.isAlarmPopupDispInstructions = true
                        timer.running = true
                        timer.repeat = true
                    }
                } else if (isErrorOccured(currentStatus,  prevStatus)) {
                    if (currentStatus !== 0 && subMainView.visible === true && top_bar.isAlreadyErrorPopupDisp === false){
                        top_bar.isAlreadyErrorPopupDisp = true
                        error_dialog.open()
                    } else {
                        top_bar.isErrorPopupDispInstructions = true
                        timer.running = true
                        timer.repeat = true
                    }
                } else {
                }
            } else if (currentStatus === 0){
                //console.log("onAxisAlarmStatusChanged:"+commonViewModel.getCurAlarmCount())
                if (commonViewModel.getCurAlarmCount() === 0){
                    if (top_bar.isAlreadyAlarmPopupDisp === true){
                        top_bar.isAlreadyAlarmPopupDisp = false
                        alarm_dialog.close();
                    }
                    if (top_bar.isAlreadyErrorPopupDisp === true){
                        top_bar.isAlreadyErrorPopupDisp = false
                        error_dialog.close();
                    }
                }
            }
            prevStatus = currentStatus
        }

        function updateEmergencyStatus(emergencyStatus){
            if (emergencyStatus === true){
                if (subMainView.visible === true && top_bar.isAlreadyEmergencyPopupDisp === false){
                    top_bar.isAlreadyEmergencyPopupDisp = true
                    emergency_dialog.open()
                } else {
                    top_bar.isEmergencyPopupDispInstructions = true
                    timer.running = true
                    timer.repeat = true
                }
            }
        }

        function updateAxisAlarmStatus(){
            let curAlarmCount = commonViewModel.getCurAlarmCount()
            if (curAlarmCount !== 0) {
                if (subMainView.visible) {
                    if (!top_bar.isAlreadyAlarmPopupDisp) {
                        if(local.pageAlarmOpeAuth[commonViewModel.userLevel] !== 0){
                            top_bar.isAlreadyAlarmPopupDisp = true
                            alarm_dialog.open(curAlarmCount, true)
                        }
                    }
                } else {
                    top_bar.isAlarmPopupDispInstructions = true
                    timer.running = true
                    timer.repeat = true
                }
            }
        }

        function updateOpeCmdStatus(opeCmdStatus){
            if (opeCmdStatus === true){
                if (subMainView.visible === true && top_bar.isAlreadyOpeCmdPopupDisp === false){
                    if(local.pageMotionOpeAuth[commonViewModel.userLevel] !== 0){
                        top_bar.isAlreadyOpeCmdPopupDisp = true
                        operateCmd_dialog.open();
                    }
                } else {
                    top_bar.isOpeCmdPopupDispInstructions = true
                    timer.running = true
                    timer.repeat = true
                }
            } else {
                //console.log("onOpeCmdStatusChanged isAlreadyOpeCmdPopupDisp:"+top_bar.isAlreadyOpeCmdPopupDisp)
                if (top_bar.isAlreadyOpeCmdPopupDisp === true){
                    operateCmd_dialog.close();
                    top_bar.isAlreadyOpeCmdPopupDisp = false
                }
            }
        }

        function updateStartupStatus(startupStatus){
            if (0 < startupStatus){
                if (subMainView.visible === true && top_bar.isAlreadyStartupPopupDisp === false){
                    top_bar.isAlreadyOpeCmdPopupDisp = true
                    startup_dialog.open();
                } else {
                    top_bar.isStartupPopupDispInstructions = true
                    timer.running = true
                    timer.repeat = true
                }
            } else {
                //console.log("updateStartupStatus isAlreadyStartupPopupDisp:"+top_bar.isAlreadyStartupPopupDisp)
                if (top_bar.isAlreadyStartupPopupDisp === true){
                    startup_dialog.close();
                    top_bar.isAlreadyStartupPopupDisp = false
                }
            }
        }

        target: commonViewModel
        onCurrentStatusChanged:{
            updateCurrentStatus(currentStatus)
        }
        onEmergencyStatusChanged:{
            updateEmergencyStatus(emergencyStatus)
        }
        onAxisAlarmStatusChanged:{
            updateAxisAlarmStatus()
        }
        onOpeCmdStatusChanged:{
            updateOpeCmdStatus(opeCmdStatus)
        }
        onStartupStatusChanged:{
            updateStartupStatus(startupStatus)
        }
    }
    Connections {
        id: teachingViewModelSaveDialogConnection
        target: local.teachingviewModel
        enabled: false
        function requestOpenSaveProgramDialog() {
            local.teachingviewModel.requestOpenSaveProgramDialog(false)
            enabled = true
        }
        function reset() {
            enabled = false
        }
        readonly property int reasonClosingSaveProgramDialog: local.teachingviewModel.reasonClosingSaveProgramDialog
        onOpenSaveProgramDialogChanged: {
            if (openSaveProgramDialog) { return }
            switch (reasonClosingSaveProgramDialog) {
            case -1: // save dialog close, reset.
                reset()
                break
            case 0: // save cancel, open file dialog.
                local.doOpenFileItem()
                reset()
                break
            case 1: // saving, skip.
                // Wait for the change in progSaveStatus.
                break
            }
        }
        onProgSaveStatusChanged: {
            switch (progSaveStatus) {
            case  0: case  1: case 2: // saving, skip.
                                  console.log(`progSaveStatus:${progSaveStatus}, saving, skip.`)  // for debug.
                                  break
                              case -2: case -1: case 4: // save failed, reset.
                                                    console.log(`progSaveStatus:${progSaveStatus}, save failed.`)   // for debug.
                                                    reset()
                                                    break
                                                case  3: // compile success, switch screen.
                                                    local.doOpenFileItem()
                                                    reset()
                                                    break
            }
        }
    }
    Connections{
        target: modbusManager
        onLinkchange:{
            if(isonoff){
                connectError_dialog.open(false)
            }else{
                connectError_dialog.close()
            }
        }
    }
}
