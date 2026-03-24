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
* File: Free_Stack.qml
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
    id:freeStackRoot
    property bool pageActive: false
    property var pageModel
    property var axisDataList: pageModel.axisInfo.axisDataList
    property var pointList: pageModel.pointList
    property int curFreeStackNum: pageModel.curFreePalletizeNum //current free stack number
    property var stagePointNum:[200, 200, 200, 200, 20, 20, 20, 164]
    property int totalPages : 1
    property int curPage : 1
    property var pointName:pageModel.pointNameList
    property var pointNameColor:pageModel.pointNameColorList
    property int maxPageNum:8
    property var axisName: pageModel.axisName
    property var freePalleColorList: pageModel.freePalletizeColorList
    property bool isScrollLinkageStauts:false
    property int limitPages:0
    property int numAxis:axisName.length
    property bool editFromProgEditScreen: pageModel.editFromProgEditScreen
    property bool isManual: commonViewModel.manipulateMode === 1

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

    signal closeButtonClicked()

    function getAxisName(index){
        if (index >= 6){
            return resourceManager.label(22, 5 + (index - 6), local.language)
        } else {
            return resourceManager.label(22, 0 + (index - 1), local.language)
        }
    }
    function getAxisColor(index){
        if (index >= 6){
            return resourceManager.rgb(22, 5 + (index - 6))
        } else {
            if (index >= 1){
                return resourceManager.rgb(22, 0 + (index - 1))
            } else {
                return "#000000";
            }
        }
    }
    function getAxisKeyType(axisIndex, itemIndex){
        let format = pageModel.getAxisInfoDispFormat(1, axisIndex, itemIndex);
        if (format < 0){
            return 0
        } else {
            if (itemIndex === 3 || itemIndex === 5){
                return 6;
            } else {
                return ((format & 0x0F) !== 0) ? 4 : 0;
            }
        }
    }

    Component.onCompleted: {
        freeStackRoot.totalPages =
            (((freeStackRoot.stagePointNum[curFreeStackNum] + 2) % freeStackRoot.maxPageNum) == 0)
                ? ((freeStackRoot.stagePointNum[curFreeStackNum] + 2) / freeStackRoot.maxPageNum)
                : ((freeStackRoot.stagePointNum[curFreeStackNum] + 2) / freeStackRoot.maxPageNum) + 1
        pageModel.setFreePalletizePageInfo(freeStackRoot.curPage, freeStackRoot.totalPages)
    }
    onPageActiveChanged: {
        if (pageActive) {
            pageModel.curTabIndex = 1;
            pageModel.modbusSetFreePalletizePointNumber(curFreeStackNum)
            if (freeStackRoot.curPage === 1){
                pageModel.modbusGetFreePalletizeUpperPagePointPos(freeStackRoot.curPage);
            } else {
                pageModel.modbusGetFreePalletizeUpperPagePointPos(freeStackRoot.curPage)
                pageModel.modbusGetFreePalletizeLowerPagePointPos(freeStackRoot.curPage);
            }
        }else{
            pageModel.curTabIndex = 0
        }
    }
    onEditFromProgEditScreenChanged: {
        if(editFromProgEditScreen){
            stackSelect.currentIndex = curFreeStackNum
        }
    }

