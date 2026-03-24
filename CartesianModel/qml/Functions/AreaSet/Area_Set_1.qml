import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import Enums 1.0
import "../../Widget"
import "../../Manual/"
import "../../Program/PosSet"
import "../../Program/PosSet/PosSetUtils.js" as Utils

Item {
    id:areaSetPage
    width: parent.width
    height: parent.height
    property bool pageActive:false
    property var viewModel: areaSetViewModel
    property var axisOperateViewmodel: manualViewModel.panelModel[1]
    property bool free: false
    property int editIndex: -1
    property int area_index: 0
    property var axisOperateViewModel: manualViewModel.panelModel[1]
    property var axisDataList: axisOperateViewModel.axisInfo.axisDataList
    property var axisOrder: commonViewModel.axisOrder
    property int numAxis: axisOrder.length
    property int numAxisPerPage: 5
    property var dataList: viewModel.dataList
    property var ioSignalList: viewModel.ioSignalList
    property int areaType: viewModel.currentAreaType
    property var overList: [Enums.AxisY, Enums.AxisMX, Enums.AxisMZ, Enums.AxisSX, Enums.AxisSZ]
    property bool dataLoaded: viewModel.dataLoaded
    property bool isManual: commonViewModel.manipulateMode === 1
    property var modepageModel: teachingViewModel.pageModel[0]

    function isAxisValid(axisIndex){
        return (axisIndex < numAxis)
    }

    function setData(areaType, axisIndex, valueStr){
        let value = parseFloat(valueStr)
        if (value !== Utils.INVALID_POSITION_VALUE) {
            value = (value * local.areaSetDataMagnification).toFixed(2)
        }
        viewModel.setDataByAreaSet(areaType, axisIndex, value)
    }

    onPageActiveChanged: {
        if (pageActive) {
            viewModel.startPageChanging()
            commonViewModel.pageChange(Enums.FunctionAreaSet)
        }
    }

    QtObject {
        id: penSet
        readonly property string groupNo: "19"
        property string selectedButtonColor: ""
        property string dataColor: ""
        property string ioOnColor: ""
        Component.onCompleted: {
            selectedButtonColor = resourceManager.dataColor(groupNo, 2)
            dataColor = resourceManager.dataColor(groupNo, 3)
            ioOnColor = resourceManager.dataColor(groupNo, 4)
        }
    }

    QtObject {
        id: local
        property int areaSetDataMagnification: 100
        property int language: 0
        Component.onCompleted: {
            areaSetDataMagnification = viewModel.dataMagnification()
        }
    }
    Binding {
        target: local
        property: "language"
        value: resourceManager.language
        when: visible
    }

    AreaSetUtils {
        id: area_set_utils
    }

/**************************************left area**********************************************************/
    Rectangle{
        width: 621
        height: 520
        x:2
        y:2
        color: "#c1c3ff"
        border.color: "grey"
        border.width: 1
        Rectangle{
            width: 621
            height: 54
            color: "#25bbed"
            border.color: "grey"
            border.width: 1
            Row{
                spacing: -1
                Rectangle{
                    width: 156
                    height: 54
                    color: "#25bbed"
                    border.color: "#898989"
                    property bool signal: ioSignalList[0]
                    Text{
                        width: 110
                        height: 54
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignRight
                        font.pixelSize: 20
                        lineHeight: 0.7
                        text: resourceManager.label(44, 3,resourceManager.language)
                        color: parent.signal ? penSet.ioOnColor : resourceManager.rgb(44, 3)
                    }
                    Rectangle{
                        width: 18
                        height: 18
                        x:120
                        anchors.verticalCenter: parent.verticalCenter
                        radius: 9
                        border.width: 2
                        border.color: "#336699"
                        color: parent.signal ? "#F0EB4C" : "#003366"
                    }
                }
                Rectangle{
                    width: 156
                    height: 54
                    color: "#25bbed"
                    border.color: "#898989"
                    property bool signal: ioSignalList[1]
                    Text{
                        width: 110
                        height: 54
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignRight
                        font.pixelSize: 20
                        text: resourceManager.label(44, 4,resourceManager.language)
                        color: parent.signal ? penSet.ioOnColor : resourceManager.rgb(44, 4)
                    }
                    Rectangle{
                        width: 18
                        height: 18
                        x:120
                        anchors.verticalCenter: parent.verticalCenter
                        radius: 9
                        border.width: 2
                        border.color: "#336699"
                        color: parent.signal ? "#F0EB4C" : "#003366"
                    }
                }
                Rectangle{
                    width: 156
                    height: 54
                    color: "#25bbed"
                    border.color: "#898989"
                    property bool signal: ioSignalList[2]
                    Text{
                        width: 110
                        height: 54
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignRight
                        font.pixelSize: 20
                        text: resourceManager.label(44, 5,resourceManager.language)
                        color: parent.signal ? penSet.ioOnColor : resourceManager.rgb(44, 5)
                    }
                    Rectangle{
                        width: 18
                        height: 18
                        x:120
                        anchors.verticalCenter: parent.verticalCenter
                        radius: 9
                        border.width: 2
                        border.color: "#336699"
                        color: parent.signal ? "#F0EB4C" : "#003366"
                    }
                }
                Rectangle{
                    width: 156
                    height: 54
                    color: "#25bbed"
                    border.color: "#898989"
                    property bool signal: ioSignalList[3]
                    Text{
                        width: 110
                        height: 54
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignRight
                        font.pixelSize: 20
                        text: resourceManager.label(44, 6,resourceManager.language)
                        color: parent.signal ? penSet.ioOnColor : resourceManager.rgb(44, 6)
                    }
                    Rectangle{
                        width: 18
                        height: 18
                        x:120
                        anchors.verticalCenter: parent.verticalCenter
                        radius: 9
                        border.width: 2
                        border.color: "#336699"
                        color: parent.signal ? "#F0EB4C" : "#003366"
                    }
                }
            }
        }//上方
        Rectangle{
            width: 621
            height: 414
            y:52
            color: "#c1c3f4"
            border.color: "grey"
            border.width: 1
            Image {
                anchors.centerIn: parent
                anchors.verticalCenterOffset: 30
                source: resourceManager.imagePath + "/19_movable/d190_03.png"
            }//图片
            Rectangle{
                x: 6
                y: 10
                width: 160
                height: 50
                border.color: "#898989"
                color: (area_index == 0) ? "yellow" : "#dcdddd"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    lineHeight: 0.7
                    font.pixelSize: 18
                    text: resourceManager.label(44,7, resourceManager.language)
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        area_index = 0
                        viewModel.currentAreaType = Enums.AreaTypeOutputSideMax
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }//取物最大
            Rectangle{
                x: 6
                y: 75
                width: 160
                height: 50
                border.color: "#898989"
                color: (area_index == 1) ? "yellow" : "#dcdddd"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    lineHeight: 0.7
                    font.pixelSize: 18
                    text: resourceManager.label(44,9, resourceManager.language)
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        area_index = 1
                        viewModel.currentAreaType = Enums.AreaTypeOutputSideMin
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }//取物最小
            Rectangle{
                x: 6
                y: 355
                width: 160
                height: 50
                border.color: "#898989"
                color: (area_index == 2) ? "yellow" : "#dcdddd"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    lineHeight: 0.7
                    font.pixelSize: 18
                    text: resourceManager.label(44,11, resourceManager.language)
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        area_index = 2
                        viewModel.currentAreaType = Enums.AreaTypeDescentMax
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }//下降最大
            Rectangle{
                x: 454
                y: 10
                width: 160
                height: 50
                //radius: 6
                border.color: "#898989"
                color: (area_index == 4) ? "yellow" : "#dcdddd"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    lineHeight: 0.7
                    font.pixelSize: 18
                    text: resourceManager.label(44,8, resourceManager.language)
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        area_index = 4
                        viewModel.currentAreaType = Enums.AreaTypeOpenSideMax
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }//置物最大
            Rectangle{
                x: 454
                y: 75
                width: 160
                height: 50
                border.color: "#898989"
                color: (area_index == 5) ? "yellow" : "#dcdddd"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    lineHeight: 0.7
                    font.pixelSize: 18
                    text: resourceManager.label(44,10, resourceManager.language)
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        area_index = 5
                        viewModel.currentAreaType = Enums.AreaTypeOpenSideMin
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }//置物最小
            Rectangle{
                x: 454
                y: 355
                width: 160
                height: 50
                border.color: "#898989"
                color: (area_index == 3) ? "yellow" : "#dcdddd"
                Text{
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    lineHeight: 0.7
                    font.pixelSize: 18
                    text: resourceManager.label(44,12, resourceManager.language)
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        area_index = 3
                        viewModel.currentAreaType = Enums.AreaTypeDescentMaxMovement
                        phyKeyManager.ctrlBuzzer()
                    }
                }
            }//最大移动
        }//中间
        Rectangle{
            width: 621
            height: 55
            anchors.bottom: parent.bottom
            color: "#25bbed"
            border.color: "grey"
            border.width: 1
            Row{
                spacing: -1
                Repeater {
                    model: overList
                    Rectangle{
                        width: 125
                        height: 55
                        color: "#25bbed"
                        border.color: "#898989"
                        property int axis: modelData
                        property bool over: axisDataList[axis].over
                        Text{
                            width: 90
                            height: 55
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignRight
                            font.pixelSize: 18
                            readonly property int axisLabelIndex: Utils.axisLabelIndex(parent.axis)
                            text: resourceManager.label(44, 13 + index,resourceManager.language)
                            color: parent.over ? penSet.ioOnColor : resourceManager.rgb(44, 13 + index)
                        }
                        Rectangle{
                            width: 18
                            height: 18
                            x:95
                            anchors.verticalCenter: parent.verticalCenter
                            radius: 9
                            border.width: 2
                            border.color: "#336699"
                            color: parent.over ? "#F0EB4C" : "#003366"
                        }
                    }
                }
            }
        }//下方
    }

