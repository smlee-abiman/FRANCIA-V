import QtQuick 2.12
import "../Widget"
import "../Program/PosSet/PosSetUtils.js" as Utils
import Enums 1.0
/*************************************************************************************
* Title: UI/UX of AEK
* File: Axis_operate.qml
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
    id:axis_operate
    width: 1018
    height: 590

    property bool pageActive: false
//    property var pageViewModel: manualViewModel.panelModel[1]
//    property var modepageModel: teachingViewModel.pageModel[0]//模式页面model(伺服释放用)
//    property var axisOrder: commonViewModel.axisOrder//axis No.
//    property int numAxis: axisOrder.length//axis number
//    property var axisDataList: pageViewModel.axisInfo.axisDataList //axis data
//    property bool viewpoint: pageViewModel.viewpoint//robot viewing angle
//    property bool freeOperate: commonViewModel.freeOperate//true:free operate; false:manual operate
//    property bool free: false//axis servo free but visible
//    property int jogenable: commonViewModel.jogEnable //pen_set Jog use(unuse:255,use:1)
//    property var jogspeed: commonViewModel.JogSpeed//Jog速度档位显示
//    property int jogMagnificationSetting: phyKeyManager.jogMagnificationSetting//Jog速度设置(三档)
//    property var jogratio: commonViewModel.jogRatio
//    property bool freeOperateIndex: false//true:free operate; false:manual operate

///********************************************************************************************************/

//    Component.onCompleted: {
//        commonViewModel.modbusFreeControl(false)
//        commonViewModel.modbusJogMagnificationControl(Enums.JogMagnification1)
//    }
//    Component.onDestruction: pageViewModel.deactivate()

//    onPageActiveChanged: {
//        if (pageActive) {
//            pageViewModel.startPageChanging()
//            pageViewModel.activate()
//            if(freeOperateIndex == true){
//                commonViewModel.modbusFreeControl(true)
//            }
//        } else {
//            pageViewModel.deactivate()
//        }
//    }

//    QtObject {
//        id: local
//        property int language: 0
//        property bool dataLoaded: pageViewModel.dataLoaded
//    }
//    Binding {
//        target: local
//        property: "language"
//        value: resourceManager.language
//        when: visible
//    }

///**************************************left area**********************************************************/
//    Rectangle{
//        width: 550
//        height: 520
//        x:6
//        y:-15
//        color: "#E6F6FC"
//        border.color: "#898989"

//        Row{
//            x:356
//            y:4
//            spacing: 10
//            Rectangle{
//                width: 90
//                height: 40
//                color: resourceManager.axis_select === 5 ? "#FAEE00" : "#A3DAFF"
//                border.width: 2
//                border.color: "#9DA3A4"
//                Text{
//                    anchors.fill: parent
//                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignHCenter
//                    font.pixelSize: 18

//                    text: resourceManager.label(20, 19, local.language)
//                }
//                MouseArea{
//                    anchors.fill: parent
//                    onClicked: {
//                        resourceManager.axis_select = 5
//                        phyKeyManager.ctrlBuzzer()
//                    }
//                }
//            }
//            Rectangle{
//                width: 90
//                height: 40
//                color: resourceManager.axis_select === 6 ? "#FAEE00" : "#A3DAFF"
//                border.width: 2
//                border.color: "#9DA3A4"
//                Text{
//                    anchors.fill: parent
//                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignHCenter
//                    font.pixelSize: 18

//                    text: resourceManager.label(20, 20, local.language)
//                }
//                MouseArea{
//                    anchors.fill: parent
//                    onClicked: {
//                        resourceManager.axis_select = 6
//                        phyKeyManager.ctrlBuzzer()
//                    }
//                }
//            }
//        }//M axis or S axis select

//        Image {
//            anchors.horizontalCenter: parent.horizontalCenter
//            y:50
//            source:{
//                if(!viewpoint) {
//                    if(resourceManager.axis_select===5){
//                        return resourceManager.imagePath + "/06_MANU/d060_04.png"
//                    }else if(resourceManager.axis_select===6){
//                        return resourceManager.imagePath + "/06_MANU/d060_05.png"
//                    }
//                } else {
//                    if(resourceManager.axis_select===5){
//                        return resourceManager.imagePath + "/06_MANU/d060_06.png"
//                    }else if(resourceManager.axis_select===6){
//                        return resourceManager.imagePath + "/06_MANU/d060_07.png"
//                    }
//                }
//            }
//        }//robot image base

