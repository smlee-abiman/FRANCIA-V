import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import Enums 1.0
import "../../../Components/RemindDialog"
/*************************************************************************************
* Title: UI/UX of AEK
* File: Driver_2.qml
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
    id:root
    width: parent.width
    height: parent.height
    property var funcPageModel:functionViewModel.mechanicalParamView
    property var pageModel:funcPageModel.drvParamView.simpleSetView
    property var axisOrder: commonViewModel.axisOrder
    property int numAxis: axisOrder.length
    property int curAxisIndex:funcPageModel.drvParamView.simpleSetView.currentAxisIndex
    property int page1CurAxisIndex: 0
    property int page2CurAxisIndex: 0
    property bool pageActive:false
    property bool jogScrollOn:false
    property bool fiveAxisMode: root.numAxis <= 5 ? true : false
    property int axisPageNo: 1
    property int maxAxisPageNo: fiveAxisMode ? 1 : 2
    property int page1HoldCurAxisIndex: 0
    property int page2HoldCurAxisIndex: 0
    property var axisName: [
        resourceManager.label(22, 0, resourceManager.language),
        resourceManager.label(22, 1, resourceManager.language),
        resourceManager.label(22, 2, resourceManager.language),
        resourceManager.label(22, 3, resourceManager.language),
        resourceManager.label(22, 4, resourceManager.language),
        resourceManager.label(22, 5, resourceManager.language),
        resourceManager.label(22, 6, resourceManager.language),
        resourceManager.label(22, 7, resourceManager.language)
    ]
    property var axisNameColor: [
        resourceManager.rgb(22, 0),
        resourceManager.rgb(22, 1),
        resourceManager.rgb(22, 2),
        resourceManager.rgb(22, 3),
        resourceManager.rgb(22, 4),
        resourceManager.rgb(22, 5),
        resourceManager.rgb(22, 6),
        resourceManager.rgb(22, 7)
    ]


    function modbusPageRead(pageNum, itemIndex, axisIndex){
        switch(pageNum){
        case 1:
//            pageModel.modbusGetValues(pageNum, itemIndex, axisIndex, true)
            pageModel.modbusGetValues(pageNum, itemIndex, axisIndex, false)
            pageModel.startModbusOneShot();
            break
        case 2:
//            pageModel.modbusGetValues(pageNum, itemIndex, axisIndex, true)
            pageModel.modbusGetValues(pageNum, itemIndex, axisIndex, false)
            pageModel.startModbusOneShot();
            break
        case 3:
//            pageModel.modbusGetValues(pageNum, itemIndex, axisIndex, true)
            pageModel.modbusGetValues(pageNum, itemIndex, axisIndex, false)
            pageModel.startModbusOneShot();
            break
        case 4:
//            pageModel.modbusGetValues(pageNum, itemIndex, axisIndex, true)
            pageModel.modbusGetValues(pageNum, itemIndex, axisIndex, false)
            pageModel.startModbusOneShot();
            break
        default:
            break
        }
    }
    function modbusPageReadHR(pageNum, itemIndex, axisIndex){
        switch(pageNum){
        case 1:
            pageModel.modbusGetValues(pageNum, itemIndex, axisIndex, true)
            pageModel.startModbusOneShot();
            break
        case 2:
            pageModel.modbusGetValues(pageNum, itemIndex, axisIndex, true)
            pageModel.startModbusOneShot();
            break
        case 3:
            pageModel.modbusGetValues(pageNum, itemIndex, axisIndex, true)
            pageModel.startModbusOneShot();
            break
        case 4:
            pageModel.modbusGetValues(pageNum, itemIndex, axisIndex, true)
            pageModel.startModbusOneShot();
            break
        default:
            break
        }
    }
    function modbusPageReadSR(pageNum, itemIndex, axisIndex){
        switch(pageNum){
        case 1:
            pageModel.modbusGetValues(pageNum, itemIndex, axisIndex, false)
            pageModel.startModbusOneShot();
            break
        case 2:
            pageModel.modbusGetValues(pageNum, itemIndex, axisIndex, false)
            pageModel.startModbusOneShot();
            break
        case 3:
            pageModel.modbusGetValues(pageNum, itemIndex, axisIndex, false)
            pageModel.startModbusOneShot();
            break
        case 4:
            pageModel.modbusGetValues(pageNum, itemIndex, axisIndex, false)
            pageModel.startModbusOneShot();
            break
        default:
            break
        }
    }
    function pageInit(pageNum, axisIndex){
        switch(pageNum){
        case 1:
            pageModel.modbusSetPageMode(pageNum, axisIndex, 0)
            break
        case 2:
            pageModel.modbusSetPageMode(pageNum, axisIndex, 0)
            break
        case 3:
            pageModel.modbusSetPageMode(pageNum, axisIndex, 0)
            break
        case 4:
            pageModel.modbusSetPageMode(pageNum, axisIndex, 0)
            break
        default:
            break
        }
    }
    function pageRead(pageNum, axisIndex){
        switch(pageNum){
        case 1:
            pageModel.getPageValues(pageNum, axisIndex, true)
            pageModel.getPageValues(pageNum, axisIndex, false);
            break
        case 2:
            pageModel.getPageValues(pageNum, axisIndex, true)
            pageModel.getPageValues(pageNum, axisIndex, false);
            break
        case 3:
            pageModel.getPageValues(pageNum, axisIndex, true)
            pageModel.getPageValues(pageNum, axisIndex, false);
            break
        case 4:
            pageModel.getPageValues(pageNum, axisIndex, true)
            pageModel.getPageValues(pageNum, axisIndex, false);
            break
        default:
            break
        }
    }
    function nextAxisPage() {
        if (axisPageNo >= maxAxisPageNo) {
            axisPageNo = 1
            root.pageRead(1, root.axisOrder[0]);
        } else {
            axisPageNo++
            root.pageRead(1, (root.numAxis >= 6 ? root.axisOrder[5]: root.axisOrder[0]));
        }
    }
    function axisSelectCtrl(pageNum, axisIndex){
        switch(pageNum){
        case 1:
            pageModel.modbusGetValues(pageNum, 0, axisIndex, true)
            root.modbusPageRead(pageNum, 1, axisIndex)
            root.modbusPageRead(pageNum, 2, axisIndex)
            root.modbusPageRead(pageNum, 3, axisIndex)
            break
        case 2:
            pageModel.modbusGetValues(pageNum, 0, axisIndex, true)
            pageModel.modbusGetValues(pageNum, 1, axisIndex, false)
            pageModel.modbusGetValues(pageNum, 2, axisIndex, false)
            pageModel.modbusGetValues(pageNum, 3, axisIndex, false)
            pageModel.modbusGetValues(pageNum, 4, axisIndex, false)
            pageModel.modbusGetValues(pageNum, 5, axisIndex, false)
            pageModel.modbusGetValues(pageNum, 6, axisIndex, false)
            break
        case 3:
            root.modbusPageRead(pageNum, 1, axisIndex)
            root.modbusPageRead(pageNum, 2, axisIndex)
            root.modbusPageRead(pageNum, 3, axisIndex)
            break
        case 4:
            root.modbusPageRead(pageNum, 1, axisIndex)
            pageModel.modbusGetValues(pageNum, 2, axisIndex, true)
            break
        default:
            break
        }
    }

    function initValues(pageNum, axisPageNum){
        if (axisPageNum === 1){
            switch(pageNum){
            case 1:
                axisPage1DrvParamSimpleData.item.initCalcValue(pageNum)
                break
            case 2:
                break
            case 3:
                axisPage1DrvParamSimpleData.item.initCalcValue(pageNum)
                break
            case 4:
                axisPage1DrvParamSimpleData.item.initCalcValue(pageNum)
                break
            default:
                break
            }
        } else if (axisPageNum === 2){
            switch(pageNum){
            case 1:
                axisPage2DrvParamSimpleData.item.initCalcValue(pageNum)
                break
            case 2:
                break
            case 3:
                axisPage2DrvParamSimpleData.item.initCalcValue(pageNum)
                break
            case 4:
                axisPage2DrvParamSimpleData.item.initCalcValue(pageNum)
                break
            default:
                break
            }
        }
    }

    function axisPageCtrl(page, axis){
        if(page === 1){
            //let paga1TargetAxisIndex = ((root.axisOrder[axis] === 2) ? 3 : (root.axisOrder[axis] === 3) ? 2: root.axisOrder[axis]);
            switch(axis){
            case 0:
//                axisPage1DrvParamSimpleData.jogScrollOn = true
//                axisPage2DrvParamSimpleData.jogScrollOn = false
                axisPage1DrvParamSimpleData.item.axisIndex = root.axisOrder[axis];
                //axisPage1DrvParamSimpleData.item.axisIndex = ((root.axisOrder[axis] === 2) ? 3 : (root.axisOrder[axis] === 3) ? 2: root.axisOrder[axis]);
                //axisPage1DrvParamSimpleData.item.axisIndex = paga1TargetAxisIndex;
                axisPage1DrvParamSimpleData.visible = true
                axisPage2DrvParamSimpleData.visible = false
                break
            case 1:
//                axisPage1DrvParamSimpleData.jogScrollOn = true
//                axisPage2DrvParamSimpleData.jogScrollOn = false
                axisPage1DrvParamSimpleData.item.axisIndex = root.axisOrder[axis];
                //axisPage1DrvParamSimpleData.item.axisIndex = ((root.axisOrder[axis] === 2) ? 3 : (root.axisOrder[axis] === 3) ? 2: root.axisOrder[axis]);
                //axisPage1DrvParamSimpleData.item.axisIndex = paga1TargetAxisIndex;
                axisPage1DrvParamSimpleData.visible = true
                axisPage2DrvParamSimpleData.visible = false
                break
            case 2:
//                axisPage1DrvParamSimpleData.jogScrollOn = true
//                axisPage2DrvParamSimpleData.jogScrollOn = false
                axisPage1DrvParamSimpleData.item.axisIndex = root.axisOrder[axis];
                //axisPage1DrvParamSimpleData.item.axisIndex = ((root.axisOrder[axis] === 2) ? 3 : (root.axisOrder[axis] === 3) ? 2: root.axisOrder[axis]);
                //axisPage1DrvParamSimpleData.item.axisIndex = paga1TargetAxisIndex;
                axisPage1DrvParamSimpleData.visible = true
                axisPage2DrvParamSimpleData.visible = false
                break
            case 3:
//                axisPage1DrvParamSimpleData.jogScrollOn = true
//                axisPage2DrvParamSimpleData.jogScrollOn = false
                axisPage1DrvParamSimpleData.item.axisIndex = root.axisOrder[axis];
                //axisPage1DrvParamSimpleData.item.axisIndex = ((root.axisOrder[axis] === 2) ? 3 : (root.axisOrder[axis] === 3) ? 2: root.axisOrder[axis]);
                //axisPage1DrvParamSimpleData.item.axisIndex = paga1TargetAxisIndex;
                axisPage1DrvParamSimpleData.visible = true
                axisPage2DrvParamSimpleData.visible = false
                break
            case 4:
//                axisPage1DrvParamSimpleData.jogScrollOn = true
//                axisPage2DrvParamSimpleData.jogScrollOn = false
                axisPage1DrvParamSimpleData.item.axisIndex = root.axisOrder[axis];
                //axisPage1DrvParamSimpleData.item.axisIndex = ((root.axisOrder[axis] === 2) ? 3 : (root.axisOrder[axis] === 3) ? 2: root.axisOrder[axis]);
                //axisPage1DrvParamSimpleData.item.axisIndex = paga1TargetAxisIndex;
                axisPage1DrvParamSimpleData.visible = true
                axisPage2DrvParamSimpleData.visible = false
                break
            default:
                break
            }
        } else {
            //let paga2TargetAxisIndex = ((root.axisOrder[5+axis] === 2) ? 3 : (root.axisOrder[5+axis] === 3) ? 2: root.axisOrder[5+axis]);
            switch(axis){
            case 0:
//                axisPage1DrvParamSimpleData.jogScrollOn = false
//                axisPage2DrvParamSimpleData.jogScrollOn = true
                axisPage2DrvParamSimpleData.item.axisIndex = root.axisOrder[5+axis]
                //axisPage2DrvParamSimpleData.item.axisIndex = paga2TargetAxisIndex
                axisPage1DrvParamSimpleData.visible = false
                axisPage2DrvParamSimpleData.visible = true
                break
            case 1:
//                axisPage1DrvParamSimpleData.jogScrollOn = false
//                axisPage2DrvParamSimpleData.jogScrollOn = true
                axisPage2DrvParamSimpleData.item.axisIndex = root.axisOrder[5+axis]
                //axisPage2DrvParamSimpleData.item.axisIndex = paga2TargetAxisIndex
                axisPage1DrvParamSimpleData.visible = false
                axisPage2DrvParamSimpleData.visible = true
                break
            case 2:
//                axisPage1DrvParamSimpleData.jogScrollOn = false
//                axisPage2DrvParamSimpleData.jogScrollOn = true
                axisPage2DrvParamSimpleData.item.axisIndex = root.axisOrder[5+axis]
                //axisPage2DrvParamSimpleData.item.axisIndex = paga2TargetAxisIndex
                axisPage1DrvParamSimpleData.visible = false
                axisPage2DrvParamSimpleData.visible = true
                break
            default:
                break
            }
        }
    }
    function getAxisCurpage(page, axisIndex) {
        let curPage = 0;
        if(page === 1){
            switch(axisIndex){
            case 0:
                curPage = axisPage1DrvParamSimpleData.item.axis0CurPage
                break
            case 1:
                curPage = axisPage1DrvParamSimpleData.item.axis1CurPage
                break
            case 2:
                curPage = axisPage1DrvParamSimpleData.item.axis2CurPage
                break
            case 3:
                curPage = axisPage1DrvParamSimpleData.item.axis3CurPage
                break
            case 4:
                curPage = axisPage1DrvParamSimpleData.item.axis4CurPage
                break
            case 5:
                curPage = axisPage1DrvParamSimpleData.item.axis5CurPage
                break
            case 6:
                curPage = axisPage1DrvParamSimpleData.item.axis6CurPage
                break
            case 7:
                curPage = axisPage1DrvParamSimpleData.item.axis7CurPage
                break
            default:
                break
            }
        } else {
            switch(axisIndex){
            case 0:
                curPage = axisPage2DrvParamSimpleData.item.axis0CurPage
                break
            case 1:
                curPage = axisPage2DrvParamSimpleData.item.axis1CurPage
                break
            case 2:
                curPage = axisPage2DrvParamSimpleData.item.axis2CurPage
                break
            case 3:
                curPage = axisPage2DrvParamSimpleData.item.axis3CurPage
                break
            case 4:
                curPage = axisPage2DrvParamSimpleData.item.axis4CurPage
                break
            case 5:
                curPage = axisPage2DrvParamSimpleData.item.axis5CurPage
                break
            case 6:
                curPage = axisPage2DrvParamSimpleData.item.axis6CurPage
                break
            case 7:
                curPage = axisPage2DrvParamSimpleData.item.axis7CurPage
                break
            default:
                break
            }
        }
        return curPage;
    }
    function initCalcUpDownbuttonStatus(page){
        if(page === 1){
            axisPage1DrvParamSimpleData.item.initCalcUpDownbtnStatus()
        } else {
            axisPage2DrvParamSimpleData.item.initCalcUpDownbtnStatus()
        }
    }

    Component.onDestruction: pageModel.deactivate()

    onPageActiveChanged: {
        if (pageActive) {
            if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                pageModel.startAxisChanging()
                let axisIndex = (root.axisOrder[0] === 3) ? 2 : ((root.axisOrder[0] === 2) ? 3 : root.axisOrder[0])
                //pageModel.setCurAxisInfo(root.axisOrder[0]);
                pageModel.setCurAxisInfo(root.curAxisIndex);
            }
            pageModel.startPageChanging()
            commonViewModel.pageChange(Enums.FunctionMPDrvParamSimpleSet)
            pageModel.activate()
            //root.pageInit(1, 0)
            root.modbusPageRead(1, 1, root.axisOrder[0])
            root.modbusPageRead(1, 2, root.axisOrder[0])
            root.modbusPageRead(1, 3, root.axisOrder[0]);
        } else {
            pageModel.deactivate()
        }
    }

    Rectangle{
        width: 932
        height: 60
        border.color: "#999999"
        color: "#ffffff"
        Text{
            width: 150
            height: 56
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
            leftPadding: 20
            font.pixelSize: 22
            font.bold: true
            color: resourceManager.rgb(43,206)
            text: resourceManager.label(43,206,resourceManager.language)
        }//标题
        Row{
            spacing: 4
            x:170
            anchors.verticalCenter: parent.verticalCenter
            Rectangle{
                visible:(root.axisPageNo === 1) && ((root.numAxis < 5 && (root.numAxis % 5) >= 1) || (root.numAxis >= 5))
                width: 138
                height: 50
                color: (page1CurAxisIndex === 0) ? "#cc3333" : "#cccccc"
                border.color: "#999999"
                anchors.verticalCenter: parent.verticalCenter
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 22
                    font.bold: true
                    color: root.axisNameColor[root.axisOrder[0]]
                    text: root.axisName[root.axisOrder[0]]
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        root.initCalcUpDownbuttonStatus(root.axisPageNo);
                        root.initValues((root.getAxisCurpage(root.axisPageNo, root.axisOrder[0]) + 1), root.axisPageNo)
                        page1CurAxisIndex = 0
                        root.axisPageCtrl(1, 0)
                        if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                            pageModel.startAxisChanging()
                            pageModel.setCurAxisInfo(root.axisOrder[0]);
                        }
                        root.axisSelectCtrl((root.getAxisCurpage(root.axisPageNo, root.axisOrder[0]) + 1), root.axisOrder[0]);
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }//轴1
            Rectangle{
                visible: (root.axisPageNo === 1) && ((root.numAxis < 5 && (root.numAxis % 5) >= 1) || (root.numAxis >= 5))
                width: 138
                height: 50
                color: (page1CurAxisIndex === 1) ? "#cc3333" : "#cccccc"
                border.color: "#999999"
                anchors.verticalCenter: parent.verticalCenter
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 22
                    font.bold: true
                    color: (root.numAxis >= 2) ? root.axisNameColor[root.axisOrder[1]] : "black"
                    text: (root.numAxis >= 2) ? root.axisName[root.axisOrder[1]] : ""
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        root.initCalcUpDownbuttonStatus(root.axisPageNo);
                        root.initValues((root.getAxisCurpage(root.axisPageNo, root.axisOrder[1]) + 1), root.axisPageNo)
                        page1CurAxisIndex = 1
                        root.axisPageCtrl(1, 1)
                        if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                            pageModel.startAxisChanging()
                            pageModel.setCurAxisInfo(root.axisOrder[1]);
                        }
                        root.axisSelectCtrl((root.getAxisCurpage(root.axisPageNo, root.axisOrder[1]) + 1), root.axisOrder[1]);
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }//轴2
            Rectangle{
                visible: (root.axisPageNo === 1) && ((root.numAxis < 5 && (root.numAxis % 5) >= 3) || (root.numAxis >= 5))
                width: 138
                height: 50
                color: (page1CurAxisIndex === 2) ? "#cc3333" : "#cccccc"
                border.color: "#999999"
                anchors.verticalCenter: parent.verticalCenter
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 22
                    font.bold: true
                    color: (root.numAxis >= 3) ? root.axisNameColor[root.axisOrder[2]] : "black"
                    text: (root.numAxis >= 3) ? root.axisName[root.axisOrder[2]] : ""
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        root.initCalcUpDownbuttonStatus(root.axisPageNo);
                        root.initValues((root.getAxisCurpage(root.axisPageNo, root.axisOrder[2]) + 1), root.axisPageNo)
                        page1CurAxisIndex = 2
                        root.axisPageCtrl(1, 2)
                        if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                            pageModel.startAxisChanging()
                            pageModel.setCurAxisInfo(root.axisOrder[2]);
                        }
                        root.axisSelectCtrl((root.getAxisCurpage(root.axisPageNo, root.axisOrder[2]) + 1), root.axisOrder[2]);
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }//轴3
            Rectangle{
                visible: (root.axisPageNo === 1) && ((root.numAxis < 5 && (root.numAxis % 5) >= 4) || (root.numAxis >= 5))
                width: 138
                height: 50
                color: (page1CurAxisIndex === 3) ? "#cc3333" : "#cccccc"
                border.color: "#999999"
                anchors.verticalCenter: parent.verticalCenter
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 22
                    font.bold: true
                    color: (root.numAxis >= 4) ? root.axisNameColor[root.axisOrder[3]]:"black"
                    text: (root.numAxis >= 4) ? root.axisName[root.axisOrder[3]]:""
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        root.initCalcUpDownbuttonStatus(root.axisPageNo);
                        root.initValues((root.getAxisCurpage(root.axisPageNo, root.axisOrder[3]) + 1), root.axisPageNo)
                        page1CurAxisIndex = 3
                        root.axisPageCtrl(1, 3)
                        if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                            pageModel.startAxisChanging()
                            pageModel.setCurAxisInfo(root.axisOrder[3]);
                        }
                        root.axisSelectCtrl((root.getAxisCurpage(root.axisPageNo, root.axisOrder[3]) + 1), root.axisOrder[3]);
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }//轴4
            Rectangle{
                visible: (root.axisPageNo === 1) && ((root.numAxis < 5 && (root.numAxis / 5) === 1) || (root.numAxis >= 5))
                width: 138
                height: 50
                color: (page1CurAxisIndex === 4) ? "#cc3333" : "#cccccc"
                border.color: "#999999"
                anchors.verticalCenter: parent.verticalCenter
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 22
                    font.bold: true
                    color: (root.numAxis >= 5) ? root.axisNameColor[root.axisOrder[4]]:"black"
                    text: (root.numAxis >= 5) ? root.axisName[root.axisOrder[4]]:""
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        root.initCalcUpDownbuttonStatus(root.axisPageNo);
                        root.initValues((root.getAxisCurpage(root.axisPageNo, root.axisOrder[4]) + 1), root.axisPageNo)
                        page1CurAxisIndex = 4
                        root.axisPageCtrl(1, 4)
                        if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                            pageModel.startAxisChanging()
                            pageModel.setCurAxisInfo(root.axisOrder[4]);
                        }
                        root.axisSelectCtrl((root.getAxisCurpage(root.axisPageNo, root.axisOrder[4]) + 1), root.axisOrder[4]);
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }//轴5
            Rectangle{
                visible: (root.axisPageNo === 2) && (root.numAxis >= 6)
                width: 138
                height: 50
                color: (page2CurAxisIndex === 0) ? "#cc3333" : "#cccccc"
                border.color: "#999999"
                anchors.verticalCenter: parent.verticalCenter
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 22
                    font.bold: true
                    color: (root.numAxis >= 6) ? root.axisNameColor[root.axisOrder[5]] : "black"
                    text: (root.numAxis >= 6) ? root.axisName[root.axisOrder[5]] : ""
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        root.initCalcUpDownbuttonStatus(root.axisPageNo);
                        root.initValues((root.getAxisCurpage(root.axisPageNo, root.axisOrder[5]) + 1), root.axisPageNo)
                        page2CurAxisIndex = 0
                        root.axisPageCtrl(2, 0)
                        if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                            pageModel.startAxisChanging()
                            pageModel.setCurAxisInfo(root.axisOrder[5]);
                        }
                        root.axisSelectCtrl((root.getAxisCurpage(root.axisPageNo, root.axisOrder[5]) + 1), root.axisOrder[5]);
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }//轴6
            Rectangle{
                visible: (root.axisPageNo === 2) && (root.numAxis >= 7)
                width: 138
                height: 56
                color: (page2CurAxisIndex === 1) ? "#cc3333" : "#cccccc"
                border.color: "#999999"
                anchors.verticalCenter: parent.verticalCenter
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 22
                    font.bold: true
                    color: (root.numAxis >= 7) ? root.axisNameColor[root.axisOrder[6]] : "black"
                    text: (root.numAxis >= 7) ? root.axisName[root.axisOrder[6]] : ""
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        root.initCalcUpDownbuttonStatus(root.axisPageNo);
                        root.initValues((root.getAxisCurpage(root.axisPageNo, root.axisOrder[6]) + 1), root.axisPageNo)
                        page2CurAxisIndex = 1
                        root.axisPageCtrl(2, 1)
                        if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                            pageModel.startAxisChanging()
                            pageModel.setCurAxisInfo(root.axisOrder[6]);
                        }
                        root.axisSelectCtrl((root.getAxisCurpage(root.axisPageNo, root.axisOrder[6]) + 1), root.axisOrder[6]);
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }//轴7
            Rectangle{
                visible: (root.axisPageNo === 2) && (root.numAxis >= 8)
                width: 138
                height: 56
                color: (page2CurAxisIndex === 2) ? "#cc3333" : "#cccccc"
                border.color: "#999999"
                anchors.verticalCenter: parent.verticalCenter
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 22
                    font.bold: true
                    color: (root.numAxis >= 8) ? root.axisNameColor[root.axisOrder[7]] : "black"
                    text: (root.numAxis >= 8) ? root.axisName[root.axisOrder[7]] : ""
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        root.initCalcUpDownbuttonStatus(root.axisPageNo);
                        root.initValues((root.getAxisCurpage(root.axisPageNo, root.axisOrder[7]) + 1), root.axisPageNo)
                        page2CurAxisIndex = 2
                        root.axisPageCtrl(2, 2)
                        if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                            pageModel.startAxisChanging()
                            pageModel.setCurAxisInfo(root.axisOrder[7]);
                        }
                        root.axisSelectCtrl((root.getAxisCurpage(root.axisPageNo, root.axisOrder[7]) + 1), root.axisOrder[7]);
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }//轴8
        }//轴选择
        Image {
            visible: !fiveAxisMode
            anchors.verticalCenter: parent.verticalCenter
            x:885
            source:  nextAxisPageButtonMouseArea.pressed ?  resourceManager.imagePath + "/26_machine_p/a260_13.png" : resourceManager.imagePath + "/26_machine_p/a260_12.png"
            MouseArea{
                id: nextAxisPageButtonMouseArea
                anchors.fill: parent
                onClicked: {
                    let axisIndex;
                    if (root.axisPageNo === 1){
                        page1HoldCurAxisIndex = page1CurAxisIndex
                    } else {
                        page2HoldCurAxisIndex = page2CurAxisIndex
                    }
                    nextAxisPage()
                    if (root.axisPageNo === 1){
                        root.initCalcUpDownbuttonStatus(1);
                        root.axisPageCtrl(1, page1HoldCurAxisIndex)
                        page1CurAxisIndex = page1HoldCurAxisIndex;
                        if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                            pageModel.startAxisChanging()
                            pageModel.setCurAxisInfo(root.axisOrder[page1CurAxisIndex]);
                        }
                    } else {
                        root.initCalcUpDownbuttonStatus(2);
                        root.axisPageCtrl(2, page2HoldCurAxisIndex)
                        page2CurAxisIndex = page2HoldCurAxisIndex;
                        if (ENABLE_SIMPLE_PARAM_MODBUS_SPECIFIC){
                            pageModel.startAxisChanging()
                            pageModel.setCurAxisInfo(root.axisOrder[5+page2CurAxisIndex]);
                        }
                    }
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }
    }
    //各轴主画面
    Item {
        id:axisPage1Item
        visible:(root.axisPageNo === 1)
        width: 1010
        height: 460
        y:60
        Loader {
            id:axisPage1DrvParamSimpleData
            source: "qrc:/qml/Functions/MechanicalConfig/DriveParameters/DrvParamSimpleData.qml";
            active: false
            property bool isActive: root.pageActive && (root.axisPageNo === 1)
            onLoaded: {
                item.pageActive = isActive;
                item.jogScrollOn = isActive;
                item.visible = (root.numAxis >= 1)
                item.pageModel = root.pageModel
                item.axisIndex = (root.numAxis >= 1) ? root.axisOrder[root.page1CurAxisIndex] : -1
                //item.axisIndex = (root.numAxis >= 1) ? (root.axisOrder[root.page1CurAxisIndex] === 2 ? 3 : (root.axisOrder[root.page1CurAxisIndex] === 3 ? 2:root.axisOrder[root.page1CurAxisIndex])) : -1
                item.jogScrollOn = root.jogScrollOn
                item.scrollTopButtonPressImage = resourceManager.imagePath + "/26_machine_p/a260_09.png"
                item.scrollTopButtonReleaseImage = resourceManager.imagePath + "/26_machine_p/a260_07.png"
                item.scrollBotomButtonPressImage = resourceManager.imagePath + "/26_machine_p/a260_10.png"
                item.scrollBotomButtonReleaseImage = resourceManager.imagePath + "/26_machine_p/a260_08.png"
            }
            onIsActiveChanged: {
                if (isActive) {
                    active = true
                }
                if(status == Loader.Ready){
                    item.pageActive = isActive;
                    item.jogScrollOn = isActive;
                }
            }
        }
    }
    Connections {
        target: axisPage1DrvParamSimpleData.item
        onRemindPopupOpen:{
            remind_save5.open()
        }
        onUpdatePage:{
            root.modbusPageRead(page, item, axis)
        }
        onInitPage:{
            root.pageInit(page, axis)
        }
        onReadPage:{
            root.pageRead(page, axis)
        }
        onUpdatePageSR:{
            root.modbusPageReadSR(page, item, axis)
        }
        onUpdatePageHR:{
            root.modbusPageReadHR(page, item, axis)
        }
    }

    Item {
        id:axisPage2Item
        visible:(root.axisPageNo === 2)
        width: 1010
        height: 460
        y:60
        Loader {
            id:axisPage2DrvParamSimpleData
            source: "qrc:/qml/Functions/MechanicalConfig/DriveParameters/DrvParamSimpleData.qml";
            active: false
            property bool isActive: root.pageActive && (root.axisPageNo === 2)
            onLoaded: {
                item.pageActive = isActive;
                item.jogScrollOn = isActive;
                item.visible = (root.numAxis >= 6)
                item.pageModel = root.pageModel
                item.axisIndex = (root.numAxis >= 6) ? root.axisOrder[5+root.page2CurAxisIndex] : -1
                //item.axisIndex = (root.numAxis >= 6) ? (root.axisOrder[5+root.page2CurAxisIndex] === 2 ? 3 : (root.axisOrder[5+root.page2CurAxisIndex] === 3 ? 2:root.axisOrder[5+root.page2CurAxisIndex])) : -1
                item.jogScrollOn = root.jogScrollOn
                item.scrollTopButtonPressImage = resourceManager.imagePath + "/26_machine_p/d260_09.png"
                item.scrollTopButtonReleaseImage = resourceManager.imagePath + "/26_machine_p/d260_07.png"
                item.scrollBotomButtonPressImage = resourceManager.imagePath + "/26_machine_p/d260_10.png"
                item.scrollBotomButtonReleaseImage = resourceManager.imagePath + "/26_machine_p/d260_08.png"
            }
            onIsActiveChanged: {
                if (isActive) {
                    active = true
                }
                if(status == Loader.Ready){
                    item.pageActive = isActive;
                    item.jogScrollOn = isActive;
                }
            }
        }
    }
    Connections {
        target: axisPage2DrvParamSimpleData.item
        onRemindPopupOpen:{
            remind_save5.open()
        }
        onUpdatePage:{
            root.modbusPageRead(page, item, axis)
        }
        onInitPage:{
            root.pageInit(page, axis)
        }
        onReadPage:{
            root.pageRead(page, axis)
        }
        onUpdatePageSR:{
            root.modbusPageReadSR(page, item, axis)
        }
        onUpdatePageHR:{
            root.modbusPageReadHR(page, item, axis)
        }
    }

    Remind_save{
        id:remind_save5
        remindindex: 6
        remindtype:{
            //console.log("Remind_save saveStatus:"+pageModel.saveStatus)
            return pageModel.saveStatus
        }
        onClickSaveButton:{
            //console.log("remind_save5 onClickSaveButton:")
            pageModel.modbusSaveCalcValue()
        }
    }
}
