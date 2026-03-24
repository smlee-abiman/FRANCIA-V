import QtQuick 2.12
import QtQml 2.3
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import TeachingViewModel 1.0
import Enums 1.0
import "../../Widget"
import "../../Widget/ComboBox"
/*************************************************************************************
* Title: UI/UX of AEK
* File: Standard_Stack.qml
**************************************************************************************
* Detail
 - Version : 1.0.0
    - design revision project to improve the UI/UX of AEK.
**************************************************************************************
* Format : tietech.aek-master_250519
* Version : 1.0.0
* Date: 2025.07.16
* Worker: SH.HONG
**************************************************************************************/
Item {
    id:stackRoot
    width: parent.width
    height: parent.height

    property bool pageActive: false
    property var pageModel
    property var axisDataList: pageModel.axisInfo.axisDataList
    property var axisOrder: pageModel.axisOrder
    property int axisOrderPattern: pageModel.curAxisOrder
    property int axisCount: 5
    property bool editFromProgEditScreen: pageModel.editFromProgEditScreen
    property int axispage: 0
    property int targetAxisIndex: (stackstyle.currentIndex === 1 || axispage === 1) ? 4 : 3 //(for 降减速/上升减速 max min)
    property bool isManual: commonViewModel.manipulateMode === 1
    property int curStandardStackNum: pageModel.curStandardPalletizeNum //current standard stack number

    signal closeButtonClicked()
    signal stackTypeChangeClicked()

/*************************************************************************************/
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

    onPageActiveChanged: {
        if (pageActive) {
            pageModel.curTabIndex = 0;
//            pageModel.curStandardPalletizeNum = stack2.currentIndex
//            commonViewModel.pageChange(Enums.TeachingStandardPalletize, curStandardStackNum)
            pageModel.modbusSetPalletizePointNumber(curStandardStackNum)
            pageModel.modbusGetAxisOrderValue(curStandardStackNum)
            pageModel.modbusGetStackType(curStandardStackNum)
            for(let i = 0; i < axisCount; i++){
                pageModel.modbusGetPitch(curStandardStackNum, 0, stackRoot.axisOrder[i] - 1)
                pageModel.modbusGetDirection(curStandardStackNum, 0, stackRoot.axisOrder[i] - 1)
            }
        }else{
            if(!pageModel.editFromProgEditScreenIng){
                pageModel.curStandardPalletizeNum = 0
            }
            stack2.currentIndex = 0
        }
    }
    onStackTypeChangeClicked: {
        stack_animator.restartshowStack()
    }
    onEditFromProgEditScreenChanged: {
        if(editFromProgEditScreen){
            stack2.currentIndex = curStandardStackNum
        }
    }

/*************************************************************************************/
    function getAxisName(index){
        if (index >= 6){
            return resourceManager.label(22, 23 + (index - 6), local.language)
        }else{
            return resourceManager.label(22, 18 + (index - 1), local.language)
        }
    }
    function getKeyType(index){
        let format = pageModel.getDecelerateInfoDispFormat(index);
        if (format < 0){
            return 0
        } else {
            return ((format & 0x0F) !== 0) ? 4 : 6
        }
    }
    function getAxisKeyType(axisIndex, itemIndex){
        let format = pageModel.getAxisInfoDispFormat(0, axisIndex, itemIndex);
        if (format < 0){
            return 0
        } else {
            return ((format & 0x0F) !== 0) ? 4 : 0
        }
    }

/*******************************left top area*****************************************/
    Stack_animation{
        id:stack_animator
        y:-60
        width: 426
        height: 280
        pageActive1: pageActive
        xLine: getAxisName(stackRoot.axisOrder[0])
        yLine:((stackstyle.currentIndex == 1) || (axispage == 1)) ? getAxisName(stackRoot.axisOrder[3]) : getAxisName(stackRoot.axisOrder[1])
        zLine:((stackstyle.currentIndex == 1) || (axispage == 1)) ? getAxisName(stackRoot.axisOrder[4]) : getAxisName(stackRoot.axisOrder[2])
        stackType: stackRoot.axisOrderPattern
    }//stack animation
/******************************** top background area**************************************/
//위쪽 백그라운드 shhong
    Rectangle{
        y:-56
        width: 930
        height: 250
        color: "#eaeaea"
        border.color: "#cccccc"
        radius: 10
        z: -1
            Rectangle{
                anchors.top: parent.top
                anchors.topMargin: 10
                anchors.right: parent.right
                anchors.rightMargin: 10
                width: 482
                height: 230
                color: "#ffffff"
                border.color: "#cccccc"
                radius: 10
        }
    }
/********************************right top area**************************************/
    ColumnLayout {
        x:450
        y:-36
        spacing: 4
        //선택
        Item{
            width: parent.width
            height: 40
            Rectangle{
                width: 110
                height: 40
                color: "#cccccc"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: resourceManager.rgb(37, 11)
                    text:  resourceManager.label(37,11, local.language)
                    font.bold: true
                    font.pixelSize: 17
                }
            }
            StringComboBox_style1{
                id: stack2
                width: 175
                height: 40
                x:120
                bgRectBorderWidth: 2
                bgPopupBorderWidth:2
                popupDelegateNum: 8
                resourceGroupId: 37
                resourceStartIndex: 25
                bgRectColor: "white"
                bgIndicatorColor: "#cccccc"
                currentIndex: curStandardStackNum
                onCurrentIndexChanged:{
                    if (stackRoot.pageActive === true){
                        pageModel.curStandardPalletizeNum = stack2.currentIndex
                        commonViewModel.pageChange(Enums.TeachingStandardPalletize, curStandardStackNum)
                        pageModel.modbusSetPalletizePointNumber(curStandardStackNum)
                        pageModel.modbusGetAxisOrderValue(curStandardStackNum);
                        //pageModel.modbusGetStackType(curStandardStackNum)
                        for(let i = 0; i < stackRoot.axisCount; i++){
                            pageModel.modbusGetPointValue(curStandardStackNum, i)
                            pageModel.modbusGetPitch(curStandardStackNum, 0, stackRoot.axisOrder[i] - 1)
                            pageModel.modbusGetDirection(curStandardStackNum, 0, stackRoot.axisOrder[i] - 1)
                        }
                    }
                }
            }
        }//选择
        Item{
            width: parent.width
            height: 40
            Rectangle{
                width: 110
                height: 40
                color: "#cccccc"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 17
                    font.bold: true
                    color: resourceManager.rgb(37, 2)
                    text: resourceManager.label(37,2, local.language)
                }
            }
            Row{
                x:120
                anchors.verticalCenter: parent.verticalCenter
                spacing: 4
                Row{
                    id: stackstyle
                    property int currentIndex: pageModel.stackType[curStandardStackNum]
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 1
                    enabled: isManual
                    visible: false ///delete
                    Rectangle{
                        width: 58
                        height: 40
                        radius: 5
                        color: stackstyle.currentIndex === 0 ? "#cc3333" : "#cccccc"
                        Text{
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            font.pixelSize: 17
                            wrapMode: Text.WordWrap
                            text: resourceManager.label(37, 51, local.language)
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                if(stackstyle.currentIndex !== 0){
                                    axispage = 0
                                    pageModel.modbusSetStackType(curStandardStackNum,0)
                                }
                            }
                        }
                    }//M axis
                    Rectangle{
                        width: 58
                        height: 40
                        radius: 5
                        color: stackstyle.currentIndex === 1 ? "#cc3333" : "#cccccc"
                        Text{
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            font.pixelSize: 17
                            wrapMode: Text.WordWrap
                            text: resourceManager.label(37, 52, local.language)
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                if(stackstyle.currentIndex !== 1){
                                    axispage = 0
                                    pageModel.modbusSetStackType(curStandardStackNum,1)
                                }
                            }
                        }
                    }//S axis
                    Rectangle{
                        width: 58
                        height: 40
                        radius: 5
                        color: stackstyle.currentIndex === 2 ? "#cc3333" : "#cccccc"
                        Text{
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            font.pixelSize: 17
                            wrapMode: Text.WordWrap
                            text: resourceManager.label(37, 53, local.language)
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                if(stackstyle.currentIndex !== 2){
                                    pageModel.modbusSetStackType(curStandardStackNum,2)
                                }
                            }
                        }
                    }//M S axis
                }
                Rectangle{
                    width: 165
                    height: 40
                    color: "#cccccc"
                    border.color: "#999999"
                    enabled: isManual
                    Text{
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        leftPadding: 4
                        font.pixelSize: 16
                        color: "black"
                        text: {
                            switch(stackRoot.axisOrderPattern){
                            case 0:
                                if((stackstyle.currentIndex == 1) || (axispage == 1)){
                                    stackRoot.getAxisName(stackRoot.axisOrder[0]) + "->" + stackRoot.getAxisName(stackRoot.axisOrder[3]) + "->" + stackRoot.getAxisName(stackRoot.axisOrder[4])
                                }else{
                                    stackRoot.getAxisName(stackRoot.axisOrder[0]) + "->" + stackRoot.getAxisName(stackRoot.axisOrder[1]) + "->" + stackRoot.getAxisName(stackRoot.axisOrder[2])
                                }
                                break
                            case 1:
                                if((stackstyle.currentIndex == 1) || (axispage == 1)){
                                    stackRoot.getAxisName(stackRoot.axisOrder[0]) + "->" + stackRoot.getAxisName(stackRoot.axisOrder[4]) + "->" + stackRoot.getAxisName(stackRoot.axisOrder[3])
                                }else{
                                    stackRoot.getAxisName(stackRoot.axisOrder[0]) + "->" + stackRoot.getAxisName(stackRoot.axisOrder[2]) + "->" + stackRoot.getAxisName(stackRoot.axisOrder[1])
                                }
                                break
                            case 2:
                                if((stackstyle.currentIndex == 1) || (axispage == 1)){
                                    stackRoot.getAxisName(stackRoot.axisOrder[4]) + "->" + stackRoot.getAxisName(stackRoot.axisOrder[0]) + "->" + stackRoot.getAxisName(stackRoot.axisOrder[3])
                                }else{
                                    stackRoot.getAxisName(stackRoot.axisOrder[2]) + "->" + stackRoot.getAxisName(stackRoot.axisOrder[0]) + "->" + stackRoot.getAxisName(stackRoot.axisOrder[1])
                                }
                                break
                            case 3:
                                if((stackstyle.currentIndex == 1) || (axispage == 1)){
                                    stackRoot.getAxisName(stackRoot.axisOrder[3]) + "->" + stackRoot.getAxisName(stackRoot.axisOrder[0]) + "->" + stackRoot.getAxisName(stackRoot.axisOrder[4])
                                }else{
                                    stackRoot.getAxisName(stackRoot.axisOrder[1]) + "->" + stackRoot.getAxisName(stackRoot.axisOrder[0]) + "->" + stackRoot.getAxisName(stackRoot.axisOrder[2])
                                }
                                break
                            case 4:
                                if((stackstyle.currentIndex == 1) || (axispage == 1)){
                                    stackRoot.getAxisName(stackRoot.axisOrder[3]) + "->" + stackRoot.getAxisName(stackRoot.axisOrder[4]) + "->" + stackRoot.getAxisName(stackRoot.axisOrder[0])
                                }else{
                                    stackRoot.getAxisName(stackRoot.axisOrder[1]) + "->" + stackRoot.getAxisName(stackRoot.axisOrder[2]) + "->" + stackRoot.getAxisName(stackRoot.axisOrder[0])
                                }
                                break
                            case 5:
                                if((stackstyle.currentIndex == 1) || (axispage == 1)){
                                    stackRoot.getAxisName(stackRoot.axisOrder[4]) + "->" + stackRoot.getAxisName(stackRoot.axisOrder[3]) + "->" + stackRoot.getAxisName(stackRoot.axisOrder[0])
                                }else{
                                    stackRoot.getAxisName(stackRoot.axisOrder[2]) + "->" + stackRoot.getAxisName(stackRoot.axisOrder[1]) + "->" + stackRoot.getAxisName(stackRoot.axisOrder[0])
                                }
                                break
                            default:
                                ""
                                break;
                            }
                        }
                    }
                    MouseArea{
                        anchors.fill: parent
                        onPressed: parent.color = "#cc3333"
                        onReleased:parent.color = "#cccccc"
                        onClicked: {
                            let prevAxisOrderPattern = stackRoot.axisOrderPattern
                            //console.debug("axisOrderPattern:", stackRoot.axisOrderPattern)
                            if(stackRoot.axisOrderPattern < 5 && stackRoot.axisOrderPattern >= 0){
                                stackRoot.axisOrderPattern++
                            }else{
                                stackRoot.axisOrderPattern = 0
                            }
                            pageModel.modbusSetPalletizeNumber(stackRoot.axisOrderPattern, curStandardStackNum)
                            pageModel.recordSetPalletizeNumber(prevAxisOrderPattern, stackRoot.axisOrderPattern, curStandardStackNum)
                            stackTypeChangeClicked()
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }
            }
        }//패킹동작
        Item{
            width: parent.width
            height: 40
            Rectangle{
                width: 110
                height: 40
                color: "#cccccc"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 17
                    font.bold: true
                    color: resourceManager.rgb(37, 12)
                    text: ((stackstyle.currentIndex == 1) || (axispage == 1)) ? resourceManager.label(37,54, local.language) : resourceManager.label(37,12, local.language)
                }
            }
            LimitNumericEditBox{
                id:downDecelerate
                width: 120
                height: 40
                x:120
                min: commonViewModel.formatPointMinValue(stackRoot.targetAxisIndex, 209)
                max: commonViewModel.formatPointMaxValue(stackRoot.targetAxisIndex, 209)
                noLimit:false
                value: ((stackstyle.currentIndex == 1) || (axispage == 1)) ? pageModel.axisSdownDeceleratePos : pageModel.axisMdownDeceleratePos
                textColor: "black"
                fontPixelSize: 22
                keyLayoutType:stackRoot.getKeyType(0)
                rectRadius: 0
                borderWidth: 1
                color: opened ? "#cc3333" : "white"
                onReady: {
                    let minValue = commonViewModel.formatPointMinValue(stackRoot.targetAxisIndex, 209)
                    let maxValue = commonViewModel.formatPointMaxValue(stackRoot.targetAxisIndex, 209)
                    //console.debug("downDecelerate minValue:"+minValue, "maxValue:"+maxValue)
                    downDecelerate.min = minValue
                    downDecelerate.max = maxValue
                }
                onEnter: {
                    let currentPos = value
                    if((stackstyle.currentIndex == 1) || (axispage == 1)){
                      pageModel.modbusSetPointData(209, 2, parseInt(newValue))
                    }else{
                      pageModel.modbusSetPointData(209, 0, parseInt(newValue))
                    }
                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                    operationRecord.recordSetStdStackDownDeceleratePosition(currentPos * 100, parseInt(newValue) * 100)
                }
            }
            Text{
                anchors.bottom: parent.bottom
                x:240
                font.pixelSize: 18
                color: resourceManager.rgb(37, 41)
                text: resourceManager.label(37,41,local.language)
            }
            LimitNumericEditBox{
                width: 120
                height: 40
                x:277
                max: "100"
                min: "0"
                unit: ""
                value: ((stackstyle.currentIndex == 1) || (axispage == 1)) ? pageModel.axisSdownDecelerateSpeed : pageModel.axisMdownDecelerateSpeed
                textColor: "black"
                fontPixelSize: 22
                keyLayoutType:stackRoot.getKeyType(1)
                rectRadius: 0
                borderWidth: 1
                color: opened ? "#cc3333" : "white"
                onEnter: {
                    let currentSpeed = parseInt(value)
                    if((stackstyle.currentIndex == 1) || (axispage == 1)){
                        pageModel.modbusSetPointData(209, 3, parseInt(newValue))
                    }else{
                        pageModel.modbusSetPointData(209, 1, parseInt(newValue))
                    }
                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                    operationRecord.recordSetStdStackDownDecelerateSpeed(currentSpeed, parseInt(newValue))
                }
            }
        }//하강감속
        Item{
            width: parent.width
            height: 40
            Rectangle{
                width: 110
                height: 40
                color: "#cccccc"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    leftPadding: 1
                    font.pixelSize: 17
                    font.bold: true
                    color: resourceManager.rgb(37, 13)
                    text: ((stackstyle.currentIndex == 1) || (axispage == 1)) ? resourceManager.label(37,55, local.language) : resourceManager.label(37,13, local.language)
                }
            }
            LimitNumericEditBox{
                id:upDecelerate
                width: 120
                height: 40
                x:120
                min: commonViewModel.formatPointMinValue(stackRoot.targetAxisIndex, 210)
                max: commonViewModel.formatPointMaxValue(stackRoot.targetAxisIndex, 210)
                noLimit:false
                value: ((stackstyle.currentIndex == 1) || (axispage == 1)) ? pageModel.axisSupDeceleratePos : pageModel.axisMupDeceleratePos
                textColor: "black"
                fontPixelSize: 22
                keyLayoutType:stackRoot.getKeyType(2)
                rectRadius: 0
                borderWidth: 1
                color: opened ? "#cc3333" : "white"
                onEnter: {
                    let currentPos = value
                    if((stackstyle.currentIndex == 1) || (axispage == 1)){
                        pageModel.modbusSetPointData(210, 2, parseInt(newValue))
                    }else{
                        pageModel.modbusSetPointData(210, 0, parseInt(newValue))
                    }
                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                    operationRecord.recordSetStdStackUpDeceleratePosition(currentPos * 100, parseInt(newValue) * 100)
                }
                onReady: {
                    let minValue = commonViewModel.formatPointMinValue(stackRoot.targetAxisIndex, 210)
                    let maxValue = commonViewModel.formatPointMaxValue(stackRoot.targetAxisIndex, 210)
                    //console.debug("upDecelerate minValue:"+minValue, "maxValue:"+maxValue)
                    upDecelerate.min = minValue
                    upDecelerate.max = maxValue
                }
            }
            Text{
                anchors.bottom: parent.bottom
                x:240
                font.pixelSize: 18
                color: resourceManager.rgb(37, 41)
                text: resourceManager.label(37,41,local.language)
            }
            LimitNumericEditBox{
                width: 120
                height: 40
                x:277
                max: "100"
                min: "0"
                unit: ""
                value: ((stackstyle.currentIndex == 1) || (axispage == 1)) ? pageModel.axisSupDecelerateSpeed : pageModel.axisMupDecelerateSpeed
                textColor: "black"
                fontPixelSize: 22
                keyLayoutType:stackRoot.getKeyType(3)
                rectRadius: 0
                borderWidth: 1
                color: opened ? "#cc3333" : "white"
                onEnter: {
                    let currentSpeed = parseInt(value)
                    if((stackstyle.currentIndex == 1) || (axispage == 1)){
                        pageModel.modbusSetPointData(210, 3, parseInt(newValue))
                    }else{
                        pageModel.modbusSetPointData(210, 1, parseInt(newValue))
                    }
                    pageModel.modbusSetPointData(210, 1, parseInt(newValue))
                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                    operationRecord.recordSetStdStackUpDecelerateSpeed(currentSpeed, parseInt(newValue))
                }
            }
        }//상승감속
        Item{
            width: parent.width
            height: 40
            Rectangle{
                width: 110
                height: 40
                color: "#cccccc"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.bold: true
                    font.pixelSize: 17
                    color: resourceManager.rgb(37, 14)
                    text: resourceManager.label(37,14, local.language)
                }
            }
            Rectangle{
                width: 120
                height: 40
                x:120
                anchors.verticalCenter: parent.verticalCenter
                border.width: 2
                radius: 5
                color: "#cccccc"
                border.color: "#999999"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 20
                    color: resourceManager.rgb(37, 15)
                    text: resourceManager.label(37,15, local.language)
                }
                MouseArea{
                    anchors.fill: parent
                    onPressed:{
                        parent.color = "#cc3333"
                        commonViewModel.modbusMemblemSwControl(true, Enums.PalletizeKeyReset)
                    }
                    onReleased:{
                        parent.color = "#cccccc"
                        commonViewModel.modbusMemblemSwControl(false, Enums.PalletizeKeyReset)
                    }
                    onClicked: phyKeyManager.ctrlBuzzer()
                }
            }
            Rectangle{
                width: 120
                height: 40
                x:243
                anchors.verticalCenter: parent.verticalCenter
                border.width: 2
                radius: 5
                color: "#cccccc"
                border.color: "#999999"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 20
                    color: resourceManager.rgb(37, 16)
                    text: resourceManager.label(37,16, local.language)
                }
                MouseArea{
                    anchors.fill: parent
                    onPressed:{
                        parent.color = "#cc3333"
                        commonViewModel.modbusMemblemSwControl(true, Enums.PalletizeKeyAllReset)
                    }
                    onReleased:{
                        parent.color = "#cccccc"
                        commonViewModel.modbusMemblemSwControl(false, Enums.PalletizeKeyAllReset)
                    }
                    onClicked: phyKeyManager.ctrlBuzzer()
                }
            }
        }//패킹
        Item{
            width: parent.width
            height: 40
            opacity: stackstyle.currentIndex === 1 ? 0 : 1
            visible: false ///delete
            Rectangle{
                width: 110
                height: 40
                color: "#cccccc"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 17
                    font.bold: true
                    color: resourceManager.rgb(37, 17)
                    text: resourceManager.label(37,17, local.language)
                }
            }
            Rectangle{
                width: 120
                height: 40
                x:120
                anchors.verticalCenter: parent.verticalCenter
                border.width: 2
                radius: 5
                enabled: isManual
                color:(pageModel.storageMethod === true) ? "#cc3333"  :"#cccccc"
                border.color: "#999999"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: (horizonMouseArea.pressed === true) ? 19 : 20
                    color: resourceManager.rgb(37, 18)
                    text: resourceManager.label(37,18, local.language)
                }
                MouseArea{
                    id:horizonMouseArea
                    anchors.fill: parent
                    onPressed: {
                        //console.debug("horizonMethod onPressed:")
                        commonViewModel.modbusMemblemSwControl(true, Enums.StorageMethodHorizon)
                        parent.color = "#cc3333"
                    }
                    onReleased: {
                        parent.color = "#cccccc"
                        if ((mouse.x < 0 || mouse.x > 118) || (mouse.y < 0 || mouse.y > 45)){
                            //console.debug("horizonMethod onReleased out of range mouse x:"+mouse.x, "mouse y:"+mouse.y)
                            commonViewModel.modbusMemblemSwControl(true, Enums.StorageMethodVertical)
                            commonViewModel.modbusMemblemSwControl(false, Enums.StorageMethodVertical)
                        } else {
                            //console.debug("horizonMethod onReleased mouse x:"+mouse.x, "mouse y:"+mouse.y)
                            commonViewModel.modbusMemblemSwControl(false, Enums.StorageMethodHorizon)
                        }
                    }
                    onClicked: phyKeyManager.ctrlBuzzer()
                }
            }
            Rectangle{
                width: 120
                height: 40
                x:243
                anchors.verticalCenter: parent.verticalCenter
                border.width: 2
                radius: 5
                enabled: isManual
                color:(pageModel.storageMethod === false) ? "#cc3333"  :"#cccccc"
                border.color: "#999999"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: (verticalMouseArea.pressed === true) ? 19 : 20
                    color: resourceManager.rgb(37, 19)
                    text: resourceManager.label(37,19, local.language)
                }
                MouseArea{
                    id:verticalMouseArea
                    anchors.fill: parent
                    onClicked: {
                        commonViewModel.modbusMemblemSwControl(true, Enums.StorageMethodVertical)
                        commonViewModel.modbusMemblemSwControl(false, Enums.StorageMethodVertical)
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }
        }//적재방식
    }
    /******************************** botoom background area**************************************/
    //아래 백그라운드 shhong
        Rectangle{
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 20
            width: 930
            height: 290
            color: "#eaeaea"
            border.color: "#cccccc"
            radius: 10
            z: -1
        }