//        Rectangle{
//            width: 550
//            height: 420
//            color: "transparent"
//            Row{
//                spacing: 60
//                x: viewpoint ? 25 : 360
//                y: 280
//                ImageButton {
//                    upImageSource: resourceManager.imagePath + "/06_MANU/d060_08.png"
//                    downImageSource: resourceManager.imagePath + "/06_MANU/d060_09.png"
//                    onPressed: viewpoint ? commonViewModel.modbusMemblemSwControl(true, 204) : commonViewModel.modbusMemblemSwControl(true, 203)
//                    onReleased: viewpoint ? commonViewModel.modbusMemblemSwControl(false, 204) : commonViewModel.modbusMemblemSwControl(false, 203)
//                    onClicked: phyKeyManager.ctrlBuzzer()
//                }
//                ImageButton {
//                    upImageSource: resourceManager.imagePath + "/06_MANU/d060_10.png"
//                    downImageSource: resourceManager.imagePath + "/06_MANU/d060_11.png"
//                    onPressed: viewpoint ? commonViewModel.modbusMemblemSwControl(true, 203) : commonViewModel.modbusMemblemSwControl(true, 204)
//                    onReleased: viewpoint ? commonViewModel.modbusMemblemSwControl(false, 203) : commonViewModel.modbusMemblemSwControl(false, 204)
//                    onClicked: phyKeyManager.ctrlBuzzer()
//                }
//            }//Y axis operate

//            Rectangle{
//                width: 130
//                height: 130
//                x:viewpoint ? 370 : 50
//                y:140
//                color: "transparent"
//                visible: resourceManager.axis_select === 5
//                Row{
//                    anchors.centerIn: parent
//                    spacing: 30
//                    ImageButton {
//                        upImageSource: resourceManager.imagePath + "/06_MANU/d060_08.png"
//                        downImageSource: resourceManager.imagePath + "/06_MANU/d060_09.png"
//                        onPressed: viewpoint ? commonViewModel.modbusMemblemSwControl(true, 205) : commonViewModel.modbusMemblemSwControl(true, 208)
//                        onReleased: viewpoint ? commonViewModel.modbusMemblemSwControl(false, 205) : commonViewModel.modbusMemblemSwControl(false, 208)
//                        onClicked: phyKeyManager.ctrlBuzzer()
//                    }
//                    ImageButton {
//                        upImageSource: resourceManager.imagePath + "/06_MANU/d060_10.png"
//                        downImageSource: resourceManager.imagePath + "/06_MANU/d060_11.png"
//                        onPressed: viewpoint ? commonViewModel.modbusMemblemSwControl(true, 208) : commonViewModel.modbusMemblemSwControl(true, 205)
//                        onReleased: viewpoint ? commonViewModel.modbusMemblemSwControl(false, 208) : commonViewModel.modbusMemblemSwControl(false, 205)
//                        onClicked: phyKeyManager.ctrlBuzzer()
//                    }
//                }
//                Column{
//                    anchors.centerIn: parent
//                    spacing: 30
//                    ImageButton {
//                        upImageSource: resourceManager.imagePath + "/06_MANU/d060_12.png"
//                        downImageSource: resourceManager.imagePath + "/06_MANU/d060_13.png"
//                        onPressed: commonViewModel.modbusMemblemSwControl(true, 206)
//                        onReleased: commonViewModel.modbusMemblemSwControl(false, 206)
//                        onClicked: phyKeyManager.ctrlBuzzer()
//                    }
//                    ImageButton {
//                        upImageSource: resourceManager.imagePath + "/06_MANU/d060_14.png"
//                        downImageSource: resourceManager.imagePath + "/06_MANU/d060_15.png"
//                        onPressed: commonViewModel.modbusMemblemSwControl(true, 207)
//                        onReleased: commonViewModel.modbusMemblemSwControl(false, 207)
//                        onClicked: phyKeyManager.ctrlBuzzer()
//                    }
//                }
//            }//M axis operate

