import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import Enums 1.0
import "../../Components/RemindDialog"
import "../../Widget"
/*************************************************************************************
* Title: UI/UX of AEK
* File: Mechanical_parameters.qml
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
    id: mecha_param_page
    property bool pageActive: false
    width: parent.width
    height: parent.height
    property var viewModel: functionViewModel.mechanicalParamView.mechaParamView
    property int pageNum: viewModel.pageNum
    property int page_max: viewModel.pageMaxNum
    property int axisPageNum: viewModel.axisPageNum
    property int axisPage_max: viewModel.axisPageMaxNum
    property var axisOrder: commonViewModel.axisOrder
    property int numAxis: axisOrder.length
    property var axisNameIndex: [0, 1, 2, 3, 4, 5, 6, 7]

    property int axisNum1: (1 <= numAxis ? axisOrder[0] : 0)
    property int axisNum2: (2 <= numAxis ? axisOrder[1] : 0)
    property int axisNum3: (3 <= numAxis ? axisOrder[2] : 0)
    property int axisNum4: (4 <= numAxis ? axisOrder[3] : 0)
    property int axisNum5: (5 <= numAxis ? axisOrder[4] : 0)
    property int axisNum6: (6 <= numAxis ? axisOrder[5] : 0)
    property int axisNum7: (7 <= numAxis ? axisOrder[6] : 0)
    property int axisNum8: (8 <= numAxis ? axisOrder[7] : 0)
    property bool isManual:phyKeyManager.selectSwMode === 2  //commonViewModel.manipulateMode === 1


    onPageActiveChanged: {
        if (pageActive) {
            viewModel.activate()
        } else {
            viewModel.deactivate()
        }
    }

    Component.onDestruction: viewModel.deactivate()

    Row{
        x:692
        y:-10
        spacing: 10
        Rectangle{
            width: 110
            height: 48
            border.width: 2
            border.color: "#999999"
            color: save_mouse.pressed ? "#cc3333" : "#cccccc"
            Text {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: save_mouse.pressed ? 22 : 21
                color: resourceManager.rgb(43,42)
                text: resourceManager.label(43,42,resourceManager.language)
            }
            MouseArea{
                id:save_mouse
                anchors.fill: parent
                onClicked: {
                    if(isManual){
                        remind_save2_3.remindindex = 2;
                        remind_save2_3.open()
                    }else{
                        remind_save_forbid.open()
                    }
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }
        Rectangle{
            width: 110
            height: 48
            border.width: 2
            border.color: "#999999"
            color: save_mouse1.pressed ? "#cc3333" : "#cccccc"
            Text {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: save_mouse1.pressed ? 22 : 21
                color: resourceManager.rgb(43,1)
                text: resourceManager.label(43,1,resourceManager.language)
            }
            MouseArea{
                id:save_mouse1
                anchors.fill: parent
                onClicked: {
                    if(isManual){
                        remind_save2_3.remindindex = 3;
                        remind_save2_3.open()
                    }else{
                        remind_save_forbid.open()
                    }
                    phyKeyManager.ctrlBuzzer()
                }
            }
        }
    }//保存按钮

    Text {
        x:12
        y:8
        font.pixelSize: 22
        color: resourceManager.dataColor(18, 32)
        text: viewModel.modelName
    }

    Row {
        x:8
        y:44
        spacing: 1
        Rectangle{
            width:196
            height: 47
            color: "#999999"
        }
        Rectangle{
            visible: (axisPageNum === 0 ? 1 <= numAxis : 6 <= numAxis)
            width:144
            height: 47
            color: "#999999"
            Text {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 22
                font.bold: true
                color: resourceManager.rgb(22, (axisPageNum == 0 ? axisNameIndex[axisNum1] : axisNameIndex[axisNum6]))
                text: resourceManager.label(22,(axisPageNum == 0 ? axisNameIndex[axisNum1] : axisNameIndex[axisNum6]),resourceManager.language)
            }
        }
        Rectangle{
            visible: (axisPageNum === 0 ? 2 <= numAxis : 7 <= numAxis)
            width:144
            height: 47
            color: "#999999"
            Text {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 22
                font.bold: true
                color: resourceManager.rgb(22, (axisPageNum == 0 ? axisNameIndex[axisNum2] : axisNameIndex[axisNum7]))
                text: resourceManager.label(22,(axisPageNum == 0 ? axisNameIndex[axisNum2] : axisNameIndex[axisNum7]),resourceManager.language)
            }
        }
        Rectangle{
            visible: (axisPageNum === 0 ? 3 <= numAxis : 8 <= numAxis)
            width:144
            height: 47
            color: "#999999"
            Text {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 22
                font.bold: true
                color: resourceManager.rgb(22, (axisPageNum == 0 ? axisNameIndex[axisNum3] : axisNameIndex[axisNum8]))
                text: resourceManager.label(22,(axisPageNum == 0 ? axisNameIndex[axisNum3] : axisNameIndex[axisNum8]),resourceManager.language)
            }
        }
        Rectangle{
            visible: (axisPageNum === 0 ? 4 <= numAxis : false)
            width:144
            height: 47
            color: "#999999"
            Text {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 22
                font.bold: true
                color: resourceManager.rgb(22,axisNameIndex[axisNum4])
                text: resourceManager.label(22,axisNameIndex[axisNum4],resourceManager.language)
            }
        }
        Rectangle{
            visible: (axisPageNum === 0 ? 5 <= numAxis : false)
            width:144
            height: 47
            color: "#999999"
            Text {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 22
                font.bold: true
                color: resourceManager.rgb(22,axisNameIndex[axisNum5])
                text: resourceManager.label(22,axisNameIndex[axisNum5],resourceManager.language)
            }
        }
    }//标题栏

    Image {
        visible: 6 <= numAxis
        x:960
        y:50
        source: resourceManager.imagePath + "/26_machine_p/d260_12.png"
        MouseArea{
            anchors.fill: parent
            onPressed: {
                parent.source = resourceManager.imagePath + "/26_machine_p/d260_13.png"
            }
            onReleased: {
                parent.source = resourceManager.imagePath + "/26_machine_p/d260_12.png"
            }
            onClicked: {
                if(axisPageNum < axisPage_max-1){
                    viewModel.axisPageNum ++
                }else{
                    viewModel.axisPageNum = 0
                }
            }
        }
    }//换页按钮

    StackLayout{
        id:stack_mec
        width: 940
        height: 430
        y:94
        currentIndex: pageNum
        //page1
        Item{
            Row{
                x:8
                spacing: 1
                Column{
                    spacing: 1
                    Repeater{
                        model: 7
                        Rectangle{
                            width:196
                            height: 60
                            color: "#999999"
                            Text {
                                anchors.fill: parent
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                font.pixelSize: lineCount != 1 ? 18 : 20
                                font.bold: true
                                color: resourceManager.rgb(43,30+index)
                                text: resourceManager.label(43,30+index,resourceManager.language)
                            }
                        }
                    }
                }
                Repeater{
                    model: viewModel.getDataList(axisPageNum)
                    Column{
                        property int colIndex: index
                        property int axisNum: (axisPageNum*5)+colIndex
                        property int axisIndex: (axisNum < numAxis ? axisOrder[axisNum] : 0)
                        x:8
                        spacing: 1
                        enabled: isManual
                        LimitNumericEditBox{
                            property int dataType: 0
                            property var format: getFormat(dataType)
                            property var formatShift: getFormatShift(dataType)

                            width:144
                            height: 60
                            color: "#ffffff"
                            borderColor: "#999999"
                            borderWidth: 1
                            fontPixelSize: 26
                            textColor: (modelData.isChangeRotationRange ? resourceManager.dataColor(18, 35) : resourceManager.dataColor(18, 33))
                            tenkeyTextColor: resourceManager.dataColor(18, 34)
                            keyLayoutType: getTenkeyType(dataType)
                            max: getMax(dataType)
                            min: getMin(dataType)
                            value: resourceManager.formatData(modelData.rotationRange, format, formatShift, false)

                            onEnter: {
                                viewModel.setValue(axisIndex, dataType, newValue)
                                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                            }
                        }
                        LimitNumericEditBox{
                            property int dataType: 1
                            property var format: getFormat(dataType)
                            property var formatShift: getFormatShift(dataType)

                            width:144
                            height: 60

                            color: "#ffffff"
                            borderColor: "#999999"
                            borderWidth: 1
                            fontPixelSize: 26
                            textColor: (modelData.isChangeRotationPulse ? resourceManager.dataColor(18, 35) : resourceManager.dataColor(18, 33))
                            tenkeyTextColor: resourceManager.dataColor(18, 34)
                            keyLayoutType: getTenkeyType(dataType)
                            max: getMax(dataType)
                            min: getMin(dataType)
                            value: resourceManager.formatData(modelData.rotationPulse, format, formatShift, false)

                            onEnter: {
                                viewModel.setValue(axisIndex, dataType, newValue)
                                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                            }
                        }
                        LimitNumericEditBox{
                            property int dataType: 2
                            property var format: getFormat(dataType)
                            property var formatShift: getFormatShift(dataType)

                            width:144
                            height: 60
                            color: "#ffffff"
                            borderColor: "#999999"
                            borderWidth: 1
                            fontPixelSize: 26
                            textColor: (modelData.isChangeMaxRotation ? resourceManager.dataColor(18, 35) : resourceManager.dataColor(18, 33))
                            tenkeyTextColor: resourceManager.dataColor(18, 34)
                            keyLayoutType: getTenkeyType(dataType)
                            max: getMax(dataType)
                            min: getMin(dataType)
                            value: resourceManager.formatData(modelData.maxRotation, format, formatShift, false)

                            onEnter: {
                                viewModel.setValue(axisIndex, dataType, newValue)
                                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                            }
                        }
                        LimitNumericEditBox{
                            property int dataType: 3
                            property var format: getFormat(dataType)
                            property var formatShift: getFormatShift(dataType)

                            width:144
                            height: 60
                            color: "#ffffff"
                            borderColor: "#999999"
                            borderWidth: 1
                            fontPixelSize: 26
                            textColor: (modelData.isChangeFrontEndRange ? resourceManager.dataColor(18, 35) : resourceManager.dataColor(18, 33))
                            tenkeyTextColor: resourceManager.dataColor(18, 34)
                            keyLayoutType: getTenkeyType(dataType)
                            max: getMax(dataType)
                            min: getMin(dataType)
                            value: resourceManager.formatData(modelData.frontEndRange, format, formatShift, false)

                            onEnter: {
                                viewModel.setValue(axisIndex, dataType, newValue)
                                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                            }
                        }
                        LimitNumericEditBox{
                            property int dataType: 4
                            property var format: getFormat(dataType)
                            property var formatShift: getFormatShift(dataType)

                            width:144
                            height: 60
                            color: "#ffffff"
                            borderColor: "#999999"
                            borderWidth: 1
                            fontPixelSize: 26
                            textColor: (modelData.isChangeOriginCorrection ? resourceManager.dataColor(18, 35) : resourceManager.dataColor(18, 33))
                            tenkeyTextColor: resourceManager.dataColor(18, 34)
                            keyLayoutType: getTenkeyType(dataType)
                            max: getMax(dataType)
                            min: getMin(dataType)
                            value: resourceManager.formatData(modelData.originCorrection, format, formatShift, false)

                            onEnter: {
                                viewModel.setValue(axisIndex, dataType, newValue)
                                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                            }
                        }
                        LimitNumericEditBox{
                            property int dataType: 5
                            property var format: getFormat(dataType)
                            property var formatShift: getFormatShift(dataType)

                            width:144
                            height: 60
                            color: "#ffffff"
                            borderColor: "#999999"
                            borderWidth: 1
                            fontPixelSize: 26
                            textColor: (modelData.isChangeBeforeOriginReturnSpeed ? resourceManager.dataColor(18, 35) : resourceManager.dataColor(18, 33))
                            tenkeyTextColor: resourceManager.dataColor(18, 34)
                            keyLayoutType: getTenkeyType(dataType)
                            max: getMax(dataType)
                            min: getMin(dataType)
                            value: resourceManager.formatData(modelData.beforeOriginReturnSpeed, format, formatShift, false)

                            onEnter: {
                                viewModel.setValue(axisIndex, dataType, newValue)
                                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                            }
                        }
                        LimitNumericEditBox{
                            property int dataType: 6
                            property var format: getFormat(dataType)
                            property var formatShift: getFormatShift(dataType)

                            width:144
                            height: 60
                            color: "#ffffff"
                            borderColor: "#999999"
                            borderWidth: 1
                            fontPixelSize: 26
                            textColor: (modelData.isChangeFreeOperationSpeedLow ? resourceManager.dataColor(18, 35) : resourceManager.dataColor(18, 33))
                            tenkeyTextColor: resourceManager.dataColor(18, 34)
                            keyLayoutType: getTenkeyType(dataType)
                            max: getMax(dataType)
                            min: getMin(dataType)
                            value: resourceManager.formatData(modelData.freeOperationSpeedLow, format, formatShift, false)

                            onEnter: {
                                viewModel.setValue(axisIndex, dataType, newValue)
                                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                            }
                        }
                    }
                }
            }
        }
        //page2
        Item{
            Row{
                x:8
                spacing: 1
                Column{
                    spacing: 1
                    Repeater{
                        model: 5
                        Rectangle{
                            width:196
                            height: 60
                            color: "#999999"
                            Text {
                                anchors.fill: parent
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                font.pixelSize: lineCount != 1 ? 18 : 20
                                font.bold: true
                                color: resourceManager.rgb(43,37+index)
                                text: resourceManager.label(43,37+index,resourceManager.language)
                            }
                        }
                    }
                }
                Repeater{
                    model: viewModel.getDataList(axisPageNum)
                    Column{
                        property int colIndex: index
                        property int axisNum: (axisPageNum*5)+colIndex
                        property int axisIndex: (axisNum < numAxis ? axisOrder[axisNum] : 0)
                        x:8
                        spacing: 1
                        enabled: isManual
                        LimitNumericEditBox{
                            property int dataType: 7
                            property var format: getFormat(dataType)
                            property var formatShift: getFormatShift(dataType)

                            width:144
                            height: 60
                            color: "#ffffff"
                            borderColor: "#999999"
                            borderWidth: 1
                            fontPixelSize: 26
                            textColor: (modelData.isChangeFreeOperationSpeedMedium ? resourceManager.dataColor(18, 35) : resourceManager.dataColor(18, 33))
                            tenkeyTextColor: resourceManager.dataColor(18, 34)
                            keyLayoutType: getTenkeyType(dataType)
                            max: getMax(dataType)
                            min: getMin(dataType)
                            value: resourceManager.formatData(modelData.freeOperationSpeedMedium, format, formatShift, false)

                            onEnter: {
                                viewModel.setValue(axisIndex, dataType, newValue)
                                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                            }
                        }
                        LimitNumericEditBox{
                            property int dataType: 8
                            property var format: getFormat(dataType)
                            property var formatShift: getFormatShift(dataType)

                            width:144
                            height: 60
                            color: "#ffffff"
                            borderColor: "#999999"
                            borderWidth: 1
                            fontPixelSize: 26
                            textColor: (modelData.isChangeFreeOperationSpeedHigh ? resourceManager.dataColor(18, 35) : resourceManager.dataColor(18, 33))
                            tenkeyTextColor: resourceManager.dataColor(18, 34)
                            keyLayoutType: getTenkeyType(dataType)
                            max: getMax(dataType)
                            min: getMin(dataType)
                            value: resourceManager.formatData(modelData.freeOperationSpeedHigh, format, formatShift, false)

                            onEnter: {
                                viewModel.setValue(axisIndex, dataType, newValue)
                                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                            }
                        }
                        LimitNumericEditBox{
                            property int dataType: 9
                            property var format: getFormat(dataType)
                            property var formatShift: getFormatShift(dataType)

                            width:144
                            height: 60
                            color: "#ffffff"
                            borderColor: "#999999"
                            borderWidth: 1
                            fontPixelSize: 26
                            textColor: (modelData.isChangeAccelerationAndDecelerationTimeMin ? resourceManager.dataColor(18, 35) : resourceManager.dataColor(18, 33))
                            tenkeyTextColor: resourceManager.dataColor(18, 34)
                            keyLayoutType: getTenkeyType(dataType)
                            max: getMax(dataType)
                            min: getMin(dataType)
                            value: resourceManager.formatData(modelData.accelerationAndDecelerationTimeMin, format, formatShift, false)

                            onEnter: {
                                viewModel.setValue(axisIndex, dataType, newValue)
                                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                            }
                        }
                        LimitNumericEditBox{
                            property int dataType: 10
                            property var format: getFormat(dataType)
                            property var formatShift: getFormatShift(dataType)

                            width:144
                            height: 60
                            color: "#ffffff"
                            borderColor: "#999999"
                            borderWidth: 1
                            fontPixelSize: 26
                            textColor: (modelData.isChangeAccelerationAndDecelerationTimeMax ? resourceManager.dataColor(18, 35) : resourceManager.dataColor(18, 33))
                            tenkeyTextColor: resourceManager.dataColor(18, 34)
                            keyLayoutType: getTenkeyType(dataType)
                            max: getMax(dataType)
                            min: getMin(dataType)
                            value: resourceManager.formatData(modelData.accelerationAndDecelerationTimeMax, format, formatShift, false)

                            onEnter: {
                                viewModel.setValue(axisIndex, dataType, newValue)
                                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                            }
                        }
                        LimitNumericEditBox{
                            property int dataType: 11
                            property var format: getFormat(dataType)
                            property var formatShift: getFormatShift(dataType)

                            width:144
                            height: 60
                            color: "#ffffff"
                            borderColor: "#999999"
                            borderWidth: 1
                            fontPixelSize: 26
                            textColor: (modelData.isChangeRefuelingRange ? resourceManager.dataColor(18, 35) : resourceManager.dataColor(18, 33))
                            tenkeyTextColor: resourceManager.dataColor(18, 34)
                            keyLayoutType: getTenkeyType(dataType)
                            max: getMax(dataType)
                            min: getMin(dataType)
                            value: resourceManager.formatData(modelData.refuelingRange, format, formatShift, false)

                            onEnter: {
                                viewModel.setValue(axisIndex, dataType, newValue)
                                commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                            }
                        }
                    }
                }
            }
        }
    }

    // ImageScrollBar{
    //     x:940
    //     y:95
    //     height: 425
    //     topButtonPressImage: resourceManager.imagePath + "/26_machine_p/d260_09" + ".png"
    //     topButtonReleaseImage: resourceManager.imagePath + "/26_machine_p/d260_07" + ".png"
    //     botomButtonPressImage: resourceManager.imagePath + "/26_machine_p/d260_10" + ".png"
    //     botomButtonReleaseImage: resourceManager.imagePath + "/26_machine_p/d260_08" + ".png"
    //     pageLabal: pageNum+1 + "/" + page_max
    //     pageLabelColor:"black"
    //     onScrollUpClicked: pageBack()
    //     onScrollDownClicked: pageNext()
    // }//换页按钮

Item {
    id: horizontalScrollBar
    width: 500
    height: 60
    x: 268
    y: 524   // 위치는 적절히 조정하세요

    Rectangle {
        width: parent.width
        height: parent.height
        radius: 30
        color: "#eaeaea"
        border.color: "#999999"
    }

    // 왼쪽 버튼 (이전 페이지)
    Image {
        id: leftButton
        width: 60
        height: 60
        source: mouseLeft.pressed ? resourceManager.imagePath + "/26_machine_p/a260_09.png"
                                  : resourceManager.imagePath + "/26_machine_p/a260_07.png"
        MouseArea {
            id: mouseLeft
            anchors.fill: parent
            onClicked: pageBack()
        }
    }

    // 오른쪽 버튼 (다음 페이지)
    Image {
        id: rightButton
        width: 60
        height: 60
        anchors.right: parent.right
        source: mouseRight.pressed ? resourceManager.imagePath + "/26_machine_p/a260_10.png"
                                   : resourceManager.imagePath + "/26_machine_p/a260_08.png"
        MouseArea {
            id: mouseRight
            anchors.fill: parent
            onClicked: pageNext()
        }
    }

    // 페이지 텍스트
    Text {
        anchors.centerIn: parent
        font.pixelSize: 20
        color: "black"
        text: (pageNum + 1) + " / " + page_max
    }
}


    Remind_save{
        id:remind_save2_3
        remindtype: viewModel.saveStatus
    }

    Remind_Forbid_Dialog{
        id:remind_save_forbid
        anchors.centerIn: parent
        message: resourceManager.label(43,279,resourceManager.language)
    }//非手动禁止参数保存弹窗

    function getMin(dataType){
        return resourceManager.getPenSetStringValue("18", 41 + dataType, 1);
    }

    function getMax(dataType){
        return resourceManager.getPenSetStringValue("18", 41 + dataType, 2);
    }

    function getTenkeyType(dataType){
        var format = resourceManager.getPenSetIntValue("18", 41 + dataType, 3);

        return (resourceManager.formatIsDecimals(format) ? 6 : 2);
    }

    function getDispData(axisPage, col){
        var dataClass;

        switch(col){
        case 0:
            dataClass = (axisPage === 0 ? viewModel.data1 : viewModel.data6);
            break;
        case 1:
            dataClass = (axisPage === 0 ? viewModel.data2 : viewModel.data7);
            break;
        case 2:
            dataClass = (axisPage === 0 ? viewModel.data3 : viewModel.data8);
            break;
        case 3:
            dataClass = viewModel.data4;
            break;
        case 4:
            dataClass = viewModel.data5;
            break;
        }

        return dataClass;
    }

    function getFormatShift(dataType){
        var format_shift;

        switch(dataType){
        case 0:     // 1回転距離
        case 3:     // 手前完了距離
        case 4:     // 原点補正
        case 9:     // 加減速時間[最小]
        case 10:    // 加減速時間[最大]
            format_shift = 2;
            break;
        case 1:     // 1回転パルス数
        case 2:     // 最大回転数
        case 5:     // 原点復帰前速度
        case 6:     // 自由操作速度[低]
        case 7:     // 自由操作速度[中]
        case 8:     // 自由操作速度[高]
        case 11:    // 給油距離
            format_shift = 0;
            break;
        }

        return format_shift;
    }

    function getFormat(dataType){
        var format = resourceManager.getPenSetIntValue("18", 41 + dataType, 3);

        return format;
    }

    function pageNext(){
        if(pageNum < page_max-1){
            pageNum ++
        }else{
            pageNum = 0
        }
    }

    function pageBack(){
        if( 0 < pageNum){
            pageNum --
        }else{
            pageNum=page_max-1
        }
    }
/***********************************************************************/
//    Connections{
//        target:phyKeyManager
//        enabled: mecha_param_page.pageActive
//        onJogScrollDownChanged:{
//            if(pageActive){
//                //console.log("ManualIO: received jogScrollDown:" + jogScrollDown)
//                pageNext();
//            }
//        }

//        onJogScrollUpChanged:{
//            if(pageActive){
//                //console.log("ManualIO: received jogScrollUp:" + jogScrollUp)
//                pageBack();
//            }
//        }
//    }
//    MouseArea {
//        anchors.fill: parent
//        propagateComposedEvents: true
//        onPressed: mouse.accepted = false
//        onReleased: mouse.accepted = false
//        onClicked: mouse.accepted = false
//        onWheel: {
//            if(!REAL_TARGET) {
//                if(wheel.angleDelta.y > 0) {
//                    //console.log("mouse wheel up")
//                    phyKeyManager.decrementEmulatedJogCount()
//                } else if(wheel.angleDelta.y < 0) {
//                    //console.log("mouse wheel down")
//                    phyKeyManager.incrementEmulatedJogCount()
//                }
//            }
//        }
//    }
}