/*******************************left top area*****************************************/
    Rectangle{
        width: 548
        height: 99
        x:4
        y:5
        color: "#50C1E9"
        border.color: "#898989"
        Column{
            spacing: -1
            Rectangle{
                width: 157
                height: 50
                color: "transparent"
                border.color: "#898989"
                Text {
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 5
                    font.pixelSize: 20
                    text: resourceManager.label(36,120,local.language)
                    color: resourceManager.rgb(36,120)
                }
            }
            Rectangle{
                width: 157
                height: 50
                color: "transparent"
                border.color: "#898989"
                Text {
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 5
                    font.pixelSize: 20
                    text: resourceManager.label(36,121,local.language)
                    color: resourceManager.rgb(36,121)
                }
            }
        }

        Column{
            x:156
            spacing: -1
            Row{
                spacing: -1
                Repeater{
                    model: 3
                    Rectangle{
                        width: 114
                        height: 50
                        color: "#c1d4f4"
                        border.color: "#898989"
                        Text {
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            font.pixelSize: 20
                            text: freeStackRoot.getAxisName(freeStackRoot.axisName[index])
                            color: "black"
                        }
                    }
                }
            }
            Row{
                spacing: -1
                Repeater{
                    model: 3
                    Rectangle{
                        width: 114
                        height: 50
                        color: "#5BE7C4"
                        border.color: "#898989"
                        Text {
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            font.pixelSize: 20
                            text: axisDataList[freeStackRoot.axisName[index] - 1].position
                        }
                    }
                }
            }
        }
    }

    Component{
        id:listdelegate
        Item {
            width: 750
            height: 50
            Rectangle{
                id:rectPointName
                width: 156
                height: 50
                color: {
                    let pointValue = parseInt(pageModel.freePointValue);
                    if (pointValue > 0){
                        return ((((freeStackRoot.curPage - 1) * 8) + index) >= pointValue + 2) ? "#B5B5B6": "#c1d4f4"
                    } else {
                        let maxFpNum = pageModel.getFreePalletizeFPNumMax(curFreeStackNum)
                        return ((((freeStackRoot.curPage - 1) * 8) + index) >= maxFpNum + 2) ? "#B5B5B6": "#c1d4f4"
                    }
                }
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 4

                    font.pixelSize: 20
                    text:freeStackRoot.pointName[index + ((freeStackRoot.curPage - 1) * freeStackRoot.maxPageNum)]
                    color:freeStackRoot.pointNameColor[index + ((freeStackRoot.curPage - 1) * freeStackRoot.maxPageNum)]
                }
            }
            LimitNumericEditBox{
                id:axis1Value
                anchors.left:rectPointName.right
                anchors.leftMargin:3
                width: 110
                height: 50
                rectRadius: 6
                max:(index === 1) ? "100":"4294967295"
                min:(index === 1) ? "0":"0"
                noLimit:(index === 1) ? false : true
                enabled: (axis1Value.value == " ") ? false : true
                textColor: (curFreeStackNum === 0) ?
                            ((index === 0) ? freeStackRoot.freePalleColorList[3]:
                            ((index === 1) ? freeStackRoot.freePalleColorList[4]:
                                             freeStackRoot.freePalleColorList[5])):
                                             freeStackRoot.freePalleColorList[5]
                value: {
                    let pointValue = parseInt(pageModel.freePointValue);
                    if (pointValue > 0){
                        return (freeStackRoot.curPage >= freeStackRoot.limitPages &&
                                (((pointValue + 2) % freeStackRoot.maxPageNum) != 0 && index >= ((pointValue + 2) % freeStackRoot.maxPageNum))) ? " " : (freeStackRoot.curPage > freeStackRoot.limitPages ? " ":
                                (curFreeStackNum >= 0) ? ((freeStackRoot.curPage === 1) ?
                                    ((index === 0) ? freeStackRoot.axisDataList[freeStackRoot.axisName[0] - 1].correctValue:
                                    ((index === 1) ? freeStackRoot.axisDataList[freeStackRoot.axisName[0] - 1].speed:
                                                    freeStackRoot.pointList[index].positionList[0].valueStr)):
                                                    freeStackRoot.pointList[index].positionList[0].valueStr):
                                                    freeStackRoot.pointList[index].positionList[0].valueStr)
                    } else {
                        let maxFpNum = pageModel.getFreePalletizeFPNumMax(curFreeStackNum)
                        return (freeStackRoot.curPage >= freeStackRoot.limitPages &&
                                (((maxFpNum + 2) % freeStackRoot.maxPageNum) != 0 && index >= ((maxFpNum + 2) % freeStackRoot.maxPageNum))) ? " " : (freeStackRoot.curPage > freeStackRoot.limitPages ? " ":
                                (curFreeStackNum >= 0) ? ((freeStackRoot.curPage === 1) ?
                                    ((index === 0) ? freeStackRoot.axisDataList[freeStackRoot.axisName[0] - 1].correctValue:
                                    ((index === 1) ? freeStackRoot.axisDataList[freeStackRoot.axisName[0] - 1].speed:
                                                    freeStackRoot.pointList[index].positionList[0].valueStr)):
                                                    freeStackRoot.pointList[index].positionList[0].valueStr):
                                                    freeStackRoot.pointList[index].positionList[0].valueStr)
                    }
                }
                color:{
                    let pointValue = parseInt(pageModel.freePointValue);
                    if (pointValue > 0){
                        return ((((freeStackRoot.curPage - 1) * 8) + index) >= pointValue + 2) ? "#b5b5b6" : opened ? "#daff00" : "white"
                    } else {
                        let maxFpNum = pageModel.getFreePalletizeFPNumMax(curFreeStackNum)
                        return ((((freeStackRoot.curPage - 1) * 8) + index) >= maxFpNum + 2) ? "#b5b5b6" : opened ? "#daff00" : "white"
                    }
                }
                borderColor: !enabled ? "#C9CACA" : "#898989"
                fontPixelSize: 21
                keyLayoutType:{
                    if (freeStackRoot.curPage === 1 && index === 0){
                        return freeStackRoot.getAxisKeyType(0, index + 3);
                    } else if (freeStackRoot.curPage === 1 && index === 1){
                        return freeStackRoot.getAxisKeyType(0, index + 3);
                    } else {
                        return freeStackRoot.getAxisKeyType(0, 5);
                    }
                }
                enablePressAndHoldClearKey:{
                    if (freeStackRoot.curPage === 1 && index === 0){
                        return false;
                    } else if (freeStackRoot.curPage === 1 && index === 1){
                        return false;
                    } else {
                        return false;
                    }
                }
                textVisible:pageModel.dataLoaded
                onReady: {
                    let pointNo
                    if (freeStackRoot.curPage === 1 && index === 0){
                        pointNo = pageModel.getPalletizePointNumber(1, curFreeStackNum)
                        axis1Value.max = commonViewModel.formatPointMaxValue(freeStackRoot.axisName[0] - 1, pointNo)
                        axis1Value.min = commonViewModel.formatPointMinValue(freeStackRoot.axisName[0] - 1, pointNo)
                        axis1Value.noLimit = false;
                        if(isManual){
                            isNumberKpad2 = false
                            setDialogPos(470,90,index,0,26,0)
                        }else{
                            isNumberKpad2 = true
                            setDialogPos(270,100,index,0,52,0)
                        }
                    } else if (freeStackRoot.curPage === 1 && index === 1){
                        axis1Value.max = "100"
                        axis1Value.min = "0"
                        axis1Value.noLimit = false;
                        if(isManual){
                            isNumberKpad2 = false
                            setDialogPos(470,90,index,0,26,0)
                        }else{
                            isNumberKpad2 = true
                            numberkpad2LayoutType = 1
                            setDialogPos(270,100,index,0,52,0)
                        }
                    } else {
                        pointNo = pageModel.getPalletizePointNumber(1, curFreeStackNum)
                        let maxValue = commonViewModel.formatPointMaxValue(freeStackRoot.axisName[0] - 1, pointNo)
                        let minValue = commonViewModel.formatPointMinValue(freeStackRoot.axisName[0] - 1, pointNo)
                        console.debug("axis1Value maxValue:"+maxValue, "minValue:"+minValue)
                        axis1Value.max = maxValue
                        axis1Value.min = minValue
                        axis1Value.noLimit = false;
                        if(isManual){
                            isNumberKpad2 = false
                            setDialogPos(470,90,index,0,26,0)
                        }else{
                            isNumberKpad2 = true
                            setDialogPos(270,100,index,0,52,0)
                        }
                    }
                }
                onEnter: {
                    //value = newValue;
                    if (freeStackRoot.curPage === 1 && index === 0){
                        pageModel.modbusSetFreePalletizeCorrectValue(0, freeStackRoot.axisName[0] - 1, parseFloat(newValue))
                    } else if (freeStackRoot.curPage === 1 && index === 1){
                        pageModel.modbusSetFreePalletizeSpeed(0, freeStackRoot.axisName[0] - 1, parseInt(newValue))
                    } else {
                        pageModel.modbusSetFreePalletizePointPos(index, freeStackRoot.axisName[0] - 1, parseInt(newValue))
                    }
                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                }
            }
            LimitNumericEditBox{
                id:axis2Value
                anchors.left:axis1Value.right
                anchors.leftMargin:3
                width: 110
                height: 50
                rectRadius: 6
                max:(index === 1) ? "100":"4294967295"
                min:(index === 1) ? "0":"0"
                noLimit:(index === 1) ? false : true
                enabled: (axis2Value.value == " " || freeStackRoot.numAxis <= 1) ? false : true
                textColor: (curFreeStackNum === 0) ?
                            ((index === 0) ? freeStackRoot.freePalleColorList[3]:
                            ((index === 1) ? freeStackRoot.freePalleColorList[4]:
                                             freeStackRoot.freePalleColorList[5])):
                                             freeStackRoot.freePalleColorList[5]
                value: {
                    let pointValue = parseInt(pageModel.freePointValue);
                    if (pointValue > 0){
                        return (freeStackRoot.numAxis <= 1) ? "":
                                (freeStackRoot.curPage >= freeStackRoot.limitPages &&
                                (((pointValue + 2) % freeStackRoot.maxPageNum) != 0 && index >= ((pointValue + 2) % freeStackRoot.maxPageNum))) ? " " : (freeStackRoot.curPage > freeStackRoot.limitPages ? " ":
                                (curFreeStackNum >= 0) ? ((freeStackRoot.curPage === 1) ?
                                    ((index === 0) ? freeStackRoot.axisDataList[freeStackRoot.axisName[1] - 1].correctValue:
                                    ((index === 1) ? freeStackRoot.axisDataList[freeStackRoot.axisName[1] - 1].speed:
                                                    freeStackRoot.pointList[index].positionList[1].valueStr)):
                                                    freeStackRoot.pointList[index].positionList[1].valueStr):
                                                    freeStackRoot.pointList[index].positionList[1].valueStr)
                    } else {
                        let maxFpNum = pageModel.getFreePalletizeFPNumMax(curFreeStackNum)
                        return (freeStackRoot.numAxis <= 1) ? "":
                                (freeStackRoot.curPage >= freeStackRoot.limitPages &&
                                (((maxFpNum + 2) % freeStackRoot.maxPageNum) != 0 && index >= ((maxFpNum + 2) % freeStackRoot.maxPageNum))) ? " " : (freeStackRoot.curPage > freeStackRoot.limitPages ? " ":
                                (curFreeStackNum >= 0) ? ((freeStackRoot.curPage === 1) ?
                                    ((index === 0) ? freeStackRoot.axisDataList[freeStackRoot.axisName[1] - 1].correctValue:
                                    ((index === 1) ? freeStackRoot.axisDataList[freeStackRoot.axisName[1] - 1].speed:
                                                    freeStackRoot.pointList[index].positionList[1].valueStr)):
                                                    freeStackRoot.pointList[index].positionList[1].valueStr):
                                                    freeStackRoot.pointList[index].positionList[1].valueStr)
                    }
                }
                color:{
                    let pointValue = parseInt(pageModel.freePointValue);
                    if (pointValue > 0){
                        return ((((freeStackRoot.curPage - 1) * 8) + index) >= pointValue + 2) ? "#b5b5b6" : opened ? "#daff00" : "white"
                    } else {
                        let maxFpNum = pageModel.getFreePalletizeFPNumMax(curFreeStackNum)
                        return ((((freeStackRoot.curPage - 1) * 8) + index) >= maxFpNum + 2) ? "#b5b5b6" : opened ? "#daff00" : "white"
                    }
                }
                borderColor: !enabled ? "#C9CACA" : "#898989"
                fontPixelSize: 21
                keyLayoutType:{
                    if (freeStackRoot.curPage === 1 && index === 0){
                        return freeStackRoot.getAxisKeyType(1, index + 3);
                    } else if (freeStackRoot.curPage === 1 && index === 1){
                        return freeStackRoot.getAxisKeyType(1, index + 3);
                    } else {
                        return freeStackRoot.getAxisKeyType(1, 5);
                    }
                }
                enablePressAndHoldClearKey:{
                    if (freeStackRoot.curPage === 1 && index === 0){
                        return false;
                    } else if (freeStackRoot.curPage === 1 && index === 1){
                        return false;
                    } else {
                        return false;
                    }
                }
                textVisible:pageModel.dataLoaded
                onReady: {
                    let pointNo
                    if (freeStackRoot.curPage === 1 && index === 0){
                        pointNo = pageModel.getPalletizePointNumber(1, curFreeStackNum)
                        axis2Value.max = commonViewModel.formatPointMaxValue(freeStackRoot.axisName[1] - 1, pointNo)
                        axis2Value.min = commonViewModel.formatPointMinValue(freeStackRoot.axisName[1] - 1, pointNo)
                        axis2Value.noLimit = false;
                        if(isManual){
                            isNumberKpad2 = false
                            setDialogPos(580,90,index,0,26,0)
                        }else{
                            isNumberKpad2 = true
                            setDialogPos(375,100,index,0,52,0)
                        }
                    } else if (freeStackRoot.curPage === 1 && index === 1){
                        axis2Value.max = "100"
                        axis2Value.min = "0"
                        axis2Value.noLimit = false;
                        if(isManual){
                            isNumberKpad2 = false
                            setDialogPos(580,90,index,0,26,0)
                        }else{
                            isNumberKpad2 = true
                            numberkpad2LayoutType = 1
                            setDialogPos(375,100,index,0,52,0)
                        }
                    } else {
                        pointNo = pageModel.getPalletizePointNumber(1, curFreeStackNum)
                        let maxValue = commonViewModel.formatPointMaxValue(freeStackRoot.axisName[1] - 1, pointNo)
                        let minValue = commonViewModel.formatPointMinValue(freeStackRoot.axisName[1] - 1, pointNo)
                        console.debug("axis2Value maxValue:"+maxValue, "minValue:"+minValue)
                        axis2Value.max = maxValue
                        axis2Value.min = minValue
                        axis2Value.noLimit = false;
                        if(isManual){
                            isNumberKpad2 = false
                            setDialogPos(580,90,index,0,26,0)
                        }else{
                            isNumberKpad2 = true
                            setDialogPos(375,100,index,0,52,0)
                        }
                    }
                }
                onEnter: {
                    //value = newValue;
                    if (freeStackRoot.curPage === 1 && index === 0){
                        pageModel.modbusSetFreePalletizeCorrectValue(1, freeStackRoot.axisName[1] - 1, parseFloat(newValue))
                    } else if (freeStackRoot.curPage === 1 && index === 1){
                        pageModel.modbusSetFreePalletizeSpeed(1, freeStackRoot.axisName[1] - 1, parseInt(newValue))
                    } else {
                        pageModel.modbusSetFreePalletizePointPos(index, freeStackRoot.axisName[1] - 1, parseInt(newValue))
                    }
                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                }
            }
            LimitNumericEditBox{
                id:axis3Value
                anchors.left:axis2Value.right
                anchors.leftMargin:3
                width: 110
                height: 50
                rectRadius: 6
                max:(index === 1) ? "100":"4294967295"
                min:(index === 1) ? "0":"0"
                noLimit:(index === 1) ? false : true
                enabled: (axis3Value.value == " " || freeStackRoot.numAxis <= 2) ? false : true
                textColor: (curFreeStackNum === 0) ?
                            ((index === 0) ? freeStackRoot.freePalleColorList[3]:
                            ((index === 1) ? freeStackRoot.freePalleColorList[4]:
                                             freeStackRoot.freePalleColorList[5])):
                                             freeStackRoot.freePalleColorList[5]
                value: {
                    let pointValue = parseInt(pageModel.freePointValue);
                    if (pointValue > 0){
                        return (freeStackRoot.numAxis <= 2) ? "":
                                (freeStackRoot.curPage >= freeStackRoot.limitPages &&
                                (((pointValue + 2) % freeStackRoot.maxPageNum) != 0 && index >= ((pointValue + 2) % freeStackRoot.maxPageNum))) ? " " : (freeStackRoot.curPage > freeStackRoot.limitPages ? " ":
                                (curFreeStackNum >= 0) ? ((freeStackRoot.curPage === 1) ?
                                    ((index === 0) ? freeStackRoot.axisDataList[freeStackRoot.axisName[2] - 1].correctValue:
                                    ((index === 1) ? freeStackRoot.axisDataList[freeStackRoot.axisName[2] - 1].speed:
                                                    freeStackRoot.pointList[index].positionList[2].valueStr)):
                                                    freeStackRoot.pointList[index].positionList[2].valueStr):
                                                    freeStackRoot.pointList[index].positionList[2].valueStr)
                    } else {
                        let maxFpNum = pageModel.getFreePalletizeFPNumMax(curFreeStackNum)
                        return (freeStackRoot.numAxis <= 2) ? "":
                                (freeStackRoot.curPage >= freeStackRoot.limitPages &&
                                (((maxFpNum + 2) % freeStackRoot.maxPageNum) != 0 && index >= ((maxFpNum + 2) % freeStackRoot.maxPageNum))) ? " " : (freeStackRoot.curPage > freeStackRoot.limitPages ? " ":
                                (curFreeStackNum >= 0) ? ((freeStackRoot.curPage === 1) ?
                                    ((index === 0) ? freeStackRoot.axisDataList[freeStackRoot.axisName[2] - 1].correctValue:
                                    ((index === 1) ? freeStackRoot.axisDataList[freeStackRoot.axisName[2] - 1].speed:
                                                    freeStackRoot.pointList[index].positionList[2].valueStr)):
                                                    freeStackRoot.pointList[index].positionList[2].valueStr):
                                                    freeStackRoot.pointList[index].positionList[2].valueStr)
                    }
                }
                color:{
                    let pointValue = parseInt(pageModel.freePointValue);
                    if (pointValue > 0){
                        return ((((freeStackRoot.curPage - 1) * 8) + index) >= pointValue + 2) ? "#b5b5b6" : opened ? "#daff00" : "white"
                    } else {
                        let maxFpNum = pageModel.getFreePalletizeFPNumMax(curFreeStackNum)
                        return ((((freeStackRoot.curPage - 1) * 8) + index) >= maxFpNum + 2) ? "#b5b5b6" : opened ? "#daff00" : "white"
                    }
                }
                borderColor: !enabled ? "#C9CACA" : "#898989"
                fontPixelSize: 21
                keyLayoutType:{
                    if (freeStackRoot.curPage === 1 && index === 0){
                        return freeStackRoot.getAxisKeyType(2, index + 3);
                    } else if (freeStackRoot.curPage === 1 && index === 1){
                        return freeStackRoot.getAxisKeyType(2, index + 3);
                    } else {
                        return freeStackRoot.getAxisKeyType(2, 5);
                    }
                }
                enablePressAndHoldClearKey:{
                    if (freeStackRoot.curPage === 1 && index === 0){
                        return false;
                    } else if (freeStackRoot.curPage === 1 && index === 1){
                        return false;
                    } else {
                        return false;
                    }
                }
                textVisible:pageModel.dataLoaded
                onReady: {
                    let pointNo
                    if (freeStackRoot.curPage === 1 && index === 0){
                        pointNo = pageModel.getPalletizePointNumber(1, curFreeStackNum)
                        axis3Value.max = commonViewModel.formatPointMaxValue(freeStackRoot.axisName[2] - 1, pointNo)
                        axis3Value.min = commonViewModel.formatPointMinValue(freeStackRoot.axisName[2] - 1, pointNo)
                        axis3Value.noLimit = false;
                        if(isManual){
                            isNumberKpad2 = false
                            setDialogPos(693,90,index,0,26,0)
                        }else{
                            isNumberKpad2 = true
                            setDialogPos(480,100,index,0,52,0)
                        }
                    } else if (freeStackRoot.curPage === 1 && index === 1){
                        axis3Value.max = "100"
                        axis3Value.min = "0"
                        axis3Value.noLimit = false;
                        if(isManual){
                            isNumberKpad2 = false
                            setDialogPos(693,90,index,0,26,0)
                        }else{
                            isNumberKpad2 = true
                            numberkpad2LayoutType = 1
                            setDialogPos(480,100,index,0,52,0)
                        }
                    } else {
                        pointNo = pageModel.getPalletizePointNumber(1, curFreeStackNum)
                        let maxValue = commonViewModel.formatPointMaxValue(freeStackRoot.axisName[2] - 1, pointNo)
                        let minValue = commonViewModel.formatPointMinValue(freeStackRoot.axisName[2] - 1, pointNo)
                        console.debug("axis3Value maxValue:"+maxValue, "minValue:"+minValue)
                        axis3Value.max = maxValue
                        axis3Value.min = minValue
                        axis3Value.noLimit = false;
                        if(isManual){
                            isNumberKpad2 = false
                            setDialogPos(693,90,index,0,26,0)
                        }else{
                            isNumberKpad2 = true
                            setDialogPos(480,100,index,0,52,0)
                        }
                    }
                }
                onEnter: {
                    //value = newValue;
                    if (freeStackRoot.curPage === 1 && index === 0){
                        pageModel.modbusSetFreePalletizeCorrectValue(2, freeStackRoot.axisName[2] - 1, parseFloat(newValue))
                    } else if (freeStackRoot.curPage === 1 && index === 1){
                        pageModel.modbusSetFreePalletizeSpeed(2, freeStackRoot.axisName[2] - 1, parseInt(newValue))
                    } else {
                        pageModel.modbusSetFreePalletizePointPos(index, freeStackRoot.axisName[2] - 1, parseInt(newValue))
                    }
                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                }
            }
        }
    }

    ListView{
        property int pg: freeStackRoot.curPage
        x:4
        y:108
        width: 750
        height: 414
        clip: true
        spacing: 2
        model: 8
        delegate: listdelegate
        interactive: false
        onPgChanged: {
            positionViewAtIndex(pg*8,ListView.Beginning)
        }
        Component.onCompleted: {
            positionViewAtIndex(pg*8,ListView.Beginning)
        }
    }

    ImageScrollBar{
        id: freePalletizeSwitch
        x:505
        y:112
        height: 408
        topButtonPressImage: resourceManager.imagePath + "/14_N_palletize/d140_13.png"
        topButtonReleaseImage: resourceManager.imagePath + "/14_N_palletize/d140_11.png"
        botomButtonPressImage: resourceManager.imagePath + "/14_N_palletize/d140_14.png"
        botomButtonReleaseImage: resourceManager.imagePath + "/14_N_palletize/d140_12.png"
        pageLabal: freeStackRoot.curPage + "/" + freeStackRoot.totalPages
        pageLabelColor: freeStackRoot.freePalleColorList[6]
        onScrollUpClicked: {
            //console.log("org_switch onScrollUpClicked:")
            if (freeStackRoot.curPage > 1) {
                freeStackRoot.curPage--
            } else {
                freeStackRoot.curPage = freeStackRoot.totalPages
            }
            pageModel.setFreePalletizePageInfo(freeStackRoot.curPage, freeStackRoot.totalPages)
            if (freeStackRoot.curPage === 1){
                pageModel.modbusGetFreePalletizeUpperPagePointPos(freeStackRoot.curPage);
            } else {
                pageModel.modbusGetFreePalletizeUpperPagePointPos(freeStackRoot.curPage)
                pageModel.modbusGetFreePalletizeLowerPagePointPos(freeStackRoot.curPage);
            }
        }
        onScrollDownClicked: {
            //console.log("org_switch onScrollDownClicked:")
            if (freeStackRoot.curPage >= freeStackRoot.totalPages) {
                freeStackRoot.curPage = 1
            } else {
                freeStackRoot.curPage++
            }
            pageModel.setFreePalletizePageInfo(freeStackRoot.curPage, freeStackRoot.totalPages)
            if (freeStackRoot.curPage === 1){
                pageModel.modbusGetFreePalletizeUpperPagePointPos(freeStackRoot.curPage);
            } else {
                pageModel.modbusGetFreePalletizeUpperPagePointPos(freeStackRoot.curPage)
                pageModel.modbusGetFreePalletizeLowerPagePointPos(freeStackRoot.curPage);
            }
        }
    }//自由堆叠位置page切换

    Rectangle{
        width: 1
        height: 588
        x:556
        y:-61
        color: "#898989"
    }//左右分隔线