//            Rectangle{
//                width: 130
//                height: 130
//                x:viewpoint ? 320 : 100
//                y:80
//                color: "transparent"
//                visible: resourceManager.axis_select === 6
//                Row{
//                    anchors.centerIn: parent
//                    spacing: 30
//                    ImageButton {
//                        upImageSource: resourceManager.imagePath + "/06_MANU/d060_08.png"
//                        downImageSource: resourceManager.imagePath + "/06_MANU/d060_09.png"
//                        onPressed: viewpoint ? commonViewModel.modbusMemblemSwControl(true, 209) : commonViewModel.modbusMemblemSwControl(true, 212)
//                        onReleased: viewpoint ? commonViewModel.modbusMemblemSwControl(false, 209) : commonViewModel.modbusMemblemSwControl(false, 212)
//                        onClicked: phyKeyManager.ctrlBuzzer()
//                    }
//                    ImageButton {
//                        upImageSource: resourceManager.imagePath + "/06_MANU/d060_10.png"
//                        downImageSource: resourceManager.imagePath + "/06_MANU/d060_11.png"
//                        onPressed: viewpoint ? commonViewModel.modbusMemblemSwControl(true, 212) : commonViewModel.modbusMemblemSwControl(true, 209)
//                        onReleased: viewpoint ? commonViewModel.modbusMemblemSwControl(false, 212) : commonViewModel.modbusMemblemSwControl(false, 209)
//                        onClicked: phyKeyManager.ctrlBuzzer()
//                    }
//                }
//                Column{
//                    anchors.centerIn: parent
//                    spacing: 30
//                    ImageButton {
//                        upImageSource: resourceManager.imagePath + "/06_MANU/d060_12.png"
//                        downImageSource: resourceManager.imagePath + "/06_MANU/d060_13.png"
//                        onPressed: commonViewModel.modbusMemblemSwControl(true, 210)
//                        onReleased: commonViewModel.modbusMemblemSwControl(false, 210)
//                        onClicked: phyKeyManager.ctrlBuzzer()
//                    }
//                    ImageButton {
//                        upImageSource: resourceManager.imagePath + "/06_MANU/d060_14.png"
//                        downImageSource: resourceManager.imagePath + "/06_MANU/d060_15.png"
//                        onPressed: commonViewModel.modbusMemblemSwControl(true, 211)
//                        onReleased: commonViewModel.modbusMemblemSwControl(false, 211)
//                        onClicked: phyKeyManager.ctrlBuzzer()
//                    }
//                }
//            }//S axis operate
//        }//axis operate buttons

//        Rectangle{
//            width: 550
//            height: 100
//            anchors.bottom: parent.bottom
//            color: "#E5ECF9"
//            border.color: "#898989"
//            border.width: 1
//            Column{
//                x:8
//                anchors.verticalCenter: parent.verticalCenter
//                anchors.verticalCenterOffset: -7
//                Text{
//                    width: 100
//                    height: 36
//                    horizontalAlignment: Text.AlignHCenter
//                    verticalAlignment: Text.AlignVCenter
//                    font.pixelSize: 21
//                    color: resourceManager.rgb(31,23)
//                    text: resourceManager.label(31, 23, local.language)
//                }
//                Rectangle{
//                    width: 100
//                    height: 46
//                    border.width: 2
//                    radius: 6
//                    color: mouse_view.pressed ?  "#FAEE00" : "#A3DAFF"
//                    border.color: "#898989"
//                    Image {
//                        anchors.centerIn: parent
//                        source: resourceManager.imagePath + "/06_MANU/d062_02.png"
//                    }
//                    MouseArea{
//                        id: mouse_view
//                        anchors.fill: parent
//                        onClicked: {
//                            var reverse = viewpoint ? false : true
//                            pageViewModel.viewPointControl(reverse)
//                            phyKeyManager.ctrlBuzzer()
//                        }
//                    }
//                }
//            }//视角按钮
//            Column{
//                width: 222
//                height: 100
//                x:160
//                anchors.verticalCenter: parent.verticalCenter
//                Text{
//                    width: 222
//                    height: 36
//                    horizontalAlignment: Text.AlignHCenter
//                    verticalAlignment: Text.AlignVCenter
//                    font.pixelSize: 21
//                    color: resourceManager.rgb(31,27)
//                    text: resourceManager.label(31, 27, local.language)
//                }

