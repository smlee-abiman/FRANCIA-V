import QtQml 2.12
import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0
import "../../Widget"
import "../../Components/RemindDialog"
import "../../Widget/ComboBox"
import Enums 1.0            // Enums.FunctionControllerSetSystem
/*************************************************************************************
* Title: UI/UX of AEK
* File: System_set.qml
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
    id: root
    width: parent.width
    height: parent.height
    property bool pageActive: false
    property var viewModel: functionViewModel.controllerSettingsView.systemSettingView
    readonly property var systemModeDisplay: viewModel.systemModeDisplay
    property bool isManual: commonViewModel.manipulateMode === 1

    QtObject {
        id: local
        property int currentPage: 0
        property var systemModeTbl: viewModel.systemModeTbl
        readonly property bool editedScreenBright: viewModel.editedScreenBright
        readonly property bool editedScreenOffTime: viewModel.editedScreenOffTime
        readonly property bool editedSystemProgram: viewModel.editedSystemProgram
        readonly property bool editedTakeUpType: viewModel.editedTakeUpType
        readonly property bool editedMachineName: viewModel.editedMachineName
        readonly property bool editedMachineSetupDate: viewModel.editedMachineSetupDate
        readonly property bool editedAcceptanceRegistrationCode: viewModel.editedAcceptanceRegistrationCode
        readonly property var  editedSystemModeTbl: viewModel.editedSystemModeTbl
        readonly property bool edited: (editedScreenBright ||
                                        editedScreenOffTime ||
                                        editedSystemProgram ||
                                        editedTakeUpType ||
                                        editedMachineName ||
                                        editedMachineSetupDate ||
                                        editedAcceptanceRegistrationCode ||
                                        editedSystemModeTbl.includes(true))
        property int language: 0
    }
    QtObject {
        id: penSet
        readonly property int screenOffTimeFormatNo: viewModel.screenOffTimeFormatNo
        readonly property var editTextColorRGB: resourceManager.dataColor(17,3)
    }

    Binding {
        target: local
        property: "language"
        value: resourceManager.language
        when: visible
    }

    function pageUp() {
        local.currentPage = Math.max(local.currentPage - 1, 0);
    }
    function pageDown() {
        local.currentPage = Math.min(local.currentPage + 1, 1);
    }

    Component.onDestruction: viewModel.deactivate()

    onPageActiveChanged: {
        if (pageActive) {
            commonViewModel.pageChange(Enums.FunctionControllerSetSystem)
            acceptanceRegistrationCode.state = "default"
            viewModel.activate();
            //commonViewModel.modbusReadBuzzer()
        } else {
            viewModel.deactivate()
        }
    }
//save
    Rectangle{
        width: 110
        height: 48
        x:816
        y:-46
        border.color: "#999999"
        color: save_mouse.pressed ? "#cc3333" : "#cccccc"
        Text {
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: save_mouse.pressed ? 25 : 24
            color: resourceManager.rgb(42,99)
            text: resourceManager.label(42,99,local.language)
        }
        MouseArea{
            id:save_mouse
            anchors.fill: parent
            onClicked: {
                if (local.edited) {
                    viewModel.saveSystemSettings()
                }
                phyKeyManager.ctrlBuzzer()
            }
        }
    }//保存按钮

    StackLayout{
        id:stack_monitor
        width: 920
        height: 522
        x:6
        y:6
        currentIndex: local.currentPage
        Column{
            x:6
            y:6
            spacing: 15
            Rectangle{
                width: 920
                height: 130
                color: "#eaeaea"
                border.color: "#999999"
                border.width: 1
                Rectangle{
                    width: 160
                    height: 130
                    color: "#999999"
                    border.color: "#999999"
                    border.width: 1
                    Text {
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                        leftPadding: 5
                        font.pixelSize: 24
                        font.bold: true
                        color: resourceManager.rgb(42,1)
                        text: resourceManager.label(42,1,local.language)
                    }
                }//标题
                Rectangle{
                    width: 758
                    height: 1
                    x:161
                    anchors.verticalCenter: parent.verticalCenter
                    color: "white"
                }//分隔线

                Text {
                    width: 180
                    height: 65
                    x:162
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                    font.bold: true
                    font.pixelSize: 22
                    color: resourceManager.rgb(42,5)
                    text: resourceManager.label(42,5,local.language)
                }//屏幕亮度
                Row{
                    x:350
                    y:17
                    spacing: 5
                    ImageButton{
                        upImageSource: resourceManager.imagePath + "/15_system_set/d150_03.png"
                        downImageSource: resourceManager.imagePath + "/15_system_set/d150_05.png"
                        onClicked: {
                            let value = screen_bright.value - 1
                            value = Math.max(screen_bright.from, value)
                            viewModel.editScreenBright(value)
                        }
                    }
                    Slider{
                        id:screen_bright
                        width: 160
                        height: 20
                        anchors.verticalCenter: parent.verticalCenter
                        from: 1
                        to: 8
                        stepSize: 1
                        value: viewModel.screenBright
                        snapMode: Slider.SnapAlways
                        onPressedChanged: {
                            if (!pressed) {
                                viewModel.editScreenBright(value)
                            }
                        }
                    }
                    ImageButton{
                        upImageSource: resourceManager.imagePath + "/15_system_set/d150_04.png"
                        downImageSource: resourceManager.imagePath + "/15_system_set/d150_06.png"
                        onClicked: {
                            let value = screen_bright.value + 1
                            value = Math.min(screen_bright.to, value)
                            viewModel.editScreenBright(value)
                        }
                    }
                }

                Text {
                    width: 180
                    height: 65
                    x:162
                    y:65
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                    font.bold: true
                    font.pixelSize: 22
                    color: resourceManager.rgb(42,6)
                    text: resourceManager.label(42,6,local.language)
                }//屏幕时间 스크린 타임
                Row{
                    x:350
                    y:74
                    spacing: 5
                    LimitNumericEditBox{
                        width: 120
                        height: 48
                        anchors.verticalCenter: parent.verticalCenter
                        textColor: local.editedScreenOffTime ? penSet.editTextColorRGB : "black"
                        color: "#ffffff"
                        borderColor: "#999999"
                        fontPixelSize: 22
                        min: "1"
                        max: "99"
                        value: resourceManager.formatData(viewModel.screenOffTime, penSet.screenOffTimeFormatNo)
                        keyLayoutType: 0
                        onEnter: {
                            viewModel.editScreenOffTime(Number(newValue))
                            commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                            commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                        }
                    }
                    Text {
                        width: 100
                        height: 48
                        anchors.verticalCenter: parent.verticalCenter
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                        font.bold: true
                        font.pixelSize: 22
                        color: resourceManager.rgb(42,7)
                        text: resourceManager.label(42,7,local.language)
                    }
                }

                Row{
                    x:515
                    spacing: 6
                    Text {
                        width: 180
                        height: 65
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignRight
                        font.pixelSize: 22
                        color: resourceManager.rgb(42,100)
                        text: resourceManager.label(42,100,local.language)
                    }
                    Image {
                        anchors.verticalCenter: parent.verticalCenter
                        source: phyKeyManager.currentBuzzerState ? resourceManager.imagePath + "/15_system_set/d150_14.png" : resourceManager.imagePath + "/15_system_set/d150_13.png"
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                phyKeyManager.currentBuzzerState = !phyKeyManager.currentBuzzerState
                                commonViewModel.modbusSetBuzzer(phyKeyManager.currentBuzzerState,phyKeyManager.currentBuzzerIndex)
                                phyKeyManager.ctrlBuzzer()
                            }
                        }
                        Text{
                            anchors.verticalCenter: parent.verticalCenter
                            x: phyKeyManager.currentBuzzerState ? 10 : 56
                            font.pixelSize: 15
                            color: phyKeyManager.currentBuzzerState ? resourceManager.rgb(42,107) : resourceManager.rgb(42,108)
                            text: phyKeyManager.currentBuzzerState ? resourceManager.label(42,107,local.language) : resourceManager.label(42,108,local.language)
                        }
                    }
                }//蜂鸣器 부저
                Row{
                    x:515
                    y:65
                    spacing: 6
                    Text {
                        width: 180
                        height: 65
                        anchors.verticalCenter: parent.verticalCenter
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignRight
                        font.bold: true
                        font.pixelSize: 22
                        color: resourceManager.rgb(42,101)
                        text: resourceManager.label(42,101,local.language)
                    }
                    StringComboBox_style1{
                        width: 200
                        height: 50
                        anchors.verticalCenter: parent.verticalCenter
                        bgRectBorderWidth: 2
                        bgPopupBorderWidth:2
                        popupDelegateNum: 5
                        resourceGroupId: 42
                        resourceStartIndex: 102
                        bgRectColor: "#ffffff"
                        bgIndicatorColor: "#999999"
                        displayTextColor: local.editedTakeUpType ? penSet.editTextColorRGB :"#000000"
                        currentIndex: phyKeyManager.currentBuzzerIndex
                        onCurrentIndexChanged:{
                            if(pageActive){
                                commonViewModel.modbusSetBuzzer(phyKeyManager.currentBuzzerState,currentIndex)
                                switch(phyKeyManager.currentBuzzerIndex){
                                case 0:
                                    phyKeyManager.currentBuzzerPitch = 40;
                                    break;
                                case 1:
                                    phyKeyManager.currentBuzzerPitch = 50;
                                    break;
                                case 2:
                                    phyKeyManager.currentBuzzerPitch = 62;
                                    break;
                                case 3:
                                    phyKeyManager.currentBuzzerPitch = 69;
                                    break;
                                case 4:
                                    phyKeyManager.currentBuzzerPitch = 75;
                                    break;
                                }
                            }
                        }
                    }
                }//蜂鸣器音色 부저 음색
            }//屏幕参数
// 교시 prog 없음으로 주석처리
//            Rectangle{
//                width: 920
//                height: 65
//                color: "#eaeaea"
//                border.color: "#999999"
//                border.width: 1
//                Rectangle{
//                    width: 160
//                    height: 65
//                    color: "#999999"
//                    border.color: "#999999"
//                    border.width: 1
//                    Text {
//                        anchors.fill: parent
//                        verticalAlignment: Text.AlignVCenter
//                        horizontalAlignment: Text.AlignLeft
//                        leftPadding: 5
//                        font.pixelSize: 24
//                        font.bold: true
//                        color: resourceManager.rgb(42,2)
//                        text: resourceManager.label(42,2,local.language)

//                    }
//                }
//                Text {
//                    width: 180
//                    height: 65
//                    x:162
//                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignRight
//                    font.bold: true
//                    font.pixelSize: 22
//                    color: resourceManager.rgb(42, 8)
//                    text: resourceManager.label(42, 8,local.language)
//                }
//                StringComboBox_style1{
//                    id: porogram_select
//                    width: 167
//                    height: 49
//                    x:350
//                    anchors.verticalCenter: parent.verticalCenter
//                    bgRectBorderWidth: 2
//                    bgPopupBorderWidth:2
//                    popupDelegateNum: 2
//                    resourceGroupId: 42
//                    resourceStartIndex: 77
//                    bgRectColor: "#ffffff"
//                    bgIndicatorColor: "#999999"
//                    currentIndex: viewModel.systemProgram
//                    displayTextColor: local.editedSystemProgram ? penSet.editTextColorRGB : "#000000"
//                    onCurrentIndexChanged:{
//                        if (currentIndex === viewModel.systemProgram) { return }
//                        viewModel.editSystemProgram(currentIndex)
//                    }
//                }
//            }//系统程序

            Rectangle{
                width: 920
                height: 130
                color: "#eaeaea"
                border.color: "#999999"
                border.width: 1
                Rectangle{
                    width: 160
                    height: 130
                    color: "#999999"
                    border.color: "#999999"
                    border.width: 1
                    Text {
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                        leftPadding: 5
                        font.pixelSize: 24
                        font.bold: true
                        color: resourceManager.rgb(42,3)
                        text: resourceManager.label(42,3,local.language)
                    }
                }
                Rectangle{
                    width: 758
                    height: 1
                    x:161
                    anchors.verticalCenter: parent.verticalCenter
                    color: "white"
                }//分隔线
                Text {
                    width: 180
                    height: 65
                    x:162
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                    font.bold: true
                    font.pixelSize: 22
                    color: resourceManager.rgb(42,9)
                    text: resourceManager.label(42,9,local.language)
                }//注塑机信号类型

                StringComboBox_style1{
                    id: signal_select
                    width: 200
                    height: 50
                    x: 350
                    y: 7
                    bgRectBorderWidth: 2
                    bgPopupBorderWidth:2
                    popupDelegateNum: 3
                    resourceGroupId: 42
                    resourceStartIndex: 95
                    bgRectColor: "#ffffff"
                    bgIndicatorColor: "#999999"
                    displayTextColor: local.editedTakeUpType ? penSet.editTextColorRGB :"#000000"
                    currentIndex: viewModel.takeUpType
                    onCurrentIndexChanged:{
                        if (currentIndex === viewModel.takeUpType) { return }
                        viewModel.editTakeUpType(currentIndex)
                    }
                }

                Text {
                    width: 180
                    height: 65
                    x:162
                    y:65
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                    font.bold: true
                    font.pixelSize: 22
                    color: resourceManager.rgb(42,10)
                    text: resourceManager.label(42,10,local.language)
                }//机器名称
                OneLineEditBox {
                    width: 200
                    height: 48
                    x:350
                    y:74
                    textColor: local.editedMachineName ? penSet.editTextColorRGB : "black"
                    color: "#ffffff"
                    borderColor: "#999999"
                    fontPixelSize: 24
                    text: viewModel.machineName
                    textMaxLength: 20
                    onEnter: {
                        viewModel.editMachineName(newValue)
                    }
                }
                Text {
                    width: 180
                    height: 65
                    x:525
                    y:65
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                    font.bold: true
                    font.pixelSize: 22
                    color: resourceManager.rgb(42,11)
                    text: resourceManager.label(42,11,local.language)
                }//机器设置日
                Rectangle{
                    width: 200
                    height: 48
                    x:710
                    y:74
                    color: "#ffffff"
                    border.color: "#999999"
                    border.width: 2
                    radius: 6
                    Text {
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.bold: true
                        font.pixelSize: 22
                        color: local.editedMachineSetupDate ? penSet.editTextColorRGB : "black"
                        text: Qt.formatDate(viewModel.machineSetupDate,"yyyy/MM/dd")
                    }
                    MouseArea {
                        anchors.fill: parent
                        pressAndHoldInterval: viewModel.machineSetupDatePressAndHoldInterval_msec
                        onPressAndHold: viewModel.resetMachineSetupDateToCurrentDate()
                    }
                }
            }//初始设置

            Rectangle{
                width: 920
                height: 130
                color: "#eaeaea"
                border.color: "#999999"
                border.width: 1
                Rectangle{
                    width: 160
                    height: 130
                    color: "#999999"
                    border.color: "#999999"
                    border.width: 1
                    Text {
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                        leftPadding: 5
                        font.pixelSize: 24
                        font.bold: true
                        color: resourceManager.rgb(42,4)
                        text: resourceManager.label(42,4,local.language)
                    }
                }
                Rectangle{
                    width: 758
                    height: 1
                    x:161
                    anchors.verticalCenter: parent.verticalCenter
                    color: "white"
                }//分隔线

                Text {
                    width: 180
                    height: 65
                    x:162
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                    lineHeight: 0.8
                    font.pixelSize: 22
                    color: resourceManager.rgb(42,13)
                    text: resourceManager.label(42,13,local.language)
                }//机器码 기계 코드
                LimitNumericEditBox{
                    readonly property var controllerValue: viewModel.acceptanceRegistrationCode
                    property string editedValue: ""
                    id: acceptanceRegistrationCode
                    width: 140
                    height: 48
                    x:350
                    y:8
                    textColor: local.editedAcceptanceRegistrationCode ? penSet.editTextColorRGB : "black"
                    color: "#ffffff"
                    borderColor: "#999999"
                    fontPixelSize: 22
                    min: "0000"
                    max: "9999"
                    keyLayoutType: 0
                    state: "default"
                    states: [
                        State {
                            name: "default"
                            PropertyChanges { target: acceptanceRegistrationCode; value: controllerValue }
                        },
                        State {
                            name: "edited"
                            PropertyChanges { target: acceptanceRegistrationCode; value: editedValue }
                        }
                    ]
                    onControllerValueChanged: {
                        state = "default"
                    }
                    onEnter: {
                        if (controllerValue === Number(newValue)) {
                            return
                        }
                        editedValue = newValue
                        state = "edited"
                    }
                }//注册码
                Rectangle{
                    width: 90
                    height: 42
                    x:500
                    y:11
                    border.width: 2
                    border.color: "#999999"
                    color: "#cccccc"
                    Text {
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 20
                        color: resourceManager.rgb(42,16)
                        text: resourceManager.label(42,16,local.language)
                    }
                    MouseArea{
                        anchors.fill: parent
                        onPressed: parent.color = "#cc3333"
                        onReleased: parent.color = "#ffffff"
                        onClicked: {
                            if (acceptanceRegistrationCode.state === "edited") {
                                viewModel.editAcceptanceRegistrationCode(Number(acceptanceRegistrationCode.editedValue))
                                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                            }
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }

                Text {
                    width: 180
                    height: 65
                    x:162
                    y:65
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                    font.bold: true
                    font.pixelSize: 22
                    color: resourceManager.rgb(42,14)
                    text: resourceManager.label(42,14,local.language)
                }//剩余验收时间
                Rectangle{
                    width: 140
                    height: 48
                    x:350
                    y:74
                    color: "#999999"
                    border.color: "#999999"
                    Text {
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        text: viewModel.acceptanceRemainingTime
                    }
                }
            }//密码验收
        }//第一页

        Item {
            x:6
            y:6
            Rectangle{
                width: 920
                height:500
                color: "#eaeaea"
                border.color: "#999999"
                border.width: 1
                // system mode label.
                Rectangle{
                    width: 160
                    height: 500
                    color: "#999999"
                    border.color: "#999999"
                    border.width: 1
                    Text {
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                        leftPadding: 5
                        font.pixelSize: 24
                        font.bold: true
                        color: resourceManager.rgb(42,98)
                        text: resourceManager.label(42,98,local.language)
                    }
                }
                Column{
                    x:161
                    y:60
                    spacing: 61
                    Repeater{
                        model: 7
                        Rectangle{
                            width: 758
                            height: 1
                            color: "white"
                        }
                    }
                }//分隔线
                Rectangle{
                    width: 1
                    height: parent.height-2
                    x:540
                    anchors.verticalCenter: parent.verticalCenter
                    color: "white"
                }//分隔线

                Grid{
                    x:164
                    y:1
                    columns: 2
                    rows: 8
                    columnSpacing: 186
                    rowSpacing: 4
                    flow: Grid.TopToBottom
                    Repeater{
                        model: 16
                        Rectangle{
                            width: 232
                            height: 58
                            color: "transparent"
                            visible: systemModeDisplay[index]
                            Text{
                                anchors.fill: parent
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignRight
                                font.bold: true
                                font.pixelSize: 22
                                wrapMode: Text.WordWrap
                                color: resourceManager.rgb(42,79+index)
                                text: resourceManager.label(42,79+index,local.language)
                            }
                        }
                    }
                }
                Grid{
                    x:400
                    y:10
                    columns: 2
                    rows: 8
                    columnSpacing: 270
                    rowSpacing: 22
                    flow: Grid.TopToBottom
                    Repeater{
                        model: local.systemModeTbl.length
                        Image {
                            readonly property bool systeMode: local.systemModeTbl[index]
                            readonly property bool editedSystemMode: local.editedSystemModeTbl[index]
                            source: systeMode ? resourceManager.imagePath + "/15_system_set/d150_14.png" : resourceManager.imagePath + "/15_system_set/d150_13.png"
                            visible: systemModeDisplay[index]
                            Text{
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: 15
                                font.pixelSize: 22
                                color: editedSystemMode ? penSet.editTextColorRGB : "black"
                                text: "ON"
                                visible: systeMode
                            }
                            Text{
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.right: parent.right
                                anchors.rightMargin: 15
                                font.pixelSize: 22
                                color: editedSystemMode ? penSet.editTextColorRGB : "black"
                                text: "OFF"
                                visible: !systeMode
                            }
                            MouseArea{
                                anchors.fill: parent
                                onClicked: {
                                    if(isManual){
                                        viewModel.editSystemMode(index, !systeMode)
                                    }else{
                                        mode_remind_dialog.open()
                                    }
                                    phyKeyManager.ctrlBuzzer()
                                }
                            }
                        }
                    }
                }
            }
        }//第二页
    }

    Item {
        x: 932
        y: 6
        width: 60          // ✅ 여기(예: 60)가 "가운데 정렬 기준 폭"
        height: 500

        ImageScrollBar {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            topButtonPressImage: resourceManager.imagePath + "/08_Monitor/a080_03.png"
            topButtonReleaseImage: resourceManager.imagePath + "/08_Monitor/a080_04.png"
            botomButtonPressImage: resourceManager.imagePath + "/08_Monitor/a080_05.png"
            botomButtonReleaseImage: resourceManager.imagePath + "/08_Monitor/a080_06.png"
            pageLabal: (local.currentPage + 1) + "/" + "2"
            pageLabelColor: "black"
            onScrollUpClicked: root.pageUp()
            onScrollDownClicked: root.pageDown()
        }
    }

    Remind_Forbid_Dialog{
        id:mode_remind_dialog
        x:60
        y:120
        message: resourceManager.label(35,67,resourceManager.language)
        modal: true
    }//非手动状态时模式切换禁止窗口
}