/**********************************bottom area**************************************/

    Column{
        x:27
        y:230
        spacing: -1
        Rectangle{
            width: 90
            height: 50
            color: "#cccccc"
            border.color: "#ffffff"
            Text{
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                leftPadding: 4
                font.pixelSize: 18
                color: resourceManager.rgb(37, 20)
                text: resourceManager.label(37,20, local.language)
                font.bold: true
            }
        }//축
        Repeater{
            model: 4
            Rectangle{
                width: 90
                height: 50
                color: "#999999"
                border.color: "white"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    leftPadding: 4
                    font.pixelSize: 18
                    font.bold: true
                    color: resourceManager.rgb(37, 21 + index)
                    text: resourceManager.label(37,21 + index, local.language)
                }
            }
        }
    }//左侧标题栏

    Row {
        x:119
        y:230
        spacing:2
        Repeater {
            id:repeaterRect
            model: (stackstyle.currentIndex == 2 && axispage == 1) ? 2 : 3
            Column{
                id:rect_data
                property var axisIndex_main: [0,1,2]
                property var axisIndex_sub: [0,3,4]
                property var axisIndex_ms: [3,4]
                property int axisIndex:(stackstyle.currentIndex == 2 && axispage == 1 ) ? axisIndex_ms[index] : stackstyle.currentIndex == 1 ? axisIndex_sub[index] : axisIndex_main[index]

                spacing: -1
                Rectangle{
                    width: 260
                    height: 50
                    color: axis_mouse.pressed ? "#cc3333" : "#999999"
                    border.color: "#999999"
                    enabled: isManual
                    Text{
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 22
                        text: stackRoot.getAxisName(stackRoot.axisOrder[rect_data.axisIndex])
                        color: "black"
                    }
                    MouseArea{
                        id: axis_mouse
                        anchors.fill: parent
                        onClicked:{
                            pageModel.modbusSetStartPos(curStandardStackNum, rect_data.axisIndex, stackRoot.axisOrder[rect_data.axisIndex] - 1, parseInt(axisDataList[stackRoot.axisOrder[rect_data.axisIndex] - 1].position), parseFloat(axisDataList[stackRoot.axisOrder[rect_data.axisIndex] - 1].position).toFixed(2))
                            phyKeyManager.ctrlBuzzer()
                        }
                    }
                }//axis title
                Rectangle{
                    width: 260
                    height: 50
                    color: "white"
                    border.color: "#cccccc"
                    Row{
                         x:4
                         anchors.verticalCenter: parent.verticalCenter
                         spacing: 2
                         Rectangle{
                             width: 100
                             height: 46
                             radius: 4
                             color: "white"
                             border.color: "#cccccc"
                             Text{
                                 anchors.fill: parent
                                 verticalAlignment: Text.AlignVCenter
                                 horizontalAlignment: Text.AlignHCenter
                                 font.pixelSize: 22
                                 text: (stackRoot.axisOrder[index] <= 0) ? "" : axisDataList[stackRoot.axisOrder[rect_data.axisIndex] - 1].position
                             }
                         }
                         Text{
                             width: 36
                             height: 46
                             verticalAlignment: Text.AlignBottom
                             horizontalAlignment: Text.AlignHCenter
                             leftPadding: 2
                             font.pixelSize: 18
                             color: "white"
                             text: resourceManager.label(37,41,local.language)
                         }
                     }//当前位置
                }//current pos
                Rectangle{
                    width: 260
                    height: 50
                    color: "#ffffff"
                    border.color: "#cccccc"
                    Row{
                        x:4
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 2
                        LimitNumericEditBox{
                            id:startPosEditBox
                            anchors.verticalCenter: parent.verticalCenter
                            width: 100
                            height: 46
                            value: (stackRoot.axisOrder[rect_data.axisIndex] <= 0) ? "" : axisDataList[stackRoot.axisOrder[rect_data.axisIndex] - 1].startPos
                            textColor: "black"
                            fontPixelSize: 22
                            keyLayoutType:6
                            rectRadius: 4
                            borderWidth: 1
                            color: opened ? "#cc3333" : "white"
                            enablePressAndHoldClearKey:true
                            textVisible:pageModel.dataLoaded
                            onReady: {
                                let pointNo = pageModel.getPalletizePointNumber(0, curStandardStackNum)
                                startPosEditBox.max = commonViewModel.formatPointMaxValue(stackRoot.axisOrder[rect_data.axisIndex] - 1, pointNo)
                                startPosEditBox.min = commonViewModel.formatPointMinValue(stackRoot.axisOrder[rect_data.axisIndex] - 1, pointNo)
                                if(isManual){
                                    isNumberKpad2 = false
                                    setDialogPos(190,72,0,index,0,242)
                                }else{
                                    isNumberKpad2 = true
                                    setDialogPos(190,320,0,index,0,242)
                                }
                            }
                            onEnter: {
                                let currentPos = value
                                pageModel.modbusSetStartPos(curStandardStackNum, rect_data.axisIndex, stackRoot.axisOrder[rect_data.axisIndex] - 1, parseInt(newValue), parseFloat(newValue).toFixed(2))
                                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                operationRecord.recordSetStdStackStartPos(curStandardStackNum, stackRoot.axisOrder[rect_data.axisIndex] - 1, currentPos * 100, parseInt(newValue) * 100)
                            }
                        }
                        Text{
                            width: 36
                            height: 50
                            verticalAlignment: Text.AlignBottom
                            horizontalAlignment: Text.AlignHCenter
                            font.pixelSize: 18
                            color: resourceManager.rgb(37, 41)
                            text: resourceManager.label(37,41,local.language)
                        }
                        LimitNumericEditBox{
                            anchors.verticalCenter: parent.verticalCenter
                            width: 100
                            height: 46
                            max: "100"
                            min: "1"
                            value: (stackRoot.axisOrder[rect_data.axisIndex] <= 0) ? "" : axisDataList[stackRoot.axisOrder[rect_data.axisIndex] - 1].startSpeed
                            textColor: "black"
                            fontPixelSize: 22
                            rectRadius: 4
                            borderWidth: 1
                            keyLayoutType:stackRoot.getAxisKeyType(rect_data.axisIndex, 2)
                            textVisible:pageModel.dataLoaded
                            color: opened ? "#cc3333" : "white"
                            onReady: {
                                if(isManual){
                                    isNumberKpad2 = false
                                    setDialogPos(190,72,0,index,0,242)
                                }else{
                                    isNumberKpad2 = true
                                    numberkpad2LayoutType = 1
                                    setDialogPos(190,320,0,index,0,242)
                                }
                            }
                            onEnter: {
                                let currentSpeed = value
                                pageModel.modbusSetStartSpeed(curStandardStackNum, stackRoot.axisOrder[rect_data.axisIndex] - 1, parseInt(newValue))
                                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                operationRecord.recordSetStdStackStartSpeed(curStandardStackNum, stackRoot.axisOrder[rect_data.axisIndex] - 1, currentSpeed, parseInt(newValue))
                            }
                        }
                    }
                }//start pos
                Rectangle{
                    width: 260
                    height: 50
                    color: "#ffffff"
                    border.color: "#cccccc"
                    Row{
                        x:4
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 2
                        LimitNumericEditBox{
                            property var index1: [0,1,2]
                            property var index2: [1,2]
                            property int axisIndex: (stackstyle.currentIndex == 2 && axispage == 1 ) ? index2[index] :index1[index]
                            anchors.verticalCenter: parent.verticalCenter
                            width: 100
                            height: 46
                            min: "0"
                            max: number_all.value
                            textColor: "black"
                            value:(stackRoot.axisOrder[axisIndex] <= 0) ? "" : axisDataList[stackRoot.axisOrder[axisIndex] - 1].pointCount
                            fontPixelSize: 22
                            keyLayoutType:stackRoot.getAxisKeyType(axisIndex, 3)
                            textVisible:pageModel.dataLoaded
                            rectRadius: 4
                            borderWidth: 1
                            color: opened ? "#cc3333" : "white"
                            enabled: isManual
                            onReady: {
                                setDialogPos(190,72,0,index,0,242)
                            }
                            onEnter: {
                                let currentValue = value
                                pageModel.modbusSetPointCount(curStandardStackNum, stackRoot.axisOrder[axisIndex] - (stackRoot.axisOrder[axisIndex] - axisIndex), parseInt(newValue))
                                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                operationRecord.recordSetStdStackCurrentPoint(curStandardStackNum, stackRoot.axisOrder[axisIndex] - (stackRoot.axisOrder[axisIndex] - axisIndex), currentValue, parseInt(newValue))
                            }
                        }
                        Text{
                            width: 36
                            height: 50
                            verticalAlignment: Text.AlignBottom
                            horizontalAlignment: Text.AlignHCenter
                            leftPadding: 2
                            font.pixelSize: 35
                            text: "/"
                        }
                        LimitNumericEditBox{
                            id:number_all
                            property var index1: [0,1,2]
                            property var index2: [1,2]
                            property int axisIndex: (stackstyle.currentIndex == 2 && axispage == 1 ) ? index2[index] :index1[index]
                            anchors.verticalCenter: parent.verticalCenter
                            width: 100
                            height: 46
                            min: "0"
                            max: "256"
                            textColor: "black"
                            value:(stackRoot.axisOrder[axisIndex] <= 0) ? "" : axisDataList[stackRoot.axisOrder[axisIndex] - 1].pointValue
                            fontPixelSize: 22
                            keyLayoutType:stackRoot.getAxisKeyType(axisIndex, 4)
                            textVisible:pageModel.dataLoaded
                            rectRadius: 4
                            borderWidth: 1
                            color: opened ? "#cc3333" : "white"
                            enabled: isManual
                            onReady: {
                                setDialogPos(190,72,0,index,0,242)
                            }
                            onEnter: {
                                let currentValue = value
                                pageModel.modbusSetPointValue(curStandardStackNum, stackRoot.axisOrder[axisIndex] - (stackRoot.axisOrder[axisIndex] - axisIndex), parseInt(newValue))
                                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                operationRecord.recordSetStdStackPoint(curStandardStackNum, stackRoot.axisOrder[axisIndex] - (stackRoot.axisOrder[axisIndex] - axisIndex), currentValue, parseInt(newValue))
                            }
                        }
                    }
                }//number
                Rectangle{
                    width: 260
                    height: 50
                    color: "white"
                    border.color: "#cccccc"
                    Row{
                        x:4
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 70
                        LimitNumericEditBox{
                            id:intervalEditBox
                            width: 100
                            height: 46
                            min:"0"
                            textColor: "black"
                            value:(stackRoot.axisOrder[rect_data.axisIndex] <= 0) ? "" : axisDataList[stackRoot.axisOrder[rect_data.axisIndex] - 1].pitch
                            fontPixelSize: 22
                            keyLayoutType:stackRoot.getAxisKeyType(rect_data.axisIndex, 5)
                            textVisible:pageModel.dataLoaded
                            rectRadius: 4
                            borderWidth: 1
                            color: opened ? "#cc3333" : "white"
                            enabled: isManual
                            onReady: {
                                let pointNo = pageModel.getPalletizePointNumber(0, curStandardStackNum)
                                intervalEditBox.max = commonViewModel.formatPointMaxValue(stackRoot.axisOrder[rect_data.axisIndex] - 1, pointNo)
                                setDialogPos(190,72,0,index,0,242)
                            }
                            onEnter: {
                                let currentValue = value
                                pageModel.modbusSetPitch(curStandardStackNum, rect_data.axisIndex, (stackRoot.axisOrder[rect_data.axisIndex] - 1), parseFloat(newValue))
                                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                            }
                        }
                        Image {
                            id:dirImg
                            anchors.verticalCenter: parent.verticalCenter
                            property int dirImgTopMargin:(stackRoot.axisOrder[rect_data.axisIndex] <= 0) ? 0 : ((axisDataList[stackRoot.axisOrder[rect_data.axisIndex] - 1].direction === true) ? 20 : 6)
                            enabled: isManual
                            scale: 1.5
                            source: {
                                if (stackRoot.axisOrder[index] <= 0){
                                    return resourceManager.imagePath + "/14_N_palletize/a140_02.png"
                                } else {
                                    return (pitchDirMouseArea.pressed === true) ?
                                                ((axisDataList[stackRoot.axisOrder[rect_data.axisIndex] - 1].direction === true) ? resourceManager.imagePath + "/14_N_palletize/a140_04.png" : resourceManager.imagePath + "/14_N_palletize/a140_02.png"):
                                                ((axisDataList[stackRoot.axisOrder[rect_data.axisIndex] - 1].direction === true) ? resourceManager.imagePath + "/14_N_palletize/a140_05.png" : resourceManager.imagePath + "/14_N_palletize/a140_03.png")
                                }
                            }
                            MouseArea{
                                id:pitchDirMouseArea
                                width: parent.width + 10
                                height: parent.height + 10
                                anchors.centerIn: parent
                                onClicked: {
                                    if (axisDataList[stackRoot.axisOrder[rect_data.axisIndex] - 1].direction === true){
                                        pageModel.modbusSetDirection(curStandardStackNum, rect_data.axisIndex, (stackRoot.axisOrder[rect_data.axisIndex] - 1), false);
                                        operationRecord.recordSetStdStackDirection(curStandardStackNum, (stackRoot.axisOrder[rect_data.axisIndex] - 1), true, false)
                                    } else {
                                        pageModel.modbusSetDirection(curStandardStackNum, rect_data.axisIndex, (stackRoot.axisOrder[rect_data.axisIndex] - 1), true);
                                        operationRecord.recordSetStdStackDirection(curStandardStackNum, (stackRoot.axisOrder[rect_data.axisIndex] - 1), false, true)
                                    }
                                    phyKeyManager.ctrlBuzzer()
                                }
                            }
                        }
                    }
                }//interval
            }
        }
    }//右侧三个信息栏[主臂堆叠 副臂堆叠]

    Row{
        x:464
        y:208
        spacing: 1
        visible: stackstyle.currentIndex == 2
        Rectangle{
            width: 130
            height: 1
            anchors.verticalCenter: parent.verticalCenter
            color: "#999999"
        }
        Rectangle{
            width: 60
            height: 23
            radius: 14
            color: "#999999"
            Text{
                anchors.centerIn: parent
                font.pixelSize: 15
                text: resourceManager.label(37,56, local.language)
            }
        }
        Rectangle{
            width: 130
            height: 1
            anchors.verticalCenter: parent.verticalCenter
            color: "#999999"
        }
    }
    Row{
        x:432
        y:208
        spacing: 324
        visible: stackstyle.currentIndex == 2
        Image {
            source: mouse_left.pressed ? resourceManager.imagePath + "/14_N_palletize/d140_18.png" : resourceManager.imagePath + "/14_N_palletize/d140_17.png"
            MouseArea{
                id:mouse_left
                width: parent.width + 20
                height: parent.height + 20
                anchors.centerIn: parent
                onClicked: axispage = 0
            }
        }
        Image {
            source: mouse_right.pressed ? resourceManager.imagePath + "/14_N_palletize/d140_20.png" : resourceManager.imagePath + "/14_N_palletize/d140_19.png"
            MouseArea{
                id:mouse_right
                width: parent.width + 20
                height: parent.height + 20
                anchors.centerIn: parent
                onClicked: axispage = 1
            }
        }
    }

    ImageButton {
        x:776
        y:-39
        visible: stackRoot.editFromProgEditScreen
        scale: 1.5
        upImageSource: resourceManager.imagePath + "/16_program/a160_03.png"
        downImageSource: resourceManager.imagePath + "/16_program/a160_04.png"
        onClicked:{
            closeButtonClicked()
            phyKeyManager.ctrlBuzzer()
        }
    }//program page jump
}
