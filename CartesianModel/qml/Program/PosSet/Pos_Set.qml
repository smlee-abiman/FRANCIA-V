import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import Enums 1.0
import "../../Manual/"
import "../../Widget"
import "../../Widget/ComboBox"
import "PosSetUtils.js" as Utils
/*************************************************************************************
* Title: UI/UX of AEK
* File: Pos_Set.qml
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
    id: posSetPage
    width: parent.width
    height: parent.height
    property bool pageActive: false
    property var pageModel
    property var axisOperateViewmodel: manualViewModel.panelModel[1]
    property var modepageModel: teachingViewModel.pageModel[0]//模式页面model(伺服释放用)
    property var axisDataList: axisOperateViewmodel.axisInfo.axisDataList //axis data
    property var axisOrder: commonViewModel.axisOrder//axis No.
    property int numAxis: axisOrder.length//axis number
    property bool viewpoint: pageModel.viewpoint//robot viewing angle
    property bool free: false
    property var positionList
    property int jogMagnificationSetting: phyKeyManager.jogMagnificationSetting
    property bool posminmaxUpdate: false
    property var pointList: pageModel.pointList
    property int pointSettable: pageModel.getPointSettable(pointList[pos_select.currentIndex].pointNo)
    property bool dataLoaded: pageModel.dataLoaded
    property bool isManual: commonViewModel.manipulateMode === 1
    property var jogspeed: commonViewModel.JogSpeed
    property int jogenable: commonViewModel.jogEnable //pen_set Jog use(unuse:255,use:1)

/********************************************************************************************************/
    onPageActiveChanged: {
        if (pageActive) {
            pointSettable = Qt.binding(function() {
                let currentPointNo = pointList[pos_select.currentIndex].pointNo
                return pageModel.getPointSettable(currentPointNo - 1)
            })
            posminmaxUpdate = true
        }else{
            posminmaxUpdate = false
        }
    }

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

