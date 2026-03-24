import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import Enums 1.0
import "../../Widget"
import "../../Program/PosSet/PosSetUtils.js" as Utils
/*************************************************************************************
* Title: UI/UX of AEK
* File: Area_Set_2.qml
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
    id: areaFixPage
    width: parent.width
    height: parent.height
    property bool pageActive:false
    property var viewModel: areaSetViewModel
    property int pageNo: 1
    property int maxPageNo: Math.ceil(numAxis / columnsPerPage)
    readonly property int columnsPerPage: 5
    property var axisOperateViewModel: manualViewModel.panelModel[1]
    property var axisDataList: axisOperateViewModel.axisInfo.axisDataList
    property var axisOrder: commonViewModel.axisOrder
    property int numAxis: axisOrder.length
    property var ioSignalList: viewModel.ioSignalList
    property int axisModelNum: getAxisModelNum(pageNo)
    property bool dataLoaded: viewModel.dataLoaded
    property bool isManual: commonViewModel.manipulateMode === 1

    function setData(areaType, axisIndex, valueStr){
        let value = parseFloat(valueStr)
        if (value !== Utils.INVALID_POSITION_VALUE) {
            value = (value * local.areaSetDataMagnification).toFixed(2)
        }
        viewModel.setDataByAreaFix(areaType, axisIndex, value)
    }

    function setMxSxSpace(valueStr){
        let value = parseFloat(valueStr)
        if (value !== Utils.INVALID_POSITION_VALUE) {
            value = (value * local.areaSetDataMagnification).toFixed(2)
        }
        viewModel.setMxSxSpaceValue(value)
    }

    function getAxisModelNum(pageNo){
        let modelNum = numAxis - (pageNo - 1) * columnsPerPage
        if (modelNum > columnsPerPage) {
            modelNum = columnsPerPage
        }
        return modelNum
    }

    onPageActiveChanged: {
        if (pageActive) {
            viewModel.startPageChanging()
            commonViewModel.pageChange(Enums.FunctionAreaFix)
        }
    }

    QtObject {
        id: local
        property int areaSetDataMagnification: 100
        Component.onCompleted: {
            areaSetDataMagnification = viewModel.dataMagnification()
        }
    }

    QtObject {
        id: penSet
        readonly property string groupNo: "19"
        property string dataColor: ""
        property string selectedDataColor: ""
        property string ioOnColor: ""
        Component.onCompleted: {
            dataColor = resourceManager.dataColor(groupNo, 12)
            selectedDataColor = resourceManager.dataColor(groupNo, 13)
            ioOnColor = resourceManager.dataColor(groupNo, 14)
        }
    }

    AreaSetUtils {
        id: area_set_utils
    }

/**************************************left area**********************************************************/
    ColumnLayout{
        x:10
        y:10
        spacing: 1

        Row {
            spacing: 1
            Rectangle{
                width:150
                height: 47
                color: "#cccccc"
            }
            Repeater {
                model: axisModelNum
                Rectangle{
                    property int axisIndex: (pageNo-1)*columnsPerPage + index
                    property int axisType: axisOrder[axisIndex]
                    width: 134
                    height: 47
                    visible: (axisIndex < numAxis)
                    color: "#999999"
                    Text {
                        readonly property int axisLabelIndex: Utils.axisLabelIndex(parent.axisType)
                        anchors.centerIn: parent
                        font.pixelSize: 22
                        font.bold: true
                        text: resourceManager.label(22, axisLabelIndex, resourceManager.language)
                        color: resourceManager.rgb(22, axisLabelIndex)
                    }
                }
            }
        }//标题
        Row {
            spacing: 1
            Rectangle{
                width:150
                height: 60
                color: "#cccccc"
                Text {
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 2
                    font.pixelSize: 18
                    font.bold: true
                    text: resourceManager.label(44, 29,resourceManager.language)
                    color: resourceManager.rgb(44, 29)
                }
            }
            Repeater {
                model: axisModelNum
                Rectangle{
                    property int axisIndex: (pageNo-1)*columnsPerPage + index
                    property int axisType: axisOrder[axisIndex]
                    width:134
                    height: 60
                    visible: (axisIndex < numAxis)
                    color: "#cccccc"
                    border.color: "#999999"
                    border.width: 1
                    Text{
                        anchors.centerIn: parent
                        visible: areaFixPage.dataLoaded
                        font.pixelSize: 26
                        font.bold: true
                        color: penSet.dataColor
                        text: axisDataList[axisType].position
                    }
                }
            }
        }//当前位置

        Row{
            spacing: 1
            Rectangle{
                width:150
                height: 60
                color: "#cccccc"
                Text {
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 2
                    font.pixelSize: 18
                    font.bold: true
                    text: resourceManager.label(44, 12,resourceManager.language)
                    color: resourceManager.rgb(44, 12)
                }
            }
            Repeater {
                id: descent_max_movement
                readonly property int areaType: Enums.AreaTypeDescentMaxMovement
                property var dataList: viewModel.getDataList(areaType)
                model: axisModelNum
                LimitNumericEditBox {
                    property int axisIndex: (pageNo-1)*columnsPerPage + index
                    property int axisType: axisOrder[axisIndex]
                    property bool enable: (axisIndex < numAxis) ? descent_max_movement.dataList[axisType].enable : false
                    width:134
                    height: 60
                    visible: (axisIndex < numAxis)
                    color: !enabled ? "#cccccc" : opened ? "#cc3333" : "white"
                    borderColor: !enabled ? "#999999" : "#999999"
                    borderWidth: 1
                    fontPixelSize: 26
                    keyLayoutType: 4
                    textColor: opened ? penSet.selectedDataColor : penSet.dataColor
                    value: ((axisIndex < numAxis) && enable) ? viewModel.formatAreaFixData(descent_max_movement.areaType, descent_max_movement.dataList[axisType].value) : ""
                    enabled: enable
                    enablePressAndHoldClearKey: true
                    onReady: {
                        max = commonViewModel.formatPointMaxValue(axisType, area_set_utils.getPointNo(Enums.AreaTypeDescentMaxMovement, axisType))
                        min = commonViewModel.formatPointMinValue(axisType, area_set_utils.getPointNo(Enums.AreaTypeDescentMaxMovement, axisType))
                        if(isManual){
                            isNumberKpad2 = false
                            index == 3 || index == 4 ? setDialogPos(-158,185,0,index,0,135) : setDialogPos(302,185,0,index,0,135)
                        }else{
                            isNumberKpad2 = true
                            setDialogPos(58,134,0,index,0,135)
                        }
                    }
                    onEnter: {
                        areaFixPage.setData(descent_max_movement.areaType, axisType, newValue)
                        commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                        commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                    }
                }
            }
        }//最大移动值

        Row{
            spacing: 1
            Rectangle{
                width: 74
                height: 121
                color: "#cccccc"
                Text {
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 2
                    font.bold: true
                    wrapMode: Text.WordWrap
                    font.pixelSize: 18
                    text: resourceManager.label(44, 20,resourceManager.language)
                    color: resourceManager.rgb(44, 20)
                }
            }
            Column{
                spacing: 1
                Row{
                    spacing: 1
                    Rectangle{
                        width:75
                        height: 60
                        color: "#999999"
                        Text {
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            leftPadding: 5
                            font.bold: true
                            font.pixelSize: 18
                            text: resourceManager.label(44, 18,resourceManager.language)
                            color: resourceManager.rgb(44, 18)
                        }
                    }
                    Repeater {
                        id: output_side_max
                        readonly property int areaType: Enums.AreaTypeOutputSideMax
                        property var dataList: viewModel.getDataList(areaType)
                        model: axisModelNum
                        LimitNumericEditBox {
                            property int axisIndex: (pageNo-1)*columnsPerPage + index
                            property int axisType: axisOrder[axisIndex]
                            property bool enable: (axisIndex < numAxis) ? output_side_max.dataList[axisType].enable : false
                            width:134
                            height: 60
                            visible: (axisIndex < numAxis)
                            color: !enabled ? "#cccccc" : opened ? "#cc3333" : "white"
                            borderColor: !enabled ? "#999999" : "#999999"
                            borderWidth: 1
                            fontPixelSize: 26
                            keyLayoutType: 4
                            enablePressAndHoldClearKey: true
                            textColor: opened ? penSet.selectedDataColor : penSet.dataColor
                            value: ((axisIndex < numAxis) && enable) ? viewModel.formatAreaFixData(output_side_max.areaType, output_side_max.dataList[axisType].value) : ""
                            enabled: enable
                            onReady: {
                                max = commonViewModel.formatPointMaxValue(axisType, area_set_utils.getPointNo(Enums.AreaTypeOutputSideMax, axisType))
                                min = commonViewModel.formatPointMinValue(axisType, area_set_utils.getPointNo(Enums.AreaTypeOutputSideMax, axisType))
                                if(isManual){
                                    isNumberKpad2 = false
                                    index == 3 || index == 4 ? setDialogPos(-158,185,0,index,0,135) : setDialogPos(302,185,0,index,0,135)
                                }else{
                                    isNumberKpad2 = true
                                    setDialogPos(58,194,0,index,0,135)
                                }
                            }
                            onEnter: {
                                if(max === "****.**"){
                                    return
                                }else{
                                    areaFixPage.setData(output_side_max.areaType, axisType, newValue)
                                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                }
                            }
                        }
                    }
                }
                Row{
                    spacing: 1
                    Rectangle{
                        width:75
                        height: 60
                        color: "#999999"
                        Text {
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            leftPadding: 5
                            font.bold: true
                            font.pixelSize: 18
                            text: resourceManager.label(44, 19,resourceManager.language)
                            color: resourceManager.rgb(44, 19)
                        }
                    }
                    Repeater {
                        id: output_side_min
                        readonly property int areaType: Enums.AreaTypeOutputSideMin
                        property var dataList: viewModel.getDataList(areaType)
                        model: axisModelNum
                        LimitNumericEditBox {
                            property int axisIndex: (pageNo-1)*columnsPerPage + index
                            property int axisType: axisOrder[axisIndex]
                            property bool enable: (axisIndex < numAxis) ? output_side_min.dataList[axisType].enable : false
                            width:134
                            height: 60
                            visible: (axisIndex < numAxis)
                            color: !enabled ? "#cccccc" : opened ? "#cc3333" : "white"
                            borderColor: !enabled ? "#999999" : "#999999"
                            borderWidth: 1
                            fontPixelSize: 26
                            keyLayoutType: 4
                            enablePressAndHoldClearKey: true
                            textColor: opened ? penSet.selectedDataColor : penSet.dataColor
                            value: ((axisIndex < numAxis) && enable) ? viewModel.formatAreaFixData(output_side_min.areaType, output_side_min.dataList[axisType].value) : ""
                            enabled: enable
                            onReady: {
                                max = commonViewModel.formatPointMaxValue(axisType, area_set_utils.getPointNo(Enums.AreaTypeOutputSideMin, axisType))
                                min = commonViewModel.formatPointMinValue(axisType, area_set_utils.getPointNo(Enums.AreaTypeOutputSideMin, axisType))
                                if(isManual){
                                    isNumberKpad2 = false
                                    index == 3 || index == 4 ? setDialogPos(-158,185,0,index,0,135) : setDialogPos(302,185,0,index,0,135)
                                }else{
                                    isNumberKpad2 = true
                                    setDialogPos(58,256,0,index,0,135)
                                }
                            }
                            onEnter: {
                                if(max === "****.**"){
                                    return
                                }else{
                                    areaFixPage.setData(output_side_min.areaType, axisType, newValue)
                                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                }
                            }
                        }
                    }
                }
            }
        }//取物侧区域

        Row{
            spacing: 1
            Rectangle{
                width: 74
                height: 121
                color: "#cccccc"
                Text {
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 2
                    font.bold: true
                    wrapMode: Text.WordWrap
                    font.pixelSize: 18
                    text: resourceManager.label(44, 21,resourceManager.language)
                    color: resourceManager.rgb(44, 21)
                }
            }
            Column{
                spacing: 1
                Row{
                    spacing: 1
                    Rectangle{
                        width:75
                        height: 60
                        color: "#999999"
                        Text {
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            leftPadding: 5
                            font.bold: true
                            font.pixelSize: 18
                            text: resourceManager.label(44, 18,resourceManager.language)
                            color: resourceManager.rgb(44, 18)
                        }
                    }
                    Repeater {
                        id: open_side_max
                        readonly property int areaType: Enums.AreaTypeOpenSideMax
                        property var dataList: viewModel.getDataList(areaType)
                        model: axisModelNum
                        LimitNumericEditBox {
                            property int axisIndex: (pageNo-1)*columnsPerPage + index
                            property int axisType: axisOrder[axisIndex]
                            property bool enable: (axisIndex < numAxis) ? open_side_max.dataList[axisType].enable : false
                            width:134
                            height: 60
                            visible: (axisIndex < numAxis)
                            color: !enabled ? "#cccccc" : opened ? "#cc3333" : "white"
                            borderColor: !enabled ? "#999999" : "#999999"
                            borderWidth: 1
                            fontPixelSize: 26
                            keyLayoutType: 4
                            enablePressAndHoldClearKey: true
                            textColor: opened ? penSet.selectedDataColor : penSet.dataColor
                            value: ((axisIndex < numAxis) && enable) ? viewModel.formatAreaFixData(open_side_max.areaType, open_side_max.dataList[axisType].value) : ""
                            enabled: enable
                            onReady: {
                                max = commonViewModel.formatPointMaxValue(axisType, area_set_utils.getPointNo(Enums.AreaTypeOpenSideMax, axisType))
                                min = commonViewModel.formatPointMinValue(axisType, area_set_utils.getPointNo(Enums.AreaTypeOpenSideMax, axisType))
                                if(isManual){
                                    isNumberKpad2 = false
                                    index == 3 || index == 4 ? setDialogPos(-158,185,0,index,0,135) : setDialogPos(302,185,0,index,0,135)
                                }else{
                                    isNumberKpad2 = true
                                    setDialogPos(58,318,0,index,0,135)
                                }
                            }
                            onEnter: {
                                if(max === "****.**"){
                                    return
                                }else{
                                    areaFixPage.setData(open_side_max.areaType, axisType, newValue)
                                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                }
                            }
                        }
                    }
                }
                Row{
                    spacing: 1
                    Rectangle{
                        width:75
                        height: 60
                        color: "#999999"
                        Text {
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            leftPadding: 5
                            font.bold: true
                            font.pixelSize: 18
                            text: resourceManager.label(44, 19,resourceManager.language)
                            color: resourceManager.rgb(44, 19)
                        }
                    }
                    Repeater {
                        id: open_side_min
                        readonly property int areaType: Enums.AreaTypeOpenSideMin
                        property var dataList: viewModel.getDataList(areaType)
                        model: axisModelNum
                        LimitNumericEditBox {
                            property int axisIndex: (pageNo-1)*columnsPerPage + index
                            property int axisType: axisOrder[axisIndex]
                            property bool enable: (axisIndex < numAxis) ? open_side_min.dataList[axisType].enable : false
                            width:134
                            height: 60
                            visible: (axisIndex < numAxis)
                            color: !enabled ? "#cccccc" : opened ? "#cc3333" : "white"
                            borderColor: !enabled ? "#999999" : "#999999"
                            borderWidth: 1
                            fontPixelSize: 26
                            keyLayoutType: 4
                            enablePressAndHoldClearKey: true
                            textColor: opened ? penSet.selectedDataColor : penSet.dataColor
                            value: ((axisIndex < numAxis) && enable) ? viewModel.formatAreaFixData(open_side_min.areaType, open_side_min.dataList[axisType].value) : ""
                            enabled: enable
                            onReady: {
                                max = commonViewModel.formatPointMaxValue(axisType, area_set_utils.getPointNo(Enums.AreaTypeOpenSideMin, axisType))
                                min = commonViewModel.formatPointMinValue(axisType, area_set_utils.getPointNo(Enums.AreaTypeOpenSideMin, axisType))
                                if(isManual){
                                    isNumberKpad2 = false
                                    index == 3 || index == 4 ? setDialogPos(-158,185,0,index,0,135) : setDialogPos(302,185,0,index,0,135)
                                }else{
                                    isNumberKpad2 = true
                                    setDialogPos(58,380,0,index,0,135)
                                }
                            }
                            onEnter: {
                                if(max === "****.**"){
                                    return
                                }else{
                                    areaFixPage.setData(open_side_min.areaType, axisType, newValue)
                                    commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                    commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                }
                            }
                        }
                    }
                }
            }
        }//置物侧区域

        Row{
            spacing: 1
            Rectangle{
                width:74
                height: 60
                color: "#cccccc"
                Text {
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 2
                    font.bold: true
                    font.pixelSize: 18
                    text: resourceManager.label(44, 22,resourceManager.language)
                    color: resourceManager.rgb(44, 22)
                }
            }
            Rectangle{
                width:75
                height: 60
                color: "#999999"
                Text {
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.bold: true
                    font.pixelSize: 18
                    text: resourceManager.label(44, 18,resourceManager.language)
                    color: resourceManager.rgb(44, 18)
                }
            }
            Repeater {
                id: descent_max
                readonly property int areaType: Enums.AreaTypeDescentMax
                property var dataList: viewModel.getDataList(areaType)
                model: axisModelNum
                LimitNumericEditBox {
                    property int axisIndex: (pageNo-1)*columnsPerPage + index
                    property int axisType: axisOrder[axisIndex]
                    property bool enable: (axisIndex < numAxis) ? descent_max.dataList[axisType].enable : false
                    width:134
                    height: 60
                    visible: (axisIndex < numAxis)
                    color: !enabled ? "#cccccc" : opened ? "#cc3333" : "white"
                    borderColor: !enabled ? "#999999" : "#999999"
                    borderWidth: 1
                    fontPixelSize: 26
                    keyLayoutType: 4
                    enablePressAndHoldClearKey: true
                    textColor: opened ? penSet.selectedDataColor : penSet.dataColor
                    value: ((axisIndex < numAxis) && enable) ? viewModel.formatAreaFixData(descent_max.areaType, descent_max.dataList[axisType].value) : ""
                    enabled: enable
                    onReady: {
                        max = commonViewModel.formatPointMaxValue(axisType, area_set_utils.getPointNo(Enums.AreaTypeDescentMax, axisType))
                        min = commonViewModel.formatPointMinValue(axisType, area_set_utils.getPointNo(Enums.AreaTypeDescentMax, axisType))
                        if(isManual){
                            isNumberKpad2 = false
                            index == 3 || index == 4 ? setDialogPos(-158,255,0,index,0,135) : setDialogPos(302,255,0,index,0,135)
                        }else{
                            isNumberKpad2 = true
                            setDialogPos(58,440,0,index,0,135)
                        }
                    }
                    onEnter: {
                        if(max === "****.**"){
                            return
                        }else{
                            areaFixPage.setData(descent_max.areaType, axisType, newValue)
                            commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                            commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                        }
                    }
                }
            }
        }//下降
    }

