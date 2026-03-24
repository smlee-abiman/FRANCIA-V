import QtQml 2.12
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import Enums 1.0
import "../../Widget"
import "../../Program/PosSet/PosSetUtils.js" as PosSetUtils
/*************************************************************************************
* Title: UI/UX of AEK
* File: Speed_Dialog.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
 - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.23
* Worker: SH.HONG
**************************************************************************************/
Dialog{
    id:speed_dialog
    width: 500
    height: 323
    modal: false
    closePolicy:Popup.NoAutoClose

    property int manualSpeed: PosSetUtils.getManualSpeedValue(top_bar.manualSpeedValue, top_bar.manualSpeedLimitSetting)
    property int jogMagnificationSetting: phyKeyManager.jogMagnificationSetting
    property var jogspeed: commonViewModel.JogSpeed
    property var axisOrder: commonViewModel.axisOrder
    property int numAxis: axisOrder.length
    property int currentoperateaxisOrder: commonViewModel.currentOperateAxisOrder
    property var jogratio: commonViewModel.jogRatio
    property var joglimitspeed: commonViewModel.jogLimitspeed

    property int displayAutoSpeed: commonViewModel.autoSpeedStatus
    property bool holdAutoSpeed: false
    Timer {
        id: autoSpeedHoldTimer
        interval: 200
        repeat: false
        onTriggered: {
            speed_dialog.holdAutoSpeed = false
            speed_dialog.displayAutoSpeed = commonViewModel.autoSpeedStatus
        }
    }
    Connections {
        target: commonViewModel
        onAutoSpeedStatusChanged: {
            if (!speed_dialog.holdAutoSpeed) {
                speed_dialog.displayAutoSpeed = commonViewModel.autoSpeedStatus
            }
        }
    }

    function recordScreenSwitching(index){
        switch (index) {
        case 0:
            operationRecord.recordScreenSwitching(30, 0)
            break
        case 1:
            operationRecord.recordScreenSwitching(30, 2)
            break
        }
    }

    background: Rectangle{
        width: 660
        height: 400
        border.width: 2
        border.color: "#999999"
        color: "#ffffff"

        Rectangle{
            width: 660
            height: 55
            border.width: 2
            border.color: "#999999"
            color: "#999999"
            Text{
                anchors.centerIn: parent
                font.pixelSize: 26
                color: resourceManager.rgb(56,0)
                text: resourceManager.label(56,0,resourceManager.language)
            }
            ImageButton{
                id:speed_dialog_close
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 15
                scale: 1.5
                upImageSource: resourceManager.imagePath + "/28_USB/a280_02.png"
                downImageSource: resourceManager.imagePath + "/28_USB/a280_02.png"
                onClicked: {
                    speed_dialog.close()
                }
            }
        }//?�题??    }

    // ??기존 Image 컨테?�너 ?�용??Item?�로 변�?(Image???�각?�소?? ?�이?�웃 컨테?�너�??�면 ?�러 가??
    Item{
        x:0
        y:57

        TabBar{
            id:speed_tab
            spacing: 6
            background: Rectangle{
                anchors.fill: parent
                color: "transparent"
            }
            TabButton {
                id: speed_tab1
                width: 100
                height: 40

                background: Rectangle {
                    anchors.fill: parent
                    color: (speed_tab.currentIndex === 0) ? "#cc3333" : "#cccccc"
                }

                contentItem: Text {
                    anchors.centerIn: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 22
                    font.bold: true
                    wrapMode: Text.WordWrap
                    text: resourceManager.label(56,11,resourceManager.language)
                    color: "white"
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        speed_tab.currentIndex = 0
                        speed_dialog.recordScreenSwitching(0)
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }

//            TabButton {
//                id: speed_tab2
//                width: 100
//                height: 36
//
//                background: Rectangle {
//                    anchors.fill: parent
//                    color: (speed_tab.currentIndex === 1) ? "#cc3333" : "#cccccc"
//                }
//
//                contentItem: Text {
//                    anchors.centerIn: parent
//                    horizontalAlignment: Text.AlignHCenter
//                    verticalAlignment: Text.AlignVCenter
//                    font.pixelSize: 18
//                    font.bold: true
//                    wrapMode: Text.WordWrap
//                    text: resourceManager.label(36,3,resourceManager.language)
//                    color: "white"
//                }
//
//                MouseArea {
//                    anchors.fill: parent
//                    onClicked: {
//                        speed_tab.currentIndex = 1
//                        speed_dialog.recordScreenSwitching(1)
//                        phyKeyManager.ctrlBuzzer()
//                    }
//                }
//            }
        }

        StackLayout{
            id:stack1
            width: 629
            height: 204
            y:40
            currentIndex: speed_tab.currentIndex

            Item{
                width: stack1.width
                height: stack1.height

                Row{
                    x:20
                    y:20
                    spacing: 4
                    Rectangle{
                        width: 300
                        height: 260
                        color: "#eaeaea"
                        border.color: "#999999"
                        Rectangle{
                            width: 140
                            height: 40
                            anchors.horizontalCenter: parent.horizontalCenter
                            y:-15
                            color: "#ffffff"
                            border.color: "#999999"
                            Text {
                                anchors.fill: parent
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                font.pixelSize: 21
                                font.bold: true
                                text: resourceManager.label(56,1,resourceManager.language)
                            }
                        }//title
                        Grid{
                            x:25
                            y:40
                            columns: 2
                            rows: 4
                            columnSpacing: 20
                            rowSpacing: 15
                            flow: Grid.TopToBottom
                            Row{
                                spacing: 20
                                visible: top_bar.manualSpeedLimitSetting & 0x01
                                ImageButton{
                                    anchors.verticalCenter: parent.verticalCenter
                                    upImageSource: speed_dialog.manualSpeed === Enums.ManualSpeed0001 ? resourceManager.imagePath + "/36_DIALOG/a360_06.png" : resourceManager.imagePath + "/36_DIALOG/a360_05.png"
                                    downImageSource: resourceManager.imagePath + "/36_DIALOG/a360_06.png"
                                    scale: 2
                                    onClicked: {
                                        local.recordSetManualSpeed(speed_dialog.manualSpeed, Enums.ManualSpeed0001)
                                        commonViewModel.modbusSpeedControl(Enums.ManualSpeed0001)
                                    }
                                }
                                Text{
                                    width: 80
                                    height: 42
                                    font.pixelSize: 20
                                    font.bold: true
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignLeft
                                    color: resourceManager.rgb(56,2)
                                    text:resourceManager.label(56,2,resourceManager.language)
                                }
                            }//0.001
                            Row{
                                spacing: 20
                                visible: top_bar.manualSpeedLimitSetting & 0x02
                                ImageButton{
                                    anchors.verticalCenter: parent.verticalCenter
                                    upImageSource: speed_dialog.manualSpeed === Enums.ManualSpeed001 ? resourceManager.imagePath + "/36_DIALOG/a360_06.png" : resourceManager.imagePath + "/36_DIALOG/a360_05.png"
                                    downImageSource: resourceManager.imagePath + "/36_DIALOG/a360_06.png"
                                    scale: 2
                                    onClicked: {
                                        local.recordSetManualSpeed(speed_dialog.manualSpeed, Enums.ManualSpeed001)
                                        commonViewModel.modbusSpeedControl(Enums.ManualSpeed001)
                                    }
                                }
                                Text{
                                    width: 80
                                    height: 42
                                    font.pixelSize: 20
                                    font.bold: true
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignLeft
                                    color: resourceManager.rgb(56,3)
                                    text:resourceManager.label(56,3,resourceManager.language)
                                }
                            }//0.01
                            Row{
                                spacing: 20
                                ImageButton{
                                    anchors.verticalCenter: parent.verticalCenter
                                    upImageSource: speed_dialog.manualSpeed === Enums.ManualSpeed01 ? resourceManager.imagePath + "/36_DIALOG/a360_06.png" : resourceManager.imagePath + "/36_DIALOG/a360_05.png"
                                    downImageSource: resourceManager.imagePath + "/36_DIALOG/a360_06.png"
                                    scale: 2
                                    onClicked: {
                                        local.recordSetManualSpeed(speed_dialog.manualSpeed, Enums.ManualSpeed01)
                                        commonViewModel.modbusSpeedControl(Enums.ManualSpeed01)
                                    }
                                }
                                Text{
                                    width: 80
                                    height: 42
                                    font.pixelSize: 20
                                    font.bold: true
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignLeft
                                    color: resourceManager.rgb(56,4)
                                    text:resourceManager.label(56,4,resourceManager.language)
                                }
                            }//0.1
                            Row{
                                spacing: 20
                                ImageButton{
                                    anchors.verticalCenter: parent.verticalCenter
                                    upImageSource: speed_dialog.manualSpeed === Enums.ManualSpeed1 ? resourceManager.imagePath + "/36_DIALOG/a360_06.png" : resourceManager.imagePath + "/36_DIALOG/a360_05.png"
                                    downImageSource: resourceManager.imagePath + "/36_DIALOG/a360_06.png"
                                    scale: 2
                                    onClicked: {
                                        local.recordSetManualSpeed(speed_dialog.manualSpeed, Enums.ManualSpeed1)
                                        commonViewModel.modbusSpeedControl(Enums.ManualSpeed1)
                                    }
                                }
                                Text{
                                    width: 80
                                    height: 42
                                    font.pixelSize: 20
                                    font.bold: true
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignLeft
                                    color: resourceManager.rgb(56,5)
                                    text:resourceManager.label(56,5,resourceManager.language)
                                }
                            }//1
                            Row{
                                spacing: 20
                                ImageButton{
                                    anchors.verticalCenter: parent.verticalCenter
                                    upImageSource: speed_dialog.manualSpeed === Enums.ManualSpeedSlow ? resourceManager.imagePath + "/36_DIALOG/a360_06.png" : resourceManager.imagePath + "/36_DIALOG/a360_05.png"
                                    downImageSource: resourceManager.imagePath + "/36_DIALOG/a360_06.png"
                                    scale: 2
                                    onClicked: {
                                        local.recordSetManualSpeed(speed_dialog.manualSpeed, Enums.ManualSpeedSlow)
                                        commonViewModel.modbusSpeedControl(Enums.ManualSpeedSlow)
                                    }
                                }
                                Text{
                                    width: 80
                                    height: 42
                                    font.pixelSize: 20
                                    font.bold: true
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignLeft
                                    color: resourceManager.rgb(56,6)
                                    text:resourceManager.label(56,6,resourceManager.language)
                                }
                            }//slow speed
                            Row{
                                spacing: 20
                                ImageButton{
                                    anchors.verticalCenter: parent.verticalCenter
                                    upImageSource: speed_dialog.manualSpeed === Enums.ManualSpeedMedium ? resourceManager.imagePath + "/36_DIALOG/a360_06.png" : resourceManager.imagePath + "/36_DIALOG/a360_05.png"
                                    downImageSource: resourceManager.imagePath + "/36_DIALOG/a360_06.png"
                                    scale: 2
                                    onClicked: {
                                        local.recordSetManualSpeed(speed_dialog.manualSpeed, Enums.ManualSpeedMedium)
                                        commonViewModel.modbusSpeedControl(Enums.ManualSpeedMedium)
                                    }
                                }
                                Text{
                                    width: 80
                                    height: 42
                                    font.pixelSize: 20
                                    font.bold: true
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignLeft
                                    color: resourceManager.rgb(56,7)
                                    text:resourceManager.label(56,7,resourceManager.language)
                                }
                            }//middle speed
                            Row{
                                spacing: 20
                                ImageButton{
                                    anchors.verticalCenter: parent.verticalCenter
                                    upImageSource: speed_dialog.manualSpeed === Enums.ManualSpeedHigh ? resourceManager.imagePath + "/36_DIALOG/a360_06.png" : resourceManager.imagePath + "/36_DIALOG/a360_05.png"
                                    downImageSource: resourceManager.imagePath + "/36_DIALOG/a360_06.png"
                                    scale: 2
                                    onClicked: {
                                        local.recordSetManualSpeed(speed_dialog.manualSpeed, Enums.ManualSpeedHigh)
                                        commonViewModel.modbusSpeedControl(Enums.ManualSpeedHigh)
                                    }
                                }
                                Text{
                                    width: 80
                                    height: 42
                                    font.pixelSize: 20
                                    font.bold: true
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignLeft
                                    color: resourceManager.rgb(56,7)
                                    text:resourceManager.label(56,8,resourceManager.language)
                                }
                            }//high speed
                        }
                    }//manaul speed

                    Rectangle{
                        width: 300
                        height: 260
                        color: "#eaeaea"
                        border.color: "#999999"
                        Rectangle{
                            width: 140
                            height: 40
                            anchors.horizontalCenter: parent.horizontalCenter
                            y:-15
                            color: "#ffffff"
                            border.color: "#999999"
                            Text {
                                anchors.fill: parent
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                font.pixelSize: 21
                                font.bold: true
                                text: resourceManager.label(56,9,resourceManager.language)
                            }
                        }//title
                        Row{
                            anchors.horizontalCenter: parent.horizontalCenter
                            y:50
                            spacing: 20
                            ImageButton{
                                anchors.verticalCenter: parent.verticalCenter
                                scale: 1.5
                                upImageSource: resourceManager.imagePath + "/36_DIALOG/a360_07.png"
                                downImageSource: resourceManager.imagePath + "/36_DIALOG/a360_10.png"
                                onClicked: {
                                    if (top_bar.autoSpeedValue > 0){
                                        let setAutoSpeed = top_bar.autoSpeedValue - 1
                                        if (setAutoSpeed < 0){
                                            setAutoSpeed = 0;
                                        }
                                        speed_dialog.holdAutoSpeed = true
                                        speed_dialog.displayAutoSpeed = setAutoSpeed
                                        autoSpeedHoldTimer.restart()
                                        local.recordSetAutoSpeed(top_bar.autoSpeedValue, setAutoSpeed)
                                        commonViewModel.modbusAutoSpeedControl(setAutoSpeed);
                                        commonViewModel.autoSpeedStatus = setAutoSpeed;
                                    }
                                }
                            }
                            Rectangle{
                                anchors.verticalCenter: parent.verticalCenter
                                width: 120
                                height: 60
                                color: "#ffffff"
                                border.color: "#999999"
                                Text{
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.verticalCenter: parent.verticalCenter
                                    text: (speed_dialog.displayAutoSpeed >= 0 && speed_dialog.displayAutoSpeed <= 9) ?
                                              top_bar.autoSpeed[speed_dialog.displayAutoSpeed] + "%":""
                                    font.pixelSize: 22
                                    color: "black"
                                }
                            }
                            ImageButton{
                                anchors.verticalCenter: parent.verticalCenter
                                scale: 1.5
                                upImageSource: resourceManager.imagePath + "/36_DIALOG/a360_09.png"
                                downImageSource: resourceManager.imagePath + "/36_DIALOG/a360_08.png"
                                onClicked: {
                                    if (top_bar.autoSpeedValue < 9){
                                        let setAutoSpeed = top_bar.autoSpeedValue + 1
                                        if (setAutoSpeed > 9){
                                            setAutoSpeed = 9;
                                        }
                                        speed_dialog.holdAutoSpeed = true
                                        speed_dialog.displayAutoSpeed = setAutoSpeed
                                        autoSpeedHoldTimer.restart()
                                        local.recordSetAutoSpeed(top_bar.autoSpeedValue, setAutoSpeed)
                                        commonViewModel.modbusAutoSpeedControl(setAutoSpeed);
                                        commonViewModel.autoSpeedStatus = setAutoSpeed;
                                    }
                                }
                            }
                        }
                    }//auto speed
                }
            }//运行?�度

            //
            Item{
                width: stack1.width
                height: stack1.height
                Row{
                    x:25
                    y:40
                    spacing: 4
                    Rectangle{
                        width: 126
                        height: 180
                        color: "#ffffff"
                        border.color: "#999999"
                        Rectangle{
                            width: 80
                            height: 30
                            anchors.horizontalCenter: parent.horizontalCenter
                            y:-15
                            color: "#cccccc"
                            border.color: "#999999"
                            Text {
                                anchors.fill: parent
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                font.pixelSize: 15
                                text: resourceManager.label(56,10,resourceManager.language)
                            }
                        }//title
                        Column{
                            x:10
                            y:20
                            spacing: 10
                            Row{
                                spacing: 5
                                ImageButton{
                                    anchors.verticalCenter: parent.verticalCenter
                                    upImageSource:(speed_dialog.jogMagnificationSetting === Enums.JogMagnification1) ? resourceManager.imagePath + "/36_DIALOG/a360_06.png" : resourceManager.imagePath + "/36_DIALOG/a360_05.png"
                                    downImageSource: resourceManager.imagePath + "/36_DIALOG/a360_06.png"
                                    scale: 2
                                    onClicked: local.updateMagnification(3)
                                }
                                Text{
                                    width: 100
                                    height: 45
                                    font.pixelSize: 20
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignLeft
                                    text: (0.01 * jogratio[0]) + "mm"
                                }
                            }//0.01
                            Row{
                                spacing: 5
                                ImageButton{
                                    anchors.verticalCenter: parent.verticalCenter
                                    upImageSource:(speed_dialog.jogMagnificationSetting === Enums.JogMagnification10) ? resourceManager.imagePath + "/36_DIALOG/a360_06.png" : resourceManager.imagePath + "/36_DIALOG/a360_05.png"
                                    downImageSource: resourceManager.imagePath + "/36_DIALOG/a360_06.png"
                                    scale: 2
                                    onClicked: local.updateMagnification(2)
                                }
                                Text{
                                    width: 100
                                    height: 45
                                    font.pixelSize: 20
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignLeft
                                    text: (0.01 * jogratio[1]) + "mm"
                                }
                            }//0.1
                            Row{
                                spacing: 5
                                ImageButton{
                                    anchors.verticalCenter: parent.verticalCenter
                                    upImageSource:(speed_dialog.jogMagnificationSetting === Enums.JogMagnification100) ? resourceManager.imagePath + "/36_DIALOG/a360_06.png" : resourceManager.imagePath + "/36_DIALOG/a360_05.png"
                                    downImageSource: resourceManager.imagePath + "/36_DIALOG/a360_06.png"
                                    scale: 2
                                    onClicked: local.updateMagnification(1)
                                }
                                Text{
                                    width: 100
                                    height: 45
                                    font.pixelSize: 20
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignLeft
                                    text: (0.01 * jogratio[2]) + "mm"
                                }
                            }//1
                        }//three gears
                    }//jog speed gears

                    Rectangle{
                        width: 220
                        height: 180
                        color: "#ffffff"
                        border.color: "#999999"
                        Rectangle{
                            width: 80
                            height: 30
                            anchors.horizontalCenter: parent.horizontalCenter
                            y:-15
                            color: "#cccccc"
                            border.color: "#999999"
                            Text {
                                anchors.fill: parent
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                font.pixelSize: 15
                                text: resourceManager.label(56,15,resourceManager.language)
                            }
                        }//title

                        Grid{
                            x:10
                            y:20
                            columns: 2
                            rows: 4
                            columnSpacing: 2
                            rowSpacing: -5
                            flow: Grid.TopToBottom

                            Repeater{
                                model: numAxis
                                Row{
                                    spacing: 5
                                    property int axisNum: axisOrder[index]
                                    ImageButton{
                                        anchors.verticalCenter: parent.verticalCenter
                                        upImageSource:currentoperateaxisOrder === parent.axisNum ? resourceManager.imagePath + "/36_DIALOG/a360_06.png" : resourceManager.imagePath + "/36_DIALOG/a360_05.png"
                                        downImageSource: resourceManager.imagePath + "/36_DIALOG/a360_06.png"
                                        scale: 2
                                        onClicked: commonViewModel.modbusSetCurrentOperateAxis(index);
                                    }
                                    Text{
                                        width: 80
                                        height: 45
                                        font.pixelSize: 20
                                        verticalAlignment: Text.AlignVCenter
                                        horizontalAlignment: Text.AlignLeft
                                        color: resourceManager.rgb(22,parent.axisNum)
                                        text:resourceManager.label(22,parent.axisNum,resourceManager.language)
                                    }
                                }
                            }
                        }
                    }//current operate axis

                    Rectangle{
                        width: 126
                        height: 180
                        color: "#ffffff"
                        border.color: "#999999"
                        // ??visible 블록 return 보강 (undefined 방�?)
                        visible: {
                            if (REAL_TARGET) return commonViewModel.userLevel === 4
                            return top_bar.usr_select === 4
                        }
                        Rectangle{
                            width: 80
                            height: 30
                            anchors.horizontalCenter: parent.horizontalCenter
                            y:-15
                            color: "#cccccc"
                            border.color: "#999999"
                            Text {
                                anchors.fill: parent
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                font.pixelSize: 15
                                text: resourceManager.label(56,16,resourceManager.language)
                            }
                        }//title
                        Column{
                            x:10
                            y:20
                            spacing: 14
                            Row{
                                spacing: 5
                                Rectangle{
                                    width: 20
                                    height: 20
                                    anchors.verticalCenter: parent.verticalCenter
                                    border.color: "#999999"
                                    color: (speed_dialog.jogMagnificationSetting === Enums.JogMagnification1)  ? "#cc3333" : "#cccccc"
                                }
                                LimitNumericEditBox {
                                    width: 80
                                    height: 38
                                    fontPixelSize: 18
                                    keyLayoutType: 6
                                    rectRadius: 0
                                    enablePressAndHoldClearKey: true
                                    color:opened ? "#cc3333" : "white"
                                    borderColor: "#999999"
                                    textColor: "black"
                                    value: jogratio[0]
                                    min: "1"
                                    max: "1000"
                                    dialogDisplayX: 200
                                    dialogDisplayY: 160
                                    onEnter: {
                                        let radio = parseInt(newValue)
                                        commonViewModel.modbusSetJogratio(0,radio)
                                    }
                                }
                            }//0.01
                            Row{
                                spacing: 5
                                Rectangle{
                                    width: 20
                                    height: 20
                                    anchors.verticalCenter: parent.verticalCenter
                                    border.color: "#999999"
                                    color: (speed_dialog.jogMagnificationSetting === Enums.JogMagnification10)  ? "#cc3333" : "#cccccc"
                                }
                                LimitNumericEditBox {
                                    width: 80
                                    height: 38
                                    fontPixelSize: 18
                                    keyLayoutType: 6
                                    rectRadius: 0
                                    enablePressAndHoldClearKey: true
                                    color:opened ? "#cc3333" : "white"
                                    borderColor: "#999999"
                                    textColor: "black"
                                    value: jogratio[1]
                                    min: "1"
                                    max: "1000"
                                    dialogDisplayX: 200
                                    dialogDisplayY: 160
                                    onEnter: {
                                        let radio = parseInt(newValue)
                                        commonViewModel.modbusSetJogratio(1,radio)
                                    }
                                }
                            }//0.1
                            Row{
                                spacing: 5
                                Rectangle{
                                    width: 20
                                    height: 20
                                    anchors.verticalCenter: parent.verticalCenter
                                    border.color: "#999999"
                                    color: (speed_dialog.jogMagnificationSetting === Enums.JogMagnification100)  ? "#cc3333" : "#cccccc"
                                }
                                LimitNumericEditBox {
                                    width: 80
                                    height: 38
                                    fontPixelSize: 18
                                    keyLayoutType: 6
                                    rectRadius: 0
                                    enablePressAndHoldClearKey: true
                                    color:opened ? "#cc3333" : "white"
                                    borderColor: "#999999"
                                    textColor: "black"
                                    value: jogratio[2]
                                    min: "1"
                                    max: "1000"
                                    dialogDisplayX: 200
                                    dialogDisplayY: 160
                                    onEnter: {
                                        let radio = parseInt(newValue)
                                        commonViewModel.modbusSetJogratio(2,radio)
                                    }
                                }
                            }//1
                        }//three rates
                    }//jog rate

                    Rectangle{
                        width: 126
                        height: 180
                        color: "#ffffff"
                        border.color: "#999999"
                        // ??visible 블록 return 보강 (undefined 방�?)
                        visible: {
                            if (REAL_TARGET) return commonViewModel.userLevel === 4
                            return top_bar.usr_select === 4
                        }
                        Rectangle{
                            width: 80
                            height: 30
                            anchors.horizontalCenter: parent.horizontalCenter
                            y:-15
                            color: "#cccccc"
                            border.color: "#999999"
                            Text {
                                anchors.fill: parent
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                font.pixelSize: 15
                                text: resourceManager.label(56,17,resourceManager.language)
                            }
                        }//title
                        Column{
                            x:10
                            y:20
                            spacing: 14
                            Row{
                                spacing: 5
                                Rectangle{
                                    width: 20
                                    height: 20
                                    anchors.verticalCenter: parent.verticalCenter
                                    border.color: "#999999"
                                    color: (speed_dialog.jogMagnificationSetting === Enums.JogMagnification1)  ? "#cc3333" : "#cccccc"
                                }
                                LimitNumericEditBox {
                                    width: 80
                                    height: 38
                                    fontPixelSize: 18
                                    keyLayoutType: 6
                                    rectRadius: 0
                                    enablePressAndHoldClearKey: true
                                    color:opened ? "#cc3333" : "white"
                                    borderColor: "#999999"
                                    textColor: "black"
                                    value: joglimitspeed[0]
                                    min: "1"
                                    max: "1000"
                                    dialogDisplayX: 200
                                    dialogDisplayY: 160
                                    onEnter: {
                                        let speed = parseInt(newValue)
                                        commonViewModel.modbusSetJoglimitspeed(0,speed)
                                    }
                                }
                            }//0.01
                            Row{
                                spacing: 5
                                Rectangle{
                                    width: 20
                                    height: 20
                                    anchors.verticalCenter: parent.verticalCenter
                                    border.color: "#999999"
                                    color: (speed_dialog.jogMagnificationSetting === Enums.JogMagnification10)  ? "#cc3333" : "#cccccc"
                                }
                                LimitNumericEditBox {
                                    width: 80
                                    height: 38
                                    fontPixelSize: 18
                                    keyLayoutType: 6
                                    rectRadius: 0
                                    enablePressAndHoldClearKey: true
                                    color:opened ? "#cc3333" : "white"
                                    borderColor: "#999999"
                                    textColor: "black"
                                    value: joglimitspeed[1]
                                    min: "1"
                                    max: "1000"
                                    dialogDisplayX: 200
                                    dialogDisplayY: 160
                                    onEnter: {
                                        let speed = parseInt(newValue)
                                        commonViewModel.modbusSetJoglimitspeed(1,speed)
                                    }
                                }
                            }//0.1
                            Row{
                                spacing: 5
                                Rectangle{
                                    width: 20
                                    height: 20
                                    anchors.verticalCenter: parent.verticalCenter
                                    border.color: "#999999"
                                    color: (speed_dialog.jogMagnificationSetting === Enums.JogMagnification100)  ? "#cc3333" : "#cccccc"
                                }
                                LimitNumericEditBox {
                                    width: 80
                                    height: 38
                                    fontPixelSize: 18
                                    keyLayoutType: 6
                                    rectRadius: 0
                                    enablePressAndHoldClearKey: true
                                    color:opened ? "#cc3333" : "white"
                                    borderColor: "#999999"
                                    textColor: "black"
                                    value: joglimitspeed[2]
                                    min: "1"
                                    max: "1000"
                                    dialogDisplayX: 200
                                    dialogDisplayY: 160
                                    onEnter: {
                                        let speed = parseInt(newValue)
                                        commonViewModel.modbusSetJoglimitspeed(2,speed)
                                    }
                                }
                            }//1
                        }//three limit speed
                    }//limit speed
                }
            }//?�轮
        }
    }
}
}