//                SpeedControl {
//                    id: manualSpeedControl
//                    maxValue: commonViewModel.maxManualSpeed()
//                    value: Utils.getManualSpeedValue(commonViewModel.manualSpeedStatus, commonViewModel.manualSpeedSetting)
//                    valueText: resourceManager.label(31,45 + value, local.language)
//                    valueTextColor: resourceManager.rgb(31,45 + value)
//                    onPlusButtonPressed: {
//                        let currentSpeed = Utils.getManualSpeedValue(commonViewModel.manualSpeedStatus, commonViewModel.manualSpeedSetting)
//                        let speed = Utils.getManualSpeedValue(value, commonViewModel.manualSpeedSetting)
//                        commonViewModel.modbusSpeedControl(speed)
//                        operationRecord.recordSetManualSpeedByManual(currentSpeed, speed)
//                    }
//                    onMinusButtonPressed: {
//                        let currentSpeed = Utils.getManualSpeedValue(commonViewModel.manualSpeedStatus, commonViewModel.manualSpeedSetting)
//                        let speed = Utils.getDecrementedManualSpeedValue(value, commonViewModel.manualSpeedSetting, maxValue)
//                        commonViewModel.modbusSpeedControl(speed)
//                        operationRecord.recordSetManualSpeedByManual(currentSpeed, speed)
//                    }
//                }
//            }//速度调整
//            Column{
//                spacing: 3
//                x:420
//                anchors.verticalCenter: parent.verticalCenter
//                Rectangle{
//                    id:free_operate
//                    width: 110
//                    height: 44
//                    border.width: 2
//                    color: axis_operate.freeOperate ?  "#FAEE00" : "#A3DAFF"
//                    border.color: "#898989"
//                    Text {
//                        anchors.fill: parent
//                        verticalAlignment: Text.AlignVCenter
//                        horizontalAlignment: Text.AlignHCenter
//                        font.pixelSize: 19

//                        color: resourceManager.rgb(31,26)
//                        text: resourceManager.label(31, 26, local.language)
//                    }
//                    MouseArea{
//                        anchors.fill: parent
//                        onClicked: {
//                            if (!axis_operate.freeOperate) {
//                                operationRecord.recordOpeFreeManual(axis_operate.freeOperate, true)
//                                commonViewModel.modbusFreeControl(true)
//                                freeOperateIndex = true
//                            }
//                            phyKeyManager.ctrlBuzzer()
//                        }
//                    }
//                }//自由操作
//                Rectangle{
//                    id:manual_operate
//                    width: 110
//                    height: 44
//                    border.width: 2
//                    color: !axis_operate.freeOperate ?  "#FAEE00" : "#A3DAFF"
//                    border.color: "#898989"
//                    Text {
//                        anchors.fill: parent
//                        horizontalAlignment: Text.AlignHCenter
//                        verticalAlignment: Text.AlignVCenter
//                        font.pixelSize: 19

//                        color: resourceManager.rgb(31,25)
//                        text: resourceManager.label(31, 25, local.language)
//                    }
//                    MouseArea{
//                        anchors.fill: parent
//                        onClicked: {
//                            if (axis_operate.freeOperate) {
//                                operationRecord.recordOpeFreeManual(axis_operate.freeOperate, false)
//                                commonViewModel.modbusFreeControl(false)
//                                freeOperateIndex = false
//                            }
//                            phyKeyManager.ctrlBuzzer()
//                        }
//                    }
//                }//手动操作
//            }// free operate manual operate
//        }//left bottom area
//    }

///**************************************right area*********************************************************/
//    Rectangle{
//        id: freebut
//        x:926
//        y:-58
//        width: 85
//        height: 40
//        border.width: 2
//        visible: modepageModel.mode64State === 1
//        color: free ? "#FAEE00" : "#A3DAFF"
//        border.color: "#898989"
//        Text {
//            anchors.fill: parent
//            verticalAlignment: Text.AlignVCenter
//            horizontalAlignment: Text.AlignHCenter
//            font.pixelSize: 20
//            color: resourceManager.rgb(31,19)
//            text: resourceManager.label(31, 19, local.language)
//        }
//        MouseArea{
//            anchors.fill: parent
//            onClicked: {
//                free = !free
//                operationRecord.recordOpeManualFree(free, free)
//                phyKeyManager.ctrlBuzzer()
//            }
//        }
//    }//servo free button

//    Rectangle{
//        width: 452
//        height: 520
//        x:560
//        y:-15
//        color: "#2AB0DD"
//        border.color: "#9DA3A4"
//        Row{
//            spacing: -1
//            Rectangle{
//                width: 67
//                height: 55
//                color: "#156BB3"
//                border.color: "#9DA3A4"
//            }//axis name
//            Rectangle{
//                width: 150
//                height: 55
//                color: "#156BB3"
//                border.color: "#9DA3A4"
//                Row{
//                    Repeater{
//                        model: 3
//                        Text{
//                            width: 50
//                            height: 55
//                            verticalAlignment: Text.AlignVCenter
//                            horizontalAlignment: Text.AlignHCenter