/**************************************right area*********************************************************/
    Rectangle {
        x:844
        y:10
        width: 160
        height: 472
        color: "#999999"
        Column{
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 1
            topPadding: 10
            Rectangle{
                width: 140
                height: 60
                border.color: "#999999"
                color: "#eaeaea"
                Text {
                    width: 110
                    height: 60
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 10
                    font.pixelSize: 20
                    font.bold: true
                    text: resourceManager.label(44, 3,resourceManager.language)
                    color: ioSignalList[0] ? penSet.ioOnColor : resourceManager.rgb(44, 3)
                }
                Rectangle{
                    width: 22
                    height: 22
                    x:100
                    anchors.verticalCenter: parent.verticalCenter
                    radius: 11
                    border.width: 2
                    border.color: "#999999"
                    color: ioSignalList[0] ? "#cc3333" : "#ffffff"
                }
            }
            Rectangle{
                width: 140
                height: 60
                border.color: "#999999"
                color: "#eaeaea"
                Text {
                    width: 110
                    height: 60
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 10
                    font.pixelSize: 20
                    font.bold: true
                    text: resourceManager.label(44, 4,resourceManager.language)
                    color: ioSignalList[1] ? penSet.ioOnColor : resourceManager.rgb(44, 4)
                }
                Rectangle{
                    width: 22
                    height: 22
                    x:100
                    anchors.verticalCenter: parent.verticalCenter
                    radius: 11
                    border.width: 2
                    border.color: "#999999"
                    color: ioSignalList[1] ? "#cc3333" : "#ffffff"
                }
            }
            Rectangle{
                width: 140
                height: 60
                border.color: "#999999"
                color: "#eaeaea"
                Text {
                    width: 110
                    height: 60
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 10
                    font.pixelSize: 20
                    font.bold: true
                    text: resourceManager.label(44, 5,resourceManager.language)
                    color: ioSignalList[2] ? penSet.ioOnColor : resourceManager.rgb(44, 5)
                }
                Rectangle{
                    width: 22
                    height: 22
                    x:100
                    anchors.verticalCenter: parent.verticalCenter
                    radius: 11
                    border.width: 2
                    border.color: "#999999"
                    color: ioSignalList[2] ? "#cc3333" : "#ffffff"
                }
            }
            Rectangle{
                width: 140
                height: 60
                border.color: "#999999"
                color: "#eaeaea"

                Text {
                    width: 110
                    height: 60
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 10
                    font.pixelSize: 20
                    font.bold: true
                    text: resourceManager.label(44, 6,resourceManager.language)
                    color: ioSignalList[3] ? penSet.ioOnColor : resourceManager.rgb(44, 6)
                }
                Rectangle{
                    width: 22
                    height: 22
                    x:100
                    anchors.verticalCenter: parent.verticalCenter
                    radius: 11
                    border.width: 2
                    border.color: "#999999"
                    color: ioSignalList[3] ? "#cc3333" : "#ffffff"
                }
            }
        }
//기능없음으로 숨김
//        Text {
//            anchors.horizontalCenter: parent.horizontalCenter
//            anchors.bottom: parent.bottom
//            anchors.bottomMargin: 90
//            font.pixelSize: 20
//            font.bold: true
//            text: resourceManager.label(44, 23,resourceManager.language)
//            color: resourceManager.rgb(44, 23)
//        }
//        LimitNumericEditBox {
//            readonly property int mxSxSpaceAxisIndex: 2
//            readonly property int mxSxSpacePointNo: 98
//            width:120
//            height: 60
//            anchors.horizontalCenter: parent.horizontalCenter
//            anchors.bottom: parent.bottom
//            anchors.bottomMargin: 16
//            color: opened ? "#cc3333" : "#ffffff"
//            borderColor: "#999999"
//            borderWidth: 2
//            fontPixelSize: 26
//            keyLayoutType: 4
//            textColor: opened ? penSet.selectedDataColor : penSet.dataColor
//            value: viewModel.formatMxSxSpace(viewModel.mxSxSpace)
//            enablePressAndHoldClearKey: true
//            onReady: {
//                max = commonViewModel.formatPointMaxValue(mxSxSpaceAxisIndex, mxSxSpacePointNo)
//                min = commonViewModel.formatPointMinValue(mxSxSpaceAxisIndex, mxSxSpacePointNo)
//            }
//            onEnter: {
//                setMxSxSpace(newValue)
//                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
//                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
//            }
//        }
    }
}