/**************************************left area**********************************************************/
    Rectangle{
        width: 400
        height: 520
        x:2
        y:4
        color: "#E6F6FC"
        border.color: "#898989"

        Rectangle{
            width: 400
            height: 50
            color: "#50C1E9"
            border.color: "#898989"
            StringComboBox_style2{
                id: pos_select
                width: 180
                height: 50
                anchors.centerIn: parent
                bgRectBorderWidth: 1
                bgRectColor: "#7A57D1"
                comboBoxMode: pageModel.pointList
                fontSize: 17
                onCurrentIndexChanged: {
                    if (pos_select.currentIndex >= 0) {
                        posSetPage.positionList = pageModel.pointList[pos_select.currentIndex].positionList
                    }
                }
            }
        }//point select

        Row{
            x:230
            y:55
            spacing: 5
            Rectangle{
                width: 80
                height: 35
                color: resourceManager.axis_select === 5 ? "#FAEE00" : "#A3DAFF"
                border.width: 2
                border.color: "#9DA3A4"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 18

                    text: resourceManager.label(20, 19, local.language)
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        resourceManager.axis_select = 5
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }
            Rectangle{
                width: 80
                height: 35
                color: resourceManager.axis_select === 6 ? "#FAEE00" : "#A3DAFF"
                border.width: 2
                border.color: "#9DA3A4"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 18

                    text: resourceManager.label(20, 20, local.language)
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        resourceManager.axis_select = 6
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }
        }//M axis or S axis select

        Image {
            anchors.horizontalCenter: parent.horizontalCenter
            y:120
            source:{
                if(!viewpoint) {
                    if(resourceManager.axis_select===5){
                        return resourceManager.imagePath + "/13_POINT_set/d130_11.png"
                    }else if(resourceManager.axis_select===6){
                        return resourceManager.imagePath + "/13_POINT_set/d130_12.png"
                    }
                } else {
                    if(resourceManager.axis_select===5){
                        return resourceManager.imagePath + "/13_POINT_set/d130_13.png"
                    }else if(resourceManager.axis_select===6){
                        return resourceManager.imagePath + "/13_POINT_set/d130_14.png"
                    }
                }
            }
        }//robot image base

        Rectangle{
            width: 400
            height: 370
            y:50
            color: "transparent"
            Row{
                spacing: 45
                x: viewpoint ? 25 : 260
                y: 260
                ImageButton {
                    upImageSource: resourceManager.imagePath + "/13_POINT_set/d130_15.png"
                    downImageSource: resourceManager.imagePath + "/13_POINT_set/d130_16.png"
                    onPressed: viewpoint ? commonViewModel.modbusMemblemSwControl(true, 204) : commonViewModel.modbusMemblemSwControl(true, 203)
                    onReleased: viewpoint ? commonViewModel.modbusMemblemSwControl(false, 204) : commonViewModel.modbusMemblemSwControl(false, 203)
                    onClicked: phyKeyManager.ctrlBuzzer()
                }
                ImageButton {
                    upImageSource: resourceManager.imagePath + "/13_POINT_set/d130_17.png"
                    downImageSource: resourceManager.imagePath + "/13_POINT_set/d130_18.png"
                    onPressed: viewpoint ? commonViewModel.modbusMemblemSwControl(true, 203) : commonViewModel.modbusMemblemSwControl(true, 204)
                    onReleased: viewpoint ? commonViewModel.modbusMemblemSwControl(false, 203) : commonViewModel.modbusMemblemSwControl(false, 204)
                    onClicked: phyKeyManager.ctrlBuzzer()
                }
            }//Y axis operate

            Rectangle{
                width: 130
                height: 130
                x:viewpoint ? 255 : 15
                y:120
                color: "transparent"
                visible: resourceManager.axis_select === 5
                Row{
                    anchors.centerIn: parent
                    spacing: 30
                    ImageButton {
                        upImageSource: resourceManager.imagePath + "/13_POINT_set/d130_15.png"
                        downImageSource: resourceManager.imagePath + "/13_POINT_set/d130_16.png"
                        onPressed: viewpoint ? commonViewModel.modbusMemblemSwControl(true, 205) : commonViewModel.modbusMemblemSwControl(true, 208)
                        onReleased: viewpoint ? commonViewModel.modbusMemblemSwControl(false, 205) : commonViewModel.modbusMemblemSwControl(false, 208)
                        onClicked: phyKeyManager.ctrlBuzzer()
                    }
                    ImageButton {
                        upImageSource: resourceManager.imagePath + "/13_POINT_set/d130_17.png"
                        downImageSource: resourceManager.imagePath + "/13_POINT_set/d130_18.png"
                        onPressed: viewpoint ? commonViewModel.modbusMemblemSwControl(true, 208) : commonViewModel.modbusMemblemSwControl(true, 205)
                        onReleased: viewpoint ? commonViewModel.modbusMemblemSwControl(false, 208) : commonViewModel.modbusMemblemSwControl(false, 205)
                        onClicked: phyKeyManager.ctrlBuzzer()
                    }
                }
                Column{
                    anchors.centerIn: parent
                    spacing: 30
                    ImageButton {
                        upImageSource: resourceManager.imagePath + "/13_POINT_set/d130_19.png"
                        downImageSource: resourceManager.imagePath + "/13_POINT_set/d130_20.png"
                        onPressed: commonViewModel.modbusMemblemSwControl(true, 206)
                        onReleased: commonViewModel.modbusMemblemSwControl(false, 206)
                        onClicked: phyKeyManager.ctrlBuzzer()
                    }
                    ImageButton {
                        upImageSource: resourceManager.imagePath + "/13_POINT_set/d130_21.png"
                        downImageSource: resourceManager.imagePath + "/13_POINT_set/d130_22.png"
                        onPressed: commonViewModel.modbusMemblemSwControl(true, 207)
                        onReleased: commonViewModel.modbusMemblemSwControl(false, 207)
                        onClicked: phyKeyManager.ctrlBuzzer()
                    }
                }
            }//M axis operate

            Rectangle{
                width: 130
                height: 130
                x:viewpoint ? 220 : 50
                y:70
                color: "transparent"
                visible: resourceManager.axis_select === 6
                Row{
                    anchors.centerIn: parent
                    spacing: 30
                    ImageButton {
                        upImageSource: resourceManager.imagePath + "/13_POINT_set/d130_15.png"
                        downImageSource: resourceManager.imagePath + "/13_POINT_set/d130_16.png"
                        onPressed: viewpoint ? commonViewModel.modbusMemblemSwControl(true, 209) : commonViewModel.modbusMemblemSwControl(true, 212)
                        onReleased: viewpoint ? commonViewModel.modbusMemblemSwControl(false, 209) : commonViewModel.modbusMemblemSwControl(false, 212)
                        onClicked: phyKeyManager.ctrlBuzzer()
                    }
                    ImageButton {
                        upImageSource: resourceManager.imagePath + "/13_POINT_set/d130_17.png"
                        downImageSource: resourceManager.imagePath + "/13_POINT_set/d130_18.png"
                        onPressed: viewpoint ? commonViewModel.modbusMemblemSwControl(true, 212) : commonViewModel.modbusMemblemSwControl(true, 209)
                        onReleased: viewpoint ? commonViewModel.modbusMemblemSwControl(false, 212) : commonViewModel.modbusMemblemSwControl(false, 209)
                        onClicked: phyKeyManager.ctrlBuzzer()
                    }
                }
                Column{
                    anchors.centerIn: parent
                    spacing: 30
                    ImageButton {
                        upImageSource: resourceManager.imagePath + "/13_POINT_set/d130_19.png"
                        downImageSource: resourceManager.imagePath + "/13_POINT_set/d130_20.png"
                        onPressed: commonViewModel.modbusMemblemSwControl(true, 210)
                        onReleased: commonViewModel.modbusMemblemSwControl(false, 210)
                        onClicked: phyKeyManager.ctrlBuzzer()
                    }
                    ImageButton {
                        upImageSource: resourceManager.imagePath + "/13_POINT_set/d130_21.png"
                        downImageSource: resourceManager.imagePath + "/13_POINT_set/d130_22.png"
                        onPressed: commonViewModel.modbusMemblemSwControl(true, 211)
                        onReleased: commonViewModel.modbusMemblemSwControl(false, 211)
                        onClicked: phyKeyManager.ctrlBuzzer()
                    }
                }
            }//S axis operate
        }//axis operate buttons

        Rectangle{
            width: 400
            height: 100
            anchors.bottom: parent.bottom
            color: "#E5ECF9"
            border.color: "#898989"
            border.width: 1
            Column{
                x:8
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: -7
                spacing: 2
                Text{
                    width: 100
                    height: 36
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 21
                    color: resourceManager.rgb(31,23)
                    text: resourceManager.label(31, 23, local.language)
                }
                Rectangle{
                    width: 100
                    height: 46
                    border.width: 2
                    color: mouse_view.pressed ?  "#FAEE00" : "#A3DAFF"
                    border.color: "#898989"
                    Image {
                        anchors.centerIn: parent
                        source: resourceManager.imagePath + "/06_MANU/d062_02.png"
                    }
                    MouseArea{
                        id: mouse_view
                        anchors.fill: parent
                        onClicked: {
                            var reverse = viewpoint ? false : true
                            axisOperateViewmodel.viewPointControl(reverse)
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }
            }//视角按钮
            Column{
                width: 222
                height: 100
                x:160
                anchors.verticalCenter: parent.verticalCenter
                spacing: 2
                Text{
                    width: 222
                    height: 36
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 21
                    color: resourceManager.rgb(31,27)
                    text: resourceManager.label(31, 27, local.language)
                }

                SpeedControl {
                    id: manualSpeedControl
                    maxValue: commonViewModel.maxManualSpeed()
                    value: Utils.getManualSpeedValue(commonViewModel.manualSpeedStatus, commonViewModel.manualSpeedSetting)
                    valueText: resourceManager.label(31,45 + value, local.language)
                    valueTextColor: resourceManager.rgb(31,45 + value)
                    onPlusButtonPressed: {
                        let currentSpeed = Utils.getManualSpeedValue(commonViewModel.manualSpeedStatus, commonViewModel.manualSpeedSetting)
                        let speed = Utils.getManualSpeedValue(value, commonViewModel.manualSpeedSetting)
                        commonViewModel.modbusSpeedControl(speed)
                        operationRecord.recordSetManualSpeedByManual(currentSpeed, speed)
                    }
                    onMinusButtonPressed: {
                        let currentSpeed = Utils.getManualSpeedValue(commonViewModel.manualSpeedStatus, commonViewModel.manualSpeedSetting)
                        let speed = Utils.getDecrementedManualSpeedValue(value, commonViewModel.manualSpeedSetting, maxValue)
                        commonViewModel.modbusSpeedControl(speed)
                        operationRecord.recordSetManualSpeedByManual(currentSpeed, speed)
                    }
                }
            }//速度调整
        }//left bottom area
    }

/**************************************right area*********************************************************/
    Rectangle{
        id: freebut
        x:926
        y:-58
        width: 85
        height: 40
        border.width: 2
        visible: modepageModel.mode64State === 1
        color: free ? "#FAEE00" : "#A3DAFF"
        border.color: "#898989"
        Text {
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 20
            color: resourceManager.rgb(31,19)
            text: resourceManager.label(31, 19, local.language)
        }
        MouseArea{
            anchors.fill: parent
            onClicked: {
                free = !free
                operationRecord.recordOpeManualFree(free, free)
                phyKeyManager.ctrlBuzzer()
            }
        }
    }//servo free button

    Rectangle{
        width: 422
        height: 520
        x:405
        y:4
        color: "#2AB0DD"
        border.color: "#9DA3A4"

        Row{
            spacing: -1
            Rectangle{
                width: 74
                height: 50
                color: "#156BB3"
                border.color: "#9DA3A4"
            }//axis name
            Rectangle{
                width: 63
                height: 50
                color: "#156BB3"
                border.color: "#9DA3A4"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 17

                    color: "white"
                    text: resourceManager.label(36, 5, local.language)
                }
            }//postion memory
            Rectangle{
                width: 93
                height: 50
                color: "#156BB3"
                border.color: "#9DA3A4"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 18

                    color: "white"
                    text: resourceManager.label(30, 16, local.language) + "\n" + resourceManager.label(30, 17, local.language)
                }
            }//encoder position
            Rectangle{
                width: 133
                height: 50
                color: "#156BB3"
                border.color: "#9DA3A4"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 18

                    color: "white"
                    text: resourceManager.label(36, 8, local.language)
                }
            }//position save value
            Rectangle{
                width: 63
                height: 50
                color: "#156BB3"
                border.color: "#9DA3A4"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 16

                    color: "white"
                    text: resourceManager.label(31, 9, local.language)
                }
            }//servo free
        }//title

        Column{
            anchors.horizontalCenter: parent.horizontalCenter
            y:55
            spacing: numAxis > 5 ? -1 : 8
            Repeater{
                model: numAxis
                Rectangle{
                    id: axisDataBase
                    width: 416
                    height: 58
                    color: "transparent"
                    border.color: "#156BB3"
                    property int axisorder: axisOrder[index]
                    property int currentPointNo: pointList[pos_select.currentIndex].pointNo
                    function formatPosition(pointIndex, axisIndex, position){
                        return pageModel.formatPosition(pointIndex, axisIndex, position)
                    }
                    function updatePosMaxMinValue(){
                        pos_min.text = commonViewModel.formatPointMinValue(axisDataBase.axisorder, axisDataBase.currentPointNo)
                        pos_max.text = commonViewModel.formatPointMaxValue(axisDataBase.axisorder, axisDataBase.currentPointNo)
                    }

                    Row{
                        spacing: -1
                        Rectangle{
                            width: 71
                            height: 58
                            border.color: "#156BB3"
                            color: "transparent"
                            Text{
                                anchors.fill: parent
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignLeft
                                leftPadding: 4
                                font.pixelSize: 18
                                color: resourceManager.rgb(22, index)
                                text: resourceManager.label(22, axisDataBase.axisorder, local.language)
                            }
                        }//axis name
                        Rectangle{
                            width: 63
                            height: 58
                            border.color: "#156BB3"
                            color: "transparent"
                            ImageButton{
                                anchors.centerIn: parent
                                enabled: isManual
                                upImageSource: resourceManager.imagePath + "/13_POINT_set/d130_07.png"
                                downImageSource: resourceManager.imagePath + "/13_POINT_set/d130_08.png"
                                onClicked: pageModel.setPosition(pos_select.currentIndex, axisDataBase.axisorder, parseInt(axisDataList[axisDataBase.axisorder].position))
                            }
                        }//position memory
                        Rectangle{
                            width: 93
                            height: 58
                            border.color: "#156BB3"
                            color: "transparent"
                            Text{
                                anchors.fill: parent
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                font.pixelSize: 17
                                text: axisDataList[axisDataBase.axisorder].position + "\n" + axisDataList[axisDataBase.axisorder].encoder
                            }
                        }//encoder position
                        Rectangle{
                            width: 133
                            height: 58
                            border.color: "#156BB3"
                            color: "transparent"
                            LimitNumericEditBox {
                                property var positionData: posSetPage.positionList[axisDataBase.axisorder] // current point's current axis's data
                                property bool disable: positionData.style === 0 || positionData.style === 255 //can't operate[pen_set style set 0 or 255]
                                property bool isPointSettable: (posSetPage.pointSettable & (0x80 >> axisDataBase.axisorder)) != 0

                                width: 95
                                height: 45
                                anchors.centerIn: parent
                                fontPixelSize: 18
                                keyLayoutType: 6
                                rectRadius: 0
                                enablePressAndHoldClearKey: true
                                enabled: disable ? false : isPointSettable
                                color: !enabled ? "#b5b5b6" : opened ? "#daff00" : "white"
                                borderColor: !enabled ? "#C9CACA" : "#898989"
                                textColor: disable ? "#9FA0A0" : isPointSettable ? "black" : "#FFB000"
                                value: axisDataBase.formatPosition(axisDataBase.currentPointNo - 1, axisDataBase.axisorder, positionData.value)
                                onReady: {
                                    let pointNo = pointList[pos_select.currentIndex].pointNo
                                    max = commonViewModel.formatPointMaxValue(axisDataBase.axisorder, pointNo)
                                    min = commonViewModel.formatPointMinValue(axisDataBase.axisorder, pointNo)
                                    if(isManual){
                                        isNumberKpad2 = false
                                        setDialogPos(520,60,index+1,0,40,0)
                                    }else{
                                        isNumberKpad2 = true
                                        setDialogPos(520,90,index+1,0,70,0)
                                    }
                                }
                                onEnter:posSetPage.setPosition(pos_select.currentIndex, axisDataBase.axisorder, newValue)
                            }//保存值
                        }//position save value
                        Rectangle{
                            width: 60
                            height: 58
                            border.color: "#156BB3"
                            color: "transparent"
                            Image {
                                anchors.centerIn: parent
                                visible: free && posSetPage.dataLoaded && freebut.visible
                                source: axisDataList[axisDataBase.axisorder].servo ? resourceManager.imagePath + "/13_POINT_set/d130_10.png" : resourceManager.imagePath + "/13_POINT_set/d130_09.png"
                                MouseArea{
                                    anchors.centerIn: parent
                                    width: parent.width + 15
                                    height: parent.height + 15
                                    onPressed: axisOperateViewmodel.modBusServoControl(axisDataBase.axisorder, false)
                                    onReleased: axisOperateViewmodel.modBusServoControl(axisDataBase.axisorder, true)
                                    onClicked: phyKeyManager.ctrlBuzzer()
                                }
                            }//伺服释放按钮
                        }//servo free
                    }
                }
            }
        }//axis data base

        Rectangle{
            width: 422
            height: 50
            anchors.bottom: parent.bottom
            color: "transparent"
            border.color: "#9DA3A4"
            visible: jogenable === 1
            Rectangle{
                width: 100
                height: 50
                color: "transparent"
                border.color: "#9DA3A4"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 18

                    text: resourceManager.label(31, 10, local.language)
                }
            }//title
            Row{
                x:110
                anchors.verticalCenter: parent.verticalCenter
                spacing: 10
                Repeater{
                    model: 3
                    Rectangle{
                        width: 80
                        height: 40
                        border.width: 2
                        border.color: "#9DA3A4"
                        visible: {
                            switch(index){
                            case 0:
                                jogspeed[0]
                                break
                            case 1:
                                jogspeed[1]
                                break
                            case 2:
                                jogspeed[2]
                                break
                            }
                        }
                        color: {
                            switch(index){
                            case 0:
                                posSetPage.jogMagnificationSetting === Enums.JogMagnification1 ? "#FAEE00" : "#A3DAFF"
                                break
                            case 1:
                                posSetPage.jogMagnificationSetting === Enums.JogMagnification10 ? "#FAEE00" : "#A3DAFF"
                                break
                            case 2:
                                posSetPage.jogMagnificationSetting === Enums.JogMagnification100 ? "#FAEE00" : "#A3DAFF"
                                break
                            }
                        }
                        Text{
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            font.pixelSize: 20
                            text: resourceManager.label(31, 20 + index, local.language)
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                switch(index){
                                case 0:
                                    commonViewModel.modbusJogMagnificationControl(Enums.JogMagnification1)
                                    break
                                case 1:
                                    commonViewModel.modbusJogMagnificationControl(Enums.JogMagnification10)
                                    break
                                case 2:
                                    commonViewModel.modbusJogMagnificationControl(Enums.JogMagnification100)
                                    break
                                }
                                phyKeyManager.ctrlBuzzer()
                            }
                        }
                    }
                }
            }
        }//jog speed
    }
}