//                            font.pixelSize: 18
//                            color: "white"
//                            text: resourceManager.label(31, 37 + index, local.language)
//                        }
//                    }
//                }
//            }//HOM OVER BM
//            Rectangle{
//                width: 60
//                height: 55
//                color: "#156BB3"
//                border.color: "#9DA3A4"
//                Text{
//                    anchors.fill: parent
//                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignHCenter
//                    font.pixelSize: 18

//                    color: "white"
////                    text: resourceManager.label(30, 16, local.language)
//                    text: resourceManager.label(30, 16, local.language) + "\n" + resourceManager.label(30, 17, local.language)
//                }
//            }//encoder position
//            Rectangle{
//                width: 60
//                height: 55
//                color: "#156BB3"
//                border.color: "#9DA3A4"
//                Text{
//                    anchors.fill: parent
//                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignHCenter
//                    font.pixelSize: 18

//                    color: "white"
//                    text: resourceManager.label(31, 42, local.language)
//                }
//            }//torque
//            Rectangle{
//                width: 60
//                height: 55
//                color: "#156BB3"
//                border.color: "#9DA3A4"
//                Text{
//                    anchors.fill: parent
//                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignHCenter
//                    font.pixelSize: 18

//                    color: "white"
//                    text: resourceManager.label(31, 8, local.language)
//                }
//            }//max
//            Rectangle{
//                width: 60
//                height: 55
//                color: "#156BB3"
//                border.color: "#9DA3A4"
//                Text{
//                    anchors.fill: parent
//                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignHCenter
//                    font.pixelSize: 16

//                    color: "white"
//                    text: resourceManager.label(31, 9, local.language)
//                }
//            }//servo free
//        }//title

//        Column{
//            anchors.horizontalCenter: parent.horizontalCenter
//            y:60
//            spacing: numAxis > 5 ? -1 : 8
//            Repeater{
//                model: numAxis
//                Rectangle{
//                    id: axisDataBase
//                    width: 448
//                    height: 58
//                    color: "transparent"
//                    border.color: "#156BB3"
//                    property int axisorder: axisOrder[index]
//                    Row{
//                        spacing: -1
//                        Rectangle{
//                            width: 65
//                            height: 58
//                            border.color: "#156BB3"
//                            color: "transparent"
//                            Text{
//                                anchors.fill: parent
//                                verticalAlignment: Text.AlignVCenter
//                                horizontalAlignment: Text.AlignLeft
//                                leftPadding: 4
//                                font.pixelSize: 18
//                                color: resourceManager.rgb(22, index)
//                                text: resourceManager.label(22, axisDataBase.axisorder, local.language)
//                            }
//                        }//axis name
//                        Rectangle{
//                            width: 150
//                            height: 58
//                            border.color: "#156BB3"
//                            color: "transparent"
//                            Row{
//                                anchors.centerIn: parent
//                                spacing: 30
//                                Repeater{
//                                    model: 3
//                                    Rectangle{
//                                        width: 20
//                                        height: 20
//                                        radius: 10
//                                        border.width: 2
//                                        border.color: "#336699"
//                                        color:{
//                                            if(index === 0){
//                                               return axisDataList[axisDataBase.axisorder].home ?  "#F0EB4C" : "#003366"
//                                            } else if(index === 1){
//                                                return axisDataList[axisDataBase.axisorder].over ?  "#F0EB4C" : "#003366"
//                                            }  else{
//                                                return axisDataList[axisDataBase.axisorder].brake ?  "#F0EB4C" : "#003366"
//                                            }
//                                        }
//                                    }
//                                }
//                            }
//                        }//HOM OVER BM
//                        Rectangle{
//                            width: 60
//                            height: 58
//                            border.color: "#156BB3"
//                            color: "transparent"
//                            Text{
//                                anchors.fill: parent
//                                verticalAlignment: Text.AlignVCenter
//                                horizontalAlignment: Text.AlignHCenter
//                                font.pixelSize: 17
//                                text: axisDataList[axisDataBase.axisorder].position + "\n" + axisDataList[axisDataBase.axisorder].encoder
////                                text: axisDataList[axisDataBase.axisorder].encoder
//                            }
//                        }//encoder position
//                        Rectangle{
//                            width: 60
//                            height: 58
//                            border.color: "#156BB3"
//                            color: "transparent"
//                            Text{
//                                anchors.fill: parent
//                                verticalAlignment: Text.AlignVCenter
//                                horizontalAlignment: Text.AlignHCenter
//                                font.pixelSize: 17
//                                text: axisDataList[axisDataBase.axisorder].torque
//                            }
//                        }//torque
//                        Rectangle{
//                            width: 60
//                            height: 58
//                            border.color: "#156BB3"
//                            color: "transparent"
//                            Text{
//                                anchors.fill: parent
//                                verticalAlignment: Text.AlignVCenter
//                                horizontalAlignment: Text.AlignHCenter
//                                font.pixelSize: 17
//                                text: pageViewModel.formatMaxPosition(axisDataBase.axisorder, parseInt(axisDataList[axisDataBase.axisorder].maxPosition))
//                            }//MAX
//                        }
//                        Rectangle{
//                            width: 58
//                            height: 58
//                            border.color: "#156BB3"
//                            color: "transparent"
//                            Image {
//                                anchors.centerIn: parent
//                                source: (axisDataList[axisDataBase.axisorder].servo) ? resourceManager.imagePath + "/13_POINT_set/d130_10.png" : resourceManager.imagePath + "/13_POINT_set/d130_09.png"
//                                visible: free && freebut.visible
//                                MouseArea{
//                                    anchors.centerIn: parent
//                                    width: parent.width + 15
//                                    height: parent.height + 15
//                                    onPressed: pageViewModel.modBusServoControl(axisDataBase.axisorder, false)
//                                    onReleased: pageViewModel.modBusServoControl(axisDataBase.axisorder, true)
//                                    onClicked:{
//                                        let currentValue = axisDataList[axisDataBase.axisorder].servo
//                                        operationRecord.recordOpeManualServo(axisDataBase.axisorder, currentValue, !currentValue)
//                                        phyKeyManager.ctrlBuzzer()
//                                    }
//                                }
//                            }//伺服on
//                        }//servo free
//                    }
//                }//axis data base
//            }
//        }//axis data