/*********************************右侧*********************************/
    ImageButton {
        x:776
        y:-56
        visible: freeStackRoot.editFromProgEditScreen
        upImageSource: resourceManager.imagePath + "/16_program/a160_03" + ".png"
        downImageSource: resourceManager.imagePath + "/16_program/a160_04" + ".png"
        onClicked: {
            closeButtonClicked()
            phyKeyManager.ctrlBuzzer()
        }
    }

    ColumnLayout{
        x:560
        y:4
        spacing: 2
        Item{
            width: parent.width
            height: 51
            Rectangle{
                width: 110
                height: 50
                color: "#c1d4f4"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 1
                    font.pixelSize: 18

                    color: resourceManager.rgb(37,45)
                    text: resourceManager.label(37,45,local.language)
                }
            }
            StringComboBox_style1{
                id: stackSelect
                width: 155
                height: 48
                x:112
                y:2
                bgRectBorderWidth: 2
                bgPopupBorderWidth:2
                popupDelegateNum: 8
                resourceGroupId: 37
                resourceStartIndex: 33
                bgRectColor: "#c1d4f4"
                bgIndicatorColor: "#8FAADC"
                currentIndex: curFreeStackNum
                onCurrentIndexChanged:{
                    if (freeStackRoot.pageActive === true){
                        if (stackSelect.currentIndex >= 0){
                            freeStackRoot.totalPages =
                                    (((freeStackRoot.stagePointNum[stackSelect.currentIndex] + 2) % freeStackRoot.maxPageNum) == 0)
                                    ? ((freeStackRoot.stagePointNum[stackSelect.currentIndex] + 2) / freeStackRoot.maxPageNum)
                                    : ((freeStackRoot.stagePointNum[stackSelect.currentIndex] + 2) / freeStackRoot.maxPageNum) + 1
                        } else {
                            freeStackRoot.totalPages = 1
                        }
                        freeStackRoot.curPage = 1;
                        pageModel.setFreePalletizePageInfo(1, freeStackRoot.totalPages)
                        pageModel.curFreePalletizeNum = stackSelect.currentIndex
                        commonViewModel.pageChange(Enums.TeachingFreePalletize, curFreeStackNum + 8)
                        pageModel.modbusSetFreePalletizePointNumber(curFreeStackNum)
                        if (freeStackRoot.curPage === 1){
                            pageModel.modbusGetFreePalletizeUpperPagePointPos(freeStackRoot.curPage);
                        } else {
                            pageModel.modbusGetFreePalletizeUpperPagePointPos(freeStackRoot.curPage)
                            pageModel.modbusGetFreePalletizeLowerPagePointPos(freeStackRoot.curPage);
                        }
                        pageModel.modbusGetPointData(-1, 0)
                        pageModel.modbusGetPointValue(curFreeStackNum, 0);
                    }
                }
            }
        }//选择
        Item{
            width: parent.width
            height: 51
            Rectangle{
                width: 110
                height: 50
                color: "#c1d4f4"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 1
                    font.pixelSize: 18

                    text: resourceManager.label(37,46,local.language)
                    color: resourceManager.rgb(37,46)
                }
            }
            LimitNumericEditBox{
                width: 120
                height: 48
                x:112
                y:2
                max:setPointNum.value
                min:"0"
                textColor: freeStackRoot.freePalleColorList[0]
                //value:axisDataList[0].pointCount
                value:pageModel.freePointCount
                fontPixelSize: 21
                keyLayoutType:freeStackRoot.getAxisKeyType(0, 0)
                rectRadius: 0
                borderWidth: 1
                color: opened ? "#daff00" : "white"
                enabled: isManual
                onEnter: {
                    let currentValue = value
                    //value = newValue;
                    pageModel.modbusSetPointData(-1, 0, parseInt(newValue))
                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                    operationRecord.recordSetFreeStackCurrentPoint(curFreeStackNum, currentValue, parseInt(newValue))
                }
            }
        }//当前数量
        Item{
            width: parent.width
            height: 51
            Rectangle{
                width: 110
                height: 50
                color: "#c1d4f4"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 1
                    font.pixelSize: 18

                    text: resourceManager.label(37,47,local.language)
                    color: resourceManager.rgb(37,47)
                }
            }
            LimitNumericEditBox{
                id:setPointNum
                width: 120
                height: 48
                x:112
                y:2
                max:pageModel.getFreePalletizeFPNumMax(curFreeStackNum)
                min:"0"
                textColor: freeStackRoot.freePalleColorList[1]
                value:{
                    let pointValue = parseInt(pageModel.freePointValue);
                    if(pointValue > 0){
                        freeStackRoot.limitPages = ((pointValue + 2) % 8 === 0) ? (pointValue + 2) / 8 : ((pointValue + 2) / 8) + 1
                    }
                    return pageModel.freePointValue
                }
                fontPixelSize: 21
                keyLayoutType:freeStackRoot.getAxisKeyType(0, 1)
                rectRadius: 0
                borderWidth: 1
                color: opened ? "#daff00" : "white"
                enabled: isManual
                onEnter: {
                    let currentValue = value
                    pageModel.modbusSetPointValue(curFreeStackNum, 0, parseInt(newValue))
                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                    freeStackRoot.limitPages = ((parseInt(newValue) + 2) % 8 === 0) ? (parseInt(newValue) + 2) / 8 : ((parseInt(newValue) + 2) / 8) + 1
                    operationRecord.recordSetFreeStackPoint(curFreeStackNum, currentValue, parseInt(newValue))
                }
            }
        }//设定数量
        Item{
            width: parent.width
            height: 51
            Rectangle{
                width: 110
                height: 50
                color: "#c1d4f4"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 1
                    font.pixelSize: 18

                    text: resourceManager.label(37,48,local.language)
                    color: resourceManager.rgb(37,48)
                }
            }
            Rectangle{
                width: 120
                height: 46
                x:112
                anchors.verticalCenter: parent.verticalCenter
                border.width: 2
                radius: 6
                color: "#DAE3F3"
                border.color: "#898989"
                Text{
                    anchors.centerIn: parent
                    font.pixelSize: 20
                    color: resourceManager.rgb(37, 15)
                    text: resourceManager.label(37,15, local.language)
                }
                MouseArea{
                    anchors.fill: parent
                    onPressed: parent.color = "#DAE000"
                    onReleased: parent.color = "#DAE3F3"
                    onClicked: {
                        commonViewModel.modbusMemblemSwControl(true, Enums.PalletizeKeyReset)
                        commonViewModel.modbusMemblemSwControl(false, Enums.PalletizeKeyReset)
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }
        }//堆叠复位
        Item{
            width: parent.width
            height: 51
            Rectangle{
                width: 110
                height: 50
                color: "#c1d4f4"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 1
                    font.pixelSize: 18

                    text: resourceManager.label(37,49,local.language)
                    color: resourceManager.rgb(37,49)
                }
            }
            Rectangle{
                width: 120
                height: 46
                x:112
                anchors.verticalCenter: parent.verticalCenter
                border.width: 2
                radius: 6
                color: "#DAE3F3"
                border.color: "#898989"
                Text{
                    anchors.centerIn: parent
                    font.pixelSize: 20
                    color: resourceManager.rgb(37, 16)
                    text: resourceManager.label(37, 16, local.language)
                }
                MouseArea{
                    anchors.fill: parent
                    onPressed: parent.color = "#DAE000"
                    onReleased: parent.color = "#DAE3F3"
                    onClicked: {
                        commonViewModel.modbusMemblemSwControl(true, Enums.PalletizeKeyAllReset)
                        commonViewModel.modbusMemblemSwControl(false, Enums.PalletizeKeyAllReset)
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }
        }//堆叠全复位
    }

/***********************************************************************/
    Connections {
        target: resourceManager
        onLanguageChanged:{
            pageModel.initPointNameList()
        }
    }
}