/**************************************right area*********************************************************/
    Rectangle{
        id: freebut
        width: 88
        height: 45
        x: 915
        y: -50
        border.width: 2
        radius: 6
        visible: modepageModel.mode64State === 1
        color: free ? "#DAE000" : "#DAE3F3"
        border.color: "#898989"
        Text {
            anchors.centerIn: parent
            font.pixelSize: 20
            color: resourceManager.rgb(44, 27)
            text: resourceManager.label(44, 27, resourceManager.language)
        }
        MouseArea{
            anchors.fill: parent
            onClicked: {
                free = !free
                phyKeyManager.ctrlBuzzer()
            }
        }
    }//servo free button

    Rectangle {
        x:628
        y:2
        width: 382
        height: 520
        color: "#2AB0DD"
        border.color: "#9DA3A4"

        Row{
            spacing: -1
            Rectangle{
                width: 63
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
                    font.pixelSize: 16
                    lineHeight: 0.8
                    color: "white"
                    text: resourceManager.label(36, 5, local.language)
                }
            }//postion memory
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
                    lineHeight: 0.8
                    color: "white"
                    text: resourceManager.label(30, 16, local.language) + "\n" + resourceManager.label(30, 17, local.language)
                }
            }//encoder position
            Rectangle{
                width: 135
                height: 50
                color: "#156BB3"
                border.color: "#9DA3A4"
                Text{
                    readonly property var currentAreaName: [7, 9, 11, 12, 8, 10]
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 16
                    font.bold: true
                    lineHeight: 0.8
                    color: "yellow"
                    text: resourceManager.label(44, currentAreaName[area_index], resourceManager.language)
                }
            }//area set
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
                    lineHeight: 0.8
                    color: "white"
                    text: resourceManager.label(31, 9, resourceManager.language)
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
                    width: 376
                    height: 58
                    color: "transparent"
                    border.color: "#156BB3"
                    property int axisorder: axisOrder[index]

                    Row{
                        spacing: -1
                        Rectangle{
                            width: 60
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
                            ImageButton {
                                anchors.centerIn: parent
                                enabled: isManual
                                upImageSource: resourceManager.imagePath + "/13_POINT_set/d130_07.png"
                                downImageSource: resourceManager.imagePath + "/13_POINT_set/d130_08.png"
                                onClicked: {
                                    if (area_sav.enabled)
                                        viewModel.setMemoryData(areaType, axisDataBase.axisorder, parseInt(axisDataList[axisDataBase.axisorder].position))
                                }
                            }
                        }//postion memory
                        Rectangle{
                            width: 63
                            height: 58
                            border.color: "#156BB3"
                            color: "transparent"
                            Text{
                                anchors.fill: parent
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignLeft
                                leftPadding: 4
                                font.pixelSize: 18
                                color: "black"
                                text: axisDataList[axisDataBase.axisorder].position + "\n" + axisDataList[axisDataBase.axisorder].encoder
                            }
                        }//encoder position
                        Rectangle{
                            width: 135
                            height: 58
                            border.color: "#156BB3"
                            color: "transparent"
                            LimitNumericEditBox {
                                id:area_sav
                                width: 100
                                height: 46
                                anchors.centerIn: parent
                                textColor: penSet.dataColor
                                color: !enabled ? "#b5b5b6" : opened ? "#daff00" : "white"
                                borderColor: !enabled ? "#C9CACA" : "#898989"
                                value: enabled ? viewModel.formatData(areaType, dataList[axisDataBase.axisorder].value) : ""
                                enabled: dataList[axisDataBase.axisorder].enable
                                fontPixelSize: 20
                                keyLayoutType: 4
                                rectRadius: 0
                                enablePressAndHoldClearKey: true
                                onReady: {
                                    max = commonViewModel.formatPointMaxValue(axisDataBase.axisorder, area_set_utils.getPointNo(areaSetPage.areaType, axisDataBase.axisorder))
                                    min = commonViewModel.formatPointMinValue(axisDataBase.axisorder, area_set_utils.getPointNo(areaSetPage.areaType, axisDataBase.axisorder))
                                    if(isManual){
                                        isNumberKpad2 = false
                                        setDialogPos(500,60,index+1,0,40,0)
                                    }else{
                                        isNumberKpad2 = true
                                        setDialogPos(500,90,index+1,0,70,0)
                                    }
                                }
                                onEnter: {
                                    if(max === "****.**"){
                                        return
                                    }else{
                                        areaSetPage.setData(areaSetPage.areaType, axisDataBase.axisorder, newValue)
                                        commonViewModel.modbusMemblemSwControl(true, Enums.TenkeyInputValid)
                                        commonViewModel.modbusMemblemSwControl(false, Enums.TenkeyInputValid)
                                    }
                                }
                            }
                        }//setting value
                        Rectangle{
                            width: 60
                            height: 58
                            border.color: "#156BB3"
                            color: "transparent"
                            Image {
                                id:axis_servo
                                property bool servo: axisDataList[axisDataBase.axisorder].servo
                                anchors.centerIn: parent
                                visible: free && freebut.visible
                                source: servo ? resourceManager.imagePath + "/13_POINT_set/d130_10.png" : resourceManager.imagePath + "/13_POINT_set/d130_09.png"
                                MouseArea{
                                    anchors.centerIn: parent
                                    width: parent.width + 10
                                    height: parent.height + 10
                                    onPressed: axisOperateViewmodel.modBusServoControl(axisDataBase.axisorder, false)
                                    onReleased: axisOperateViewmodel.modBusServoControl(axisDataBase.axisorder, true)
                                }
                            }
                        }//servo free
                    }
                }
            }
        }//axis data base
    }
}