//        Rectangle{
//            width: 452
//            height: 50
//            anchors.bottom: parent.bottom
//            color: "transparent"
//            border.color: "#9DA3A4"
//            visible: jogenable === 1
//            Rectangle{
//                width: 100
//                height: 50
//                color: "transparent"
//                border.color: "#9DA3A4"
//                Text{
//                    anchors.fill: parent
//                    verticalAlignment: Text.AlignVCenter
//                    horizontalAlignment: Text.AlignHCenter
//                    font.pixelSize: 18

//                    text: resourceManager.label(31, 10, local.language)
//                }
//            }//title
//            Row{
//                x:110
//                anchors.verticalCenter: parent.verticalCenter
//                spacing: 10
//                Repeater{
//                    model: 3
//                    Rectangle{
//                        width: 80
//                        height: 40
//                        border.width: 2
//                        border.color: "#9DA3A4"
//                        visible: {
//                            switch(index){
//                            case 0:
//                                jogspeed[0]
//                                break
//                            case 1:
//                                jogspeed[1]
//                                break
//                            case 2:
//                                jogspeed[2]
//                                break
//                            }
//                        }
//                        color: {
//                            switch(index){
//                            case 0:
//                                axis_operate.jogMagnificationSetting === Enums.JogMagnification1 ? "#FAEE00" : "#A3DAFF"
//                                break
//                            case 1:
//                                axis_operate.jogMagnificationSetting === Enums.JogMagnification10 ? "#FAEE00" : "#A3DAFF"
//                                break
//                            case 2:
//                                axis_operate.jogMagnificationSetting === Enums.JogMagnification100 ? "#FAEE00" : "#A3DAFF"
//                                break
//                            }
//                        }
//                        Text{
//                            anchors.fill: parent
//                            verticalAlignment: Text.AlignVCenter
//                            horizontalAlignment: Text.AlignHCenter
//                            font.pixelSize: 18
//                            text: (0.01 * jogratio[index]) + "mm"
//                            //text: resourceManager.label(31, 20 + index, local.language)
//                        }
//                        MouseArea{
//                            anchors.fill: parent
//                            onClicked: {
//                                switch(index){
//                                case 0:
//                                    commonViewModel.modbusJogMagnificationControl(Enums.JogMagnification1)
//                                    break
//                                case 1:
//                                    commonViewModel.modbusJogMagnificationControl(Enums.JogMagnification10)
//                                    break
//                                case 2:
//                                    commonViewModel.modbusJogMagnificationControl(Enums.JogMagnification100)
//                                    break
//                                }
//                                phyKeyManager.ctrlBuzzer()
//                            }
//                        }
//                    }
//                }
//            }
//        }//jog speed
//    }//axis data [main]
}
